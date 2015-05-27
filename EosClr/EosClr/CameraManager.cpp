// This is the main DLL file.

#include "stdafx.h"
#include "CameraManager.h"
#include "Utility.h"

namespace EosClr
{
	CameraManager::CameraManager()
	{
		ErrorCheck(EdsInitializeSDK());
	}

	CameraManager::~CameraManager()
	{
		ErrorCheck(EdsTerminateSDK());
	}

	List<Camera^>^ CameraManager::GetCameraList()
	{
		EdsCameraListRef cameraHandleList;
		EdsUInt32 cameraCount;
		List<Camera^>^ cameraList = gcnew List<Camera^>();

		ErrorCheck(EdsGetCameraList(&cameraHandleList));
		ErrorCheck(EdsGetChildCount(cameraHandleList, &cameraCount));
		for (int i = 0; i < cameraCount; i++)
		{
			EdsCameraRef cameraHandle;
			ErrorCheck(EdsGetChildAtIndex(cameraHandleList, i, &cameraHandle));
			cameraList->Add(gcnew Camera(cameraHandle));
		}

		return cameraList;
	}
}