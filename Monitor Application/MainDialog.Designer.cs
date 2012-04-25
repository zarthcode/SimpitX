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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainDialog));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.statusPage = new System.Windows.Forms.TabPage();
            this.optionsPage = new System.Windows.Forms.TabPage();
            this.gamesPage = new System.Windows.Forms.TabPage();
            this.pluginPage = new System.Windows.Forms.TabPage();
            this.helpPage = new System.Windows.Forms.TabPage();
            this.aboutPage = new System.Windows.Forms.TabPage();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.startupBox = new System.Windows.Forms.CheckBox();
            this.minimizeToTrayBox = new System.Windows.Forms.CheckBox();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.richTextBox2 = new System.Windows.Forms.RichTextBox();
            this.updateCheckButton = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.optionsPage.SuspendLayout();
            this.gamesPage.SuspendLayout();
            this.pluginPage.SuspendLayout();
            this.aboutPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
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
            this.statusPage.Location = new System.Drawing.Point(4, 22);
            this.statusPage.Name = "statusPage";
            this.statusPage.Padding = new System.Windows.Forms.Padding(3);
            this.statusPage.Size = new System.Drawing.Size(303, 212);
            this.statusPage.TabIndex = 0;
            this.statusPage.Text = "Status";
            this.statusPage.UseVisualStyleBackColor = true;
            // 
            // optionsPage
            // 
            this.optionsPage.Controls.Add(this.richTextBox2);
            this.optionsPage.Controls.Add(this.label1);
            this.optionsPage.Controls.Add(this.numericUpDown1);
            this.optionsPage.Controls.Add(this.minimizeToTrayBox);
            this.optionsPage.Controls.Add(this.startupBox);
            this.optionsPage.Location = new System.Drawing.Point(4, 22);
            this.optionsPage.Name = "optionsPage";
            this.optionsPage.Padding = new System.Windows.Forms.Padding(3);
            this.optionsPage.Size = new System.Drawing.Size(303, 212);
            this.optionsPage.TabIndex = 1;
            this.optionsPage.Text = "Options";
            this.optionsPage.UseVisualStyleBackColor = true;
            // 
            // gamesPage
            // 
            this.gamesPage.Controls.Add(this.groupBox1);
            this.gamesPage.Controls.Add(this.button2);
            this.gamesPage.Controls.Add(this.button1);
            this.gamesPage.Controls.Add(this.comboBox1);
            this.gamesPage.Location = new System.Drawing.Point(4, 22);
            this.gamesPage.Name = "gamesPage";
            this.gamesPage.Size = new System.Drawing.Size(303, 212);
            this.gamesPage.TabIndex = 5;
            this.gamesPage.Text = "Games";
            this.gamesPage.UseVisualStyleBackColor = true;
            // 
            // pluginPage
            // 
            this.pluginPage.Controls.Add(this.button3);
            this.pluginPage.Location = new System.Drawing.Point(4, 22);
            this.pluginPage.Name = "pluginPage";
            this.pluginPage.Size = new System.Drawing.Size(303, 212);
            this.pluginPage.TabIndex = 2;
            this.pluginPage.Text = "Plugins";
            this.pluginPage.UseVisualStyleBackColor = true;
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
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(3, 3);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(297, 21);
            this.comboBox1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(7, 30);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(141, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Add New Game...";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(154, 30);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(141, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Remove Game";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new System.Drawing.Point(3, 59);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(296, 152);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Game Settings";
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
            // startupBox
            // 
            this.startupBox.AutoSize = true;
            this.startupBox.Location = new System.Drawing.Point(199, 6);
            this.startupBox.Name = "startupBox";
            this.startupBox.Size = new System.Drawing.Size(93, 17);
            this.startupBox.TabIndex = 0;
            this.startupBox.Text = "Run at startup";
            this.startupBox.UseVisualStyleBackColor = true;
            // 
            // minimizeToTrayBox
            // 
            this.minimizeToTrayBox.AutoSize = true;
            this.minimizeToTrayBox.Location = new System.Drawing.Point(199, 29);
            this.minimizeToTrayBox.Name = "minimizeToTrayBox";
            this.minimizeToTrayBox.Size = new System.Drawing.Size(98, 17);
            this.minimizeToTrayBox.TabIndex = 1;
            this.minimizeToTrayBox.Text = "Minimize to tray";
            this.minimizeToTrayBox.UseVisualStyleBackColor = true;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(172, 52);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(120, 20);
            this.numericUpDown1.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(102, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Polling Rate";
            // 
            // richTextBox2
            // 
            this.richTextBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox2.Location = new System.Drawing.Point(105, 78);
            this.richTextBox2.Name = "richTextBox2";
            this.richTextBox2.ReadOnly = true;
            this.richTextBox2.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.richTextBox2.Size = new System.Drawing.Size(178, 44);
            this.richTextBox2.TabIndex = 4;
            this.richTextBox2.Text = "You may want to reduce the polling rate if there is too much delay between game s" +
    "tartup and injection.";
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
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(3, 186);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(297, 23);
            this.button3.TabIndex = 0;
            this.button3.Text = "Get New Plugins...";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // MainDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(335, 262);
            this.Controls.Add(this.tabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainDialog";
            this.Text = "SimpitX";
            this.tabControl1.ResumeLayout(false);
            this.optionsPage.ResumeLayout(false);
            this.optionsPage.PerformLayout();
            this.gamesPage.ResumeLayout(false);
            this.pluginPage.ResumeLayout(false);
            this.aboutPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
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
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.RichTextBox richTextBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.CheckBox minimizeToTrayBox;
        private System.Windows.Forms.CheckBox startupBox;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button updateCheckButton;
    }
}

