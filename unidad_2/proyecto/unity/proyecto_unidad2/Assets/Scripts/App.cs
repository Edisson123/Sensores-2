
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;

public class App : MonoBehaviour
{
    public Controller serialController;
    private float timer = 0.0f;
    private float waitTime = 1.0f;

    private Transform objTransform;
    private Vector3 scaleChange;


    // Initialization
    void Start()
    {
        serialController = GameObject.Find("SerialController").GetComponent<Controller>();
  
    }

    // Executed each frame
    void Update()
    {

        //---------------------------------------------------------------------
        // Send data
        //---------------------------------------------------------------------
        if (Input.GetKeyUp(KeyCode.Q))
        {

            byte[] buffer = { 03, 00, 09, 08, 04, 22, 04, 21 };
            Debug.Log("Se escribe el reloj enviando 0x03");
            serialController.SendSerialMessage(buffer);

            byte[] mensaje = serialController.ReadSerialMessage();
            
            /*
            if(mensaje[0] == 0x4A)
            {
                Debug.Log("Reloj escrito");
            }
            else
            {
                Debug.Log("Error de escritura");
            }
            */
        }

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

        if(message[0] == 0x4A)
        {
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

        }
        else if(message[0] == 0x05)
        {
            Debug.Log("Error de lectura");
        }
        else
        {
            Debug.Log("Error desconocido xD");
        }




    }
}