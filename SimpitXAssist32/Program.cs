using System;
using System.Windows.Forms;

namespace SimpitXAssist32
{
	static class Program
	{

		static InjectorHelper injectorHelper;

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
	
		// Create the notify icon
			System.Windows.Forms.NotifyIcon notifyIcon = new System.Windows.Forms.NotifyIcon();
			notifyIcon.Text = "SimpitX 32-bit Helper";
			notifyIcon.Icon = Properties.Resources.Zarthcode;
			
			notifyIcon.MouseDoubleClick += new MouseEventHandler(notifyIcon_MouseDoubleClick);

		// Add a menu
			ContextMenu contextMenu = new ContextMenu();

			MenuItem menuItem = new MenuItem();
			MenuItem menuItem2 = new MenuItem();
			contextMenu.MenuItems.AddRange(new MenuItem[] { menuItem, menuItem2 });

			menuItem.Index = 0;
			menuItem.Text = "A&bout";
			menuItem.Click += new EventHandler(menuItem_Click);

			menuItem2.Index = 1;
			menuItem2.Text = "Ex&it";
			menuItem2.Click += new EventHandler(exitMenuItem_Click);

			notifyIcon.ContextMenu = contextMenu;
			notifyIcon.Visible = true;

		// Start the helper

			injectorHelper = new InjectorHelper();	// RAII FTW

		
			Application.Run();

		// Shutting down.
			injectorHelper.Stop();
			notifyIcon.Visible = false;
		}
		
		static void exitMenuItem_Click(object sender, EventArgs e)
		{
			if (injectorHelper != null)
			{
				injectorHelper.Stop();
			}
			Application.Exit();
		}

		static void menuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show("This is a helper application for SimpitX that allows operation with 32-bit software on 64bit systems.", "SimpitXAssist32", MessageBoxButtons.OK);
		}

		private static void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			// Display a message box telling the user to move along.
			MessageBox.Show("This is a helper application for SimpitX that allows operation with 32-bit software on 64bit systems.", "SimpitXAssist32", MessageBoxButtons.OK);
		}
	}
}
