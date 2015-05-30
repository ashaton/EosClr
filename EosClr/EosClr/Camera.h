
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
		initonly String^ Name;
		initonly String^ Port;
		initonly CameraType Type;

		void Connect();
		void Disconnect();
		void ActivateLiveView();
		void DeactivateLiveView();

	internal:
		Camera(EdsCameraRef CameraHandle);
	
	private:
		EdsCameraRef CameraHandle;
		EdsStreamRef LiveViewStream;
		EdsEvfImageRef LiveViewImage;
		static Camera^ CurrentCamera;
	};

}
