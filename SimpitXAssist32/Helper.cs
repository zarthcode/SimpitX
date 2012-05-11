using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Threading;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;

namespace SimpitXAssist32
{
	public class InjectorHelper
	{

		private static AutoResetEvent asyncMonitorEvent;
		private Thread AsyncMonitorThread;
		private static bool shutdownFlag = false;

		private NamedPipeServerStream injectionHelperPipeStream;
		private IAsyncResult activePipeServer;

		[DllImport("newloaderWin32.dll", CharSet = CharSet.Unicode)]
		private static extern bool InjectIntoProcess(uint ProcessId, string LibPath);

		[DllImport("newloaderWin32.dll", CharSet = CharSet.Unicode)]
		private static extern string GetErrorMsgW();

		public static bool Is64BitProcess
		{
			get { return IntPtr.Size == 8; }
		}

		LogWriter log = new LogWriter();

		public InjectorHelper()
		{
#if DEBUG
			FileStream stream;
			if (!File.Exists("simpitxassist32.log"))
			{
				stream = File.Create("simpitxassist32.log");
			}
			else
			{
				stream = new FileStream("simpitxassist32.log", FileMode.Truncate, FileAccess.Write);
			}
			log.SetWriter(stream);
			Console.SetOut(log);
#endif
			if (Is64BitProcess)
			{
				// Not cool!
				Exception ex = new Exception("Critical Error: This helper is running as a 64bit process!");
				Console.WriteLine(ex.Message);
				throw ex;
			}

			try
			{

				injectionHelperPipeStream = new NamedPipeServerStream("SimpitXInjectionHelper", PipeDirection.In, 1, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);

			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception thrown while creating pipe. " + ex.Message);
				if (ex.InnerException != null)
				{
					Console.WriteLine("\tInnerexception: " + ex.InnerException.Message);
				}

				throw ex;
			}

			asyncMonitorEvent = new AutoResetEvent(false);
			AsyncMonitorThread = new Thread(AsyncMonitor);
			AsyncMonitorThread.Start();

			Console.WriteLine("Helper Successfully Started.");
		}

		public void Stop()
		{

			Console.WriteLine("Stopping Helper...");

			shutdownFlag = true;
			asyncMonitorEvent.Set();	// Wakeup the monitor thread.
			AsyncMonitorThread.Join(2000);

			// Stops listening for connections
			if (injectionHelperPipeStream.IsConnected)
			{
				Console.WriteLine("Command pipe is connected, closing.");
				injectionHelperPipeStream.Disconnect();
			}

			Console.WriteLine("Helper stopped successfully");

		}


		private void AsyncMonitor()
		{
			Console.WriteLine("Monitor started");
			do
			{
				// Ensure there are open connections available

				if ((activePipeServer == null) || (activePipeServer.IsCompleted))
				{

					Console.WriteLine("Starting pipe");
					// Start waiting for connection
					try
					{
						activePipeServer = injectionHelperPipeStream.BeginWaitForConnection(InjectionServiceCallback, null);
					}
					catch (System.Exception ex)
					{
						Console.WriteLine("Cannot begin new connection yet! " + ex.Message);
					}
				}


				Console.WriteLine("Monitor going to sleep");

				// Yield, until awakened
				asyncMonitorEvent.WaitOne();

				Console.WriteLine("Monitor triggered");

				if (!shutdownFlag)
				{
					asyncMonitorEvent.Reset();
				}


			} while (!shutdownFlag);

			Console.WriteLine("Monitor exiting");

		}

		public void InjectionServiceCallback(IAsyncResult ar)
		{
			Console.WriteLine("Callback triggered");

			try
			{
				injectionHelperPipeStream.EndWaitForConnection(ar);
				asyncMonitorEvent.Set();
			}
			catch (ObjectDisposedException ex)
			{
				Console.WriteLine("Command pipe closed.");
				// The pipe is closed.
				return;
			}
			catch (InvalidOperationException ex)
			{
				Console.WriteLine("Invalid Operation Exception: " + ex.Message);
				return;
			}
			catch (IOException ex)
			{
				Console.WriteLine("IOException: " + ex.Message);
				return;
			}
			catch (Exception ex)
			{
				Console.WriteLine("Exception! " + ex.Message);
			}
			List<Object> commandDetails = new List<Object>();


			// Receive the data
			try
			{

				BinaryFormatter formatter = new BinaryFormatter();

				// Deserialize the command string.
				commandDetails = (List<Object>)formatter.Deserialize(injectionHelperPipeStream);
				injectionHelperPipeStream.Disconnect();
				asyncMonitorEvent.Set();

				if (commandDetails.Count == 0)
				{
					Console.WriteLine("Empty command received!");
					return;
				}

				String command = (String)commandDetails[0];

				switch (command)
				{
					case "StopService":
						{

							Console.WriteLine("'StopService' command received.");
							shutdownFlag = true;
							asyncMonitorEvent.Set();
							Application.Exit();
							break;

						}

					case "Ping":
						{

							Console.WriteLine("'Ping' command received.");
							asyncMonitorEvent.Set();
							break;

						}
					case "InjectIntoProcess":

						Console.WriteLine("'InjectIntoProcess' command received.");

						//  InjectIntoProcess(DWORD dwProcessId, LPCWSTR lpLibPath)
						if (commandDetails.Count != 3)
						{
							throw new Exception("Invalid command number arguments received! (" + commandDetails.Count + ")");

						}
						uint dwProcessId = (uint)commandDetails[1];
						String libPath = (String)commandDetails[2];

						// Todo: Ensure target is 32bit.


						// Call injection method.
						if (!InjectIntoProcess(dwProcessId, libPath))
						{
							// Error encountered
							Console.WriteLine("InjectIntoProcess " + dwProcessId + ", ' " + libPath + "') failed:\n" + GetErrorMsgW());

						}
						else
						{
							// Report the results.
							Console.WriteLine("InjectIntoProcess(" + dwProcessId + ", '" + libPath + "') successful.");
						}
						break;

					default:

						Console.WriteLine("Unknown command received through named-pipe: \'" + commandDetails + "'");
						break;
				}

			}
			catch (System.Exception ex)
			{

				// Write to the event log
				Console.WriteLine("Exception: " + ex.ToString() + "\nSource: " + ex.Source + "\nMessage:" + ex.Message);

			}



		}

		

	}
}
