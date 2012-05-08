using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Threading;
using System.Security;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Runtime.Serialization.Formatters.Binary;

namespace SimpitXInjectionHelperSvc
{
	
	public partial class InjectionService : ServiceBase
	{

		EventLog log;
		string logSource = "SimpitX";


		NamedPipeServerStream injectionHelperPipeStream;
		List<IAsyncResult> activePipeServers = new List<IAsyncResult>();


		public InjectionService()
		{
			InitializeComponent();
			this.ServiceName = logSource;

			// Initialize event log

			log = new EventLog();
			log.Source = logSource;
			log.Log = "Application";

			// Write the startup even.
			log.WriteEntry("Initialization.",  EventLogEntryType.Information);

		}

		protected override void OnStart(string[] args)
		{
			base.OnStart(args);
			log.WriteEntry("Service starting.", EventLogEntryType.Information);

			// Configure pipe security to allow users
			PipeSecurity ps = new PipeSecurity();
			System.Security.Principal.SecurityIdentifier securityId = new System.Security.Principal.SecurityIdentifier(WellKnownSidType.WorldSid, null);
			PipeAccessRule psRule = new PipeAccessRule(securityId, PipeAccessRights.ReadWrite, AccessControlType.Allow);
			ps.AddAccessRule(psRule);
			try
			{
			
				injectionHelperPipeStream = new NamedPipeServerStream("SimpitXInjectionHelper", PipeDirection.In, 1, PipeTransmissionMode.Byte, PipeOptions.Asynchronous, 32, 1, ps);

			}
			catch (System.Exception ex)
			{
				log.WriteEntry("Exception thrown while creating pipe. " + ex.Message, EventLogEntryType.Error);
				if (ex.InnerException != null)
				{
					log.WriteEntry("\tInnerexception: " + ex.InnerException.Message, EventLogEntryType.Error);
				}
			}
			// Process arguments


			// Start waiting for connection
			activePipeServers.Add(injectionHelperPipeStream.BeginWaitForConnection(InjectionServiceCallback, null));

			log.WriteEntry("Service started successfully.", EventLogEntryType.SuccessAudit);

		}

		/// <summary>
		/// Stops the service.
		/// </summary>
		protected override void OnStop()
		{
			base.OnStop();
			log.WriteEntry("OnStop() received", EventLogEntryType.Information);
			// Stops listening for connections
			if (injectionHelperPipeStream.IsConnected)
			{
				log.WriteEntry("Command pipe is connected, closing.", EventLogEntryType.Information);
				injectionHelperPipeStream.Disconnect();
			}

			log.WriteEntry("OnStop() successful", EventLogEntryType.SuccessAudit);

		}

		public void InjectionServiceCallback( IAsyncResult ar)
		{
			log.WriteEntry("Callback triggered", EventLogEntryType.Information);

			try
			{
				injectionHelperPipeStream.EndWaitForConnection(ar);
			}
			catch(ObjectDisposedException ex)
			{
				log.WriteEntry("Command pipe closed.", EventLogEntryType.Information);
				// The pipe is closed.
				return;
			}
			catch (InvalidOperationException ex)
			{
				log.WriteEntry("Invalid Operation Exception: " + ex.Message, EventLogEntryType.Error);
				return;
			}
			catch (IOException ex)
			{
				log.WriteEntry("IOException: " + ex.Message, EventLogEntryType.Error);
				return;
			}
			catch (Exception ex)
			{
				log.WriteEntry("Exception! " + ex.Message, EventLogEntryType.Error);
			}
			List<Object> commandDetails = new List<Object>();

					
			// Receive the data
			try
			{

				BinaryFormatter formatter = new BinaryFormatter();

				// Deserialize the command string.
				commandDetails = (List<Object>)formatter.Deserialize(injectionHelperPipeStream);
				if (commandDetails.Count == 0)
				{
					log.WriteEntry("Empty command received!", EventLogEntryType.Error);
				}

				String command = (String)commandDetails[0];

				switch (command)
				{
					case "StopService":
						{

							log.WriteEntry("'StopService' command received.", EventLogEntryType.Information);
							break;

						}
					case "InjectIntoProcess":
						{
							log.WriteEntry("'InjectIntoProcess' command received.", EventLogEntryType.Information);

							//  InjectIntoProcess(DWORD dwProcessId, LPCWSTR lpLibPath)
							if (commandDetails.Count != 3)
							{
								log.WriteEntry("Invalid command number arguments received! (" + commandDetails.Count + ")", EventLogEntryType.Error);
							}
							uint dwProcessId = (uint)commandDetails[1];
							String libPath = (String)commandDetails[2];

							// Call injection method.


							// Report the results.

							break;
						}
					default:

						log.WriteEntry("Unknown command received through named-pipe: \'" + commandDetails + "'", EventLogEntryType.Error);
						break;
				}
							
			}
			catch (System.Exception ex)
			{

				// Write to the event log
				log.WriteEntry("Exception: " + ex.Message, EventLogEntryType.FailureAudit);

			}
			

			
				
		}
	}
}
