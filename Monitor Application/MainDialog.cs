using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Monitor_Application
{
    public partial class MainDialog : Form
    {
        public MainDialog()
        {
            InitializeComponent();
        }

        /// <summary>
        ///  Load settings from disk and initialize application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainDialog_Load(object sender, EventArgs e)
        {
            // Load values from settings.

            // Load Plugins

            // Load Session Data
            this.lastSessionName.Text = Monitor_Application.Properties.Settings.Default.lastSessionGame;
            this.lastSessionTime.Text = Monitor_Application.Properties.Settings.Default.lastSessionTime;
            this.lastSessionErrors.Text = Monitor_Application.Properties.Settings.Default.lastSessionErrors;

            // Load generic options
            this.runOnStartupCheckBox.Checked = Monitor_Application.Properties.Settings.Default.runOnStartup;
            this.minimizeToTrayBox.Checked = Monitor_Application.Properties.Settings.Default.MinimizeToTray;

            // Polling settings
            this.pollOnStartup.Checked = Monitor_Application.Properties.Settings.Default.PollOnStartup;
            this.enablePollingCheckbox.Checked = this.pollOnStartup.Checked;
            this.pollingRate.Value = Monitor_Application.Properties.Settings.Default.PollingRate;



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

            // Start/Stop polling timer

        }

        private void pollingTimer_Tick(object sender, EventArgs e)
        {
            // Obtain a list of running processes.


            // If a game is running

                // check process status

                    // If closed, restore polling state

                

            // Check against configured games/executables.

                // If a match is found

                    // Greatly reduce polling rate, temporarily.

                    // Start the injector

        }

        private void loadGames()
        {

            // Open 

        }

        private void MainDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Save settings.

            Monitor_Application.Properties.Settings.Default.MinimizeToTray = this.minimizeToTrayBox.Checked;
            Monitor_Application.Properties.Settings.Default.PollOnStartup = this.pollOnStartup.Checked;

            Monitor_Application.Properties.Settings.Default.PollingRate = this.pollingRate.Value;
            Monitor_Application.Properties.Settings.Default.lastSessionGame = this.lastSessionName.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionTime = this.lastSessionTime.Text;
            Monitor_Application.Properties.Settings.Default.lastSessionErrors = this.lastSessionErrors.Text;

            Monitor_Application.Properties.Settings.Default.runOnStartup = this.runOnStartupCheckBox.Checked;

            Monitor_Application.Properties.Settings.Default.Save();

        }
    }
}
