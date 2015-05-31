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

		Handler = gcnew PropertyEventHandlerDelegate(this, &Camera::OnPropertyEvent);
		IntPtr delegatePointer = Marshal::GetFunctionPointerForDelegate(Handler);
		EdsPropertyEventHandler handler = (EdsPropertyEventHandler)delegatePointer.ToPointer();
		ErrorCheck(EdsSetPropertyEventHandler(CameraHandle, kEdsPropertyEvent_All, handler, NULL));
		
		Name = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szDeviceDescription);
		Port = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szPortName);
		Type = static_cast<CameraType>(deviceInfo.deviceSubType);
	}

	String^ Camera::Name::get()
	{
		return _Name;
	}

	void Camera::Name::set(String^ NewName)
	{
		_Name = NewName;
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

	int Camera::ZoomLevel::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		EdsUInt32 zoomLevel;
		EdsError result = EdsGetPropertyData(LiveViewImage, kEdsPropID_Evf_Zoom, 0, sizeof(zoomLevel), &zoomLevel);
		if (result != EDS_ERR_OK)
		{
			zoomLevel = result * -1;
		}
		return zoomLevel;
	}

	void Camera::ZoomLevel::set(int NewLevel)
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		EdsUInt32 zoomLevel = NewLevel;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Evf_Zoom, 0, sizeof(zoomLevel), &zoomLevel));
	}

	String^ Camera::CropPosition::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		EdsPoint point;
		EdsError result = EdsGetPropertyData(LiveViewImage, kEdsPropID_Evf_ImagePosition, 0, sizeof(point), &point);
		return point.x + ", " + point.y;
	}

	int Camera::Mode::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		EdsUInt32 mode;
		ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_AEMode, 0, sizeof(mode), &mode));
		return mode;
	}

	void Camera::Mode::set(int Mode)
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		EdsUInt32 mode = Mode;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_AEModeSelect, 0, sizeof(mode), &mode));
	}

	EdsError Camera::OnPropertyEvent(EdsPropertyEvent inEvent,
		EdsPropertyID inPropertyID,
		EdsUInt32 inParam,
		IntPtr inContext)
	{
		PropertyChanged("Event = " + inEvent.ToString("X") + ", Prop = " + inPropertyID.ToString("X") + ", Param = " + inParam.ToString("X"));
		/*Console::WriteLine("PROP EVENT HAPPENED");
		Console::WriteLine("EVENT = " + inEvent.ToString("X"));
		Console::WriteLine("PROP = " + inPropertyID.ToString("X"));
		Console::WriteLine("PARAM = " + inParam.ToString("X"));*/
		return EDS_ERR_OK;
	}
}
