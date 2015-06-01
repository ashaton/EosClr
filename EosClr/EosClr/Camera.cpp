#include "stdafx.h"
#include "Camera.h"
#include "Utility.h"
#include "CameraNotConnectedException.h"
#include "IsoManager.h"
#include "ExposureTimeManager.h"

namespace EosClr
{
	Camera::Camera(EdsCameraRef CameraHandle)
	{
		this->CameraHandle = CameraHandle;
		_SupportedIsoSpeeds = gcnew List<IsoSpeed>();
		_SupportedExposureTimes = gcnew List<ExposureTime>();
		
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
		if (!_SupportedIsoSpeeds->Contains(Iso))
		{
			throw gcnew Exception("This camera doesn't support ISO " + Iso.ToString());
		}

		// Send it down to the camera.
		EdsUInt32 edsIsoValue = IsoManager::GetEdsIsoValue(Iso);
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_ISOSpeed, 0, sizeof(edsIsoValue), &edsIsoValue));
		_Iso = Iso;
		IsoChanged(Iso);
	}

	int Camera::PicturesRemaining::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		return _PicturesRemaining;
	}

	void Camera::PicturesRemaining::set(int PicturesRemaining)
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		_PicturesRemaining = PicturesRemaining;
		PicturesRemainingChanged(PicturesRemaining);
	}

	ExposureTime Camera::Exposure::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		return _Exposure;
	}

	void Camera::Exposure::set(ExposureTime Exposure)
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}

		// If the user sets it to the value that's already assigned, just ignore it.
		if (Exposure == _Exposure)
		{
			return;
		}
		if (!_SupportedExposureTimes->Contains(Exposure))
		{
			throw gcnew Exception("This camera doesn't support exposure " + Exposure.ToString());
		}

		// Send it down to the camera.
		EdsUInt32 edsExposureValue = ExposureTimeManager::GetEdsExposureValue(Exposure);
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Tv, 0, sizeof(edsExposureValue), &edsExposureValue));
		_Exposure = Exposure;
		ExposureTimeChanged(Exposure);
	}

	IEnumerable<IsoSpeed>^ Camera::SupportedIsoSpeeds::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		return _SupportedIsoSpeeds;
	}

	IEnumerable<ExposureTime>^ Camera::SupportedExposureTimes::get()
	{
		if (CurrentCamera != this)
		{
			throw gcnew CameraNotConnectedException();
		}
		return _SupportedExposureTimes;
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

		// Get the exposure times this camera supports upon first connect
		if (_SupportedExposureTimes->Count == 0)
		{
			EdsPropertyDesc exposureTimes;
			ErrorCheck(EdsGetPropertyDesc(CameraHandle, kEdsPropID_Tv, &exposureTimes));
			for (int i = 0; i < exposureTimes.numElements; i++)
			{
				EdsInt32 exposureValue = exposureTimes.propDesc[i];
				ExposureTime exposure = ExposureTimeManager::GetExposureTime(exposureValue);
				_SupportedExposureTimes->Add(exposure);
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
				_Iso = IsoManager::GetIsoSpeed(currentIsoValue);
				IsoChanged(_Iso);
				break;
			}
			case kEdsPropID_AvailableShots: // How many pictures can fit in the available disk space
			{
				EdsUInt32 numShots;
				ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_AvailableShots, 0, sizeof(numShots), &numShots));
				_PicturesRemaining = numShots;
				PicturesRemainingChanged(_PicturesRemaining);
				break;
			}
			case kEdsPropID_Tv: // Exposure time
			{
				EdsUInt32 exposure;
				ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_Tv, 0, sizeof(exposure), &exposure));
				_Exposure = ExposureTimeManager::GetExposureTime(exposure);
				ExposureTimeChanged(_Exposure);
				break;
			}
			default: // Anything that we haven't handled yet gets printed to the debug event
				PropertyChanged("Prop = " + PropertyID.ToString("X") + ", Param = " + Param.ToString("X"));
				break;
		}
	}
}
