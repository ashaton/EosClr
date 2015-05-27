
#pragma once

namespace EosClr
{
	public ref class Camera
	{
	public:
	internal:
		Camera(EdsCameraRef CameraHandle);
	private:
		EdsCameraRef CameraHandle;
	};

}
