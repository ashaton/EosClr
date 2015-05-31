
#pragma once

#include "IsoSpeed.h"

namespace EosClr
{
	public enum class CameraType
	{
		WindowsPtp = 0,
		CanonPtp = 1,
		CanonPtpIp = 2
	};

	public ref class Camera
	{
	public:
		property String^ Name
		{
			String^ get();
		private:
			void set(String^);
		};

		initonly String^ Port;
		initonly CameraType Type;

		property int ZoomLevel
		{
			int get();
			void set(int);
		};

		property String^ CropPosition
		{
			String^ get();
		}

		property int Mode
		{
			int get();
			void set(int);
		}

		property IsoSpeed Iso
		{
			IsoSpeed get();
			void set(IsoSpeed);
		}

		property IEnumerable<IsoSpeed>^ SupportedIsoSpeeds
		{
			IEnumerable<IsoSpeed>^ get();
		}

		event Action<IsoSpeed>^ IsoChanged;
		event Action<int>^ NumberOfPicturesLeftChanged;
		event Action<String^>^ PropertyChanged;

		void Connect();
		void Disconnect();
		void ActivateLiveView();
		void DeactivateLiveView();

	internal:
		Camera(EdsCameraRef CameraHandle);
	
	private:
		String^ _Name;
		IsoSpeed _Iso;
		EdsCameraRef CameraHandle;
		EdsStreamRef LiveViewStream;
		EdsEvfImageRef LiveViewImage;
		List<IsoSpeed>^ _SupportedIsoSpeeds;

		static Camera^ CurrentCamera;
		
		delegate EdsError PropertyEventHandlerDelegate(EdsPropertyEvent, EdsPropertyID, EdsUInt32, EdsVoid*);
		PropertyEventHandlerDelegate^ Handler;

		EdsError OnPropertyEvent(EdsPropertyEvent EventType,
			EdsPropertyID PropertyID,
			EdsUInt32 Param,
			EdsVoid* Context);

		void OnPropertyValueChanged(EdsPropertyID PropertyID, EdsUInt32 Param);
	};

}
