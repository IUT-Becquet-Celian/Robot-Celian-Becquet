using ExtendedSerialPort;
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
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;
using Utilities;


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

        private readonly KeyboardHookListener m_KeyboardHookManager;

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM9", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.Open();
            serialPort1.DataReceived += SerialPort1_DataReceived;
            timerAffichage = new System.Windows.Threading.DispatcherTimer();
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Start();

            m_KeyboardHookManager = new KeyboardHookListener(new GlobalHooker());
            m_KeyboardHookManager.Enabled = true;
            m_KeyboardHookManager.KeyDown += HookManager_KeyDown;
        }

        private void HookManager_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (robot.autoControlActivated == false)
            {
                switch (e.KeyCode)
                {
                    case System.Windows.Forms.Keys.Left:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_TOURNE_SUR_PLACE_GAUCHE });
                        break;
                    case System.Windows.Forms.Keys.Right:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_TOURNE_SUR_PLACE_DROITE });
                        break;
                    case System.Windows.Forms.Keys.Up:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_AVANCE });
                        break;
                    case System.Windows.Forms.Keys.Down:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_ARRET });
                        break;
                    case System.Windows.Forms.Keys.PageDown:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_RECULE });
                        break;

                }

            }
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
        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,

            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,

            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,

            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,

            STATE_TOURNE_DROITE_FAIBLE = 12,
            STATE_TOURNE_DROITE_FAIBLE_EN_COURS = 13,

            STATE_TOURNE_GAUCHE_FAIBLE = 14,
            STATE_TOURNE_GAUCHE_FAIBLE_EN_COURS = 15,

            STATE_ARRET = 16,
            STATE_ARRET_EN_COURS = 17,

            STATE_RECULE = 18,
            STATE_RECULE_EN_COURS = 19,

            STATE_AVANCE_FOND = 20,
            STATE_AVANCE_FOND_EN_COURS = 21,
        }

        bool messageDecode = false;

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            if (robot.flagNewIrData)
            {
                Label_IRGauche.Content = "IR Gauche : " + robot.distanceTelemetreGauche + "cm";
                Label_IRCentre.Content = "IR Centre : " + robot.distanceTelemetreCentre + "cm";
                Label_IRDroit.Content = "IR Droit : " + robot.distanceTelemetreDroit + "cm";
                robot.flagNewIrData = false;
            }

            if (robot.flagNewVitesseData)
            {
                Label_VitesseGauche.Content = "Vitesse Gauche : " + robot.vitesseMoteurGauche + " %";
                Label_VitesseDroit.Content = "Vitesse Droit : " + robot.vitesseMoteurDroit + " %";
                robot.flagNewVitesseData = false;
            }

            if (robot.flagNewReceptionData)
            {
                textBoxReception.Text += "Message reçu : " + robot.receivedMessage + "\n";
                textBoxReception.ScrollToEnd();
                robot.flagNewReceptionData = false;
            }
            if (robot.flagNewEtapeData)
            {
                textBoxReception.Text += "Robot state : " + (StateRobot)robot.receivedEtape + "\n" + "Instant courant : " + robot.receivedInstantCourant + " ms\n" ;
                textBoxReception.ScrollToEnd();
                robot.flagNewEtapeData = false;
            }
            if (robot.flagChecksum)
            {
                textBoxInfo.Text += "Checksum pas valide \n";
                textBoxReception.ScrollToEnd();
                robot.flagChecksum = false;
            }
            /*if (robot.flagNewPositionData)
            {
                textBoxInfo.Text += "Info de position :" + ;
                textBoxInfo.ScrollToEnd();
                robot.flagNewPositionData = false;
            } */          


        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //robot.receivedMessage += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach(byte b in e.Data)
            {
                DecodeMessage(b);
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
                    msgDecodedPayloadIndex=0;
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
                    if (calculatedChecksum == receivedChecksum)
                    {
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        robot.flagChecksum = true;
                        
                    }
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        private void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch (msgFunction)
            {
                
                case 0x30: // IR data
                    robot.flagNewIrData = true;
                    robot.distanceTelemetreGauche = msgPayload[0];
                    robot.distanceTelemetreCentre = msgPayload[1];
                    robot.distanceTelemetreDroit = msgPayload[2];
                    break;

                case 0x50: //numero etape robot + instant courant en ms
                    robot.flagNewEtapeData = true;
                    robot.receivedEtape = msgPayload[0];
                    robot.receivedInstantCourant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) + (((int)msgPayload[3]) << 8) + (((int)msgPayload[4]) << 0);
                    break;

                case 0x40:
                    robot.flagNewVitesseData = true;
                    robot.vitesseMoteurGauche= msgPayload[0];
                    robot.vitesseMoteurDroit = msgPayload[1];
                    break;

                case 0x80: // Text transmission
                    robot.flagNewReceptionData = true;
                    robot.receivedMessage = System.Text.Encoding.UTF8.GetString(msgPayload);
                    break;

                case 0x61:
                    //robot.flagNewPositionData = true;
                    //string display = " ";
                    byte[] xpos_array = msgPayload.GetRange(4, 4);
                    float xpos = xpos_array.GetFloat();

                    byte[] ypos_array = msgPayload.GetRange(8, 4);
                    float ypos = ypos_array.GetFloat();

                    byte[] angle_array = msgPayload.GetRange(12, 4);
                    float angle = angle_array.GetFloat();

                    byte[] vitesseLineaire_array = msgPayload.GetRange(16, 4);
                    float vitesseLineaire = vitesseLineaire_array.GetFloat();

                    byte[] vitesseAngulaire_array = msgPayload.GetRange(20, 4);
                    float vitesseAngulaire = vitesseAngulaire_array.GetFloat();

                    //display = "x=" + xpos;  //+ ";y=" + ypos + ";angle=" + angle + ";vit_lin=" + vitesseLineaire + ";vit_angle="+ vitesseAngulaire;
                    AsservDisplay.UpdatePolarSpeedConsigneValues(xpos, ypos, angle);
                    break;

                case 0x51: //SetRobotState

                    break;

                case 0x52: //SetRobotAutoControlState
                    
                    break;

                default: // Unknow command
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
            textBoxReception.Text += textBoxEmission.Text + "\n";
            textBoxEmission.Text = "";
        }
        private void Clear()
        {
            textBoxReception.Text = "";
        }
        private void Test()
        {
            //byte[] array = new byte[] { 0xFF,20,30}/*Encoding.ASCII.GetBytes("Bonjour")*/;
            //UartEncodeAndSendMessage(0x0030, array.Length, array);
            
            byte[] array1 = Encoding.ASCII.GetBytes("Bonjour");
            UartEncodeAndSendMessage(0x0080, array1.Length, array1);
            
            //byte[] array2 = new byte[] { 50, 70 }/*Encoding.ASCII.GetBytes("Bonjour")*/;
            //UartEncodeAndSendMessage(0x0040, array2.Length, array2);
            

            //Label_IRGauche.Content = "IR Gauche : " + robot.distanceTelemetreGauche + "cm";
            //Label_IRCentre.Content = "IR Centre : " + robot.distanceTelemetreCentre + "cm";
            //Label_IRDroit.Content = "IR Droit : " + robot.distanceTelemetreDroit + "cm";
            //Label_VitesseGauche.Content = "Vitesse Gauche : " + 10 + " %";
            //Label_VitesseDroit.Content = "Vitesse Droit : " + 10 + " %";

            
        }

        private void checkBox_Checked(object sender, RoutedEventArgs e)
        {         
            byte[] Auto = new byte[] { 1 };
            UartEncodeAndSendMessage(0x0052, Auto.Length, Auto);
            robot.autoControlActivated = true;
        }

        private void checkBox_Unchecked(object sender, RoutedEventArgs e)
        {
            byte[] Manuel = new byte[] { 0 };
            UartEncodeAndSendMessage(0x0052, Manuel.Length, Manuel);
            robot.autoControlActivated = false;
        }

        private void comboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void textBoxReception_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
   

}
