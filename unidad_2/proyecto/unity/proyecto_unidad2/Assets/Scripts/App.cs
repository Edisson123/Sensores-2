
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;


public class App : MonoBehaviour
{
    public Controller serialController;
    public Text text_fecha, text_hora, text_temp, text_pres, text_alt, text_hum;

    private float timer = 0.0f;
    private float waitTime = 1.0f;

    private Transform objTransform;
    private Vector3 scaleChange;
    byte[] buffer = { 03, 00, 00, 00, 00, 00, 00, 00 };
    public Dropdown dropdown;
    bool bandera = false;


    // Initialization
    void Start()
    {
        serialController = GameObject.Find("SerialController").GetComponent<Controller>();
        dropdown = GameObject.Find("menu_dia").GetComponent<Dropdown>();
  
    }

    // Executed each frame
    void Update()
    {

        if (bandera)
        {

            timer += Time.deltaTime;
            if (timer > waitTime)
            {
                timer = timer - waitTime;

                serialController.SendSerialMessage(new byte[] { 0x04 });
            }

            //---------------------------------------------------------------------
            // Receive data
            //---------------------------------------------------------------------

            byte[] message = serialController.ReadSerialMessage();

            if (message == null)
                return;

            if (message[0] == 0x4A)
            {
                /*
                Debug.Log("Segundos: " + message[1]);
                Debug.Log("minutos: " + message[2]);
                Debug.Log("Horas: " + message[3]);
                Debug.Log("wday: " + message[4]);
                Debug.Log("Dia: " + message[5]);
                Debug.Log("Mes: " + message[6]);
                Debug.Log("Año: " + message[7]);
                float temperatura = (float)System.BitConverter.ToSingle(message, 8);
                float presion = (float)System.BitConverter.ToSingle(message, 12);
                float altura = (float)System.BitConverter.ToSingle(message, 16);
                float humedad = (float)System.BitConverter.ToSingle(message, 20);
                Debug.Log("Temperatura: " + temperatura);
                Debug.Log("Presion: " + presion);
                Debug.Log("Altura: " + altura);
                Debug.Log("Humedad: " + humedad);
                */
                text_fecha.text = dia_semana(message[4]) + " " + message[5] + " de " + mes_año(message[6]) + " del " + message[7];
                text_hora.text = message[3] + ":" + message[2] + ":" + message[1];
                float temperatura = (float)System.BitConverter.ToSingle(message, 8);
                float presion = (float)System.BitConverter.ToSingle(message, 12);
                float altura = (float)System.BitConverter.ToSingle(message, 16);
                float humedad = (float)System.BitConverter.ToSingle(message, 20);

                text_temp.text = temperatura.ToString();
                text_pres.text = presion.ToString();
                text_alt.text = altura.ToString();
                text_hum.text = humedad.ToString();

            }
            else if (message[0] == 0x05)
            {
                Debug.Log("Error de lectura");
            }
            else
            {
                Debug.Log("Error desconocido xD");
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
        serialController.SendSerialMessage(buffer);
    }

    public void init(bool b)
    {
        bandera = b;
    }
}