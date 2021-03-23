using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;

public class SampleBinario : MonoBehaviour
{
    public SerialControllerBinario serialController;

    // Initialization
    void Start()
    {
        serialController = GameObject.Find("SerialController").GetComponent<SerialControllerBinario>();
        
    }

    // Executed each frame
    void Update()
    {


        //---------------------------------------------------------------------
        // Receive data
        //---------------------------------------------------------------------

        byte[] message = serialController.ReadSerialMessage();

        if (message == null)
            return;
        StringBuilder sb = new StringBuilder();
        sb.Append("Packet: ");
        foreach (byte data in message)
        {
            sb.Append(data.ToString("X2") + " ");
        }
        Debug.Log(sb);
    }
}