namespace GpsVisualizer
{
	partial class CourseView
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

		#region コンポーネント デザイナーで生成されたコード

		/// <summary> 
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.mapMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addControlPointMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.controlPointMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addControlPointBeforeMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.addControlPointAfterMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.removeControlPointMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.containerPanel = new System.Windows.Forms.Panel();
            this.coursePicture = new System.Windows.Forms.PictureBox();
            this.removeAllControlPointMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.mapMenu.SuspendLayout();
            this.controlPointMenu.SuspendLayout();
            this.containerPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.coursePicture)).BeginInit();
            this.SuspendLayout();
            // 
            // mapMenu
            // 
            this.mapMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addControlPointMenu,
            this.toolStripMenuItem1,
            this.removeAllControlPointMenu});
            this.mapMenu.Name = "contextMenuStrip1";
            this.mapMenu.Size = new System.Drawing.Size(181, 76);
            // 
            // addControlPointMenu
            // 
            this.addControlPointMenu.Name = "addControlPointMenu";
            this.addControlPointMenu.Size = new System.Drawing.Size(180, 22);
            this.addControlPointMenu.Text = "制御点をここに作成(&A)";
            this.addControlPointMenu.Click += new System.EventHandler(this.addControlPointMenu_Click);
            // 
            // controlPointMenu
            // 
            this.controlPointMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addControlPointBeforeMenu,
            this.addControlPointAfterMenu,
            this.toolStripSeparator1,
            this.removeControlPointMenu});
            this.controlPointMenu.Name = "controlPointMenu";
            this.controlPointMenu.Size = new System.Drawing.Size(186, 76);
            // 
            // addControlPointBeforeMenu
            // 
            this.addControlPointBeforeMenu.Name = "addControlPointBeforeMenu";
            this.addControlPointBeforeMenu.Size = new System.Drawing.Size(185, 22);
            this.addControlPointBeforeMenu.Text = "前に制御点を挿入(&B)";
            this.addControlPointBeforeMenu.Click += new System.EventHandler(this.addControlPointBeforeMenu_Click);
            // 
            // addControlPointAfterMenu
            // 
            this.addControlPointAfterMenu.Name = "addControlPointAfterMenu";
            this.addControlPointAfterMenu.Size = new System.Drawing.Size(185, 22);
            this.addControlPointAfterMenu.Text = "後ろに制御点を挿入(&A)";
            this.addControlPointAfterMenu.Click += new System.EventHandler(this.addControlPointAfterMenu_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(182, 6);
            // 
            // removeControlPointMenu
            // 
            this.removeControlPointMenu.Name = "removeControlPointMenu";
            this.removeControlPointMenu.Size = new System.Drawing.Size(185, 22);
            this.removeControlPointMenu.Text = "制御点を削除(&R)";
            this.removeControlPointMenu.Click += new System.EventHandler(this.removeControlPointMenu_Click);
            // 
            // containerPanel
            // 
            this.containerPanel.AutoSize = true;
            this.containerPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.containerPanel.Controls.Add(this.coursePicture);
            this.containerPanel.Location = new System.Drawing.Point(0, 0);
            this.containerPanel.Name = "containerPanel";
            this.containerPanel.Size = new System.Drawing.Size(60, 76);
            this.containerPanel.TabIndex = 2;
            // 
            // coursePicture
            // 
            this.coursePicture.ContextMenuStrip = this.mapMenu;
            this.coursePicture.Location = new System.Drawing.Point(1, 1);
            this.coursePicture.Name = "coursePicture";
            this.coursePicture.Size = new System.Drawing.Size(56, 72);
            this.coursePicture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.coursePicture.TabIndex = 1;
            this.coursePicture.TabStop = false;
            this.coursePicture.MouseMove += new System.Windows.Forms.MouseEventHandler(this.coursePicture_MouseMove);
            this.coursePicture.MouseDown += new System.Windows.Forms.MouseEventHandler(this.coursePicture_MouseDown);
            this.coursePicture.MouseUp += new System.Windows.Forms.MouseEventHandler(this.coursePicture_MouseUp);
            // 
            // removeAllControlPointMenu
            // 
            this.removeAllControlPointMenu.Name = "removeAllControlPointMenu";
            this.removeAllControlPointMenu.Size = new System.Drawing.Size(180, 22);
            this.removeAllControlPointMenu.Text = "制御点を全て削除(&C)";
            this.removeAllControlPointMenu.Click += new System.EventHandler(this.removeAllControlPointMenu_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(177, 6);
            // 
            // CourseView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.Controls.Add(this.containerPanel);
            this.Name = "CourseView";
            this.mapMenu.ResumeLayout(false);
            this.controlPointMenu.ResumeLayout(false);
            this.containerPanel.ResumeLayout(false);
            this.containerPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.coursePicture)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ContextMenuStrip mapMenu;
		private System.Windows.Forms.ToolStripMenuItem addControlPointMenu;
		private System.Windows.Forms.ContextMenuStrip controlPointMenu;
		private System.Windows.Forms.ToolStripMenuItem addControlPointBeforeMenu;
		private System.Windows.Forms.ToolStripMenuItem addControlPointAfterMenu;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem removeControlPointMenu;
		private System.Windows.Forms.Panel containerPanel;
		private System.Windows.Forms.PictureBox coursePicture;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem removeAllControlPointMenu;
	}
}
