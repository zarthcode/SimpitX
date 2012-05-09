using System;
using System.Windows.Forms;

namespace SimpitXAssist32
{
	static class Program
	{
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
			contextMenu.MenuItems.AddRange(new MenuItem[] { menuItem });

			menuItem.Index = 0;
			menuItem.Text = "A&bout";
			menuItem.Click += new EventHandler(menuItem_Click);

			notifyIcon.ContextMenu = contextMenu;
			notifyIcon.Visible = true;

		// Start the helper

			InjectorHelper injectorHelper = new InjectorHelper();	// RAII FTW

		
			Application.Run();

		// Shutting down.
			injectorHelper.Stop();
			notifyIcon.Visible = false;
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
