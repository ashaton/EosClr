// This is the main DLL file.

#include "stdafx.h"
#include "CameraManager.h"
#include "Utility.h"

namespace EosClr
{
	void CameraManager::Initialize()
	{
		ErrorCheck(EdsInitializeSDK());
	}

	void CameraManager::Close()
	{
		ErrorCheck(EdsTerminateSDK());
	}

	IEnumerable<Camera^>^ CameraManager::GetCameraList()
	{
		EdsCameraListRef cameraHandleList;
		EdsUInt32 cameraCount;
		List<Camera^>^ cameraList = gcnew List<Camera^>();

		// Iterate through the camera handles and create a new Camera wrapper for each one.
		ErrorCheck(EdsGetCameraList(&cameraHandleList));
		ErrorCheck(EdsGetChildCount(cameraHandleList, &cameraCount));
		for (int i = 0; i < (int)cameraCount; i++)
		{
			EdsCameraRef cameraHandle;
			ErrorCheck(EdsGetChildAtIndex(cameraHandleList, i, &cameraHandle));
			cameraList->Add(gcnew Camera(cameraHandle));
		}

		return cameraList;
	}
}