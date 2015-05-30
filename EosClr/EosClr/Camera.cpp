#include "stdafx.h"
#include "Camera.h"
#include "Utility.h"
#include "CameraNotConnectedException.h"

namespace EosClr
{
	Camera::Camera(EdsCameraRef CameraHandle)
	{
		this->CameraHandle = CameraHandle;
		EdsDeviceInfo deviceInfo;
		ErrorCheck(EdsGetDeviceInfo(CameraHandle, &deviceInfo));
		Name = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szDeviceDescription);
		Port = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szPortName);
		Type = static_cast<CameraType>(deviceInfo.deviceSubType);
	}

	void Camera::Connect()
	{
		if (CurrentCamera != nullptr)
		{
			CurrentCamera->Disconnect();
		}
		ErrorCheck(EdsOpenSession(CameraHandle));
		CurrentCamera = this;
	}

	void Camera::Disconnect()
	{
		if (CurrentCamera == this)
		{
			ErrorCheck(EdsCloseSession(CameraHandle));
			CurrentCamera = nullptr;
		}
	}

	void Camera::ActivateLiveView()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		EdsUInt32 propValue;
		ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		propValue |= kEdsEvfOutputDevice_PC;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		
		pin_ptr<EdsStreamRef> pinnedLiveViewStream = &LiveViewStream;
		pin_ptr<EdsEvfImageRef> pinnedLiveViewImage = &LiveViewImage;
		ErrorCheck(EdsCreateMemoryStream(0, pinnedLiveViewStream));
		ErrorCheck(EdsCreateEvfImageRef(LiveViewStream, pinnedLiveViewImage));

		// TODO: DOWNLOAD IMAGES ON A SEPARATE THREAD
		//ErrorCheck(EdsDownloadEvfImage(CameraHandle, ))
	}

	void Camera::DeactivateLiveView()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		ErrorCheck(EdsRelease(LiveViewImage));
		ErrorCheck(EdsRelease(LiveViewStream));
		LiveViewImage = NULL;
		LiveViewStream = NULL;

		EdsUInt32 propValue;
		ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		propValue &= ~kEdsEvfOutputDevice_PC;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
	}
}
