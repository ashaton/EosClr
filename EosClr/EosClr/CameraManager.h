// EosClr.h

#pragma once
#include "Camera.h"

namespace EosClr
{
	public ref class CameraManager abstract sealed
	{
	public:
		static void Initialize();
		static void Close();
		static List<Camera^>^ GetCameraList();
	};
}
