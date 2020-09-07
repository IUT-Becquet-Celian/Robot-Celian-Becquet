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
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM4", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.Open();
            serialPort1.DataReceived += SerialPort1_DataReceived;
        }
        
        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            string dataReceived = Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            textBoxReception.Text += "Reçu : " + dataReceived + "\n";
        }

        bool toggle = false;
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

        private void Envoi()
        {            
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = "";            
        }

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

    }
}
