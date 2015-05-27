// EosClr.h

#pragma once
#include "Camera.h"

namespace EosClr
{
	public ref class CameraManager : IDisposable
	{
	public:
		CameraManager();
		~CameraManager();
		List<Camera^>^ GetCameraList();
	private:
		Camera^ CurrentCamera;
	};
}
