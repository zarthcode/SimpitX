﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO.Pipes;
using System.ServiceProcess;
using System.Runtime.Serialization.Formatters.Binary;
using System.Diagnostics;

namespace Monitor_Application
{
	public class ProgramInstance
	{
		// Used to control the injection helper.
		private ProgramConfiguration instanceConfiguration;
		private WMI.Win32.Win32_Process processInformation;

		public static Dictionary<uint, ProgramInstance> runningProgramInstances = new Dictionary<uint, ProgramInstance>();


		[DllImport("newloaderx64.dll")]
		static extern bool Is64Bit(uint ProcessId);
		
		[DllImport("newloaderx64.dll", CharSet = CharSet.Unicode)]
		static extern bool InjectIntoProcess(uint ProcessId, string LibPath);

		[DllImport("newloaderx64.dll", CharSet = CharSet.Unicode)]
		static extern string GetErrorMsgW();

		[DllImport("newloaderwin32.dll", EntryPoint = "Is64Bit")]
		static extern bool Is64Bit_x86(uint ProcessId);

		[DllImport("newloaderWin32.dll", EntryPoint = "InjectIntoProcess", CharSet = CharSet.Unicode)]
		static extern bool InjectIntoProcess_x86(uint ProcessId, string LibPath);

		[DllImport("newloaderWin32.dll", EntryPoint = "GetErrorMsgW", CharSet = CharSet.Unicode)]
		static extern string GetErrorMsgW_x86();

		public static bool Is64BitProcess
		{
			get { return IntPtr.Size == 8; }
		}



		/// <summary>
		/// Constructor for the ProgramInstance class
		/// </summary>
		/// <param name="config">reference to configuration data describing what must be done.</param>
		/// <param name="process">process information unique for this instance</param>
		public ProgramInstance(ProgramConfiguration config, WMI.Win32.Win32_Process process)
		{
			instanceConfiguration = config;
			processInformation = process;

			// Self-register
			ProgramInstance.runningProgramInstances.Add(process.ProcessId, this);

		}

		/// <summary>
		/// Runs/Starts all plugins for this running program
		/// </summary>
		public void Start()
		{

			Console.WriteLine("ProgramInstance.Start() Initiated.");

			// Obtain the list of plugins.
			foreach (Plugin plugin in getPluginProcedure())
			{

				// Perform the plugin's action.
				if (plugin.GetAction() == Plugin.ActionType.Inject)
				{
					Console.WriteLine("Injecting " + plugin.Name);
					try
					{

						Console.WriteLine("Determining target process architecture...");

						if (Is64BitProcess)
						{
							if (Is64Bit(processInformation.ProcessId))
							{
								Console.WriteLine("\t...64-bit");
								Console.WriteLine("Injecting '" + plugin.GetLibraryx64() + "'into PID " + processInformation.ProcessId + ".");
								if (!InjectIntoProcess(processInformation.ProcessId, plugin.GetLibraryx64()))
								{
									Console.WriteLine("\tFailed!");
								
									throw new Exception(GetErrorMsgW());
								}
							}
							else
							{
								Console.WriteLine("\t...32-bit (across WOW64 boundary)");
								
								Console.WriteLine("Opening connection to injection helper service.");
								// Connect to the service 
								using (NamedPipeClientStream pipeStream = new NamedPipeClientStream(".", "SimpitXInjectionHelper", PipeDirection.Out, PipeOptions.None, System.Security.Principal.TokenImpersonationLevel.Impersonation))
								{
									// 200mS timeout on connecting.
									pipeStream.Connect(200);

									Console.WriteLine("Connection established, sending command.");

									// Format the command
									BinaryFormatter formatter = new BinaryFormatter();

									List<Object> commandDetails = new List<Object>();

									String command = "InjectIntoProcess";
									
									commandDetails.Add((String)command);
									commandDetails.Add((uint)processInformation.ProcessId);
									commandDetails.Add((String)plugin.GetLibraryx86());

									// Send.
									formatter.Serialize(pipeStream, commandDetails);
									pipeStream.Flush();

									// Disconnect

								}

							}
						}
						else
						{
							if (Is64Bit_x86(processInformation.ProcessId))
							{
								
								// Exactly how does one run a 64bit process on a 32bit OS, anyway?
								// Sarcasm aside, maybe the user forced a 32bit mode?...exceptional, indeed!
								throw new Exception("Plugin injection across WOW64 into x64 targets from x86 is unsupported (Please contact support!)");
							} 
							else
							{

								// 32bit target from 32bit executable.
								Console.WriteLine("\t...32-bit");
								Console.WriteLine("Injecting '" + plugin.GetLibraryx86() + "'into PID " + processInformation.ProcessId + ".");
								if (!InjectIntoProcess_x86(processInformation.ProcessId, plugin.GetLibraryx86()))
								{
									Console.WriteLine("\tFailed!");
								
									throw new Exception(GetErrorMsgW_x86());
								}
							}
						}


					}
					catch (System.Exception ex)
					{
						Console.WriteLine("Remote injection attempt failed: \n\t" + ex.Message);
						throw ex;
					}
				}
				else
				{
					// Run

				}

				// Add the plugin to the stack.
				runningPlugins.Push(plugin);

			}

			Console.WriteLine("ProgramInstance.Start() Complete. " + runningPlugins.Count + " plugin" + (runningPlugins.Count > 1 ? "s" : "") + " running.");
		}

		public void Stop()
		{
			Console.WriteLine("ProgramInstance.Stop() Unimplemented.");

		}

		public void Abort()
		{
			Console.WriteLine("ProgramInstance.Abort() Unimplemented.");
		}

		/// <summary>
		/// A stack of running plugins, in dependency order.
		/// </summary>
		private Stack<Plugin> runningPlugins = new Stack<Plugin>();

		/// <summary>
		/// Creates a list of configured plugins to execute and inject
		/// </summary>
		/// <returns>An ordered list of Plugin objects, in injection/execution order.</returns>
		public List<Plugin> getPluginProcedure()
		{
			// List of plugins
			List<Plugin> pluginList = new List<Plugin>();

			if (instanceConfiguration.SettingsDictionary.ContainsKey("Plugins") == false)
			{
				Console.WriteLine("Program has no 'Plugins' configuration section...creating");
				instanceConfiguration.SettingsDictionary.Add("Plugins", new SerializableDictionary<String, String>());
			}

			// Search the "Plugins" configuration for all enabled plugins.
			foreach (KeyValuePair<String,String> pluginEntry in instanceConfiguration.SettingsDictionary["Plugins"])
			{
				if (Convert.ToBoolean(pluginEntry.Value))
				{
					// pluginEntry.Key is a configured/enabled plugin

					// Locate the plugin.
					if (Plugin.LoadedPlugins.ContainsKey(pluginEntry.Key))
					{
						pluginList.Add(Plugin.LoadedPlugins[pluginEntry.Key]);
					}
					
				}
			}

			// Iterate the list of plugins
			bool ordered = true;
			List<Plugin> orderedPlugins = new List<Plugin>();
			int thisCount = 0;
			do 
			{
				ordered = true;
				thisCount = 0;
				
				// Iterate of the entire list of plugins,
				foreach (Plugin currentPlugin in pluginList)
				{
					foreach (String dependencyName in currentPlugin.Dependencies)
					{
						// If the dependency isn't in the encountered list, it isn't in order.
						if (!orderedPlugins.Contains(Plugin.LoadedPlugins[dependencyName]))
						{
							ordered = false;
							break;
						}
					}
					// Add this plugin to the enountered list.
					if (ordered)
					{
						orderedPlugins.Add(currentPlugin);
						thisCount++;
					}

				}				

			} while (!ordered && (thisCount > 0));
			
			// There is an open/circular dependency
			if (!ordered)
			{
				String errorString = "Unresolved/Circular dependencies detected among: " + (pluginList.Count - orderedPlugins.Count) + " plugins: ";

				foreach (Plugin found in orderedPlugins)
				{
					// Remove the plugins that are resolved.
					pluginList.Remove(found);
				}

				foreach (Plugin missing in pluginList)
				{
					errorString += "\n" + missing.Name; 
				}

				throw new Exception(errorString);
			}

			return orderedPlugins;
		}

		static bool lastKnownStatus = false;

		public static void StartInjectionHelperSvc()
		{

			// Start the 32-bit Injection helper service (64bit-only)
			if (ProgramInstance.Is64BitProcess)
			{

				try
				{

					// Attempt to connect and send a "ping" command.
					using (NamedPipeClientStream pipeStream = new NamedPipeClientStream(".", "SimpitXInjectionHelper", PipeDirection.Out, PipeOptions.None, System.Security.Principal.TokenImpersonationLevel.Impersonation))
					{
						// 200mS timeout on connecting.
						pipeStream.Connect(200);

						Console.WriteLine("Connection established, sending command.");

						// Format the command
						BinaryFormatter formatter = new BinaryFormatter();

						List<Object> commandDetails = new List<Object>();

						String command = "Ping";

						commandDetails.Add((String)command);

						// Send.
						formatter.Serialize(pipeStream, commandDetails);
						pipeStream.Flush();

						// Disconnect
					}

				}
				catch (System.TimeoutException ex)
				{
					// Process isn't running
					Process.Start("SimpitXAssist32.exe");
				}
				catch (Exception ex)
				{
					// Service isn't running properly.
					Console.WriteLine("Exception thrown while contacting Helper service: " + ex.Message);
					Console.WriteLine(ex.ToString());
					
				}

				
			}
			else
			{
				Console.WriteLine("(Skipping Injection Helper Svc Start Call)");
			}

			
		}

		public static void StopInjectionHelperSvc()
		{

			// Start the 32-bit Injection helper service (64bit-only)
			if (ProgramInstance.Is64BitProcess)
			{
				try
				{

					// Attempt to connect and send a "ping" command.
					using (NamedPipeClientStream pipeStream = new NamedPipeClientStream(".", "SimpitXInjectionHelper", PipeDirection.Out, PipeOptions.None, System.Security.Principal.TokenImpersonationLevel.Impersonation))
					{
						// 200mS timeout on connecting.
						pipeStream.Connect(200);

						Console.WriteLine("Connection established, sending command.");

						// Format the command
						BinaryFormatter formatter = new BinaryFormatter();

						List<Object> commandDetails = new List<Object>();

						String command = "StopService";

						commandDetails.Add((String)command);

						// Send.
						formatter.Serialize(pipeStream, commandDetails);
						pipeStream.Flush();

						// Disconnect

					}

				}
				catch (System.TimeoutException ex)
				{
					// Process isn't running
					Process.Start("SimpitXAssist32.exe");
				}
				catch (Exception ex)
				{
					// Service isn't running properly.
					Console.WriteLine("Exception thrown while contacting Helper service: " + ex.Message);
					Console.WriteLine(ex.ToString());
				}
			}
			else
			{
				Console.WriteLine("(Skipping Injection Helper Svc Stop Call)");
			}
		}

	}
}
