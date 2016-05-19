Option Strict On
Option Infer On
Imports System.Math
Public Class Form1




    Private readBuffer As String = String.Empty
    Private Bytenumber As Integer
    Private ByteToRead As Integer
    Private byteEnd(2) As Char
    Private comOpen As Boolean
    Public initPoint As Double(,) = {{-0.41732, -0.68464}, {0.9354, -0.2459}, {1.97807, -1.207}, {1.62212, -2.6591}, {1.17214, 1.05198}, {-1.3192, 0.279}, {0.02279, 2.14064}, {-1.8499, 2.23004}}
    Public rtheta As Double(,) = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
    Public rotPoint As Double(,) = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
    Public relPoint As Double(,) = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}

#Region "form events"
    ''' <summary>
    ''' close application and COM Port
    ''' </summary>
    Private Sub Form1_FormClosed(ByVal sender As System.Object,
                                 ByVal e As System.Windows.Forms.FormClosedEventArgs) _
                                 Handles MyBase.FormClosed
        If comOpen Then SerialPort1.Close()
    End Sub

    ''' <summary>
    ''' open Windows Form
    ''' </summary>
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        chartBoard.Series.Clear()
        chartBoard.Series.Add("Board Center")
        chartBoard.Series("Board Center").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartBoard.Series("Board Center").Points.AddXY(0, 0)
        chartBoard.Series.Add("Pick Points")
        For i As Integer = 0 To 7 Step 1
            chartBoard.Series("Pick Points").Points.AddXY(initPoint(i, 0), initPoint(i, 1))
        Next
        chartBoard.Series("Pick Points").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartBoard.ResetAutoValues()
        ' read avaiable COM Ports:
        Dim Portnames As String() = System.IO.Ports.SerialPort.GetPortNames
        If Portnames Is Nothing Then
            MsgBox("There are no Com Ports detected!")
            Me.Close()
        End If
        cboComPort.Items.AddRange(Portnames)
        cboComPort.Text = Portnames(0)
        cboBaudRate.Text = "9600"

    End Sub

    ''' <summary>
    ''' Open Com Port here
    ''' </summary>
    Private Sub btnComOpen_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnComOpen.Click

        ' device params
        With SerialPort1

            .ParityReplace = &H3B                    ' replace ";" when parity error occurs
            .PortName = cboComPort.Text
            .BaudRate = CInt(cboBaudRate.Text)
            .Parity = IO.Ports.Parity.None
            .DataBits = 8
            .StopBits = IO.Ports.StopBits.One
            .Handshake = IO.Ports.Handshake.None
            .RtsEnable = False
            .ReceivedBytesThreshold = 1             'threshold: one byte in buffer > event is fired
            .NewLine = vbCr         ' CR must be the last char in frame. This terminates the SerialPort.readLine
            .ReadTimeout = 10000

        End With

        ' check whether device is avaiable:
        Try
            SerialPort1.Open()
            comOpen = SerialPort1.IsOpen
        Catch ex As Exception
            comOpen = False
            MsgBox("Error Open: " & ex.Message)
            picOpen.BackColor = Color.Red
        End Try

        If comOpen Then
            picOpen.BackColor = Color.Green
            cboComPort.Enabled = False
            cboBaudRate.Enabled = False
        End If

    End Sub

    ''' <summary>
    ''' close ComPort
    ''' </summary>
    Private Sub Button_Close_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnComClose.Click
        If comOpen Then
            ' clear input buffer
            SerialPort1.DiscardInBuffer()
            SerialPort1.Close()
        End If
        comOpen = False
        picOpen.BackColor = Color.Red
        picDataReceived.BackColor = Color.Gray
        cboComPort.Enabled = True
        cboBaudRate.Enabled = True
    End Sub

    ''' <summary>
    ''' clear TextBoxes
    ''' </summary>
    Private Sub Button_clear_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnClear.Click
        tbRx.Text = String.Empty
        tbTx.Text = String.Empty
    End Sub

    ''' <summary>
    ''' write content of Textbox to Port
    ''' </summary>
    Private Sub button_send_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSend.Click
        If comOpen Then SerialPort1.WriteLine(tbTx.Text)
    End Sub

    ''' <summary>
    ''' close app
    ''' </summary>
    Private Sub Button_ende_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnExit.Click
        If comOpen Then
            ' clear input buffer
            SerialPort1.DiscardInBuffer()
            SerialPort1.Close()
        End If
        comOpen = False
        Me.Close()
    End Sub

    ''' <summary>
    ''' Timer datareceived event
    ''' </summary>
    Private Sub Timer1_Tick(ByVal sender As System.Object,
                            ByVal e As System.EventArgs) Handles Timer1.Tick
        picDataReceived.BackColor = Color.Gray
        Timer1.Enabled = False
    End Sub

#End Region

#Region "ComPort read data"

    ''' <summary>
    ''' async read on secondary thread
    ''' </summary>
    Private Sub SerialPort1_DataReceived(ByVal sender As System.Object,
                                         ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) _
                                         Handles SerialPort1.DataReceived
        If comOpen Then
            Try
                byteEnd = SerialPort1.NewLine.ToCharArray

                ' get number off bytes in buffer
                Bytenumber = SerialPort1.BytesToRead

                ' read one byte from buffer
                'ByteToRead = SerialPort1.ReadByte()

                ' read one char from buffer
                'CharToRead = SerialPort1.ReadChar()

                ' read until string "90"
                'readBuffer1 = SerialPort1.ReadTo("90")

                ' read entire string until .Newline 
                readBuffer = SerialPort1.ReadLine()

                'data to UI thread
                Me.Invoke(New EventHandler(AddressOf DoUpdate))

            Catch ex As Exception
                MsgBox("read " & ex.Message)
            End Try
        End If
    End Sub

    ''' <summary>
    ''' update received string in UI
    ''' </summary>
    ''' <remarks></remarks>
    Public Sub DoUpdate(ByVal sender As Object, ByVal e As System.EventArgs)
        tbRx.Text = readBuffer
        picDataReceived.BackColor = Color.Green
        Timer1.Enabled = True
    End Sub

    Private Sub cboBaudRate_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cboBaudRate.SelectedIndexChanged

    End Sub

    Private Sub numBoardAngle_ValueChanged(sender As Object, e As EventArgs) Handles numBoardAngle.ValueChanged
        For i As Integer = 0 To 7 Step 1
            rtheta(i, 0) = Sqrt(initPoint(i, 0) ^ 2 + initPoint(i, 1) ^ 2)
            rtheta(i, 1) = Atan2(initPoint(i, 0), initPoint(i, 1))
        Next

        For i As Integer = 0 To 7 Step 1
            rotPoint(i, 0) = rtheta(i, 0) * Sin(rtheta(i, 1) + (numBoardAngle.Value / (360 / (PI * 2))))
            rotPoint(i, 1) = rtheta(i, 0) * Cos(rtheta(i, 1) + (numBoardAngle.Value / (360 / (PI * 2))))
        Next

        chartRot.Series.Clear()
        chartRot.Series.Add("Board Center")
        chartRot.Series("Board Center").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartRot.Series(0).Points.AddXY(0, 0)
        chartRot.Series.Add("Pick Points")
        For i As Integer = 0 To 7 Step 1
            chartRot.Series("Pick Points").Points.AddXY(rotPoint(i, 0), rotPoint(i, 1))
        Next
        chartRot.Series("Pick Points").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartRot.ChartAreas(0).AxisX.Maximum = 4
        chartRot.ChartAreas(0).AxisX.Minimum = -4
        chartRot.ChartAreas(0).AxisY.Maximum = 4
        chartRot.ChartAreas(0).AxisY.Minimum = -4

    End Sub

    Private Sub numHeadAngleZero_ValueChanged(sender As Object, e As EventArgs) Handles numHeadAngleZero.ValueChanged
        Dim xoff As Double
        Dim yoff As Double

        xoff = numArmRad.Value * Sin(numHeadAngleZero.Value / (360 / (2 * PI)))
        yoff = numArmRad.Value * Cos(numHeadAngleZero.Value / (360 / (2 * PI)))

        For i As Integer = 0 To 7 Step 1
            relPoint(i, 0) = rotPoint(i, 0) + xoff
            relPoint(i, 1) = rotPoint(i, 1) + yoff
        Next

        chartRel.Series.Clear()
        chartRel.Series.Add("Robot Zero")
        chartRel.Series("Robot Zero").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartRel.Series.Add("Board Zero")
        chartRel.Series("Board Zero").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartRel.Series("Robot Zero").Points.AddXY(0, 0)
        chartRel.Series("Board Zero").Points.AddXY(xoff, yoff)
        chartRel.Series.Add("Pick Points")
        For i As Integer = 0 To 7 Step 1
            chartRel.Series("Pick Points").Points.AddXY(relPoint(i, 0), relPoint(i, 1))
        Next
        chartRel.Series("Pick Points").ChartType = DataVisualization.Charting.SeriesChartType.Point
        chartRel.ChartAreas(0).AxisX.Maximum = 8
        chartRot.ChartAreas(0).AxisX.Minimum = 0
        chartRot.ChartAreas(0).AxisY.Maximum = 8
        chartRot.ChartAreas(0).AxisY.Minimum = 0

    End Sub






#End Region
End Class
