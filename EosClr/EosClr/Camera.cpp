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
		LiveViewClosingLock = gcnew Object();
		
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

		RefreshSupportedIsoSpeeds();
		RefreshSupportedExposureTimes();
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

		JpegDecompressor = tjInitDecompress();

		// This is pretty much taken straight from the reference docs.
		EdsUInt32 propValue;
		ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		propValue = kEdsEvfOutputDevice_PC;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		
		pin_ptr<EdsStreamRef> pinnedLiveViewStream = &LiveViewStream;
		pin_ptr<EdsEvfImageRef> pinnedLiveViewImage = &LiveViewImage;
		ErrorCheck(EdsCreateMemoryStream(0, pinnedLiveViewStream));
		ErrorCheck(EdsCreateEvfImageRef(LiveViewStream, pinnedLiveViewImage));
		LiveViewClosing = false;
		EdsSize size;
		ErrorCheck(EdsDownloadEvfImage(CameraHandle, LiveViewImage));
		ErrorCheck(EdsGetPropertyData(LiveViewImage, kEdsPropID_Evf_CoordinateSystem, 0, sizeof(size), &size));
		PropertyChanged("EVF Size! Height = " + size.height + ", width = " + size.width);
		LiveViewReadTask = Task::Factory->StartNew(gcnew Action(this, &Camera::LiveViewReadLoop));
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

		try
		{
			Monitor::Enter(LiveViewClosingLock);
			LiveViewClosing = true;
		}
		finally
		{
			Monitor::Exit(LiveViewClosingLock);
		}
		LiveViewReadTask->Wait();

		tjDestroy(JpegDecompressor);

		ErrorCheck(EdsRelease(LiveViewImage));
		ErrorCheck(EdsRelease(LiveViewStream));
		LiveViewImage = NULL;
		LiveViewStream = NULL;

		EdsUInt32 propValue;
		ErrorCheck(EdsGetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
		propValue = kEdsEvfOutputDevice_TFT;
		ErrorCheck(EdsSetPropertyData(CameraHandle, kEdsPropID_Evf_OutputDevice, 0, sizeof(propValue), &propValue));
	}

	int imgCounter = 0;

	void Camera::LiveViewReadLoop()
	{
		while (true)
		{
			// Check to see if LiveView is closing, and return if it is.
			try
			{
				Monitor::Enter(LiveViewClosingLock);
				if (LiveViewClosing)
				{
					return;
				}
			}
			finally
			{
				Monitor::Exit(LiveViewClosingLock);
			}

			// Download the image from the camera
			EdsError result = EdsDownloadEvfImage(CameraHandle, LiveViewImage);
			if (result == EDS_ERR_OBJECT_NOTREADY)
			{
				Thread::Sleep(500);
				continue;
			}
			else if (result == EDS_ERR_CANNOT_MAKE_OBJECT)
			{
				PropertyChanged("Can't make object :/");
				Thread::Sleep(1000);
				continue;
			}
			ErrorCheck(result);

			EdsUInt32 streamPos;
			EdsVoid* streamPtr;
			EdsImageRef imageRef;
			EdsImageInfo imageInfo;
			ErrorCheck(EdsGetPosition(LiveViewStream, &streamPos));
			ErrorCheck(EdsGetPointer(LiveViewStream, &streamPtr));
			ErrorCheck(EdsCreateImageRef(LiveViewStream, &imageRef));
			ErrorCheck(EdsGetImageInfo(imageRef, kEdsImageSrc_FullView, &imageInfo));
			
			int bytesPerPixel = imageInfo.componentDepth / 8;
			int numberOfChannels = imageInfo.numOfComponents;
			int width = imageInfo.width;
			int height = imageInfo.height;
			int totalSize = width * height * numberOfChannels * bytesPerPixel;

			unsigned char* jpegBuffer = reinterpret_cast<unsigned char*>(streamPtr);



			//tjDecompress2(JpegDecompressor, jpegBuffer, totalSize, NULL, width, 0, height, TJPF_RGB, TJFLAG_FASTDCT);

			array<unsigned char>^ imgBytes = gcnew array<unsigned char>(streamPos);
			Marshal::Copy((IntPtr)streamPtr, imgBytes, 0, imgBytes->Length);
			System::IO::File::WriteAllBytes("C:\\kappa\\img" + (imgCounter++) + ".jpg", imgBytes);

			//ErrorCheck(EdsGetLength(LiveViewStream, &streamLength));
			PropertyChanged("New image!" + Environment::NewLine + 
				"\tPos = " + streamPos + Environment::NewLine + 
				"\tPtr = " + ((unsigned int)streamPtr).ToString("X") + Environment::NewLine + 
				"\tHeight = " + imageInfo.height + Environment::NewLine +
				"\tWidth = " + imageInfo.width);
			Thread::Sleep(1000);
		}
	}

	void Camera::RefreshSupportedIsoSpeeds()
	{
		_SupportedIsoSpeeds->Clear();
		EdsPropertyDesc isoSpeeds;
		ErrorCheck(EdsGetPropertyDesc(CameraHandle, kEdsPropID_ISOSpeed, &isoSpeeds));
		for (int i = 0; i < isoSpeeds.numElements; i++)
		{
			EdsInt32 speedValue = isoSpeeds.propDesc[i];
			IsoSpeed speed = IsoManager::GetIsoSpeed(speedValue);
			_SupportedIsoSpeeds->Add(speed);
		}
	}

	void Camera::RefreshSupportedExposureTimes()
	{
		_SupportedExposureTimes->Clear();
		EdsPropertyDesc exposureTimes;
		ErrorCheck(EdsGetPropertyDesc(CameraHandle, kEdsPropID_Tv, &exposureTimes));
		for (int i = 0; i < exposureTimes.numElements; i++)
		{
			EdsInt32 exposureValue = exposureTimes.propDesc[i];
			ExposureTime exposure = ExposureTimeManager::GetExposureTime(exposureValue);
			_SupportedExposureTimes->Add(exposure);
		}
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
		else
		{
			OnPropertyOptionsChanged(PropertyID, Param);
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
				PropertyChanged("PropertyValueChanged, Prop = " + PropertyID.ToString("X") + ", Param = " + Param.ToString("X"));
				break;
		}
	}

	void Camera::OnPropertyOptionsChanged(EdsPropertyID PropertyID, EdsUInt32 Param)
	{
		switch (PropertyID)
		{
		case kEdsPropID_ISOSpeed: // ISO Speed
		{
			RefreshSupportedIsoSpeeds();
			SupportedIsoSpeedsChanged(_SupportedIsoSpeeds);
			break;
		}
		case kEdsPropID_Tv: // Exposure time
		{
			RefreshSupportedExposureTimes();
			SupportedExposureTimesChanged(_SupportedExposureTimes);
			break;
		}
		case kEdsPropID_Evf_AFMode:
		{
			EdsPropertyDesc afModes;
			ErrorCheck(EdsGetPropertyDesc(CameraHandle, kEdsPropID_Evf_AFMode, &afModes));
			PropertyChanged("AF MODE OPTIONS:");
			for (int i = 0; i < afModes.numElements; i++)
			{
				EdsInt32 mode = afModes.propDesc[i];
				PropertyChanged("\t" + mode.ToString("X"));
			}
		}
		default: // Anything that we haven't handled yet gets printed to the debug event
			PropertyChanged("PropertyOptionsChanged, Prop = " + PropertyID.ToString("X") + ", Param = " + Param.ToString("X"));
			break;
		}
	}
}
