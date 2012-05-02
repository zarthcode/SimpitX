namespace Monitor_Application
{
    partial class EditConfigurationDialog
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
			this.programIcon = new System.Windows.Forms.PictureBox();
			this.label1 = new System.Windows.Forms.Label();
			this.programNameTextBox = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.descriptionTextBox = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.commentTextBox = new System.Windows.Forms.TextBox();
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.executablePathTextBox = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.companyLabel = new System.Windows.Forms.Label();
			this.companyTextBox = new System.Windows.Forms.TextBox();
			this.errorProvider = new System.Windows.Forms.ErrorProvider(this.components);
			((System.ComponentModel.ISupportInitialize)(this.programIcon)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.errorProvider)).BeginInit();
			this.SuspendLayout();
			// 
			// programIcon
			// 
			this.programIcon.Location = new System.Drawing.Point(12, 12);
			this.programIcon.Name = "programIcon";
			this.programIcon.Size = new System.Drawing.Size(32, 32);
			this.programIcon.TabIndex = 0;
			this.programIcon.TabStop = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(50, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(77, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Program Name";
			// 
			// programNameTextBox
			// 
			this.programNameTextBox.Location = new System.Drawing.Point(50, 25);
			this.programNameTextBox.Name = "programNameTextBox";
			this.programNameTextBox.Size = new System.Drawing.Size(266, 20);
			this.programNameTextBox.TabIndex = 2;
			this.programNameTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.programNameTextBox_Validating);
			this.programNameTextBox.Validated += new System.EventHandler(this.programNameTextBox_Validated);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 47);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(60, 13);
			this.label2.TabIndex = 3;
			this.label2.Text = "Description";
			// 
			// descriptionTextBox
			// 
			this.descriptionTextBox.Location = new System.Drawing.Point(12, 67);
			this.descriptionTextBox.Name = "descriptionTextBox";
			this.descriptionTextBox.Size = new System.Drawing.Size(304, 20);
			this.descriptionTextBox.TabIndex = 4;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 182);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(84, 13);
			this.label3.TabIndex = 5;
			this.label3.Text = "Comment/Notes";
			// 
			// commentTextBox
			// 
			this.commentTextBox.Location = new System.Drawing.Point(12, 198);
			this.commentTextBox.Name = "commentTextBox";
			this.commentTextBox.Size = new System.Drawing.Size(304, 20);
			this.commentTextBox.TabIndex = 6;
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(160, 235);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(75, 23);
			this.okButton.TabIndex = 10;
			this.okButton.Text = "OK";
			this.okButton.UseVisualStyleBackColor = true;
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(241, 235);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(75, 23);
			this.cancelButton.TabIndex = 11;
			this.cancelButton.Text = "Cancel";
			this.cancelButton.UseVisualStyleBackColor = true;
			// 
			// executablePathTextBox
			// 
			this.executablePathTextBox.Location = new System.Drawing.Point(12, 159);
			this.executablePathTextBox.Name = "executablePathTextBox";
			this.executablePathTextBox.ReadOnly = true;
			this.executablePathTextBox.Size = new System.Drawing.Size(304, 20);
			this.executablePathTextBox.TabIndex = 12;
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(12, 143);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(148, 13);
			this.label7.TabIndex = 13;
			this.label7.Text = "Executable Path (Main Game)";
			// 
			// companyLabel
			// 
			this.companyLabel.AutoSize = true;
			this.companyLabel.Location = new System.Drawing.Point(12, 90);
			this.companyLabel.Name = "companyLabel";
			this.companyLabel.Size = new System.Drawing.Size(51, 13);
			this.companyLabel.TabIndex = 14;
			this.companyLabel.Text = "Company";
			// 
			// companyTextBox
			// 
			this.companyTextBox.Location = new System.Drawing.Point(12, 106);
			this.companyTextBox.Name = "companyTextBox";
			this.companyTextBox.ReadOnly = true;
			this.companyTextBox.Size = new System.Drawing.Size(304, 20);
			this.companyTextBox.TabIndex = 15;
			// 
			// errorProvider
			// 
			this.errorProvider.ContainerControl = this;
			// 
			// EditConfigurationDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(328, 270);
			this.Controls.Add(this.companyTextBox);
			this.Controls.Add(this.companyLabel);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.executablePathTextBox);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.commentTextBox);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.descriptionTextBox);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.programNameTextBox);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.programIcon);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EditConfigurationDialog";
			this.ShowIcon = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Edit Program";
			((System.ComponentModel.ISupportInitialize)(this.programIcon)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.errorProvider)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

		private System.Windows.Forms.PictureBox programIcon;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox programNameTextBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox descriptionTextBox;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox commentTextBox;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.TextBox executablePathTextBox;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label companyLabel;
		private System.Windows.Forms.TextBox companyTextBox;
		private System.Windows.Forms.ErrorProvider errorProvider;
    }
}