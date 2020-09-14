using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>   

    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        Robot robot = new Robot();
        System.Windows.Threading.DispatcherTimer timerAffichage;

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM5", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.Open();
            serialPort1.DataReceived += SerialPort1_DataReceived;
            timerAffichage = new System.Windows.Threading.DispatcherTimer();
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Start();
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            //if (robot.receivedMessage != "")
            //{
            //    textBoxReception.Text += robot.receivedMessage;
            //    robot.receivedMessage = "";
            //}
            while(robot.byteListReceived.Count>0)
            {
                byte b = robot.byteListReceived.Dequeue();
                textBoxReception.Text += "Ox" +  b.ToString("X2")+" " ;
            }
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //robot.receivedMessage += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach(byte b in e.Data)
            {
                robot.byteListReceived.Enqueue(b);
            }
            
        }

        bool toggle = false;
       


        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Envoi();
            }

        }
        

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }


        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            Envoi();

            if (toggle)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            toggle = !toggle;
        }
        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            Clear();

            if (toggle)
            {
                buttonClear.Background = Brushes.RoyalBlue;
            }
            else
            {
                buttonClear.Background = Brushes.Beige;
            }
            toggle = !toggle;
        }
        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            Test();

            if(toggle)
            {
                buttonTest.Background = Brushes.RoyalBlue;
            }
            else
            {
                buttonTest.Background = Brushes.Beige;
            }
            toggle = !toggle;
        }


        private void Envoi()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = "";
        }
        private void Clear()
        {
            textBoxReception.Text = "";
        }
        private void Test()
        {
            byte[] byteList = new byte[20];
            int i;
            for (i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, byteList.Count());
            textBoxReception.Text += "\n"+"\n";

        }
    }
}
