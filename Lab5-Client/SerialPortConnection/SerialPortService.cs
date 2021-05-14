using System;
using System.IO.Ports;

namespace SerialPortConnection
{
    public class SerialPortService
    {
        private static readonly SerialPort _serialPort = new SerialPort("COM1",
            9600, Parity.None, 8, StopBits.One);
        
        public void SubscribeHandlerToDataReceivedEvent(SerialDataReceivedEventHandler handler)
        {
            _serialPort.DataReceived += handler;
        }

        public void OpenPort()
        {
            _serialPort.Open();
        }

        public void ClosePort()
        {
            _serialPort.Close();
        }

        public void EditPortSettings(string portName, int baudRate, int dataBits)
        {
            _serialPort.PortName = portName;
            _serialPort.BaudRate = baudRate;
            _serialPort.DataBits = dataBits;
        }

        public SerialPort GetSerialPort()
        {
            return _serialPort;
        }

    }
}