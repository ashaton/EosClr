using EosClr;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace TestUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public Camera ActiveCamera { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            CameraManager.Initialize();
            IEnumerable<Camera> cameras = CameraManager.GetCameraList();
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
            ActiveCamera.IsoChanged += ActiveCamera_IsoChanged;
            ActiveCamera.ExposureTimeChanged += ActiveCamera_ExposureTimeChanged;
            ActiveCamera.PicturesRemainingChanged += ActiveCamera_PicturesRemainingChanged;
            ActiveCamera.SupportedIsoSpeedsChanged += ActiveCamera_SupportedIsoSpeedsChanged;
            ActiveCamera.SupportedExposureTimesChanged += ActiveCamera_SupportedExposureTimesChanged;

            ActiveCamera.Connect();
        }

        void ActiveCamera_SupportedExposureTimesChanged(IEnumerable<ExposureTime> SupportedExposureTimes)
        {
            ExposureBox.ItemsSource = SupportedExposureTimes;
            ExposureBox.Items.Refresh();
        }

        void ActiveCamera_SupportedIsoSpeedsChanged(IEnumerable<IsoSpeed> SupportedIsoSpeeds)
        {
            IsoBox.ItemsSource = SupportedIsoSpeeds;
            IsoBox.Items.Refresh();
        }

        void ActiveCamera_ExposureTimeChanged(ExposureTime Exposure)
        {
            ExposureBox.SelectedItem = Exposure;
        }

        void ActiveCamera_PicturesRemainingChanged(int PicturesRemaining)
        {
            PicturesLeftLabel.Content = "Pictures Left: [" + PicturesRemaining + "]";
        }

        void ActiveCamera_IsoChanged(IsoSpeed Iso)
        {
            IsoBox.SelectedItem = Iso;
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

        private void IsoBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ActiveCamera.Iso = (IsoSpeed)IsoBox.SelectedItem;
        }

        private void ExposureBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(e.RemovedItems.Count > 0)
            {
                ExposureTime oldSetting = (ExposureTime)e.RemovedItems[0];
                try
                {
                    ActiveCamera.Exposure = (ExposureTime)ExposureBox.SelectedItem;
                }
                catch (EosException)
                {
                    MessageBox.Show("That setting is invalid.", "Invalid setting", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    ExposureBox.SelectedItem = oldSetting;
                }
            }
        }
    }
}
