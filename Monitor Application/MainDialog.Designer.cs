namespace Monitor_Application
{
    partial class MainDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainDialog));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.statusPage = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.saveButton = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lastSessionErrors = new System.Windows.Forms.Label();
            this.lastSessionTime = new System.Windows.Forms.Label();
            this.lastSessionName = new System.Windows.Forms.Label();
            this.lastSessionIconBox = new System.Windows.Forms.PictureBox();
            this.optionsPage = new System.Windows.Forms.TabPage();
            this.genericGroupBox = new System.Windows.Forms.GroupBox();
            this.pollOnStartup = new System.Windows.Forms.CheckBox();
            this.minimizeToTrayBox = new System.Windows.Forms.CheckBox();
            this.runOnStartupCheckBox = new System.Windows.Forms.CheckBox();
            this.pollingGroupBox = new System.Windows.Forms.GroupBox();
            this.enablePollingCheckbox = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.pollingRate = new System.Windows.Forms.NumericUpDown();
            this.gamesPage = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkedListBox1 = new System.Windows.Forms.CheckedListBox();
            this.gameIconBox = new System.Windows.Forms.PictureBox();
            this.removeGameButton = new System.Windows.Forms.Button();
            this.addGameButton = new System.Windows.Forms.Button();
            this.gameComboBox1 = new System.Windows.Forms.ComboBox();
            this.pluginPage = new System.Windows.Forms.TabPage();
            this.label2 = new System.Windows.Forms.Label();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.button3 = new System.Windows.Forms.Button();
            this.helpPage = new System.Windows.Forms.TabPage();
            this.aboutPage = new System.Windows.Forms.TabPage();
            this.updateCheckButton = new System.Windows.Forms.Button();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.pollingTimer = new System.Windows.Forms.Timer(this.components);
            this.tabControl1.SuspendLayout();
            this.statusPage.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.lastSessionIconBox)).BeginInit();
            this.optionsPage.SuspendLayout();
            this.genericGroupBox.SuspendLayout();
            this.pollingGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pollingRate)).BeginInit();
            this.gamesPage.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gameIconBox)).BeginInit();
            this.pluginPage.SuspendLayout();
            this.aboutPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.statusPage);
            this.tabControl1.Controls.Add(this.optionsPage);
            this.tabControl1.Controls.Add(this.gamesPage);
            this.tabControl1.Controls.Add(this.pluginPage);
            this.tabControl1.Controls.Add(this.helpPage);
            this.tabControl1.Controls.Add(this.aboutPage);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(311, 238);
            this.tabControl1.TabIndex = 0;
            // 
            // statusPage
            // 
            this.statusPage.Controls.Add(this.groupBox3);
            this.statusPage.Controls.Add(this.groupBox2);
            this.statusPage.Location = new System.Drawing.Point(4, 22);
            this.statusPage.Name = "statusPage";
            this.statusPage.Padding = new System.Windows.Forms.Padding(3);
            this.statusPage.Size = new System.Drawing.Size(303, 212);
            this.statusPage.TabIndex = 0;
            this.statusPage.Text = "Status";
            this.statusPage.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.saveButton);
            this.groupBox3.Controls.Add(this.listBox1);
            this.groupBox3.Location = new System.Drawing.Point(6, 106);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(291, 100);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Log";
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(242, 18);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(43, 69);
            this.saveButton.TabIndex = 1;
            this.saveButton.Text = "Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(6, 18);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(230, 69);
            this.listBox1.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.lastSessionErrors);
            this.groupBox2.Controls.Add(this.lastSessionTime);
            this.groupBox2.Controls.Add(this.lastSessionName);
            this.groupBox2.Controls.Add(this.lastSessionIconBox);
            this.groupBox2.Location = new System.Drawing.Point(6, 6);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(285, 94);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Last Session";
            // 
            // lastSessionErrors
            // 
            this.lastSessionErrors.AutoSize = true;
            this.lastSessionErrors.Location = new System.Drawing.Point(80, 53);
            this.lastSessionErrors.Name = "lastSessionErrors";
            this.lastSessionErrors.Size = new System.Drawing.Size(116, 13);
            this.lastSessionErrors.TabIndex = 3;
            this.lastSessionErrors.Text = "No errors encountered.";
            // 
            // lastSessionTime
            // 
            this.lastSessionTime.AutoSize = true;
            this.lastSessionTime.Location = new System.Drawing.Point(80, 37);
            this.lastSessionTime.Name = "lastSessionTime";
            this.lastSessionTime.Size = new System.Drawing.Size(36, 13);
            this.lastSessionTime.TabIndex = 2;
            this.lastSessionTime.Text = "Never";
            // 
            // lastSessionName
            // 
            this.lastSessionName.AutoSize = true;
            this.lastSessionName.Location = new System.Drawing.Point(80, 19);
            this.lastSessionName.Name = "lastSessionName";
            this.lastSessionName.Size = new System.Drawing.Size(88, 13);
            this.lastSessionName.TabIndex = 1;
            this.lastSessionName.Text = "No game loaded.";
            // 
            // lastSessionIconBox
            // 
            this.lastSessionIconBox.Location = new System.Drawing.Point(6, 19);
            this.lastSessionIconBox.Name = "lastSessionIconBox";
            this.lastSessionIconBox.Size = new System.Drawing.Size(64, 64);
            this.lastSessionIconBox.TabIndex = 0;
            this.lastSessionIconBox.TabStop = false;
            // 
            // optionsPage
            // 
            this.optionsPage.Controls.Add(this.genericGroupBox);
            this.optionsPage.Controls.Add(this.pollingGroupBox);
            this.optionsPage.Location = new System.Drawing.Point(4, 22);
            this.optionsPage.Name = "optionsPage";
            this.optionsPage.Padding = new System.Windows.Forms.Padding(3);
            this.optionsPage.Size = new System.Drawing.Size(303, 212);
            this.optionsPage.TabIndex = 1;
            this.optionsPage.Text = "Options";
            this.optionsPage.UseVisualStyleBackColor = true;
            // 
            // genericGroupBox
            // 
            this.genericGroupBox.Controls.Add(this.pollOnStartup);
            this.genericGroupBox.Controls.Add(this.minimizeToTrayBox);
            this.genericGroupBox.Controls.Add(this.runOnStartupCheckBox);
            this.genericGroupBox.Location = new System.Drawing.Point(159, 6);
            this.genericGroupBox.Name = "genericGroupBox";
            this.genericGroupBox.Size = new System.Drawing.Size(138, 91);
            this.genericGroupBox.TabIndex = 6;
            this.genericGroupBox.TabStop = false;
            this.genericGroupBox.Text = "Generic Options";
            // 
            // pollOnStartup
            // 
            this.pollOnStartup.AutoSize = true;
            this.pollOnStartup.Location = new System.Drawing.Point(5, 65);
            this.pollOnStartup.Name = "pollOnStartup";
            this.pollOnStartup.Size = new System.Drawing.Size(134, 17);
            this.pollOnStartup.TabIndex = 2;
            this.pollOnStartup.Text = "Poll on program startup";
            this.pollOnStartup.UseVisualStyleBackColor = true;
            // 
            // minimizeToTrayBox
            // 
            this.minimizeToTrayBox.AutoSize = true;
            this.minimizeToTrayBox.Location = new System.Drawing.Point(5, 42);
            this.minimizeToTrayBox.Name = "minimizeToTrayBox";
            this.minimizeToTrayBox.Size = new System.Drawing.Size(98, 17);
            this.minimizeToTrayBox.TabIndex = 1;
            this.minimizeToTrayBox.Text = "Minimize to tray";
            this.minimizeToTrayBox.UseVisualStyleBackColor = true;
            // 
            // runOnStartupCheckBox
            // 
            this.runOnStartupCheckBox.AutoSize = true;
            this.runOnStartupCheckBox.Location = new System.Drawing.Point(5, 19);
            this.runOnStartupCheckBox.Name = "runOnStartupCheckBox";
            this.runOnStartupCheckBox.Size = new System.Drawing.Size(96, 17);
            this.runOnStartupCheckBox.TabIndex = 0;
            this.runOnStartupCheckBox.Text = "Run on startup";
            this.runOnStartupCheckBox.UseVisualStyleBackColor = true;
            // 
            // pollingGroupBox
            // 
            this.pollingGroupBox.Controls.Add(this.enablePollingCheckbox);
            this.pollingGroupBox.Controls.Add(this.label1);
            this.pollingGroupBox.Controls.Add(this.pollingRate);
            this.pollingGroupBox.Location = new System.Drawing.Point(6, 6);
            this.pollingGroupBox.Name = "pollingGroupBox";
            this.pollingGroupBox.Size = new System.Drawing.Size(147, 91);
            this.pollingGroupBox.TabIndex = 5;
            this.pollingGroupBox.TabStop = false;
            this.pollingGroupBox.Text = "Polling";
            // 
            // enablePollingCheckbox
            // 
            this.enablePollingCheckbox.AutoSize = true;
            this.enablePollingCheckbox.Location = new System.Drawing.Point(6, 19);
            this.enablePollingCheckbox.Name = "enablePollingCheckbox";
            this.enablePollingCheckbox.Size = new System.Drawing.Size(59, 17);
            this.enablePollingCheckbox.TabIndex = 5;
            this.enablePollingCheckbox.Text = "Enable";
            this.enablePollingCheckbox.UseVisualStyleBackColor = true;
            this.enablePollingCheckbox.CheckedChanged += new System.EventHandler(this.enablePollingCheckbox_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 39);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Rate (mS)";
            // 
            // pollingRate
            // 
            this.pollingRate.Location = new System.Drawing.Point(63, 37);
            this.pollingRate.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.pollingRate.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.pollingRate.Name = "pollingRate";
            this.pollingRate.Size = new System.Drawing.Size(64, 20);
            this.pollingRate.TabIndex = 2;
            this.pollingRate.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.pollingRate.ValueChanged += new System.EventHandler(this.pollingRate_ValueChanged);
            // 
            // gamesPage
            // 
            this.gamesPage.Controls.Add(this.groupBox1);
            this.gamesPage.Controls.Add(this.removeGameButton);
            this.gamesPage.Controls.Add(this.addGameButton);
            this.gamesPage.Controls.Add(this.gameComboBox1);
            this.gamesPage.Location = new System.Drawing.Point(4, 22);
            this.gamesPage.Name = "gamesPage";
            this.gamesPage.Size = new System.Drawing.Size(303, 212);
            this.gamesPage.TabIndex = 5;
            this.gamesPage.Text = "Games";
            this.gamesPage.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkedListBox1);
            this.groupBox1.Controls.Add(this.gameIconBox);
            this.groupBox1.Location = new System.Drawing.Point(3, 59);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(296, 152);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Game Settings";
            // 
            // checkedListBox1
            // 
            this.checkedListBox1.FormattingEnabled = true;
            this.checkedListBox1.Location = new System.Drawing.Point(6, 82);
            this.checkedListBox1.Name = "checkedListBox1";
            this.checkedListBox1.Size = new System.Drawing.Size(284, 64);
            this.checkedListBox1.TabIndex = 1;
            // 
            // gameIconBox
            // 
            this.gameIconBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.gameIconBox.Location = new System.Drawing.Point(226, 12);
            this.gameIconBox.MaximumSize = new System.Drawing.Size(64, 64);
            this.gameIconBox.MinimumSize = new System.Drawing.Size(64, 64);
            this.gameIconBox.Name = "gameIconBox";
            this.gameIconBox.Size = new System.Drawing.Size(64, 64);
            this.gameIconBox.TabIndex = 0;
            this.gameIconBox.TabStop = false;
            // 
            // removeGameButton
            // 
            this.removeGameButton.Location = new System.Drawing.Point(154, 30);
            this.removeGameButton.Name = "removeGameButton";
            this.removeGameButton.Size = new System.Drawing.Size(141, 23);
            this.removeGameButton.TabIndex = 2;
            this.removeGameButton.Text = "Remove Game";
            this.removeGameButton.UseVisualStyleBackColor = true;
            // 
            // addGameButton
            // 
            this.addGameButton.Location = new System.Drawing.Point(7, 30);
            this.addGameButton.Name = "addGameButton";
            this.addGameButton.Size = new System.Drawing.Size(141, 23);
            this.addGameButton.TabIndex = 1;
            this.addGameButton.Text = "Add New Game...";
            this.addGameButton.UseVisualStyleBackColor = true;
            // 
            // gameComboBox1
            // 
            this.gameComboBox1.FormattingEnabled = true;
            this.gameComboBox1.Location = new System.Drawing.Point(3, 3);
            this.gameComboBox1.Name = "gameComboBox1";
            this.gameComboBox1.Size = new System.Drawing.Size(297, 21);
            this.gameComboBox1.TabIndex = 0;
            // 
            // pluginPage
            // 
            this.pluginPage.Controls.Add(this.label2);
            this.pluginPage.Controls.Add(this.linkLabel1);
            this.pluginPage.Controls.Add(this.button3);
            this.pluginPage.Location = new System.Drawing.Point(4, 22);
            this.pluginPage.Name = "pluginPage";
            this.pluginPage.Size = new System.Drawing.Size(303, 212);
            this.pluginPage.TabIndex = 2;
            this.pluginPage.Text = "Plugins";
            this.pluginPage.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 199);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Add your own plugin!";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(125, 199);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(175, 13);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "http://www.zarthcode.com/SimpitX";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(3, 173);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(297, 23);
            this.button3.TabIndex = 0;
            this.button3.Text = "Get New Plugins...";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // helpPage
            // 
            this.helpPage.Location = new System.Drawing.Point(4, 22);
            this.helpPage.Name = "helpPage";
            this.helpPage.Size = new System.Drawing.Size(303, 212);
            this.helpPage.TabIndex = 3;
            this.helpPage.Text = "Help";
            this.helpPage.UseVisualStyleBackColor = true;
            // 
            // aboutPage
            // 
            this.aboutPage.Controls.Add(this.updateCheckButton);
            this.aboutPage.Controls.Add(this.richTextBox1);
            this.aboutPage.Controls.Add(this.pictureBox1);
            this.aboutPage.Location = new System.Drawing.Point(4, 22);
            this.aboutPage.Name = "aboutPage";
            this.aboutPage.Size = new System.Drawing.Size(303, 212);
            this.aboutPage.TabIndex = 4;
            this.aboutPage.Text = "About";
            this.aboutPage.UseVisualStyleBackColor = true;
            // 
            // updateCheckButton
            // 
            this.updateCheckButton.Location = new System.Drawing.Point(5, 182);
            this.updateCheckButton.Name = "updateCheckButton";
            this.updateCheckButton.Size = new System.Drawing.Size(292, 24);
            this.updateCheckButton.TabIndex = 2;
            this.updateCheckButton.Text = "Check for updates...";
            this.updateCheckButton.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox1.Location = new System.Drawing.Point(74, 12);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(226, 76);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "SimpitX - Realtime Simulation Graphics Exporter\n\n©2012 Anthony Clay, ZarthCode LL" +
    "C\nwww.zarthcode.com/simpitx";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::Monitor_Application.Properties.Resources.Fist___64;
            this.pictureBox1.Location = new System.Drawing.Point(4, 5);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(64, 64);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // notifyIcon
            // 
            this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon.Icon")));
            this.notifyIcon.Text = "SimpitX";
            this.notifyIcon.Visible = true;
            // 
            // pollingTimer
            // 
            this.pollingTimer.Tick += new System.EventHandler(this.pollingTimer_Tick);
            // 
            // MainDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(335, 262);
            this.Controls.Add(this.tabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(351, 300);
            this.Name = "MainDialog";
            this.Text = "SimpitX";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainDialog_FormClosing);
            this.Load += new System.EventHandler(this.MainDialog_Load);
            this.tabControl1.ResumeLayout(false);
            this.statusPage.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.lastSessionIconBox)).EndInit();
            this.optionsPage.ResumeLayout(false);
            this.genericGroupBox.ResumeLayout(false);
            this.genericGroupBox.PerformLayout();
            this.pollingGroupBox.ResumeLayout(false);
            this.pollingGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pollingRate)).EndInit();
            this.gamesPage.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.gameIconBox)).EndInit();
            this.pluginPage.ResumeLayout(false);
            this.pluginPage.PerformLayout();
            this.aboutPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage statusPage;
        private System.Windows.Forms.TabPage optionsPage;
        private System.Windows.Forms.TabPage gamesPage;
        private System.Windows.Forms.TabPage pluginPage;
        private System.Windows.Forms.TabPage helpPage;
        private System.Windows.Forms.TabPage aboutPage;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button removeGameButton;
        private System.Windows.Forms.Button addGameButton;
        private System.Windows.Forms.ComboBox gameComboBox1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown pollingRate;
        private System.Windows.Forms.CheckBox minimizeToTrayBox;
        private System.Windows.Forms.CheckBox runOnStartupCheckBox;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button updateCheckButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.NotifyIcon notifyIcon;
        private System.Windows.Forms.GroupBox genericGroupBox;
        private System.Windows.Forms.GroupBox pollingGroupBox;
        private System.Windows.Forms.CheckBox enablePollingCheckbox;
        private System.Windows.Forms.PictureBox gameIconBox;
        private System.Windows.Forms.CheckedListBox checkedListBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.PictureBox lastSessionIconBox;
        private System.Windows.Forms.Label lastSessionName;
        private System.Windows.Forms.Label lastSessionErrors;
        private System.Windows.Forms.Label lastSessionTime;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.CheckBox pollOnStartup;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.Timer pollingTimer;
    }
}

