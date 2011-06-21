namespace GpsVisualizer
{
	partial class MainWindow
	{
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
            this.mainMenu = new System.Windows.Forms.MenuStrip();
            this.FileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.OpenVirtualLineMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveVirtualLineMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ExportVirtualLine = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CursorPosStatLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusBar = new System.Windows.Forms.StatusStrip();
            this.playTimeStatLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.PlayStatLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.mainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.infoSplitContainer = new System.Windows.Forms.SplitContainer();
            this.infoList = new System.Windows.Forms.ListView();
            this.logLabel = new System.Windows.Forms.Label();
            this.ControlPointValPanel = new System.Windows.Forms.Panel();
            this.controlPointProperty = new System.Windows.Forms.PropertyGrid();
            this.controlPointLabel = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.guidanceGroup = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.OperationPanel = new System.Windows.Forms.Panel();
            this.seekPanel = new System.Windows.Forms.Panel();
            this.seekBar = new System.Windows.Forms.TrackBar();
            this.buttonsPanel = new System.Windows.Forms.Panel();
            this.hrLineLabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.courseView = new GpsVisualizer.CourseView();
            this.mainMenu.SuspendLayout();
            this.statusBar.SuspendLayout();
            this.mainPanel.SuspendLayout();
            this.mainSplitContainer.Panel1.SuspendLayout();
            this.mainSplitContainer.Panel2.SuspendLayout();
            this.mainSplitContainer.SuspendLayout();
            this.infoSplitContainer.Panel1.SuspendLayout();
            this.infoSplitContainer.Panel2.SuspendLayout();
            this.infoSplitContainer.SuspendLayout();
            this.ControlPointValPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.guidanceGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.OperationPanel.SuspendLayout();
            this.seekPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.seekBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.SuspendLayout();
            // 
            // mainMenu
            // 
            this.mainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenuItem});
            this.mainMenu.Location = new System.Drawing.Point(0, 0);
            this.mainMenu.Name = "mainMenu";
            this.mainMenu.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.mainMenu.Size = new System.Drawing.Size(839, 24);
            this.mainMenu.TabIndex = 7;
            this.mainMenu.Text = "menuStrip1";
            // 
            // FileMenuItem
            // 
            this.FileMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.OpenMenuItem,
            this.saveMenuItem,
            this.toolStripMenuItem2,
            this.OpenVirtualLineMenuItem,
            this.SaveVirtualLineMenuItem,
            this.ExportVirtualLine,
            this.toolStripMenuItem1,
            this.exitMenuItem});
            this.FileMenuItem.Name = "FileMenuItem";
            this.FileMenuItem.Size = new System.Drawing.Size(66, 20);
            this.FileMenuItem.Text = "ファイル(&F)";
            // 
            // OpenMenuItem
            // 
            this.OpenMenuItem.Name = "OpenMenuItem";
            this.OpenMenuItem.Size = new System.Drawing.Size(229, 22);
            this.OpenMenuItem.Text = "走行ログを開く(&O)";
            this.OpenMenuItem.Click += new System.EventHandler(this.OpenMenuItem_Click);
            // 
            // saveMenuItem
            // 
            this.saveMenuItem.Name = "saveMenuItem";
            this.saveMenuItem.Size = new System.Drawing.Size(229, 22);
            this.saveMenuItem.Text = "走行ログを可視化して保存(&V)";
            this.saveMenuItem.Click += new System.EventHandler(this.saveMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(226, 6);
            // 
            // OpenVirtualLineMenuItem
            // 
            this.OpenVirtualLineMenuItem.Name = "OpenVirtualLineMenuItem";
            this.OpenVirtualLineMenuItem.Size = new System.Drawing.Size(229, 22);
            this.OpenVirtualLineMenuItem.Text = "仮想ラインを開く(&I)";
            this.OpenVirtualLineMenuItem.Click += new System.EventHandler(this.OpenVirtualLineMenuItem_Click);
            // 
            // SaveVirtualLineMenuItem
            // 
            this.SaveVirtualLineMenuItem.Name = "SaveVirtualLineMenuItem";
            this.SaveVirtualLineMenuItem.Size = new System.Drawing.Size(229, 22);
            this.SaveVirtualLineMenuItem.Text = "仮想ラインを保存(&S)";
            this.SaveVirtualLineMenuItem.Click += new System.EventHandler(this.SaveVirtualLineMenuItem_Click);
            // 
            // ExportVirtualLine
            // 
            this.ExportVirtualLine.Name = "ExportVirtualLine";
            this.ExportVirtualLine.Size = new System.Drawing.Size(229, 22);
            this.ExportVirtualLine.Text = "仮想ラインをnxtにエクスポート(&E)...";
            this.ExportVirtualLine.Click += new System.EventHandler(this.ExportVirtualLine_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(226, 6);
            // 
            // exitMenuItem
            // 
            this.exitMenuItem.Name = "exitMenuItem";
            this.exitMenuItem.Size = new System.Drawing.Size(229, 22);
            this.exitMenuItem.Text = "終了(&E)";
            this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
            // 
            // CursorPosStatLabel
            // 
            this.CursorPosStatLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.CursorPosStatLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.CursorPosStatLabel.Name = "CursorPosStatLabel";
            this.CursorPosStatLabel.Size = new System.Drawing.Size(736, 17);
            this.CursorPosStatLabel.Spring = true;
            this.CursorPosStatLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // statusBar
            // 
            this.statusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CursorPosStatLabel,
            this.playTimeStatLabel,
            this.PlayStatLabel});
            this.statusBar.Location = new System.Drawing.Point(0, 579);
            this.statusBar.Name = "statusBar";
            this.statusBar.Size = new System.Drawing.Size(839, 22);
            this.statusBar.TabIndex = 13;
            // 
            // playTimeStatLabel
            // 
            this.playTimeStatLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.playTimeStatLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.playTimeStatLabel.Name = "playTimeStatLabel";
            this.playTimeStatLabel.Size = new System.Drawing.Size(55, 17);
            this.playTimeStatLabel.Text = "hh:mm:ss";
            // 
            // PlayStatLabel
            // 
            this.PlayStatLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.PlayStatLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.PlayStatLabel.Name = "PlayStatLabel";
            this.PlayStatLabel.Size = new System.Drawing.Size(33, 17);
            this.PlayStatLabel.Text = "停止";
            // 
            // mainPanel
            // 
            this.mainPanel.Controls.Add(this.mainSplitContainer);
            this.mainPanel.Controls.Add(this.OperationPanel);
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(0, 24);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(839, 555);
            this.mainPanel.TabIndex = 14;
            // 
            // mainSplitContainer
            // 
            this.mainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.mainSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.mainSplitContainer.Name = "mainSplitContainer";
            // 
            // mainSplitContainer.Panel1
            // 
            this.mainSplitContainer.Panel1.Controls.Add(this.courseView);
            // 
            // mainSplitContainer.Panel2
            // 
            this.mainSplitContainer.Panel2.Controls.Add(this.infoSplitContainer);
            this.mainSplitContainer.Panel2MinSize = 128;
            this.mainSplitContainer.Size = new System.Drawing.Size(839, 519);
            this.mainSplitContainer.SplitterDistance = 627;
            this.mainSplitContainer.TabIndex = 11;
            // 
            // infoSplitContainer
            // 
            this.infoSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoSplitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.infoSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.infoSplitContainer.Name = "infoSplitContainer";
            this.infoSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // infoSplitContainer.Panel1
            // 
            this.infoSplitContainer.Panel1.Controls.Add(this.infoList);
            this.infoSplitContainer.Panel1.Controls.Add(this.logLabel);
            // 
            // infoSplitContainer.Panel2
            // 
            this.infoSplitContainer.Panel2.Controls.Add(this.ControlPointValPanel);
            this.infoSplitContainer.Panel2.Controls.Add(this.controlPointLabel);
            this.infoSplitContainer.Panel2.Controls.Add(this.panel1);
            this.infoSplitContainer.Size = new System.Drawing.Size(208, 519);
            this.infoSplitContainer.SplitterDistance = 112;
            this.infoSplitContainer.TabIndex = 0;
            // 
            // infoList
            // 
            this.infoList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoList.Location = new System.Drawing.Point(0, 23);
            this.infoList.Name = "infoList";
            this.infoList.Size = new System.Drawing.Size(208, 89);
            this.infoList.TabIndex = 6;
            this.infoList.UseCompatibleStateImageBehavior = false;
            // 
            // logLabel
            // 
            this.logLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this.logLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.logLabel.Location = new System.Drawing.Point(0, 0);
            this.logLabel.Name = "logLabel";
            this.logLabel.Size = new System.Drawing.Size(208, 23);
            this.logLabel.TabIndex = 5;
            this.logLabel.Text = "ログ取得値(&L)：";
            this.logLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ControlPointValPanel
            // 
            this.ControlPointValPanel.AutoScroll = true;
            this.ControlPointValPanel.Controls.Add(this.controlPointProperty);
            this.ControlPointValPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ControlPointValPanel.Location = new System.Drawing.Point(0, 23);
            this.ControlPointValPanel.Name = "ControlPointValPanel";
            this.ControlPointValPanel.Size = new System.Drawing.Size(208, 326);
            this.ControlPointValPanel.TabIndex = 3;
            // 
            // controlPointProperty
            // 
            this.controlPointProperty.Dock = System.Windows.Forms.DockStyle.Fill;
            this.controlPointProperty.Location = new System.Drawing.Point(0, 0);
            this.controlPointProperty.Name = "controlPointProperty";
            this.controlPointProperty.Size = new System.Drawing.Size(208, 326);
            this.controlPointProperty.TabIndex = 30;
            this.controlPointProperty.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.controlPointProperty_PropertyValueChanged);
            // 
            // controlPointLabel
            // 
            this.controlPointLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this.controlPointLabel.Location = new System.Drawing.Point(0, 0);
            this.controlPointLabel.Name = "controlPointLabel";
            this.controlPointLabel.Size = new System.Drawing.Size(208, 23);
            this.controlPointLabel.TabIndex = 2;
            this.controlPointLabel.Text = "制御点(&P)：";
            this.controlPointLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.guidanceGroup);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 349);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(208, 54);
            this.panel1.TabIndex = 31;
            // 
            // guidanceGroup
            // 
            this.guidanceGroup.Controls.Add(this.label2);
            this.guidanceGroup.Controls.Add(this.label1);
            this.guidanceGroup.Controls.Add(this.pictureBox2);
            this.guidanceGroup.Controls.Add(this.pictureBox1);
            this.guidanceGroup.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.guidanceGroup.Location = new System.Drawing.Point(0, 6);
            this.guidanceGroup.Name = "guidanceGroup";
            this.guidanceGroup.Size = new System.Drawing.Size(208, 48);
            this.guidanceGroup.TabIndex = 29;
            this.guidanceGroup.TabStop = false;
            this.guidanceGroup.Text = "制御点の記号";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(121, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "最後尾";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(34, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 11;
            this.label1.Text = "先頭";
            // 
            // pictureBox2
            // 
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Image = global::GpsVisualizer.Properties.Resources.finishPoint;
            this.pictureBox2.Location = new System.Drawing.Point(93, 18);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(22, 22);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox2.TabIndex = 10;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.Image = global::GpsVisualizer.Properties.Resources.startPoint;
            this.pictureBox1.Location = new System.Drawing.Point(6, 18);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(22, 22);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 9;
            this.pictureBox1.TabStop = false;
            // 
            // OperationPanel
            // 
            this.OperationPanel.Controls.Add(this.seekPanel);
            this.OperationPanel.Controls.Add(this.buttonsPanel);
            this.OperationPanel.Controls.Add(this.hrLineLabel);
            this.OperationPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.OperationPanel.Location = new System.Drawing.Point(0, 519);
            this.OperationPanel.Name = "OperationPanel";
            this.OperationPanel.Size = new System.Drawing.Size(839, 36);
            this.OperationPanel.TabIndex = 10;
            // 
            // seekPanel
            // 
            this.seekPanel.Controls.Add(this.seekBar);
            this.seekPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.seekPanel.Enabled = false;
            this.seekPanel.Location = new System.Drawing.Point(0, 2);
            this.seekPanel.Name = "seekPanel";
            this.seekPanel.Padding = new System.Windows.Forms.Padding(2);
            this.seekPanel.Size = new System.Drawing.Size(826, 34);
            this.seekPanel.TabIndex = 7;
            // 
            // seekBar
            // 
            this.seekBar.AutoSize = false;
            this.seekBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.seekBar.Location = new System.Drawing.Point(2, 2);
            this.seekBar.Maximum = 0;
            this.seekBar.Name = "seekBar";
            this.seekBar.Size = new System.Drawing.Size(822, 30);
            this.seekBar.TabIndex = 3;
            this.seekBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.seekBar.Scroll += new System.EventHandler(this.seekBar_Scroll);
            // 
            // buttonsPanel
            // 
            this.buttonsPanel.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonsPanel.Location = new System.Drawing.Point(826, 2);
            this.buttonsPanel.Name = "buttonsPanel";
            this.buttonsPanel.Size = new System.Drawing.Size(13, 34);
            this.buttonsPanel.TabIndex = 6;
            // 
            // hrLineLabel
            // 
            this.hrLineLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.hrLineLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this.hrLineLabel.Location = new System.Drawing.Point(0, 0);
            this.hrLineLabel.Name = "hrLineLabel";
            this.hrLineLabel.Size = new System.Drawing.Size(839, 2);
            this.hrLineLabel.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(121, 28);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 12;
            this.label3.Text = "最後尾";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(34, 28);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 12);
            this.label4.TabIndex = 11;
            this.label4.Text = "先頭";
            // 
            // pictureBox3
            // 
            this.pictureBox3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox3.Image = global::GpsVisualizer.Properties.Resources.finishPoint;
            this.pictureBox3.Location = new System.Drawing.Point(93, 18);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(22, 22);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox3.TabIndex = 10;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox4.Image = global::GpsVisualizer.Properties.Resources.startPoint;
            this.pictureBox4.Location = new System.Drawing.Point(6, 18);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(22, 22);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox4.TabIndex = 9;
            this.pictureBox4.TabStop = false;
            // 
            // courseView
            // 
            this.courseView.AutoScroll = true;
            this.courseView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.courseView.Location = new System.Drawing.Point(0, 0);
            this.courseView.Name = "courseView";
            this.courseView.Size = new System.Drawing.Size(627, 519);
            this.courseView.TabIndex = 0;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(839, 601);
            this.Controls.Add(this.mainPanel);
            this.Controls.Add(this.mainMenu);
            this.Controls.Add(this.statusBar);
            this.MainMenuStrip = this.mainMenu;
            this.Name = "MainWindow";
            this.Text = "GPS Visualizer";
            this.mainMenu.ResumeLayout(false);
            this.mainMenu.PerformLayout();
            this.statusBar.ResumeLayout(false);
            this.statusBar.PerformLayout();
            this.mainPanel.ResumeLayout(false);
            this.mainSplitContainer.Panel1.ResumeLayout(false);
            this.mainSplitContainer.Panel2.ResumeLayout(false);
            this.mainSplitContainer.ResumeLayout(false);
            this.infoSplitContainer.Panel1.ResumeLayout(false);
            this.infoSplitContainer.Panel2.ResumeLayout(false);
            this.infoSplitContainer.ResumeLayout(false);
            this.ControlPointValPanel.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.guidanceGroup.ResumeLayout(false);
            this.guidanceGroup.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.OperationPanel.ResumeLayout(false);
            this.seekPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.seekBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip mainMenu;
		private System.Windows.Forms.ToolStripMenuItem FileMenuItem;
		private System.Windows.Forms.ToolStripMenuItem OpenMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem exitMenuItem;
		private System.Windows.Forms.ToolStripStatusLabel CursorPosStatLabel;
		private System.Windows.Forms.StatusStrip statusBar;
		private System.Windows.Forms.Panel mainPanel;
		private System.Windows.Forms.SplitContainer mainSplitContainer;
		private CourseView courseView;
		private System.Windows.Forms.Panel OperationPanel;
        private System.Windows.Forms.Panel seekPanel;
		private System.Windows.Forms.Label hrLineLabel;
		private System.Windows.Forms.ToolStripStatusLabel playTimeStatLabel;
		private System.Windows.Forms.ToolStripStatusLabel PlayStatLabel;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem saveMenuItem;
        private System.Windows.Forms.TrackBar seekBar;
		private System.Windows.Forms.ToolStripMenuItem OpenVirtualLineMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveVirtualLineMenuItem;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
		private System.Windows.Forms.SplitContainer infoSplitContainer;
        private System.Windows.Forms.Panel buttonsPanel;
		private System.Windows.Forms.ToolStripMenuItem ExportVirtualLine;
		private System.Windows.Forms.ListView infoList;
		private System.Windows.Forms.Label logLabel;
		private System.Windows.Forms.Panel ControlPointValPanel;
        private System.Windows.Forms.Label controlPointLabel;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.GroupBox guidanceGroup;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.PictureBox pictureBox2;
		private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PropertyGrid controlPointProperty;

	}
}

