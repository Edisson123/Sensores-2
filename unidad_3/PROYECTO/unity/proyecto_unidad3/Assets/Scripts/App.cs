
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;



public class App : MonoBehaviour
{
    
    public Text text_fecha, text_hora, text_temp, text_pres, text_alt, text_hum, text_dia;

    byte[] buffer = { 03, 00, 00, 00, 00, 00, 00, 00 };
    byte[] messageClock, messageBME;
    public Dropdown dropdown;
    bool bandera = false;
    bool banderaInit = true;

    private static App instance;
    private Thread receiveThread;
    private UdpClient receiveClientClock;
    private UdpClient receiveClientBME;
    private UdpClient sendClient;
    private IPEndPoint receiveEndPointClock;
    private IPEndPoint receiveEndPointBME;
    private IPEndPoint sendPoint;
    private string ip = "192.168.1.66";
    public string ipSend = "192.168.1.103";
    public int receivePortClock = 666;
    public int receivePortBME = 911;
    public int UDP_port = 4023;
    private bool isInitialized;
    private Queue receiveQueue;


    private void Awake()
    {
        //Initialize();
    }

    private void Initialize()
    {

        instance = this;
        receiveEndPointClock = new IPEndPoint(IPAddress.Parse(ip), receivePortClock);
        receiveEndPointBME = new IPEndPoint(IPAddress.Parse(ip), receivePortBME);
        sendPoint = new IPEndPoint(IPAddress.Parse(ipSend), UDP_port);
        receiveClientClock = new UdpClient(receivePortClock);
        receiveClientBME = new UdpClient(receivePortBME);
        sendClient = new UdpClient();
        receiveQueue = Queue.Synchronized(new Queue());
        receiveThread = new Thread(new ThreadStart(ReceiveDataListener));
        receiveThread.IsBackground = true;
        receiveThread.Start();
        isInitialized = true;
    }

    private void ReceiveDataListener()
    {
        while (true)
        {
            try
            {
                byte[] dataClock = receiveClientClock.Receive(ref receiveEndPointClock);              
                receiveQueue.Enqueue(dataClock);

                byte[] dataBME = receiveClientBME.Receive(ref receiveEndPointBME);
                receiveQueue.Enqueue(dataBME);

            }
            catch (System.Exception ex)
            {
                Debug.Log(ex.ToString());
            }
        }
    }


    private void OnDestroy()
    {
        TryKillThread();
    }

    private void OnApplicationQuit()
    {
        TryKillThread();
    }

    private void TryKillThread()
    {
        if (isInitialized)
        {
            receiveThread.Abort();
            receiveThread = null;
            receiveClientClock.Close();
            receiveClientClock = null;
            receiveClientBME.Close();
            sendClient.Close();
            sendClient = null;
            receiveClientBME = null;

            Debug.Log("Thread killed");
            isInitialized = false;
        }
    }

    // Initialization
    void Start()
    {

        dropdown = GameObject.Find("menu_dia").GetComponent<Dropdown>();
    }

    // Executed each frame
    void Update()
    {

        if (bandera)
        {

            if(banderaInit == true)
            {
                Initialize();
                banderaInit = false;
            }
            
            //---------------------------------------------------------------------
            // Receive data
            //---------------------------------------------------------------------

            if (receiveQueue.Count != 0)
            {
                messageClock = (byte[])receiveQueue.Dequeue();
                //messageBME = (byte[])receiveQueue.Dequeue();

                //Debug.Log("encolo");
                if (messageClock[0] == 0x4A)
                {
                    text_dia.text = dia_semana(messageClock[4]);
                    text_fecha.text = messageClock[5] + " de " + mes_año(messageClock[6]) + " 20" + messageClock[7];
                    text_hora.text = messageClock[3] + ":" + messageClock[2] + ":" + messageClock[1];

                }else if (messageClock[0] == 0x47) {
                

                    float temperatura = (float)System.BitConverter.ToSingle(messageClock, 1);
                    float presion = (float)System.BitConverter.ToSingle(messageClock, 5);
                    float altura = (float)System.BitConverter.ToSingle(messageClock, 9);
                    float humedad = (float)System.BitConverter.ToSingle(messageClock, 13);

                    text_temp.text = temperatura.ToString();
                    text_pres.text = presion.ToString();
                    text_alt.text = altura.ToString();
                    text_hum.text = humedad.ToString();

                }
                else if (messageClock[0] == 0x05)
                {
                    Debug.Log("Error de lectura en el reloj");
                }
                else
                {
                    Debug.Log("Error desconocido xD");
                }



            }


        }

        
    }

    string dia_semana(int dia)
    {
        switch (dia)
        {
            case 1:
                return "Lunes";
            case 2:
                return "Martes";
            case 3:
                return "Miercoles";
            case 4:
                return "Jueves";
            case 5:
                return "Viernes";
            case 6:
                return "Sabado";
            case 7:
                return "Domingo";
                
        }
        return "error";
    }

    string mes_año(int mes)
    {
        switch (mes)
        {
            case 1:
                return "Enero";
            case 2:
                return "Febrero";
            case 3:
                return "Marzo";
            case 4:
                return "Abril";
            case 5:
                return "Mayo";
            case 6:
                return "Junio";
            case 7:
                return "Julio";
            case 8:
                return "Agosto";
            case 9:
                return "Septiembre";
            case 10:
                return "Octubre";
            case 11:
                return "Noviembre";
            case 12:
                return "Diciembre";

        }
        return "error";
    }

    public void setFecha(string s)
    {
        string[] datos = s.Split('/');
        buffer[5] = (byte)int.Parse(datos[0]);
        buffer[6] = (byte)int.Parse(datos[1]);
        buffer[7] = (byte)int.Parse(datos[2]);
    }

    public void setHora(string s)
    {
        string[] datos = s.Split(':');
        buffer[3] = (byte)int.Parse(datos[0]);
        buffer[2] = (byte)int.Parse(datos[1]);
        buffer[1] = (byte)int.Parse(datos[2]);
    }

    
    public void setClock()
    {
        //---------------------------------------------------------------------
        // Send data
        //---------------------------------------------------------------------
        buffer[4] = (byte)(dropdown.value+1);
        sendClient.Send(buffer, 8,sendPoint);
        //serialController.SendSerialMessage(buffer);
    }

    public void init(bool b)
    {
        bandera = b;
    }

    public void readIP(string s)
    {
        ip = s;
    }

    public void readIPsend(string s)
    {
        ipSend = s;
    }

    public void readPortClock(string s)
    {
        receivePortClock = int.Parse(s);
    }

    public void readPortBME(string s)
    {
        receivePortBME = int.Parse(s);
    }

    public void readPortSend(string s)
    {
        UDP_port = int.Parse(s);
    }
}