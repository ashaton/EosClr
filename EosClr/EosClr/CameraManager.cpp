// This is the main DLL file.

#include "stdafx.h"
#include "CameraManager.h"
#include "Utility.h"

namespace EosClr
{
	CameraManager::CameraManager()
	{
		//LoopTask = Task::Factory->StartNew(gcnew Action(&MessageLoop));
	}

	void CameraManager::Initialize()
	{
		ErrorCheck(EdsInitializeSDK());
	}

	void CameraManager::Close()
	{
		//LoopTask->Wait();
		ErrorCheck(EdsTerminateSDK());
	}

	List<Camera^>^ CameraManager::GetCameraList()
	{
		EdsCameraListRef cameraHandleList;
		EdsUInt32 cameraCount;
		List<Camera^>^ cameraList = gcnew List<Camera^>();

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

	void CameraManager::MessageLoop()
	{
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		MSG winMessage;
		BOOL result = GetMessage(&winMessage, NULL, 0, 0);
		while (true)
		{
			if (result == 0)
			{
				break;
			}
			else if (result == -1)
			{
				Console::WriteLine("MESSAGE LOOP ERROR: " + GetLastError().ToString("X"));
				break;
			}
			TranslateMessage(&winMessage);
			DispatchMessage(&winMessage);
			result = GetMessage(&winMessage, NULL, 0, 0);
		}
		CoUninitialize();
	}
}