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
	public partial class EditConfigurationDialog : Form
	{

		public EditConfigurationDialog()
		{
			InitializeComponent();
		}

		ProgramConfiguration ProgramConfig;

		public void Config(String titleText, ProgramConfiguration progConfig)
		{
			this.Name = titleText;
			ProgramConfig = progConfig;

			// Prefill the fields
			if (!progConfig.SettingsDictionary.ContainsKey("Info"))
			{
				throw new System.Exception("Logic error - Info key should be present!");
			}

			programNameTextBox.Text = progConfig.SettingsDictionary["Info"]["Program Name"];
			descriptionTextBox.Text = progConfig.SettingsDictionary["Info"]["Description"];
			companyTextBox.Text = progConfig.SettingsDictionary["Info"]["Company Name"];
			executablePathTextBox.Text = progConfig.SettingsDictionary["Info"]["ExecutablePath"];
			
			if (progConfig.SettingsDictionary["Info"].ContainsKey("Comment"))
			{
				commentTextBox.Text = progConfig.SettingsDictionary["Info"]["Comment"];
			}
			else
			{
				commentTextBox.Text = "";
			}

			if (progConfig.GetIcon() != null)
			{

				programIcon.Image = (Image)progConfig.GetIcon().ToBitmap();

			}

		}

		private void programNameTextBox_Validating(object sender, CancelEventArgs e)
		{
			// Cannot be empty.
			if (programNameTextBox.Text.Length == 0 || programNameTextBox.Text == "Configured Programs...")
			{
				e.Cancel = true;
				programNameTextBox.Select(0, programNameTextBox.Text.Length);
				errorProvider.SetError(programNameTextBox, "Program name required.");
			}
		}

		private void programNameTextBox_Validated(object sender, EventArgs e)
		{
			// Reset
			errorProvider.SetError(programNameTextBox, "");
		}

		private void okButton_Click(object sender, EventArgs e)
		{

			ProgramConfig.SettingsDictionary["Info"]["Program Name"] = programNameTextBox.Text;
			ProgramConfig.SettingsDictionary["Info"]["Description"] = descriptionTextBox.Text;			

			if (ProgramConfig.SettingsDictionary["Info"].ContainsKey("Comment"))
			{
				ProgramConfig.SettingsDictionary["Info"]["Comment"] = commentTextBox.Text;
			}
			else
			{
				ProgramConfig.SettingsDictionary["Info"].Add("Comment", commentTextBox.Text);
			}

		}

	
	}
}
