﻿using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.RightsManagement;
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
        bool toggle = false;

        //Declarations a propos de DecodeMessage :
        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;
        //fin de declarations a propos de DecodeMessage


        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM4", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.Open();
            serialPort1.DataReceived += SerialPort1_DataReceived;
            timerAffichage = new System.Windows.Threading.DispatcherTimer();
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Start();
        }
        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
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
                textBoxReception.Text +=  Convert.ToChar(b);
                //textBoxReception.Text += "Ox" + b.ToString("X2") + " ";
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

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {

            byte[] trame = new byte[6+msgPayloadLength];

            int pos = 0;

            trame[pos++] = 0xFE;

            trame[pos++] = (byte)(msgFunction >> 8);
            trame[pos++] = (byte)(msgFunction >> 0);

            trame[pos++] = (byte)(msgPayloadLength >> 8);
            trame[pos++] = (byte)(msgPayloadLength >> 0);

            for(int j=0 ; j<msgPayloadLength; j++)
            {
                trame[pos++] = msgPayload[j];
            }

            trame[pos++] = (byte)(CalculateChecksum(msgFunction, msgPayloadLength, msgPayload));
            serialPort1.Write(trame, 0, pos);

        }


        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;                    
                    break;

                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;
                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    rcvState = StateReception.Payload;
                    break;

                case StateReception.Payload:
                    if (msgDecodedPayloadIndex < msgDecodedPayloadLength)
                    {
                        msgDecodedPayload[msgDecodedPayloadIndex] = c;
                        msgDecodedPayloadIndex++;
                    }    
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;

                case StateReception.CheckSum:
                    byte receivedChecksum = c;
                    byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    //if(calculatedChecksum == receivedChecksum)
                    //{
                    //    ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    //}
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
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
            byte[] array = Encoding.ASCII.GetBytes("Bonjour");
            UartEncodeAndSendMessage(0x0080, array.Length, array);
        }
    }
}
