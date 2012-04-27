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

namespace Monitor_Application
{
    public partial class MainDialog : Form
    {
        public MainDialog()
        {
            InitializeComponent();
        }

        SerializableDictionary<String, SerializableDictionary<String, String>> GameSettingsDictionary;

        // WMI Process Watcher
        ProcessWatcher procWatcher;

        /// <summary>
        ///  Load settings from disk and initialize application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainDialog_Load(object sender, EventArgs e)
        {
            // Load values from settings.

            // Load Plugins

            // Load Games
            loadGameConfig();       

            // Load Session Data
            this.lastSessionName.Text = Monitor_Application.Properties.Settings.Default.lastSessionGame;
            this.lastSessionTime.Text = Monitor_Application.Properties.Settings.Default.lastSessionTime;
            this.lastSessionErrors.Text = Monitor_Application.Properties.Settings.Default.lastSessionErrors;

            // Load generic options
            this.runOnStartupCheckBox.Checked = Monitor_Application.Properties.Settings.Default.runOnStartup;
            this.minimizeToTrayBox.Checked = Monitor_Application.Properties.Settings.Default.MinimizeToTray;

            // Polling settings
            this.pollingRate.Value = Monitor_Application.Properties.Settings.Default.PollingRate;
            this.pollOnStartup.Checked = Monitor_Application.Properties.Settings.Default.PollOnStartup;
            this.enablePollingCheckbox.Checked = this.pollOnStartup.Checked;
            



        }


        /// Save the log contents to a file
        private void saveButton_Click(object sender, EventArgs e)
        {
            // Open a save file dialog

            // Open the file

            // Save logging to the file.

            // Finished.
             
        }

        private void enablePollingCheckbox_CheckedChanged(object sender, EventArgs e)
        {

            // Start/Stop polling timer.
            pollingTimer.Enabled = enablePollingCheckbox.Checked;


        }

        private bool Tick = true;

        private void pollingTimer_Tick(object sender, EventArgs e)
        {
            


            // If a game is running

                // check process status

                    // If closed, restore polling state

                

            // Check against configured games/executables.

                // If a match is found

                    // Greatly reduce polling rate, temporarily.

                    // Start the injector

        }

        private void loadGameConfig()
        {

            AddProcessWatcher();

            if (Monitor_Application.Properties.Settings.Default.gameSettingsXML.Length == 0)
                return;

            // Open XML and restore game settings
            StringReader sr = new StringReader(Monitor_Application.Properties.Settings.Default.gameSettingsXML);
            
            XmlSerializer deserializer = new XmlSerializer(typeof(SerializableDictionary<String,SerializableDictionary<String, String>>));

            GameSettingsDictionary = (SerializableDictionary<String, SerializableDictionary<String, String>>)deserializer.Deserialize(sr);

            

        }

        private void saveGameConfig()
        {

            // Serialize the config to xml.
            XmlSerializer serializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));
            StringWriter sw = new StringWriter();

            serializer.Serialize(sw, GameSettingsDictionary);
         
            Monitor_Application.Properties.Settings.Default.gameSettingsXML = sw.ToString();

        }

        private void AddProcessWatcher()
        {
            procWatcher = new ProcessWatcher("dcs.exe");

            procWatcher.ProcessCreated += new ProcessEventHandler(ProcessCreatedEventHandler);
            // procWatcher.ProcessDeleted += new ProcessEventHandler(procWatcher_ProcessDeleted);
            // procWatcher.ProcessModified += new ProcessEventHandler(procWatcher_ProcessModified);
            procWatcher.Start();
            // 
            // // Do Work
            // 
            // procWatcher.Stop();

        }

        public void ProcessCreatedEventHandler(WMI.Win32.Win32_Process proc)
        {
            Console.WriteLine("Process Created:");
            // Output some console info.
            Console.WriteLine("Name:" + proc.Name);
            Console.WriteLine("Path:" + proc.ExecutablePath);
            Console.WriteLine("ProcessId: " + proc.ProcessId);

        }

        private void MainDialog_FormClosing(object sender, FormClosingEventArgs e)
        {

            procWatcher.Stop();

            // Save settings.

            Monitor_Application.Properties.Settings.Default.MinimizeToTray = this.minimizeToTrayBox.Checked;
            Monitor_Application.Properties.Settings.Default.PollOnStartup = this.pollOnStartup.Checked;

            Monitor_Application.Properties.Settings.Default.PollingRate = this.pollingRate.Value;
            Monitor_Application.Properties.Settings.Default.lastSessionGame = this.lastSessionName.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionTime = this.lastSessionTime.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionErrors = this.lastSessionErrors.Text;

            Monitor_Application.Properties.Settings.Default.runOnStartup = this.runOnStartupCheckBox.Checked;

            // Save Configured Games
            saveGameConfig();
            



            Monitor_Application.Properties.Settings.Default.Save();

        }

        private void pollingRate_ValueChanged(object sender, EventArgs e)
        {
            if (pollingRate.Value <= 0)
                pollingRate.Value = 1;

            pollingTimer.Interval = (int)pollingRate.Value;
        }
    }
}
