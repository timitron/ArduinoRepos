<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Dim ChartArea4 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend2 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim ChartArea5 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Series3 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim ChartArea6 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Series4 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Me.groupboxRobot = New System.Windows.Forms.GroupBox()
        Me.txtSerialText = New System.Windows.Forms.TextBox()
        Me.Label_CCecho = New System.Windows.Forms.Label()
        Me.picDataReceived = New System.Windows.Forms.PictureBox()
        Me.btnComClose = New System.Windows.Forms.Button()
        Me.btnComOpen = New System.Windows.Forms.Button()
        Me.Label_COMasigns = New System.Windows.Forms.Label()
        Me.cboBaudRate = New System.Windows.Forms.ComboBox()
        Me.Label_Baud = New System.Windows.Forms.Label()
        Me.Label_TX = New System.Windows.Forms.Label()
        Me.Label_Rx = New System.Windows.Forms.Label()
        Me.btnSend = New System.Windows.Forms.Button()
        Me.tbTx = New System.Windows.Forms.TextBox()
        Me.Label_port = New System.Windows.Forms.Label()
        Me.picOpen = New System.Windows.Forms.PictureBox()
        Me.cboComPort = New System.Windows.Forms.ComboBox()
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.buttonFish = New System.Windows.Forms.Button()
        Me.buttonManualMove = New System.Windows.Forms.Button()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.numHeight = New System.Windows.Forms.NumericUpDown()
        Me.numHeadAngle = New System.Windows.Forms.NumericUpDown()
        Me.numXstep = New System.Windows.Forms.NumericUpDown()
        Me.buttonCompetition = New System.Windows.Forms.Button()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.numArmRad = New System.Windows.Forms.NumericUpDown()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.TabControl1 = New System.Windows.Forms.TabControl()
        Me.tabBoardDefault = New System.Windows.Forms.TabPage()
        Me.chartBoard = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.tabRotatedboard = New System.Windows.Forms.TabPage()
        Me.chartRot = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.tabRelativetoBot = New System.Windows.Forms.TabPage()
        Me.chartRel = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.numHeadAngleZero = New System.Windows.Forms.NumericUpDown()
        Me.numDistancetoCenter = New System.Windows.Forms.NumericUpDown()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.numBoardAngle = New System.Windows.Forms.NumericUpDown()
        Me.lstRawBitePos = New System.Windows.Forms.ListView()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.lstrobobitepos = New System.Windows.Forms.ListView()
        Me.txtCode = New System.Windows.Forms.TextBox()
        Me.groupboxRobot.SuspendLayout()
        CType(Me.picDataReceived, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.picOpen, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBox1.SuspendLayout()
        CType(Me.numHeight, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.numHeadAngle, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.numXstep, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBox2.SuspendLayout()
        CType(Me.numArmRad, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.TabControl1.SuspendLayout()
        Me.tabBoardDefault.SuspendLayout()
        CType(Me.chartBoard, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.tabRotatedboard.SuspendLayout()
        CType(Me.chartRot, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.tabRelativetoBot.SuspendLayout()
        CType(Me.chartRel, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.numHeadAngleZero, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.numDistancetoCenter, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.numBoardAngle, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'groupboxRobot
        '
        Me.groupboxRobot.Controls.Add(Me.txtSerialText)
        Me.groupboxRobot.Controls.Add(Me.Label_CCecho)
        Me.groupboxRobot.Controls.Add(Me.picDataReceived)
        Me.groupboxRobot.Controls.Add(Me.btnComClose)
        Me.groupboxRobot.Controls.Add(Me.btnComOpen)
        Me.groupboxRobot.Controls.Add(Me.Label_COMasigns)
        Me.groupboxRobot.Controls.Add(Me.cboBaudRate)
        Me.groupboxRobot.Controls.Add(Me.Label_Baud)
        Me.groupboxRobot.Controls.Add(Me.Label_TX)
        Me.groupboxRobot.Controls.Add(Me.Label_Rx)
        Me.groupboxRobot.Controls.Add(Me.btnSend)
        Me.groupboxRobot.Controls.Add(Me.tbTx)
        Me.groupboxRobot.Controls.Add(Me.Label_port)
        Me.groupboxRobot.Controls.Add(Me.picOpen)
        Me.groupboxRobot.Controls.Add(Me.cboComPort)
        Me.groupboxRobot.Location = New System.Drawing.Point(37, 31)
        Me.groupboxRobot.Name = "groupboxRobot"
        Me.groupboxRobot.Size = New System.Drawing.Size(376, 154)
        Me.groupboxRobot.TabIndex = 0
        Me.groupboxRobot.TabStop = False
        Me.groupboxRobot.Text = "Connection:"
        '
        'txtSerialText
        '
        Me.txtSerialText.Location = New System.Drawing.Point(54, 106)
        Me.txtSerialText.Multiline = True
        Me.txtSerialText.Name = "txtSerialText"
        Me.txtSerialText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.txtSerialText.Size = New System.Drawing.Size(306, 36)
        Me.txtSerialText.TabIndex = 50
        '
        'Label_CCecho
        '
        Me.Label_CCecho.AutoSize = True
        Me.Label_CCecho.Location = New System.Drawing.Point(562, 21)
        Me.Label_CCecho.Name = "Label_CCecho"
        Me.Label_CCecho.Size = New System.Drawing.Size(74, 13)
        Me.Label_CCecho.TabIndex = 49
        Me.Label_CCecho.Text = "dataReceived"
        '
        'picDataReceived
        '
        Me.picDataReceived.BackColor = System.Drawing.SystemColors.Control
        Me.picDataReceived.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.picDataReceived.InitialImage = CType(resources.GetObject("picDataReceived.InitialImage"), System.Drawing.Image)
        Me.picDataReceived.Location = New System.Drawing.Point(642, 19)
        Me.picDataReceived.Name = "picDataReceived"
        Me.picDataReceived.Size = New System.Drawing.Size(20, 20)
        Me.picDataReceived.TabIndex = 48
        Me.picDataReceived.TabStop = False
        '
        'btnComClose
        '
        Me.btnComClose.Location = New System.Drawing.Point(164, 45)
        Me.btnComClose.Name = "btnComClose"
        Me.btnComClose.Size = New System.Drawing.Size(81, 20)
        Me.btnComClose.TabIndex = 45
        Me.btnComClose.Text = "Close Port"
        Me.btnComClose.UseVisualStyleBackColor = True
        '
        'btnComOpen
        '
        Me.btnComOpen.Location = New System.Drawing.Point(31, 45)
        Me.btnComOpen.Name = "btnComOpen"
        Me.btnComOpen.Size = New System.Drawing.Size(89, 20)
        Me.btnComOpen.TabIndex = 44
        Me.btnComOpen.Text = "Open Port"
        Me.btnComOpen.UseVisualStyleBackColor = True
        '
        'Label_COMasigns
        '
        Me.Label_COMasigns.AutoSize = True
        Me.Label_COMasigns.Location = New System.Drawing.Point(236, 21)
        Me.Label_COMasigns.Name = "Label_COMasigns"
        Me.Label_COMasigns.Size = New System.Drawing.Size(27, 13)
        Me.Label_COMasigns.TabIndex = 43
        Me.Label_COMasigns.Text = "8N1"
        '
        'cboBaudRate
        '
        Me.cboBaudRate.FormattingEnabled = True
        Me.cboBaudRate.Items.AddRange(New Object() {"9600", "38400", "115200"})
        Me.cboBaudRate.Location = New System.Drawing.Point(164, 18)
        Me.cboBaudRate.Name = "cboBaudRate"
        Me.cboBaudRate.Size = New System.Drawing.Size(66, 21)
        Me.cboBaudRate.TabIndex = 42
        '
        'Label_Baud
        '
        Me.Label_Baud.AutoSize = True
        Me.Label_Baud.Location = New System.Drawing.Point(126, 21)
        Me.Label_Baud.Name = "Label_Baud"
        Me.Label_Baud.Size = New System.Drawing.Size(32, 13)
        Me.Label_Baud.TabIndex = 41
        Me.Label_Baud.Text = "Baud"
        '
        'Label_TX
        '
        Me.Label_TX.AutoSize = True
        Me.Label_TX.Location = New System.Drawing.Point(27, 80)
        Me.Label_TX.Name = "Label_TX"
        Me.Label_TX.Size = New System.Drawing.Size(21, 13)
        Me.Label_TX.TabIndex = 38
        Me.Label_TX.Text = "TX"
        '
        'Label_Rx
        '
        Me.Label_Rx.AutoSize = True
        Me.Label_Rx.Location = New System.Drawing.Point(27, 113)
        Me.Label_Rx.Name = "Label_Rx"
        Me.Label_Rx.Size = New System.Drawing.Size(22, 13)
        Me.Label_Rx.TabIndex = 37
        Me.Label_Rx.Text = "RX"
        '
        'btnSend
        '
        Me.btnSend.Location = New System.Drawing.Point(269, 45)
        Me.btnSend.Name = "btnSend"
        Me.btnSend.Size = New System.Drawing.Size(91, 20)
        Me.btnSend.TabIndex = 35
        Me.btnSend.Text = "send TX Box"
        Me.btnSend.UseVisualStyleBackColor = True
        '
        'tbTx
        '
        Me.tbTx.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.tbTx.Location = New System.Drawing.Point(54, 74)
        Me.tbTx.Name = "tbTx"
        Me.tbTx.Size = New System.Drawing.Size(306, 26)
        Me.tbTx.TabIndex = 34
        '
        'Label_port
        '
        Me.Label_port.AutoSize = True
        Me.Label_port.Location = New System.Drawing.Point(13, 21)
        Me.Label_port.Name = "Label_port"
        Me.Label_port.Size = New System.Drawing.Size(32, 13)
        Me.Label_port.TabIndex = 32
        Me.Label_port.Text = "Port :"
        '
        'picOpen
        '
        Me.picOpen.BackColor = System.Drawing.SystemColors.Control
        Me.picOpen.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.picOpen.InitialImage = CType(resources.GetObject("picOpen.InitialImage"), System.Drawing.Image)
        Me.picOpen.Location = New System.Drawing.Point(129, 45)
        Me.picOpen.Name = "picOpen"
        Me.picOpen.Size = New System.Drawing.Size(20, 20)
        Me.picOpen.TabIndex = 46
        Me.picOpen.TabStop = False
        '
        'cboComPort
        '
        Me.cboComPort.FormattingEnabled = True
        Me.cboComPort.Items.AddRange(New Object() {"COM1", "COM2", "COM3", "COM4"})
        Me.cboComPort.Location = New System.Drawing.Point(51, 18)
        Me.cboComPort.Name = "cboComPort"
        Me.cboComPort.Size = New System.Drawing.Size(69, 21)
        Me.cboComPort.TabIndex = 40
        '
        'Timer1
        '
        Me.Timer1.Interval = 1000
        '
        'SerialPort1
        '
        Me.SerialPort1.PortName = "COM2"
        Me.SerialPort1.ReceivedBytesThreshold = 30
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.buttonFish)
        Me.GroupBox1.Controls.Add(Me.buttonManualMove)
        Me.GroupBox1.Controls.Add(Me.Label3)
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.numHeight)
        Me.GroupBox1.Controls.Add(Me.numHeadAngle)
        Me.GroupBox1.Controls.Add(Me.numXstep)
        Me.GroupBox1.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.GroupBox1.Location = New System.Drawing.Point(37, 302)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(376, 117)
        Me.GroupBox1.TabIndex = 1
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Robot Control"
        '
        'buttonFish
        '
        Me.buttonFish.Font = New System.Drawing.Font("Microsoft Sans Serif", 10.0!)
        Me.buttonFish.Location = New System.Drawing.Point(262, 74)
        Me.buttonFish.Name = "buttonFish"
        Me.buttonFish.Size = New System.Drawing.Size(98, 33)
        Me.buttonFish.TabIndex = 7
        Me.buttonFish.Text = "Go Fish!"
        Me.buttonFish.UseVisualStyleBackColor = True
        '
        'buttonManualMove
        '
        Me.buttonManualMove.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!)
        Me.buttonManualMove.Location = New System.Drawing.Point(262, 23)
        Me.buttonManualMove.Name = "buttonManualMove"
        Me.buttonManualMove.Size = New System.Drawing.Size(98, 34)
        Me.buttonManualMove.TabIndex = 6
        Me.buttonManualMove.Text = "Manual Move"
        Me.buttonManualMove.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!)
        Me.Label3.Location = New System.Drawing.Point(6, 87)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(141, 15)
        Me.Label3.TabIndex = 5
        Me.Label3.Text = "Rod height (servo angle)"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!)
        Me.Label2.Location = New System.Drawing.Point(41, 57)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(106, 15)
        Me.Label2.TabIndex = 4
        Me.Label2.Text = "Head Angle (deg.)"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!)
        Me.Label1.Location = New System.Drawing.Point(51, 27)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(96, 15)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "X Step Position: "
        '
        'numHeight
        '
        Me.numHeight.Location = New System.Drawing.Point(153, 83)
        Me.numHeight.Maximum = New Decimal(New Integer() {180, 0, 0, 0})
        Me.numHeight.Name = "numHeight"
        Me.numHeight.Size = New System.Drawing.Size(92, 24)
        Me.numHeight.TabIndex = 2
        '
        'numHeadAngle
        '
        Me.numHeadAngle.Location = New System.Drawing.Point(153, 53)
        Me.numHeadAngle.Maximum = New Decimal(New Integer() {180, 0, 0, 0})
        Me.numHeadAngle.Name = "numHeadAngle"
        Me.numHeadAngle.Size = New System.Drawing.Size(92, 24)
        Me.numHeadAngle.TabIndex = 1
        '
        'numXstep
        '
        Me.numXstep.Location = New System.Drawing.Point(153, 23)
        Me.numXstep.Maximum = New Decimal(New Integer() {750, 0, 0, 0})
        Me.numXstep.Name = "numXstep"
        Me.numXstep.Size = New System.Drawing.Size(92, 24)
        Me.numXstep.TabIndex = 0
        '
        'buttonCompetition
        '
        Me.buttonCompetition.Font = New System.Drawing.Font("Microsoft Sans Serif", 14.0!)
        Me.buttonCompetition.Location = New System.Drawing.Point(31, 615)
        Me.buttonCompetition.Name = "buttonCompetition"
        Me.buttonCompetition.Size = New System.Drawing.Size(376, 41)
        Me.buttonCompetition.TabIndex = 2
        Me.buttonCompetition.Text = "Competition"
        Me.buttonCompetition.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.numArmRad)
        Me.GroupBox2.Controls.Add(Me.Label8)
        Me.GroupBox2.Controls.Add(Me.TabControl1)
        Me.GroupBox2.Controls.Add(Me.numHeadAngleZero)
        Me.GroupBox2.Controls.Add(Me.numDistancetoCenter)
        Me.GroupBox2.Controls.Add(Me.Label6)
        Me.GroupBox2.Controls.Add(Me.Label5)
        Me.GroupBox2.Controls.Add(Me.Label4)
        Me.GroupBox2.Controls.Add(Me.numBoardAngle)
        Me.GroupBox2.Font = New System.Drawing.Font("Microsoft Sans Serif", 14.0!)
        Me.GroupBox2.Location = New System.Drawing.Point(419, 31)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(430, 586)
        Me.GroupBox2.TabIndex = 3
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "The Board"
        '
        'numArmRad
        '
        Me.numArmRad.DecimalPlaces = 3
        Me.numArmRad.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!)
        Me.numArmRad.Increment = New Decimal(New Integer() {1, 0, 0, 131072})
        Me.numArmRad.Location = New System.Drawing.Point(149, 116)
        Me.numArmRad.Maximum = New Decimal(New Integer() {12, 0, 0, 0})
        Me.numArmRad.Minimum = New Decimal(New Integer() {6, 0, 0, 0})
        Me.numArmRad.Name = "numArmRad"
        Me.numArmRad.Size = New System.Drawing.Size(77, 26)
        Me.numArmRad.TabIndex = 10
        Me.numArmRad.UseWaitCursor = True
        Me.numArmRad.Value = New Decimal(New Integer() {6254, 0, 0, 196608})
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.Label8.Location = New System.Drawing.Point(29, 124)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(114, 18)
        Me.Label8.TabIndex = 9
        Me.Label8.Text = "Arm Radius(in.):"
        '
        'TabControl1
        '
        Me.TabControl1.Controls.Add(Me.tabBoardDefault)
        Me.TabControl1.Controls.Add(Me.tabRotatedboard)
        Me.TabControl1.Controls.Add(Me.tabRelativetoBot)
        Me.TabControl1.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.TabControl1.Location = New System.Drawing.Point(6, 148)
        Me.TabControl1.Name = "TabControl1"
        Me.TabControl1.SelectedIndex = 0
        Me.TabControl1.Size = New System.Drawing.Size(418, 381)
        Me.TabControl1.TabIndex = 8
        '
        'tabBoardDefault
        '
        Me.tabBoardDefault.Controls.Add(Me.chartBoard)
        Me.tabBoardDefault.Location = New System.Drawing.Point(4, 27)
        Me.tabBoardDefault.Name = "tabBoardDefault"
        Me.tabBoardDefault.Padding = New System.Windows.Forms.Padding(3)
        Me.tabBoardDefault.Size = New System.Drawing.Size(410, 350)
        Me.tabBoardDefault.TabIndex = 0
        Me.tabBoardDefault.Text = "Board"
        Me.tabBoardDefault.UseVisualStyleBackColor = True
        '
        'chartBoard
        '
        ChartArea4.Name = "ChartArea1"
        Me.chartBoard.ChartAreas.Add(ChartArea4)
        Legend2.Name = "Legend1"
        Me.chartBoard.Legends.Add(Legend2)
        Me.chartBoard.Location = New System.Drawing.Point(3, 3)
        Me.chartBoard.Name = "chartBoard"
        Me.chartBoard.Size = New System.Drawing.Size(401, 341)
        Me.chartBoard.TabIndex = 0
        Me.chartBoard.Text = "chartBoard"
        '
        'tabRotatedboard
        '
        Me.tabRotatedboard.Controls.Add(Me.chartRot)
        Me.tabRotatedboard.Location = New System.Drawing.Point(4, 27)
        Me.tabRotatedboard.Name = "tabRotatedboard"
        Me.tabRotatedboard.Padding = New System.Windows.Forms.Padding(3)
        Me.tabRotatedboard.Size = New System.Drawing.Size(410, 350)
        Me.tabRotatedboard.TabIndex = 1
        Me.tabRotatedboard.Text = "Rotated"
        Me.tabRotatedboard.UseVisualStyleBackColor = True
        '
        'chartRot
        '
        ChartArea5.Name = "ChartArea1"
        Me.chartRot.ChartAreas.Add(ChartArea5)
        Me.chartRot.Location = New System.Drawing.Point(7, 6)
        Me.chartRot.Name = "chartRot"
        Series3.ChartArea = "ChartArea1"
        Series3.Name = "Series1"
        Me.chartRot.Series.Add(Series3)
        Me.chartRot.Size = New System.Drawing.Size(397, 338)
        Me.chartRot.TabIndex = 1
        '
        'tabRelativetoBot
        '
        Me.tabRelativetoBot.Controls.Add(Me.chartRel)
        Me.tabRelativetoBot.Location = New System.Drawing.Point(4, 27)
        Me.tabRelativetoBot.Name = "tabRelativetoBot"
        Me.tabRelativetoBot.Padding = New System.Windows.Forms.Padding(3)
        Me.tabRelativetoBot.Size = New System.Drawing.Size(410, 350)
        Me.tabRelativetoBot.TabIndex = 2
        Me.tabRelativetoBot.Text = "Relative"
        Me.tabRelativetoBot.UseVisualStyleBackColor = True
        '
        'chartRel
        '
        ChartArea6.Name = "ChartArea1"
        Me.chartRel.ChartAreas.Add(ChartArea6)
        Me.chartRel.Location = New System.Drawing.Point(7, 6)
        Me.chartRel.Name = "chartRel"
        Series4.ChartArea = "ChartArea1"
        Series4.Name = "Series1"
        Me.chartRel.Series.Add(Series4)
        Me.chartRel.Size = New System.Drawing.Size(397, 338)
        Me.chartRel.TabIndex = 1
        Me.chartRel.Text = "chartRel"
        '
        'numHeadAngleZero
        '
        Me.numHeadAngleZero.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!)
        Me.numHeadAngleZero.Location = New System.Drawing.Point(149, 87)
        Me.numHeadAngleZero.Maximum = New Decimal(New Integer() {180, 0, 0, 0})
        Me.numHeadAngleZero.Name = "numHeadAngleZero"
        Me.numHeadAngleZero.Size = New System.Drawing.Size(77, 26)
        Me.numHeadAngleZero.TabIndex = 6
        Me.numHeadAngleZero.Value = New Decimal(New Integer() {90, 0, 0, 0})
        '
        'numDistancetoCenter
        '
        Me.numDistancetoCenter.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!)
        Me.numDistancetoCenter.Location = New System.Drawing.Point(149, 58)
        Me.numDistancetoCenter.Maximum = New Decimal(New Integer() {750, 0, 0, 0})
        Me.numDistancetoCenter.Minimum = New Decimal(New Integer() {100, 0, 0, 0})
        Me.numDistancetoCenter.Name = "numDistancetoCenter"
        Me.numDistancetoCenter.Size = New System.Drawing.Size(77, 26)
        Me.numDistancetoCenter.TabIndex = 5
        Me.numDistancetoCenter.Value = New Decimal(New Integer() {600, 0, 0, 0})
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.Label6.Location = New System.Drawing.Point(21, 90)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(122, 18)
        Me.Label6.TabIndex = 3
        Me.Label6.Text = "Head Angle Zero:"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.Label5.Location = New System.Drawing.Point(35, 61)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(108, 18)
        Me.Label5.TabIndex = 2
        Me.Label5.Text = "Steps to center"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.0!)
        Me.Label4.Location = New System.Drawing.Point(34, 31)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(109, 18)
        Me.Label4.TabIndex = 1
        Me.Label4.Text = "Angle of Board:"
        '
        'numBoardAngle
        '
        Me.numBoardAngle.DecimalPlaces = 2
        Me.numBoardAngle.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!)
        Me.numBoardAngle.Location = New System.Drawing.Point(149, 28)
        Me.numBoardAngle.Maximum = New Decimal(New Integer() {360, 0, 0, 0})
        Me.numBoardAngle.Name = "numBoardAngle"
        Me.numBoardAngle.Size = New System.Drawing.Size(77, 26)
        Me.numBoardAngle.TabIndex = 0
        Me.numBoardAngle.Value = New Decimal(New Integer() {45, 0, 0, 0})
        '
        'lstRawBitePos
        '
        Me.lstRawBitePos.Location = New System.Drawing.Point(37, 438)
        Me.lstRawBitePos.Name = "lstRawBitePos"
        Me.lstRawBitePos.Size = New System.Drawing.Size(120, 161)
        Me.lstRawBitePos.TabIndex = 4
        Me.lstRawBitePos.UseCompatibleStateImageBehavior = False
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(43, 422)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(74, 13)
        Me.Label7.TabIndex = 6
        Me.Label7.Text = "Raw Bite Pos."
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(201, 422)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(81, 13)
        Me.Label9.TabIndex = 7
        Me.Label9.Text = "Robot Bite Pos."
        '
        'lstrobobitepos
        '
        Me.lstrobobitepos.AutoArrange = False
        Me.lstrobobitepos.HideSelection = False
        Me.lstrobobitepos.Location = New System.Drawing.Point(179, 438)
        Me.lstrobobitepos.MultiSelect = False
        Me.lstrobobitepos.Name = "lstrobobitepos"
        Me.lstrobobitepos.Size = New System.Drawing.Size(218, 161)
        Me.lstrobobitepos.TabIndex = 5
        Me.lstrobobitepos.TileSize = New System.Drawing.Size(150, 200)
        Me.lstrobobitepos.UseCompatibleStateImageBehavior = False
        Me.lstrobobitepos.View = System.Windows.Forms.View.List
        '
        'txtCode
        '
        Me.txtCode.Location = New System.Drawing.Point(37, 192)
        Me.txtCode.Multiline = True
        Me.txtCode.Name = "txtCode"
        Me.txtCode.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.txtCode.Size = New System.Drawing.Size(360, 104)
        Me.txtCode.TabIndex = 8
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.ClientSize = New System.Drawing.Size(869, 677)
        Me.Controls.Add(Me.txtCode)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.lstrobobitepos)
        Me.Controls.Add(Me.lstRawBitePos)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.buttonCompetition)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.groupboxRobot)
        Me.Name = "Form1"
        Me.Text = "formBoard"
        Me.groupboxRobot.ResumeLayout(False)
        Me.groupboxRobot.PerformLayout()
        CType(Me.picDataReceived, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.picOpen, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        CType(Me.numHeight, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.numHeadAngle, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.numXstep, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        CType(Me.numArmRad, System.ComponentModel.ISupportInitialize).EndInit()
        Me.TabControl1.ResumeLayout(False)
        Me.tabBoardDefault.ResumeLayout(False)
        CType(Me.chartBoard, System.ComponentModel.ISupportInitialize).EndInit()
        Me.tabRotatedboard.ResumeLayout(False)
        CType(Me.chartRot, System.ComponentModel.ISupportInitialize).EndInit()
        Me.tabRelativetoBot.ResumeLayout(False)
        CType(Me.chartRel, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.numHeadAngleZero, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.numDistancetoCenter, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.numBoardAngle, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents groupboxRobot As GroupBox
    Friend WithEvents Label_CCecho As Label
    Friend WithEvents picDataReceived As PictureBox
    Friend WithEvents btnComClose As Button
    Friend WithEvents btnComOpen As Button
    Friend WithEvents Label_COMasigns As Label
    Friend WithEvents cboBaudRate As ComboBox
    Friend WithEvents Label_Baud As Label
    Friend WithEvents Label_TX As Label
    Friend WithEvents Label_Rx As Label
    Friend WithEvents btnSend As Button
    Friend WithEvents tbTx As TextBox
    Friend WithEvents Label_port As Label
    Friend WithEvents picOpen As PictureBox
    Friend WithEvents cboComPort As ComboBox
    Friend WithEvents Timer1 As Timer
    Friend WithEvents SerialPort1 As IO.Ports.SerialPort
    Friend WithEvents Label3 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents numHeight As NumericUpDown
    Friend WithEvents numHeadAngle As NumericUpDown
    Friend WithEvents numXstep As NumericUpDown
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents buttonFish As Button
    Friend WithEvents buttonManualMove As Button
    Friend WithEvents buttonCompetition As Button
    Friend WithEvents GroupBox2 As GroupBox
    Friend WithEvents Label6 As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents numBoardAngle As NumericUpDown
    Friend WithEvents numHeadAngleZero As NumericUpDown
    Friend WithEvents numDistancetoCenter As NumericUpDown
    Friend WithEvents TabControl1 As TabControl
    Friend WithEvents tabBoardDefault As TabPage
    Friend WithEvents tabRotatedboard As TabPage
    Friend WithEvents chartBoard As DataVisualization.Charting.Chart
    Friend WithEvents tabRelativetoBot As TabPage
    Friend WithEvents chartRot As DataVisualization.Charting.Chart
    Friend WithEvents chartRel As DataVisualization.Charting.Chart
    Friend WithEvents numArmRad As NumericUpDown
    Friend WithEvents Label8 As Label
    Friend WithEvents lstRawBitePos As ListView
    Friend WithEvents Label7 As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents txtSerialText As TextBox
    Friend WithEvents txtCode As TextBox
    Friend WithEvents lstrobobitepos As ListView
End Class
