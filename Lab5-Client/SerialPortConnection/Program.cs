using System;
using System.Diagnostics;
using System.IO.Ports;

namespace SerialPortConnection
{
    static class Program
    {
        private static SerialPortService _service;
        
        static void Main(string[] args)
        {
            _service = new SerialPortService();
            
            Console.WriteLine("Введіть назву порту (напр. COM1):");
            string port = Console.ReadLine();
            Console.WriteLine("Введіть швидкість порту (напр. 115200):");
            int speed = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("Біти даних (напр. 8):");
            int bits = Convert.ToInt32(Console.ReadLine());

            _service.EditPortSettings(port, speed, bits);
            _service.SubscribeHandlerToDataReceivedEvent(port_DataReceived);

            Console.WriteLine("Введіть будь-який символ, щоб читати порт");
            Console.ReadLine();
            try
            {
                _service.OpenPort();
            }
            catch (Exception e)
            {
                Console.WriteLine("Порт зайнятий");
            }
            Console.ReadLine();
            _service.ClosePort();
        }

        private static void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var serialPort = _service.GetSerialPort(); // next you can read info from this port
            
            byte[] buffer = new byte[4];
            serialPort.Read(buffer, 0, 4);
            float amplitude = BitConverter.ToSingle(buffer, 0);
            
            Console.WriteLine("Температура - " + amplitude + " гр. C");
        }
    }
}