using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EosClr;

namespace TestConsole
{
    class Program
    {
        private static List<Camera> Cameras;

        static void Main(string[] args)
        {
            try
            {
                CameraManager.Initialize();
                Cameras = CameraManager.GetCameraList();
                PrintCameraList();
                Console.WriteLine("Initialized, ready to test!");
                Camera activeCamera = SelectCameraPrompt();
                PromptForAction(activeCamera);
                if(activeCamera != null)
                {
                    activeCamera.Disconnect();
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine("Error: " + ex.GetType().Name + " - " + ex.Message + 
                    Environment.NewLine + ex.StackTrace);
            }
            CameraManager.Close();
            Console.WriteLine("Closed.");
            Console.ReadLine();
        }

        static void PrintCameraList()
        {
            for (int i = 0; i < Cameras.Count; i++)
            {
                Camera camera = Cameras[i];
                Console.WriteLine("--- Camera " + i + " ---");
                Console.WriteLine("\tName: " + camera.Name);
                Console.WriteLine("\tPort: " + camera.Port);
                Console.WriteLine("\tType: " + camera.Type);
                Console.WriteLine();
            }
        }

        static Camera SelectCameraPrompt()
        {
            Console.Write("Select a camera: ");
            string input = Console.ReadLine();
            int index;
            while(!Int32.TryParse(input, out index) || index < 0 || index >= Cameras.Count)
            {
                Console.Write("Invalid choice, try again: ");
                input = Console.ReadLine();
            }
            Camera activeCamera = Cameras[index];
            activeCamera.Connect();
            Console.WriteLine("Camera " + index + " active.");
            return activeCamera;
        }

        static void PromptForAction(Camera ActiveCamera)
        {
            Console.Write("> ");
            string command = Console.ReadLine();
            while(true)
            {
                switch(command)
                {
                    case "q":
                        return;
                    case "lv start":
                        ActiveCamera.ActivateLiveView();
                        break;
                    case "lv stop":
                        ActiveCamera.DeactivateLiveView();
                        break;
                }
                Console.Write("> ");
                command = Console.ReadLine();
            }
        }
    }
}
