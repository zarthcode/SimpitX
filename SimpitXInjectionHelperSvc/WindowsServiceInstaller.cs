using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration.Install;
using System.ComponentModel;
using System.ServiceProcess;

namespace SimpitXInjectionHelperSvc
{
	[RunInstaller(true)]
	public class WindowsServiceInstaller : Installer
	{
		ServiceProcessInstaller procInstaller = new ServiceProcessInstaller();
		ServiceInstaller svcInstaller = new ServiceInstaller();

		public WindowsServiceInstaller()
		{
	
			// set privileges
			procInstaller.Account = ServiceAccount.NetworkService;
	
	
			svcInstaller.DisplayName = "SimpitX 32-bit DLL Injection Service";
			svcInstaller.StartType = ServiceStartMode.Automatic;			
			svcInstaller.Description = "32-bit DLL Injection helper for 64-bit systems.";
			// Must be the same
			svcInstaller.ServiceName = "SimpitX";
	
			this.Installers.Add(procInstaller);
			this.Installers.Add(svcInstaller);

			
		}
	}
}
