﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using EasyHook;
namespace Monitor_Application
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{

			try
			{
				Config.Register("SimpitX");
			}
			catch (ApplicationException)
			{

			}
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainDialog());
		}
	}
}
