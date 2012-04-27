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


        /// <summary>
        ///  Load settings from disk and initialize application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainDialog_Load(object sender, EventArgs e)
        {
            // Load values from settings.
            LoadSettingsData();

            



        }


        private void LoadSettingsData()
        {

            // Load Session Data
            this.lastSessionName.Text = Monitor_Application.Properties.Settings.Default.lastSessionGame;
            this.lastSessionTime.Text = Monitor_Application.Properties.Settings.Default.lastSessionTime;
            this.lastSessionErrors.Text = Monitor_Application.Properties.Settings.Default.lastSessionErrors;

            // Load generic options
            this.runOnStartupCheckBox.Checked = Monitor_Application.Properties.Settings.Default.runOnStartup;
            this.minimizeToTrayBox.Checked = Monitor_Application.Properties.Settings.Default.MinimizeToTray;

            
            // Load Games configuration from XML
            if (Monitor_Application.Properties.Settings.Default.gameSettingsXML.Length != 0)
            {
                StringReader sr = new StringReader(Monitor_Application.Properties.Settings.Default.gameSettingsXML);

                XmlSerializer deserializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));

                GameSettingsDictionary = (SerializableDictionary<String, SerializableDictionary<String, String>>)deserializer.Deserialize(sr);
            }


        }

        private void SaveSettingsData()
        {
            // Save settings.

            Monitor_Application.Properties.Settings.Default.MinimizeToTray = this.minimizeToTrayBox.Checked;

            Monitor_Application.Properties.Settings.Default.lastSessionGame = this.lastSessionName.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionTime = this.lastSessionTime.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionErrors = this.lastSessionErrors.Text;

            Monitor_Application.Properties.Settings.Default.runOnStartup = this.runOnStartupCheckBox.Checked;

            // Serialize Games configuration to XML.
            XmlSerializer serializer = new XmlSerializer(typeof(SerializableDictionary<String, SerializableDictionary<String, String>>));
            StringWriter sw = new StringWriter();

            serializer.Serialize(sw, GameSettingsDictionary);

            Monitor_Application.Properties.Settings.Default.gameSettingsXML = sw.ToString();

            // Save changes.
            Monitor_Application.Properties.Settings.Default.Save();
        }

        /// Save the log contents to a file
        private void saveButton_Click(object sender, EventArgs e)
        {
            // Open a save file dialog

            // Open the file

            // Save logging to the file.

            // Finished.
             
        }

      

        private void InitGameConfiguration()
        {
            // Create a separate game configuration object for each supported/configured game.
      

            

            

        }


        private void AddGameConfiguration()
        {
           // procWatcher = new ProcessWatcher("dcs.exe");

            // procWatcher.ProcessCreated += new ProcessEventHandler(ProcessCreatedEventHandler);
            // procWatcher.ProcessDeleted += new ProcessEventHandler(procWatcher_ProcessDeleted);
            // procWatcher.ProcessModified += new ProcessEventHandler(procWatcher_ProcessModified);
            // procWatcher.Start();
            // 
            // // Do Work
            // 
            // procWatcher.Stop();

        }

        public void ProcessCreatedEventHandler(WMI.Win32.Win32_Process proc)
        {
            // Output some console info.
            Console.WriteLine("Process Created:");
            Console.WriteLine("Name:" + proc.Name);
            Console.WriteLine("Path:" + proc.ExecutablePath);
            Console.WriteLine("ProcessId: " + proc.ProcessId);

            
            // Minimize and enter "active" mode.
            
            // Locate the matching game settings entry


            // Inject applicable plugins.





        }

        private void MainDialog_FormClosing(object sender, FormClosingEventArgs e)
        {

            // Tear down program objects
            


            // Save Settings
            SaveSettingsData();
            



            

        }

        private void addProgramButton_Click(object sender, EventArgs e)
        {
            // Display a dialog to locate the executable
            findProgramDialog.ShowDialog();

            // Ensure this isn't a duplicate


            // Create a new configuration object




        }

        private void findProgramDialog_FileOk(object sender, CancelEventArgs e)
        {
            // Create a new program object.


            // Add the default settings for the object.

            // Save settings data.


        }

    }
}
