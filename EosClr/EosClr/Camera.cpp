#include "stdafx.h"
#include "Camera.h"
#include "Utility.h"
#include "CameraNotConnectedException.h"
#include "IsoManager.h"

namespace EosClr
{
	Camera::Camera(EdsCameraRef CameraHandle)
	{
		this->CameraHandle = CameraHandle;
		_SupportedIsoSpeeds = gcnew List<IsoSpeed>();
		
		// Initialize the camera and its details
		EdsDeviceInfo deviceInfo;
		ErrorCheck(EdsGetDeviceInfo(CameraHandle, &deviceInfo));
		Name = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szDeviceDescription);
		Port = Marshal::PtrToStringAnsi((IntPtr)deviceInfo.szPortName);
		Protocol = static_cast<ConnectionProtocol>(deviceInfo.deviceSubType);

		// Subscribe to the property change event
		Handler = gcnew PropertyEventHandlerDelegate(this, &Camera::OnPropertyEvent);
		IntPtr delegatePointer = Marshal::GetFunctionPointerForDelegate(Handler);
		EdsPropertyEventHandler handler = static_cast<EdsPropertyEventHandler>(delegatePointer.ToPointer());
		ErrorCheck(EdsSetPropertyEventHandler(CameraHandle, kEdsPropertyEvent_All, handler, NULL));
	}

	String^ Camera::Name::get()
	{
		return _Name;
	}

	void Camera::Name::set(String^ Name)
	{
		_Name = Name;
	}

	String^ Camera::Port::get()
	{
		return _Port;
	}

	void Camera::Port::set(String^ Port)
	{
		_Port = Port;
	}

	ConnectionProtocol Camera::Protocol::get()
	{
		return _Protocol;
	}

	void Camera::Protocol::set(ConnectionProtocol Protocol)
	{
		_Protocol = Protocol;
	}

	IsoSpeed Camera::Iso::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		return _Iso;
	}

	void Camera::Iso::set(IsoSpeed Iso)
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		// If the user sets it to the value that's already assigned, just ignore it.
		if (Iso == _Iso)
		{
			return;
		}

		// Otherwise, send it down to the camera.
		EdsUInt32 edsIsoValue = IsoManager::GetEdsIsoValue(Iso);
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_ISOSpeed, 0, sizeof(edsIsoValue), &edsIsoValue));
		_Iso = Iso;
	}

	IEnumerable<IsoSpeed>^ Camera::SupportedIsoSpeeds::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		return _SupportedIsoSpeeds;
	}

	void Camera::Connect()
	{
		if (CurrentCamera != nullptr)
		{
			CurrentCamera->Disconnect();
		}
		CurrentCamera = this;
		ErrorCheck(EdsOpenSession(CameraHandle));

		// Get the ISO settings this camera supports upon first connect
		if (_SupportedIsoSpeeds->Count == 0)
		{
			EdsPropertyDesc isoSpeeds;
			ErrorCheck(EdsGetPropertyDesc(CameraHandle, kEdsPropID_ISOSpeed, &isoSpeeds));
			for (int i = 0; i < isoSpeeds.numElements; i++)
			{
				EdsInt32 speedValue = isoSpeeds.propDesc[i];
				IsoSpeed speed = IsoManager::GetIsoSpeed(speedValue);
				_SupportedIsoSpeeds->Add(speed);
			}
		}
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

		// This is pretty much taken straight from the reference docs.
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

		// This is also taken straight from the reference docs.
		if (LiveViewImage == NULL || LiveViewStream == NULL)
		{
			return;
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

	EdsError Camera::OnPropertyEvent(EdsPropertyEvent EventType,
		EdsPropertyID PropertyID,
		EdsUInt32 Param,
		EdsVoid* Context)
	{
		if (EventType == kEdsPropertyEvent_PropertyChanged)
		{
			OnPropertyValueChanged(PropertyID, Param);
		}
		return EDS_ERR_OK;
	}

	void Camera::OnPropertyValueChanged(EdsPropertyID PropertyID, EdsUInt32 Param)
	{
		switch (PropertyID)
		{
			case kEdsPropID_ISOSpeed: // ISO Speed
			{
				EdsUInt32 currentIsoValue;
				ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_ISOSpeed, 0, sizeof(currentIsoValue), &currentIsoValue));
				IsoSpeed newSpeed = IsoManager::GetIsoSpeed(currentIsoValue);
				Iso = newSpeed;
				IsoChanged(newSpeed);
				break;
			}
			case kEdsPropID_AvailableShots: // How many pictures can fit in the available disk space
			{
				EdsUInt32 numShots;
				ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_AvailableShots, 0, sizeof(numShots), &numShots));
				NumberOfPicturesLeftChanged(numShots);
				break;
			}
			default: // Anything that we haven't handled yet gets printed to the debug event
				PropertyChanged("Prop = " + PropertyID.ToString("X") + ", Param = " + Param.ToString("X"));
				break;
		}
	}
}
