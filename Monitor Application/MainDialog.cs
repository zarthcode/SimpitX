using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;
using System.Diagnostics;
using System.Reflection;
using System.ServiceProcess;

namespace Monitor_Application
{
	public partial class MainDialog : Form
	{
		/// <summary>
		/// Generic cross-thread access control
		/// </summary>
		/// <typeparam name="t"></typeparam>
		/// <param name="cont"></param>
		/// <param name="action"></param>
		public static void InvokeControlAction<t>(t cont, Action<t> action) where t : Control
		{
			if (cont.InvokeRequired)
			{
				cont.Invoke(new Action<t, Action<t>>(InvokeControlAction),
						  new object[] { cont, action });
			}
			else
			{ action(cont); }
		}


		public MainDialog()
		{
			InitializeComponent();
		}

		SerializableDictionary<String, ProgramConfiguration> ConfiguredPrograms = new SerializableDictionary<String, ProgramConfiguration>();
		

		/// <summary>
		///  Load settings from disk and initialize application.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void MainDialog_Load(object sender, EventArgs e)
		{
			// Load values from settings.


			try
			{
				LoadSettingsData();
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception thrown during LoadSettingsData(): \n" + ex.Message);
				throw ex;
			}

			// Override window state.
			if (startMinimizedCheckBox.Checked)
			{
				this.WindowState = FormWindowState.Minimized;
			}

			// Load Plugins
			try
			{
				LoadPlugins();
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception thrown during LoadPlugins(): \n" + ex.Message);
				throw ex;
			}

			// UpdateUI
			UpdateProgramComboList();
			UpdatePluginList();

			// Start monitoring programs
			StartMonitoring();

		}

		/// <summary>
		/// Loads all settings from disk/persistence
		/// </summary>
		private void LoadSettingsData()
		{

			// Load Session Data
			this.lastSessionName.Text = Monitor_Application.Properties.Settings.Default.lastSessionGame;
			this.lastSessionTime.Text = Monitor_Application.Properties.Settings.Default.lastSessionTime;
			this.lastSessionErrors.Text = Monitor_Application.Properties.Settings.Default.lastSessionErrors;
			this.closeToTrayCheckBox.Checked = Monitor_Application.Properties.Settings.Default.CloseToTray;
			this.showBalloonNotificationsInTrayCheckBox.Checked = Monitor_Application.Properties.Settings.Default.ShowBalloonNotifications;
			this.alwaysShowTrayIconCheckBox.Checked = Monitor_Application.Properties.Settings.Default.AlwaysShowTrayIcon;
			// Load generic options
			this.runOnStartupCheckBox.Checked = Monitor_Application.Properties.Settings.Default.runOnStartup;
			this.minimizeToTrayBox.Checked = Monitor_Application.Properties.Settings.Default.MinimizeToTray;
			this.startInjectorHelperCheckBox.Checked = Monitor_Application.Properties.Settings.Default.StartInjectorHelper;
			
			// Load window state
			this.Size = Properties.Settings.Default.WndSize;
			this.Location = Properties.Settings.Default.WndLoc;
			this.startMinimizedCheckBox.Checked = Properties.Settings.Default.StartMinimized;
			
			// Not used, but stored.
			// this.WindowState = Properties.Settings.Default.WndState;
	
			// Deserialize the list of strings
			
			List<String> StringList;

			if (Monitor_Application.Properties.Settings.Default.ProgramConfigurationXML.Length != 0)
			{

				XmlSerializer serializer = new XmlSerializer(typeof(List<String>));

				StringReader sr = new StringReader(Monitor_Application.Properties.Settings.Default.ProgramConfigurationXML);
				StringList = (List<String>)serializer.Deserialize(sr);

				
			}
			else
			{
				StringList = new List<String>();
			}


			// Deserialize configured programs
			foreach (String progConfigXML in StringList)
			{

				XmlSerializer serializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));
				StringReader sr = new StringReader(progConfigXML);
				ProgramConfiguration loadedConfig = new ProgramConfiguration();
				loadedConfig.SettingsDictionary = (SerializableDictionary<String, SerializableDictionary<String, String>>)serializer.Deserialize(sr);
				ConfiguredPrograms.Add(loadedConfig.SettingsDictionary["Info"]["Program Name"], loadedConfig);

			}



		}

		/// <summary>
		/// Stores all configuration/settings to disk.
		/// </summary>
		private void SaveSettingsData()
		{
			// Save/create default settings. 

			Properties.Settings.Default.MinimizeToTray = this.minimizeToTrayBox.Checked;
			Properties.Settings.Default.lastSessionGame = this.lastSessionName.Text;
			Properties.Settings.Default.lastSessionTime = this.lastSessionTime.Text;
			Properties.Settings.Default.lastSessionErrors = this.lastSessionErrors.Text;
			Properties.Settings.Default.CloseToTray = this.closeToTrayCheckBox.Checked;
			Properties.Settings.Default.ShowBalloonNotifications = this.showBalloonNotificationsInTrayCheckBox.Checked;
			Properties.Settings.Default.AlwaysShowTrayIcon = this.alwaysShowTrayIconCheckBox.Checked;
			Properties.Settings.Default.runOnStartup = this.runOnStartupCheckBox.Checked;
			Properties.Settings.Default.StartMinimized = this.startMinimizedCheckBox.Checked;
			Properties.Settings.Default.StartInjectorHelper = this.startInjectorHelperCheckBox.Checked;
			// Save window position
			Properties.Settings.Default.WndState = this.WindowState;

			if (this.WindowState == FormWindowState.Normal)
			{

				Properties.Settings.Default.WndSize = this.Size;
				Properties.Settings.Default.WndLoc = this.Location;
				
			}
			else
			{

				Properties.Settings.Default.WndSize = this.RestoreBounds.Size;
				Properties.Settings.Default.WndLoc = this.RestoreBounds.Location;

			}

			// Serialize program configuration to XML. (DEVELOPMENT ONLY)
			{


				//	XmlSerializer serializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));
				//	StringWriter sw = new StringWriter();
				//	serializer.Serialize(sw, GameSettingsDictionary);
				//	Monitor_Application.Properties.Settings.Default.DefaultGameSettings = sw.ToString();

			}

			// Serialize configured programs
			List<String> StringList = new List<String>();
			foreach( KeyValuePair<String, ProgramConfiguration> progconfig in ConfiguredPrograms)
			{
				
				XmlSerializer serializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));
				StringWriter sw = new StringWriter();
				serializer.Serialize(sw, progconfig.Value.SettingsDictionary);
				StringList.Add(sw.ToString());

			}

			// Serialize the list of strings
			{

				XmlSerializer serializer = new XmlSerializer(typeof(List<String>));
				StringWriter sw = new StringWriter();
				serializer.Serialize(sw, StringList);
				
				Monitor_Application.Properties.Settings.Default.ProgramConfigurationXML = sw.ToString();
			}

			// Save changes.
			Monitor_Application.Properties.Settings.Default.Save();
		}


		/// <summary>
		/// Enables capture of program start/stop/change events.
		/// </summary>
		public void StartMonitoring()
		{
			
			foreach( KeyValuePair<String, ProgramConfiguration> progconfig in ConfiguredPrograms)
			{
				progconfig.Value.Initialize(ProcessCreatedEventHandler, ProcessDeletedEventHandler);
			}

		}

		/// <summary>
		/// Disables capture of program start/stop/change events.
		/// </summary>
		public void StopMonitoring()
		{

			foreach (KeyValuePair<String, ProgramConfiguration> progconfig in ConfiguredPrograms)
			{
				progconfig.Value.Shutdown();
			}
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="program"></param>
		/// <param name="proc"></param>
		public void ProcessCreatedEventHandler(ProgramConfiguration program, WMI.Win32.Win32_Process proc)
		{
			// Output some console info.
			Console.WriteLine("Process Created:");
			Console.WriteLine("Name:" + proc.Name);
			Console.WriteLine("Path:" + proc.ExecutablePath);
			Console.WriteLine("ProcessId: " + proc.ProcessId);


			// Minimize and enter "active" mode.


			// Update session info.
			UpdateSessionInfo(program, proc.CreationDate);

			// Create program instance
			ProgramInstance progInstance = new ProgramInstance(program, proc);

			// Activate plugins.
			progInstance.Start();




		}

		public void ProcessDeletedEventHandler(ProgramConfiguration program, WMI.Win32.Win32_Process proc)
		{
			// Output some console info.
			Console.WriteLine("Process deleted for program:" + program.SettingsDictionary["Info"]["Program Name"]);
			
			// Restore window settings.

			// Locate the matching entry
			if (ProgramInstance.runningProgramInstances.ContainsKey(proc.ProcessId))
			{
				// Kill off any non-injected processes.
				ProgramInstance.runningProgramInstances[proc.ProcessId].Abort();

				// Update session info.
				InvokeControlAction<Label>
					(lastSessionErrors, lse => lse.Text = program["Info", "Program Name"] + " closed.");
			}
			else
			{

				// Update session info.
				InvokeControlAction<Label>
					(lastSessionErrors, lse => lse.Text = program["Info","Program Name"] + " closed, no startup detected.");
			}




		}

		public void ProcessModifiedEventHandler(ProgramConfiguration program, WMI.Win32.Win32_Process proc)
		{
			
			// Do nothing but log the occurance
			Console.WriteLine("Process Modify Event: " + proc.Name);

			// Update session info.
			lastSessionErrors.Text = "Program has been modified.";

		}

		public void UpdateSessionInfo(ProgramConfiguration program, DateTime startTime)
		{
			
			// Update the icon
			if (program.GetIcon() != null)
			{
				InvokeControlAction<PictureBox>
					(lastSessionIconBox, lsib => lsib.Image = program.GetIcon().ToBitmap());
			}

			// Switch to status tab
			InvokeControlAction<TabControl>
				(tabControl, tabctrl => tabctrl.SelectedIndex = 0);

			// Program name
			InvokeControlAction<Label>
				(lastSessionName, lsn => lsn.Text = program.SettingsDictionary["Info"]["Program Name"]);

			// Session Time
			InvokeControlAction<Label>
				(lastSessionTime, lst => lst.Text = startTime.ToShortTimeString());
			
			// Clear Errors encountered
			InvokeControlAction<Label>
				(lastSessionErrors, lse => lse.Text = "Program Startup Detected.");

			// Clear log
			InvokeControlAction<ListBox>
				(logListBox, llb => llb.Items.Clear());
			
			

		}

		public void LoadPlugins()
		{

			// Load plugin data from settings?


			// Load plugin data from xml files/descriptor

			// find plugins directory
			String programFileStr = Assembly.GetExecutingAssembly().Location;
			FileInfo programFile = new FileInfo(programFileStr);
			DirectoryInfo pluginDirectory = new DirectoryInfo(programFile.DirectoryName);

			foreach (DirectoryInfo directory in pluginDirectory.GetDirectories())
				if (directory.Name == "plugins")
				{
					pluginDirectory = directory;
				}



			// Iterate on each plugin's subdirectory.
			foreach (DirectoryInfo directory in pluginDirectory.GetDirectories())
			{

				// Search for "plugin.simpitx.xml"
				FileInfo[] pluginConfigFile = directory.GetFiles("*.plugin.xml");
				if (pluginConfigFile.Length == 0)
				{
					// No plugin config files in this plugin directory - ignore it.
					continue;
				}
				else
				{

					if (pluginConfigFile.Length > 1)
					{

						// Why choose when I can give up?
						throw new Exception("Multiple *.plugin.xml configuration files exist in a single directory.");

					}

					// We've got a plugin!  Configure it!
					Plugin tempPlugin = new Plugin(directory);

				}
			}

		}

		public void UpdatePluginList()
		{

			// Add each plugin to the pluginlistbox.
			pluginListBox.Items.Clear();
			foreach (Plugin pluginInstance in Plugin.LoadedPlugins.Values)
			{
				pluginListBox.Items.Add(pluginInstance.Name);
			}

			if (pluginListBox.Items.Count > 0)
			{
				pluginListBox.SelectedItem = 0;
			}
						
		}

		/// <summary>
		/// Returns the base configuration needed for a program configuration object.
		/// </summary>
		/// <returns></returns>
		public SerializableDictionary<String, SerializableDictionary<String, String>> GetDefaultGameConfigurationSettings()
		{

			// Load Games configuration from XML
			if (Monitor_Application.Properties.Settings.Default.DefaultGameSettings.Length != 0)
			{
				StringReader sr = new StringReader(Monitor_Application.Properties.Settings.Default.DefaultGameSettings);

				XmlSerializer deserializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));

				return (SerializableDictionary<String, SerializableDictionary<String, String>>)deserializer.Deserialize(sr);
			}

			// TODO: Throw an exception, there is no default config!
			return new SerializableDictionary<String, SerializableDictionary<String, String>>();

		}

		/// Save the log contents to a file
		private void saveButton_Click(object sender, EventArgs e)
		{
			// Open a save file dialog

			// Open the file

			// Save logging to the file.

			// Finished.

		}


		/// <summary>
		/// Updates all entries in the dropdown list in the 'Programs' tab.
		/// </summary>
		private void UpdateProgramComboList()
		{

			programComboBox.Items.Clear();
			programComboBox.Items.Add("Configured Programs...");
			foreach(String key in ConfiguredPrograms.Keys)
				programComboBox.Items.Add(key);

			programComboBox.SelectedItem = programComboBox.Items[0];

		}


		private void AddGameConfiguration(ProgramConfiguration configObject)
		{
			

			// procWatcher.ProcessCreated += new ProcessEventHandler(ProcessCreatedEventHandler);
			// procWatcher.ProcessDeleted += new ProcessEventHandler(procWatcher_ProcessDeleted);
			// procWatcher.ProcessModified += new ProcessEventHandler(procWatcher_ProcessModified);
			// procWatcher.Start();
			// 
			// // Do Work
			// 
			// procWatcher.Stop();

		}



		private void MainDialog_FormClosing(object sender, FormClosingEventArgs e)
		{

			// Close to tray functionality
			if (closeToTrayCheckBox.Checked)
			{
				e.Cancel = true;
				this.Hide();
				notifyIcon.Visible = true;

				if (showBalloonNotificationsInTrayCheckBox.Checked)
				{
					// Show a balloon now.
					notifyIcon.ShowBalloonTip(500, "SimpitX is still running!", "You can change this setting using the options menu", ToolTipIcon.Info);
				}

				return;
			}

			// Stop running plugins

			// Tear down program objects
			StopMonitoring();		

			// Save Settings
			SaveSettingsData();




		}

		private void addProgramButton_Click(object sender, EventArgs e)
		{
			// Offer a dialog of known/supported programs

			// Display a dialog to locate the executable
			if (findProgramDialog.ShowDialog() != DialogResult.OK)
				return;



			// Ensure this isn't a duplicate
			foreach (KeyValuePair<String, ProgramConfiguration> entry in ConfiguredPrograms)
			{
				// do something with entry.Value or entry.Key
				if (entry.Value.GetExecutablePath() == findProgramDialog.FileName)
				{
					MessageBox.Show("There is already an entry for this executable.");
					return;
				}
			}

			// Create a new program configuration object
			ProgramConfiguration AddedProgram = new ProgramConfiguration();

			// Insert default program settings.
			AddedProgram.SettingsDictionary = GetDefaultGameConfigurationSettings();

			// Add program specific data.
			if (!AddedProgram.SettingsDictionary.ContainsKey("Info"))
			{

				// Add the info element
				AddedProgram.SettingsDictionary.Add("Info", new SerializableDictionary<String, String>());

				// Add the executable
				AddedProgram.SettingsDictionary["Info"].Add("ExecutablePath", findProgramDialog.FileName);

				// Add the executable name
				AddedProgram.SettingsDictionary["Info"].Add("ExecutableName", findProgramDialog.SafeFileName);

				// Pre-extract file information from executable
				#region GetVersionInfo() extraction
				
				// Get the file version for the notepad.
				FileVersionInfo programFileVersionInfo = FileVersionInfo.GetVersionInfo(findProgramDialog.FileName);

				// Extract the name of the program
				if (programFileVersionInfo.ProductName == null)
				{
					AddedProgram.SettingsDictionary["Info"].Add("Program Name", "");
				}
				else
				{
					AddedProgram.SettingsDictionary["Info"].Add("Program Name", programFileVersionInfo.ProductName);
				}
				
				// Extract other program details.
				if (programFileVersionInfo.CompanyName == null)
				{
					AddedProgram.SettingsDictionary["Info"].Add("Company Name", "");
				} 
				else
				{
					AddedProgram.SettingsDictionary["Info"].Add("Company Name", programFileVersionInfo.CompanyName);
				}

				if (programFileVersionInfo.FileDescription == null)
				{
					AddedProgram.SettingsDictionary["Info"].Add("Description", "");
				} 
				else
				{
					AddedProgram.SettingsDictionary["Info"].Add("Description", programFileVersionInfo.FileDescription);
				}
				#endregion
				
			}

			// Display a dialog to set or override the game's remaining details and settings, and validate all values.
			EditConfigurationDialog configForm = new EditConfigurationDialog();
			configForm.Config("Add Program", AddedProgram);
			if (configForm.ShowDialog() != DialogResult.OK)
			{
				// Abort! Abort!
				return;
			}

			// Save
			ConfiguredPrograms.Add(AddedProgram.SettingsDictionary["Info"]["Program Name"], AddedProgram);
			
			SaveSettingsData();
			UpdateProgramComboList();
			StartMonitoring();



		}

		private void editProgramButton_Click(object sender, EventArgs e)
		{
			// Validate the currently selected program
			if (programComboBox.SelectedIndex == 0)
			{
				// Invalid selection!
				throw new System.Exception("Logic Error - 'Edit Program' button should be disabled when selected program is reference index 0.");
				
			}

			// Display the edit program dialog box.
			EditConfigurationDialog editDialog = new EditConfigurationDialog();
			editDialog.Config("Edit Program", ConfiguredPrograms[(String)programComboBox.SelectedItem]);
			editDialog.ShowDialog();
			SaveSettingsData();

		}

		private void removeProgramButton_Click(object sender, EventArgs e)
		{
			// Validate the currently selected program
			if (programComboBox.SelectedIndex == 0)
			{
				// Invalid selection!
				throw new System.Exception("Logic Error - 'Remove Program' button should be disabled when selected program is reference index 0.");

			}

			// Remove the selected item
			ConfiguredPrograms.Remove((String)programComboBox.SelectedItem);
			programComboBox.Items.Remove(programComboBox.SelectedItem);
			SaveSettingsData();

		}

		private void resetDebugButton_Click(object sender, EventArgs e)
		{
			StopMonitoring();
			ConfiguredPrograms.Clear();
			SaveSettingsData();
			UpdateProgramComboList();
		}

		private void programComboBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (programComboBox.SelectedIndex == 0)
			{

				// Disable Edit/Remove program boxes.
				editProgramButton.Enabled = false;
				removeProgramButton.Enabled = false;

				// Clear program settings
				programIconBox.Image = null;
				programNameLabel.Text = "";
				companyNameLabel.Text = "";
				descriptionLabel.Text = "";

				// Clear Enabled Plugins list
				enabledPluginListBox.Items.Clear();

				return;
			}
			String currentProgramName = (String)programComboBox.SelectedItem;
			ProgramConfiguration currentProgram = ConfiguredPrograms[currentProgramName];
			
			// Update program settings to that of the currently selected program.

			programIconBox.Image = currentProgram.GetIcon().ToBitmap();

			programNameLabel.Text = currentProgram.SettingsDictionary["Info"]["Program Name"];
			companyNameLabel.Text = currentProgram.SettingsDictionary["Info"]["Company Name"];
			descriptionLabel.Text = currentProgram.SettingsDictionary["Info"]["Description"];

			// Update plugin configuration data.
			enabledPluginListBox.Items.Clear();

			// For the master plugin config section.
			if (!currentProgram.SettingsDictionary.ContainsKey("Plugins"))
			{
				// Create the plugin key.
				currentProgram.SettingsDictionary.Add("Plugins", new SerializableDictionary<String, String>());
			}

			// Update checked state of each plugin
			foreach (Plugin availablePlugin in Plugin.LoadedPlugins.Values)
			{
				

				// Get the plugin state
				bool pluginState = false;
				
				if (currentProgram.SettingsDictionary["Plugins"].ContainsKey(availablePlugin.Name))
				{
					pluginState = Convert.ToBoolean(currentProgram.SettingsDictionary["Plugins"][availablePlugin.Name]);
				}
				else
				{
					// Add the key
					currentProgram.SettingsDictionary["Plugins"].Add(availablePlugin.Name, Convert.ToString(pluginState));
				}

				// Add the entry/state to the Plugin list
				int addedPlugin = enabledPluginListBox.Items.Add(availablePlugin.Name);
				enabledPluginListBox.SetItemChecked(addedPlugin, pluginState);
				
			}
			

			// Allow edit/removal.
			editProgramButton.Enabled = true;
			removeProgramButton.Enabled = true;

		}

		private void enabledPluginListBox_ItemCheck(object sender, ItemCheckEventArgs e)
		{

			// Locate the currently selected program configuration
			String currentProgramName = (String)programComboBox.SelectedItem;
			ProgramConfiguration currentProgram = ConfiguredPrograms[currentProgramName];

			// Locate the plugin being configured.
			Plugin selectedPlugin = Plugin.LoadedPlugins[(String)enabledPluginListBox.Items[e.Index]];

			// If enabling
			if ((e.NewValue == CheckState.Checked) && (e.CurrentValue == CheckState.Unchecked))
			{				

				// Update dependency requirements.
				foreach (String dependency in selectedPlugin.Dependencies)
				{
					// Make sure the dependency is available.
					if (!Plugin.LoadedPlugins.ContainsKey(dependency))
					{
						MessageBox.Show("Required plugin not found:\n" + dependency + "\n(Check your settings, some may have been changed.)", "Cannot Enable Plugin", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly );
						// Cancel the attempt.
						e.NewValue = e.CurrentValue;
						return;
					}
					// Force-Enable the dependency.
					currentProgram["Plugins", dependency] = Convert.ToString(true);

					// Update the item in the listbox
					int dependencyIndex = enabledPluginListBox.Items.IndexOf(dependency);
					
					enabledPluginListBox.SetItemChecked(dependencyIndex, true);
				}

			}
			// If disabling
			if ((e.NewValue == CheckState.Unchecked) && (e.CurrentValue == CheckState.Checked))
			{
	
				HashSet<String> foundDependents = new HashSet<String>();

				// Determine if another (enabled) plugin depends on this one.
				foreach (Plugin itPlugin in Plugin.LoadedPlugins.Values)
				{
					if (itPlugin.Name == selectedPlugin.Name)
					{
						// Skip this plugin.
						continue;
					}
					foreach (String dependency in itPlugin.Dependencies)
					{
						if ((dependency == selectedPlugin.Name) && (Convert.ToBoolean(currentProgram["Plugins", itPlugin.Name, Convert.ToString(false)]) == true))
						{
							
							// Enabled, dependent plugin located.
							foundDependents.Add(itPlugin.Name);
							
						}
					}
				}

				// Display a message box warning why this cannot be disabled.
				if (foundDependents.Count > 0)
				{
					String tempStr = "Plugin is a dependency of:";
					
					foreach(String item in foundDependents)
						tempStr += "\n\t" + item;

					MessageBox.Show(tempStr,"Cannot disable plugin.",MessageBoxButtons.OK);
					
					// Cancel
					e.NewValue = e.CurrentValue;
				}


			}
		
			// Add/update program configuration
			bool setState = false;
			switch (e.NewValue)
			{
				case CheckState.Checked:
					setState = true;
					break;

				case CheckState.Unchecked:
					setState = false;
					break;

				case CheckState.Indeterminate:
					// No change
					return;

				default:
					throw new Exception("Invalid plugin checkstate");
	
			}

			currentProgram["Plugins", selectedPlugin.Name] = Convert.ToString(setState);
		}

		private void MainDialog_Resize(object sender, EventArgs e)
		{
			if (FormWindowState.Minimized == this.WindowState && minimizeToTrayBox.Checked)
			{
				notifyIcon.Visible = true;

				if (showBalloonNotificationsInTrayCheckBox.Checked)
				{
					// Show a balloon now.
					notifyIcon.ShowBalloonTip(500, "SimpitX is still running!", "You can change this setting using the options menu", ToolTipIcon.Info);
				}

				this.Hide();
			}
			else if (FormWindowState.Normal == this.WindowState && !alwaysShowTrayIconCheckBox.Checked)
			{
				notifyIcon.Visible = false;
			}
		}

		private void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			// Reopen SimpitX
			if (FormWindowState.Minimized == this.WindowState)
			{
				this.Show();
				this.WindowState = FormWindowState.Normal;

				if (!alwaysShowTrayIconCheckBox.Checked)
				{
					notifyIcon.Visible = false;
				}
			}
			else
			{
				this.Activate();
			}
		}

		private void installPluginButton_Click(object sender, EventArgs e)
		{
			// Pick the directory


			// Ensure it isn't a duplicate.


			// Update the UI
			UpdatePluginList();
		}

		private void removePluginButton_Click(object sender, EventArgs e)
		{
			// remove the selected plugin.

		}

		private void pluginListBox_SelectedValueChanged(object sender, EventArgs e)
		{
			if (pluginListBox.Items.Count == 0)
			{
				pluginDetailLabel.Text = "";
				pluginDescLabel.Text = "";
				return;
			}

			Plugin currentPlugin = Plugin.LoadedPlugins[(String)pluginListBox.SelectedItem];

			pluginDetailLabel.Text = currentPlugin.Name + ", Version " + currentPlugin.Version + " by " + currentPlugin.Author;
			pluginDescLabel.Text = currentPlugin.Description;
		}

		private void startInjectorHelperCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (startInjectorHelperCheckBox.Checked)
			{
				Console.WriteLine("Starting Injection Helper");
				try
				{
					ProgramInstance.StartInjectionHelperSvc();
				}
				catch(Exception ex)
				{
					Console.WriteLine("\tException thrown: " + ex.Message);
				}
			} 
			else
			{
				Console.WriteLine("Stopping Injection Helper");
				try
				{
					ProgramInstance.StopInjectionHelperSvc();
				}
				catch (Exception ex)
				{
					Console.WriteLine("\tException thrown: " + ex.Message);
				}
			}
		}


	}
}
