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
			this.tabControl = new System.Windows.Forms.TabControl();
			this.statusPage = new System.Windows.Forms.TabPage();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.saveButton = new System.Windows.Forms.Button();
			this.logListBox = new System.Windows.Forms.ListBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.lastSessionErrors = new System.Windows.Forms.Label();
			this.lastSessionTime = new System.Windows.Forms.Label();
			this.lastSessionName = new System.Windows.Forms.Label();
			this.lastSessionIconBox = new System.Windows.Forms.PictureBox();
			this.optionsPage = new System.Windows.Forms.TabPage();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.showBalloonNotificationsInTrayCheckBox = new System.Windows.Forms.CheckBox();
			this.alwaysShowTrayIconCheckBox = new System.Windows.Forms.CheckBox();
			this.closeToTrayCheckBox = new System.Windows.Forms.CheckBox();
			this.minimizeToTrayBox = new System.Windows.Forms.CheckBox();
			this.genericGroupBox = new System.Windows.Forms.GroupBox();
			this.startMinimizedCheckBox = new System.Windows.Forms.CheckBox();
			this.runOnStartupCheckBox = new System.Windows.Forms.CheckBox();
			this.programsPage = new System.Windows.Forms.TabPage();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.enabledPluginListBox = new System.Windows.Forms.CheckedListBox();
			this.editProgramButton = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.descriptionLabel = new System.Windows.Forms.Label();
			this.companyNameLabel = new System.Windows.Forms.Label();
			this.programNameLabel = new System.Windows.Forms.Label();
			this.programIconBox = new System.Windows.Forms.PictureBox();
			this.removeProgramButton = new System.Windows.Forms.Button();
			this.addProgramButton = new System.Windows.Forms.Button();
			this.programComboBox = new System.Windows.Forms.ComboBox();
			this.pluginPage = new System.Windows.Forms.TabPage();
			this.installPluginButton = new System.Windows.Forms.Button();
			this.groupBox7 = new System.Windows.Forms.GroupBox();
			this.pluginDescLabel = new System.Windows.Forms.Label();
			this.pluginDetailLabel = new System.Windows.Forms.Label();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.pluginListBox = new System.Windows.Forms.ListBox();
			this.label2 = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.removePluginButton = new System.Windows.Forms.Button();
			this.helpPage = new System.Windows.Forms.TabPage();
			this.aboutPage = new System.Windows.Forms.TabPage();
			this.updateCheckButton = new System.Windows.Forms.Button();
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.debugTab = new System.Windows.Forms.TabPage();
			this.resetDebugButton = new System.Windows.Forms.Button();
			this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);
			this.findProgramDialog = new System.Windows.Forms.OpenFileDialog();
			this.mainDialogBindingSource = new System.Windows.Forms.BindingSource(this.components);
			this.tabControl.SuspendLayout();
			this.statusPage.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.lastSessionIconBox)).BeginInit();
			this.optionsPage.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.genericGroupBox.SuspendLayout();
			this.programsPage.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.programIconBox)).BeginInit();
			this.pluginPage.SuspendLayout();
			this.groupBox7.SuspendLayout();
			this.groupBox6.SuspendLayout();
			this.aboutPage.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.debugTab.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.mainDialogBindingSource)).BeginInit();
			this.SuspendLayout();
			// 
			// tabControl
			// 
			this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl.Controls.Add(this.statusPage);
			this.tabControl.Controls.Add(this.optionsPage);
			this.tabControl.Controls.Add(this.programsPage);
			this.tabControl.Controls.Add(this.pluginPage);
			this.tabControl.Controls.Add(this.helpPage);
			this.tabControl.Controls.Add(this.aboutPage);
			this.tabControl.Controls.Add(this.debugTab);
			this.tabControl.Location = new System.Drawing.Point(12, 12);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(311, 238);
			this.tabControl.TabIndex = 0;
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
			this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox3.Controls.Add(this.saveButton);
			this.groupBox3.Controls.Add(this.logListBox);
			this.groupBox3.Location = new System.Drawing.Point(6, 84);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(291, 122);
			this.groupBox3.TabIndex = 1;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Log";
			// 
			// saveButton
			// 
			this.saveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.saveButton.Location = new System.Drawing.Point(242, 18);
			this.saveButton.Name = "saveButton";
			this.saveButton.Size = new System.Drawing.Size(43, 36);
			this.saveButton.TabIndex = 1;
			this.saveButton.Text = "Save";
			this.saveButton.UseVisualStyleBackColor = true;
			this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
			// 
			// logListBox
			// 
			this.logListBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.logListBox.FormattingEnabled = true;
			this.logListBox.Location = new System.Drawing.Point(6, 18);
			this.logListBox.Name = "logListBox";
			this.logListBox.Size = new System.Drawing.Size(230, 95);
			this.logListBox.TabIndex = 0;
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.lastSessionErrors);
			this.groupBox2.Controls.Add(this.lastSessionTime);
			this.groupBox2.Controls.Add(this.lastSessionName);
			this.groupBox2.Controls.Add(this.lastSessionIconBox);
			this.groupBox2.Location = new System.Drawing.Point(6, 6);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(291, 72);
			this.groupBox2.TabIndex = 0;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Last Session";
			// 
			// lastSessionErrors
			// 
			this.lastSessionErrors.AutoSize = true;
			this.lastSessionErrors.Location = new System.Drawing.Point(44, 45);
			this.lastSessionErrors.Name = "lastSessionErrors";
			this.lastSessionErrors.Size = new System.Drawing.Size(116, 13);
			this.lastSessionErrors.TabIndex = 3;
			this.lastSessionErrors.Text = "No errors encountered.";
			// 
			// lastSessionTime
			// 
			this.lastSessionTime.AutoSize = true;
			this.lastSessionTime.Location = new System.Drawing.Point(44, 32);
			this.lastSessionTime.Name = "lastSessionTime";
			this.lastSessionTime.Size = new System.Drawing.Size(36, 13);
			this.lastSessionTime.TabIndex = 2;
			this.lastSessionTime.Text = "Never";
			// 
			// lastSessionName
			// 
			this.lastSessionName.AutoSize = true;
			this.lastSessionName.Location = new System.Drawing.Point(44, 19);
			this.lastSessionName.Name = "lastSessionName";
			this.lastSessionName.Size = new System.Drawing.Size(88, 13);
			this.lastSessionName.TabIndex = 1;
			this.lastSessionName.Text = "No game loaded.";
			// 
			// lastSessionIconBox
			// 
			this.lastSessionIconBox.Location = new System.Drawing.Point(6, 19);
			this.lastSessionIconBox.Name = "lastSessionIconBox";
			this.lastSessionIconBox.Size = new System.Drawing.Size(32, 32);
			this.lastSessionIconBox.TabIndex = 0;
			this.lastSessionIconBox.TabStop = false;
			// 
			// optionsPage
			// 
			this.optionsPage.Controls.Add(this.groupBox4);
			this.optionsPage.Controls.Add(this.genericGroupBox);
			this.optionsPage.Location = new System.Drawing.Point(4, 22);
			this.optionsPage.Name = "optionsPage";
			this.optionsPage.Padding = new System.Windows.Forms.Padding(3);
			this.optionsPage.Size = new System.Drawing.Size(303, 212);
			this.optionsPage.TabIndex = 1;
			this.optionsPage.Text = "Options";
			this.optionsPage.UseVisualStyleBackColor = true;
			// 
			// groupBox4
			// 
			this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox4.Controls.Add(this.showBalloonNotificationsInTrayCheckBox);
			this.groupBox4.Controls.Add(this.alwaysShowTrayIconCheckBox);
			this.groupBox4.Controls.Add(this.closeToTrayCheckBox);
			this.groupBox4.Controls.Add(this.minimizeToTrayBox);
			this.groupBox4.Location = new System.Drawing.Point(6, 78);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(291, 71);
			this.groupBox4.TabIndex = 7;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "System Tray";
			// 
			// showBalloonNotificationsInTrayCheckBox
			// 
			this.showBalloonNotificationsInTrayCheckBox.AutoSize = true;
			this.showBalloonNotificationsInTrayCheckBox.Location = new System.Drawing.Point(6, 42);
			this.showBalloonNotificationsInTrayCheckBox.Name = "showBalloonNotificationsInTrayCheckBox";
			this.showBalloonNotificationsInTrayCheckBox.Size = new System.Drawing.Size(149, 17);
			this.showBalloonNotificationsInTrayCheckBox.TabIndex = 4;
			this.showBalloonNotificationsInTrayCheckBox.Text = "Show balloon notifications";
			this.showBalloonNotificationsInTrayCheckBox.UseVisualStyleBackColor = true;
			// 
			// alwaysShowTrayIconCheckBox
			// 
			this.alwaysShowTrayIconCheckBox.AutoSize = true;
			this.alwaysShowTrayIconCheckBox.Location = new System.Drawing.Point(6, 19);
			this.alwaysShowTrayIconCheckBox.Name = "alwaysShowTrayIconCheckBox";
			this.alwaysShowTrayIconCheckBox.Size = new System.Drawing.Size(130, 17);
			this.alwaysShowTrayIconCheckBox.TabIndex = 3;
			this.alwaysShowTrayIconCheckBox.Text = "Always show tray icon";
			this.alwaysShowTrayIconCheckBox.UseVisualStyleBackColor = true;
			// 
			// closeToTrayCheckBox
			// 
			this.closeToTrayCheckBox.AutoSize = true;
			this.closeToTrayCheckBox.Location = new System.Drawing.Point(187, 42);
			this.closeToTrayCheckBox.Name = "closeToTrayCheckBox";
			this.closeToTrayCheckBox.Size = new System.Drawing.Size(84, 17);
			this.closeToTrayCheckBox.TabIndex = 2;
			this.closeToTrayCheckBox.Text = "Close to tray";
			this.closeToTrayCheckBox.UseVisualStyleBackColor = true;
			// 
			// minimizeToTrayBox
			// 
			this.minimizeToTrayBox.AutoSize = true;
			this.minimizeToTrayBox.Location = new System.Drawing.Point(187, 19);
			this.minimizeToTrayBox.Name = "minimizeToTrayBox";
			this.minimizeToTrayBox.Size = new System.Drawing.Size(98, 17);
			this.minimizeToTrayBox.TabIndex = 1;
			this.minimizeToTrayBox.Text = "Minimize to tray";
			this.minimizeToTrayBox.UseVisualStyleBackColor = true;
			// 
			// genericGroupBox
			// 
			this.genericGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.genericGroupBox.Controls.Add(this.startMinimizedCheckBox);
			this.genericGroupBox.Controls.Add(this.runOnStartupCheckBox);
			this.genericGroupBox.Location = new System.Drawing.Point(6, 6);
			this.genericGroupBox.Name = "genericGroupBox";
			this.genericGroupBox.Size = new System.Drawing.Size(291, 66);
			this.genericGroupBox.TabIndex = 6;
			this.genericGroupBox.TabStop = false;
			this.genericGroupBox.Text = "Generic Options";
			// 
			// startMinimizedCheckBox
			// 
			this.startMinimizedCheckBox.AutoSize = true;
			this.startMinimizedCheckBox.Location = new System.Drawing.Point(187, 19);
			this.startMinimizedCheckBox.Name = "startMinimizedCheckBox";
			this.startMinimizedCheckBox.Size = new System.Drawing.Size(96, 17);
			this.startMinimizedCheckBox.TabIndex = 5;
			this.startMinimizedCheckBox.Text = "Start minimized";
			this.startMinimizedCheckBox.UseVisualStyleBackColor = true;
			// 
			// runOnStartupCheckBox
			// 
			this.runOnStartupCheckBox.AutoSize = true;
			this.runOnStartupCheckBox.Location = new System.Drawing.Point(6, 19);
			this.runOnStartupCheckBox.Name = "runOnStartupCheckBox";
			this.runOnStartupCheckBox.Size = new System.Drawing.Size(96, 17);
			this.runOnStartupCheckBox.TabIndex = 0;
			this.runOnStartupCheckBox.Text = "Run on startup";
			this.runOnStartupCheckBox.UseVisualStyleBackColor = true;
			// 
			// programsPage
			// 
			this.programsPage.Controls.Add(this.groupBox5);
			this.programsPage.Controls.Add(this.editProgramButton);
			this.programsPage.Controls.Add(this.groupBox1);
			this.programsPage.Controls.Add(this.removeProgramButton);
			this.programsPage.Controls.Add(this.addProgramButton);
			this.programsPage.Controls.Add(this.programComboBox);
			this.programsPage.Location = new System.Drawing.Point(4, 22);
			this.programsPage.Name = "programsPage";
			this.programsPage.Size = new System.Drawing.Size(303, 212);
			this.programsPage.TabIndex = 5;
			this.programsPage.Text = "Programs";
			this.programsPage.UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox5.Controls.Add(this.enabledPluginListBox);
			this.groupBox5.Location = new System.Drawing.Point(3, 127);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(296, 82);
			this.groupBox5.TabIndex = 5;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Plugins";
			// 
			// enabledPluginListBox
			// 
			this.enabledPluginListBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.enabledPluginListBox.CheckOnClick = true;
			this.enabledPluginListBox.FormattingEnabled = true;
			this.enabledPluginListBox.Location = new System.Drawing.Point(6, 19);
			this.enabledPluginListBox.Name = "enabledPluginListBox";
			this.enabledPluginListBox.Size = new System.Drawing.Size(284, 49);
			this.enabledPluginListBox.TabIndex = 1;
			this.enabledPluginListBox.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.enabledPluginListBox_ItemCheck);
			// 
			// editProgramButton
			// 
			this.editProgramButton.Anchor = System.Windows.Forms.AnchorStyles.Top;
			this.editProgramButton.Location = new System.Drawing.Point(103, 30);
			this.editProgramButton.Name = "editProgramButton";
			this.editProgramButton.Size = new System.Drawing.Size(85, 23);
			this.editProgramButton.TabIndex = 4;
			this.editProgramButton.Text = "Edit Program...";
			this.editProgramButton.UseVisualStyleBackColor = true;
			this.editProgramButton.Click += new System.EventHandler(this.editProgramButton_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.descriptionLabel);
			this.groupBox1.Controls.Add(this.companyNameLabel);
			this.groupBox1.Controls.Add(this.programNameLabel);
			this.groupBox1.Controls.Add(this.programIconBox);
			this.groupBox1.Location = new System.Drawing.Point(3, 59);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(296, 62);
			this.groupBox1.TabIndex = 3;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Program Settings";
			// 
			// descriptionLabel
			// 
			this.descriptionLabel.AutoSize = true;
			this.descriptionLabel.Location = new System.Drawing.Point(44, 42);
			this.descriptionLabel.Name = "descriptionLabel";
			this.descriptionLabel.Size = new System.Drawing.Size(60, 13);
			this.descriptionLabel.TabIndex = 3;
			this.descriptionLabel.Text = "Description";
			// 
			// companyNameLabel
			// 
			this.companyNameLabel.AutoSize = true;
			this.companyNameLabel.Location = new System.Drawing.Point(44, 29);
			this.companyNameLabel.Name = "companyNameLabel";
			this.companyNameLabel.Size = new System.Drawing.Size(82, 13);
			this.companyNameLabel.TabIndex = 2;
			this.companyNameLabel.Text = "Company Name";
			// 
			// programNameLabel
			// 
			this.programNameLabel.AutoSize = true;
			this.programNameLabel.Location = new System.Drawing.Point(44, 16);
			this.programNameLabel.Name = "programNameLabel";
			this.programNameLabel.Size = new System.Drawing.Size(77, 13);
			this.programNameLabel.TabIndex = 1;
			this.programNameLabel.Text = "Program Name";
			// 
			// programIconBox
			// 
			this.programIconBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.programIconBox.Location = new System.Drawing.Point(6, 19);
			this.programIconBox.MaximumSize = new System.Drawing.Size(32, 32);
			this.programIconBox.MinimumSize = new System.Drawing.Size(32, 32);
			this.programIconBox.Name = "programIconBox";
			this.programIconBox.Size = new System.Drawing.Size(32, 32);
			this.programIconBox.TabIndex = 0;
			this.programIconBox.TabStop = false;
			// 
			// removeProgramButton
			// 
			this.removeProgramButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.removeProgramButton.Location = new System.Drawing.Point(194, 30);
			this.removeProgramButton.Name = "removeProgramButton";
			this.removeProgramButton.Size = new System.Drawing.Size(101, 23);
			this.removeProgramButton.TabIndex = 2;
			this.removeProgramButton.Text = "Remove Program";
			this.removeProgramButton.UseVisualStyleBackColor = true;
			this.removeProgramButton.Click += new System.EventHandler(this.removeProgramButton_Click);
			// 
			// addProgramButton
			// 
			this.addProgramButton.Location = new System.Drawing.Point(7, 30);
			this.addProgramButton.Name = "addProgramButton";
			this.addProgramButton.Size = new System.Drawing.Size(90, 23);
			this.addProgramButton.TabIndex = 1;
			this.addProgramButton.Text = "Add Program...";
			this.addProgramButton.UseVisualStyleBackColor = true;
			this.addProgramButton.Click += new System.EventHandler(this.addProgramButton_Click);
			// 
			// programComboBox
			// 
			this.programComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.programComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.programComboBox.FormattingEnabled = true;
			this.programComboBox.Location = new System.Drawing.Point(3, 3);
			this.programComboBox.Name = "programComboBox";
			this.programComboBox.Size = new System.Drawing.Size(297, 21);
			this.programComboBox.TabIndex = 0;
			this.programComboBox.SelectedIndexChanged += new System.EventHandler(this.programComboBox_SelectedIndexChanged);
			// 
			// pluginPage
			// 
			this.pluginPage.Controls.Add(this.installPluginButton);
			this.pluginPage.Controls.Add(this.groupBox7);
			this.pluginPage.Controls.Add(this.groupBox6);
			this.pluginPage.Controls.Add(this.label2);
			this.pluginPage.Controls.Add(this.linkLabel1);
			this.pluginPage.Controls.Add(this.removePluginButton);
			this.pluginPage.Location = new System.Drawing.Point(4, 22);
			this.pluginPage.Name = "pluginPage";
			this.pluginPage.Size = new System.Drawing.Size(303, 212);
			this.pluginPage.TabIndex = 2;
			this.pluginPage.Text = "Plugins";
			this.pluginPage.UseVisualStyleBackColor = true;
			// 
			// installPluginButton
			// 
			this.installPluginButton.Location = new System.Drawing.Point(3, 173);
			this.installPluginButton.Name = "installPluginButton";
			this.installPluginButton.Size = new System.Drawing.Size(145, 23);
			this.installPluginButton.TabIndex = 5;
			this.installPluginButton.Text = "Install...";
			this.installPluginButton.UseVisualStyleBackColor = true;
			this.installPluginButton.Click += new System.EventHandler(this.installPluginButton_Click);
			// 
			// groupBox7
			// 
			this.groupBox7.Controls.Add(this.pluginDescLabel);
			this.groupBox7.Controls.Add(this.pluginDetailLabel);
			this.groupBox7.Location = new System.Drawing.Point(3, 109);
			this.groupBox7.Name = "groupBox7";
			this.groupBox7.Size = new System.Drawing.Size(297, 58);
			this.groupBox7.TabIndex = 4;
			this.groupBox7.TabStop = false;
			this.groupBox7.Text = "Plugin Details";
			// 
			// pluginDescLabel
			// 
			this.pluginDescLabel.AutoSize = true;
			this.pluginDescLabel.Location = new System.Drawing.Point(7, 33);
			this.pluginDescLabel.Name = "pluginDescLabel";
			this.pluginDescLabel.Size = new System.Drawing.Size(60, 13);
			this.pluginDescLabel.TabIndex = 1;
			this.pluginDescLabel.Text = "Description";
			// 
			// pluginDetailLabel
			// 
			this.pluginDetailLabel.AutoSize = true;
			this.pluginDetailLabel.Location = new System.Drawing.Point(7, 20);
			this.pluginDetailLabel.Name = "pluginDetailLabel";
			this.pluginDetailLabel.Size = new System.Drawing.Size(38, 13);
			this.pluginDetailLabel.TabIndex = 0;
			this.pluginDetailLabel.Text = "Author";
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.pluginListBox);
			this.groupBox6.Location = new System.Drawing.Point(3, 3);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(297, 100);
			this.groupBox6.TabIndex = 3;
			this.groupBox6.TabStop = false;
			this.groupBox6.Text = "Installed Plugins";
			// 
			// pluginListBox
			// 
			this.pluginListBox.FormattingEnabled = true;
			this.pluginListBox.Location = new System.Drawing.Point(6, 19);
			this.pluginListBox.Name = "pluginListBox";
			this.pluginListBox.Size = new System.Drawing.Size(285, 69);
			this.pluginListBox.TabIndex = 0;
			this.pluginListBox.SelectedValueChanged += new System.EventHandler(this.pluginListBox_SelectedValueChanged);
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 199);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(106, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Add your own plugin!";
			// 
			// linkLabel1
			// 
			this.linkLabel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.linkLabel1.AutoSize = true;
			this.linkLabel1.Location = new System.Drawing.Point(125, 199);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.Size = new System.Drawing.Size(175, 13);
			this.linkLabel1.TabIndex = 1;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "http://www.zarthcode.com/SimpitX";
			// 
			// removePluginButton
			// 
			this.removePluginButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.removePluginButton.Location = new System.Drawing.Point(154, 173);
			this.removePluginButton.Name = "removePluginButton";
			this.removePluginButton.Size = new System.Drawing.Size(146, 23);
			this.removePluginButton.TabIndex = 0;
			this.removePluginButton.Text = "Remove";
			this.removePluginButton.UseVisualStyleBackColor = true;
			this.removePluginButton.Click += new System.EventHandler(this.removePluginButton_Click);
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
			this.updateCheckButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
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
			// debugTab
			// 
			this.debugTab.Controls.Add(this.resetDebugButton);
			this.debugTab.Location = new System.Drawing.Point(4, 22);
			this.debugTab.Name = "debugTab";
			this.debugTab.Size = new System.Drawing.Size(303, 212);
			this.debugTab.TabIndex = 6;
			this.debugTab.Text = "Debug";
			this.debugTab.UseVisualStyleBackColor = true;
			// 
			// resetDebugButton
			// 
			this.resetDebugButton.Location = new System.Drawing.Point(16, 15);
			this.resetDebugButton.Name = "resetDebugButton";
			this.resetDebugButton.Size = new System.Drawing.Size(75, 23);
			this.resetDebugButton.TabIndex = 0;
			this.resetDebugButton.Text = "Reset";
			this.resetDebugButton.UseVisualStyleBackColor = true;
			this.resetDebugButton.Click += new System.EventHandler(this.resetDebugButton_Click);
			// 
			// notifyIcon
			// 
			this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon.Icon")));
			this.notifyIcon.Text = "SimpitX";
			this.notifyIcon.Visible = true;
			this.notifyIcon.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon_MouseDoubleClick);
			// 
			// findProgramDialog
			// 
			this.findProgramDialog.DefaultExt = "exe";
			this.findProgramDialog.Filter = "Executable Files|*.exe";
			this.findProgramDialog.InitialDirectory = "C:\\";
			this.findProgramDialog.ReadOnlyChecked = true;
			this.findProgramDialog.RestoreDirectory = true;
			// 
			// mainDialogBindingSource
			// 
			this.mainDialogBindingSource.DataSource = typeof(Monitor_Application.MainDialog);
			// 
			// MainDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(335, 262);
			this.Controls.Add(this.tabControl);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimumSize = new System.Drawing.Size(351, 300);
			this.Name = "MainDialog";
			this.Text = "SimpitX";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainDialog_FormClosing);
			this.Load += new System.EventHandler(this.MainDialog_Load);
			this.Resize += new System.EventHandler(this.MainDialog_Resize);
			this.tabControl.ResumeLayout(false);
			this.statusPage.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.lastSessionIconBox)).EndInit();
			this.optionsPage.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.genericGroupBox.ResumeLayout(false);
			this.genericGroupBox.PerformLayout();
			this.programsPage.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.programIconBox)).EndInit();
			this.pluginPage.ResumeLayout(false);
			this.pluginPage.PerformLayout();
			this.groupBox7.ResumeLayout(false);
			this.groupBox7.PerformLayout();
			this.groupBox6.ResumeLayout(false);
			this.aboutPage.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.debugTab.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.mainDialogBindingSource)).EndInit();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage statusPage;
        private System.Windows.Forms.TabPage optionsPage;
        private System.Windows.Forms.TabPage programsPage;
        private System.Windows.Forms.TabPage pluginPage;
        private System.Windows.Forms.TabPage helpPage;
        private System.Windows.Forms.TabPage aboutPage;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button removeProgramButton;
        private System.Windows.Forms.Button addProgramButton;
        private System.Windows.Forms.ComboBox programComboBox;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.CheckBox minimizeToTrayBox;
        private System.Windows.Forms.CheckBox runOnStartupCheckBox;
        private System.Windows.Forms.Button removePluginButton;
        private System.Windows.Forms.Button updateCheckButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.NotifyIcon notifyIcon;
        private System.Windows.Forms.GroupBox genericGroupBox;
        private System.Windows.Forms.PictureBox programIconBox;
        private System.Windows.Forms.CheckedListBox enabledPluginListBox;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ListBox logListBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.PictureBox lastSessionIconBox;
        private System.Windows.Forms.Label lastSessionName;
        private System.Windows.Forms.Label lastSessionErrors;
        private System.Windows.Forms.Label lastSessionTime;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.OpenFileDialog findProgramDialog;
		private System.Windows.Forms.Button editProgramButton;
		private System.Windows.Forms.TabPage debugTab;
		private System.Windows.Forms.Button resetDebugButton;
		private System.Windows.Forms.BindingSource mainDialogBindingSource;
		private System.Windows.Forms.CheckBox closeToTrayCheckBox;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.CheckBox alwaysShowTrayIconCheckBox;
		private System.Windows.Forms.CheckBox showBalloonNotificationsInTrayCheckBox;
		private System.Windows.Forms.CheckBox startMinimizedCheckBox;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.Label programNameLabel;
		private System.Windows.Forms.Label descriptionLabel;
		private System.Windows.Forms.Label companyNameLabel;
		private System.Windows.Forms.Button installPluginButton;
		private System.Windows.Forms.GroupBox groupBox7;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.ListBox pluginListBox;
		private System.Windows.Forms.Label pluginDescLabel;
		private System.Windows.Forms.Label pluginDetailLabel;
    }
}

