using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.ComponentModel;
using System.Collections;
using System.Globalization;
using System.Management;
using WMI.Win32;

namespace Monitor_Application
{

	[Serializable]
	public class ProcessInfo
	{
		public String FileName;
		public Int32 Id;
		public Boolean Is64Bit;
		public String User;
	}

	public delegate void ProcessEventHandler(ProgramConfiguration parent, WMI.Win32.Win32_Process proc);

	public class ProcessWatcher : ManagementEventWatcher
	{

		// GameConfiguration object this process watcher belongs to
		ProgramConfiguration Parent;

		// Process Events
		public event ProcessEventHandler ProcessCreated;
		public event ProcessEventHandler ProcessDeleted;
		public event ProcessEventHandler ProcessModified;

		// WMI WQL process query strings
		static readonly string WMI_OPER_EVENT_QUERY = @"SELECT * FROM __InstanceOperationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'";
		
		static readonly string WMI_OPER_EVENT_QUERY_WITH_PROC = WMI_OPER_EVENT_QUERY + " and TargetInstance.ExecutablePath = '{0}'";

		public ProcessWatcher(ProgramConfiguration MyParent)
		{
			Parent = MyParent;
			Init(string.Empty);
		}


		public ProcessWatcher(ProgramConfiguration MyParent, string processName)
		{
			Parent = MyParent;
			Init(processName);
		}


		private void Init(string processName)
		{
			this.Query.QueryLanguage = "WQL";
			if (string.IsNullOrEmpty(processName))
			{
				this.Query.QueryString = WMI_OPER_EVENT_QUERY;
			}
			else
			{
				this.Query.QueryString =
					string.Format(WMI_OPER_EVENT_QUERY_WITH_PROC, processName.Replace("\\", "\\\\"));
			}
			Console.WriteLine("Basic Working Query:" + WMI_OPER_EVENT_QUERY);
			Console.WriteLine("Starting processWatcher with query \"" + this.Query.QueryString + "\"");
			this.EventArrived += new EventArrivedEventHandler(watcher_EventArrived);
		}


		private void watcher_EventArrived(object sender, EventArrivedEventArgs e)
		{
			string eventType = e.NewEvent.ClassPath.ClassName;

			Win32_Process proc = new Win32_Process(e.NewEvent["TargetInstance"] as ManagementBaseObject);

			// Dispatch the event
			switch (eventType)
			{
				case "__InstanceCreationEvent":
					if (ProcessCreated != null)
						ProcessCreated(Parent, proc);
					break;
				case "__InstanceDeletionEvent":
					if (ProcessDeleted != null)
						ProcessDeleted(Parent, proc);
					break;
				case "__InstanceModificationEvent":
					if (ProcessModified != null)
						ProcessModified(Parent, proc);
					break;
			}
		}
	}

	
}


