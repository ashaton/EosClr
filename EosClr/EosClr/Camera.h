
#pragma once

namespace EosClr
{
	public enum class CameraType
	{
		WindowsPtp = 0,
		CanonPtp = 1,
		CanonPtpIp = 2
	};

	public ref class Camera
	{
	public:
		property String^ Name
		{
			String^ get();
		private:
			void set(String^);
		};

		initonly String^ Port;
		initonly CameraType Type;
		property int ZoomLevel
		{
			int get();
			void set(int);
		};
		property String^ CropPosition
		{
			String^ get();
		}
		property int Mode
		{
			int get();
			void set(int);
		}

		event Action<String^>^ PropertyChanged;

		void Connect();
		void Disconnect();
		void ActivateLiveView();
		void DeactivateLiveView();

	internal:
		Camera(EdsCameraRef CameraHandle);
	
	private:
		String^ _Name;
		EdsCameraRef CameraHandle;
		EdsStreamRef LiveViewStream;
		EdsEvfImageRef LiveViewImage;
		static Camera^ CurrentCamera;
		
		delegate EdsError PropertyEventHandlerDelegate(EdsPropertyEvent, EdsPropertyID, EdsUInt32, IntPtr);
		PropertyEventHandlerDelegate^ Handler;

		EdsError OnPropertyEvent(EdsPropertyEvent inEvent,
			EdsPropertyID inPropertyID,
			EdsUInt32 inParam,
			IntPtr inContext);
	};

}
