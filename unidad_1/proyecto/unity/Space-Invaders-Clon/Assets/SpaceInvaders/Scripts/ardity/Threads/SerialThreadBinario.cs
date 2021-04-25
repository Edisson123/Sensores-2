using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

using System.Text;

public class SerialThreadBinario: AbstractSerialThread
{
    // Buffer where a single message must fit
    private byte[] buffer = new byte[1024];
    //private int bufferUsed = 0;
    private const int PRESET_VALUE = 0x0000FFFF;
    private const int POLYNOMIAL = 0x00008408;


    public SerialThreadBinario(string portName,
                                       int baudRate,
                                       int delayBeforeReconnecting,
                                       int maxUnreadMessages)
        : base(portName, baudRate, delayBeforeReconnecting, maxUnreadMessages, false)
    {

    }

    protected override void SendToWire(object message, SerialPort serialPort)
    {
        byte[] binaryMessage = (byte[])message;
        serialPort.Write(binaryMessage, 0, binaryMessage.Length);
    }

    protected override object ReadFromWire(SerialPort serialPort)
    {
        if (serialPort.BytesToRead > 0)
        {
            serialPort.Read(buffer, 0, 1);


            return buffer;
            
        }
        else
        {
            return null;
        }
    }


}