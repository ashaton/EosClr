using EosClr;
using System;
using System.Collections.Generic;
using System.Linq;
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

namespace TestUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Camera ActiveCamera;

        public MainWindow()
        {
            InitializeComponent();
            CameraManager.Initialize();
            List<Camera> cameras = CameraManager.GetCameraList();
            CameraSelectBox.ItemsSource = cameras;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(ActiveCamera != null)
            {
                ActiveCamera.Disconnect();
            }
            CameraManager.Close();
        }

        private void CameraSelectBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ActiveCamera = (Camera)CameraSelectBox.SelectedItem;
            ActiveCamera.PropertyChanged += OnCameraPropertyChanged;
            ActiveCamera.Connect();
        }

        void OnCameraPropertyChanged(string Message)
        {
            LogBox.Text += Message + Environment.NewLine;
            LogScroller.ScrollToEnd();
        }

        private void StartLiveViewButton_Click(object sender, RoutedEventArgs e)
        {
            ActiveCamera.ActivateLiveView();
        }

        private void StopLiveViewButton_Click(object sender, RoutedEventArgs e)
        {
            ActiveCamera.DeactivateLiveView();
        }
    }
}
