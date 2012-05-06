using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EasyHook;
namespace Monitor_Application
{
	public class ProgramInstance
	{

		private ProgramConfiguration instanceConfiguration;
		private WMI.Win32.Win32_Process processInformation;

		public static Dictionary<uint, ProgramInstance> runningProgramInstances = new Dictionary<uint, ProgramInstance>();

		public ProgramInstance(ProgramConfiguration config, WMI.Win32.Win32_Process process)
		{
			instanceConfiguration = config;
			processInformation = process;

			// Self-register
			ProgramInstance.runningProgramInstances.Add(process.ProcessId, this);

		}

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
						Console.WriteLine("Calling \n\t NativeAPI.RhInjectLibrary(" +
							(int)processInformation.ProcessId + ", 0, 0, " +
							plugin.GetLibraryx86() + ", " +
							plugin.GetLibraryx64() + ", " +
							IntPtr.Zero + ", 0);");
						// RemoteHooking.Inject((int)processInformation.ProcessId, InjectionOptions.DoNotRequireStrongName, plugin.GetLibraryx86(), plugin.GetLibraryx64(), null);
						NativeAPI.RhInjectLibrary((int)processInformation.ProcessId, 0, NativeAPI.EASYHOOK_INJECT_DEFAULT, plugin.GetLibraryx86(), plugin.GetLibraryx64(), IntPtr.Zero, 0);
					}
					catch (System.Exception ex)
					{
						Console.WriteLine("Remote injection attempt failed: " + ex.Message);
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
				throw new Exception("Program has no 'Plugins' configuration section.");
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

	}
}
