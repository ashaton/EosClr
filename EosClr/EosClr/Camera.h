
#pragma once

#include "IsoSpeed.h"
#include "ExposureTime.h"

namespace EosClr
{
	/// <summary>
	/// The type of connection and protocol the camera is using.
	/// </summary>
	public enum class ConnectionProtocol
	{
		/// <summary>
		/// Standard WIA PTP
		/// </summary>
		WiaPtp = 0,

		/// <summary>
		/// PTP via Canon's own drivers
		/// </summary>
		CanonPtp = 1,

		/// <summary>
		/// PTP/IP over the network (for WiFi cameras)
		/// </summary>
		CanonPtpIp = 2
	};

	/// <summary>
	/// The Camera class implements all of the properties and methods used to read from and write to
	/// a Canon camera. Use this to interact with a physical camera that is connected to the host machine.
	/// </summary>
	public ref class Camera
	{
	public:
#pragma region Properties
		/// <summary>
		/// The camera's name
		/// </summary>
		property String^ Name
		{
			String^ get();
		private:
			void set(String^);
		};

		/// <summary>
		/// The USB, serial or other communications port the camera is currently connected to
		/// </summary>
		property String^ Port
		{
			String^ get();
		private:
			void set(String^);
		}

		/// <summary>
		/// The protocol the camera is using to communicate with the host
		/// </summary>
		property ConnectionProtocol Protocol
		{
			ConnectionProtocol get();
		private:
			void set(ConnectionProtocol);
		}

		/// <summary>
		/// The current ISO setting
		/// </summary>
		property IsoSpeed Iso
		{
			IsoSpeed get();
			void set(IsoSpeed);
		}

		/// <summary>
		/// The estimated number of pictures that can fit on the
		/// host's available disk space
		/// </summary>
		property int PicturesRemaining
		{
			int get();
		private:
			void set(int);
		}

		/// <summary>
		/// The current exposure time (shutter speed)
		/// </summary>
		property ExposureTime Exposure
		{
			ExposureTime get();
			void set(ExposureTime);
		}

		/// <summary>
		/// The ISO settings that this camera supports
		/// </summary>
		property IEnumerable<IsoSpeed>^ SupportedIsoSpeeds
		{
			IEnumerable<IsoSpeed>^ get();
		}

		/// <summary>
		/// The exposure times that this camera supports
		/// </summary>
		property IEnumerable<ExposureTime>^ SupportedExposureTimes
		{
			IEnumerable<ExposureTime>^ get();
		}
#pragma endregion

#pragma region Events
		/// <summary>
		/// This is called when the camera's ISO setting changed.
		/// </summary>
		event Action<IsoSpeed>^ IsoChanged;

		/// <summary>
		/// This is called when the estimated number of pictures remaining has changed.
		/// The estimate is based on the average picture size using the current settings,
		/// and the amount of hard drive space left on the host computer.
		/// </summary>
		event Action<int>^ PicturesRemainingChanged;

		/// <summary>
		/// This is called when the camera's exposure time (shutter speed) changed.
		/// </summary>
		event Action<ExposureTime>^ ExposureTimeChanged;

		/// <summary>
		/// This is called when the list of ISO speeds the camera currently supports
		/// has changed.
		/// </summary>
		event Action<IEnumerable<IsoSpeed>^>^ SupportedIsoSpeedsChanged;

		/// <summary>
		/// This is called when the list of exposure times the camera currently supports
		/// has changed.
		/// </summary>
		event Action<IEnumerable<ExposureTime>^>^ SupportedExposureTimesChanged;

		/// <summary>
		/// Debug event for notifications that haven't been implemented yet.
		/// </summary>
		event Action<String^>^ PropertyChanged;
#pragma endregion

#pragma region Methods
		/// <summary>
		/// Connects to this camera, so it can be controlled remotely. Only one
		/// camera can be connected at a time. If a different camera is currently
		/// connected, this will disconnect that one first.
		/// </summary>
		void Connect();

		/// <summary>
		/// Disconnects from the camera.
		/// </summary>
		void Disconnect();

		/// <summary>
		/// Activates the LiveView feed so the camera's EVF stream can be displayed
		/// on the host.
		/// </summary>
		void ActivateLiveView();

		/// <summary>
		/// Stops the LiveView stream.
		/// </summary>
		void DeactivateLiveView();
#pragma endregion

	internal:
		/// <summary>
		/// Creates a new Camera instance.
		/// </summary>
		/// <param name="CameraHandle">The EDSDK handle to the Camera</param>
		Camera(EdsCameraRef CameraHandle);
	
	private:
		/// <summary>
		/// The underlying field for the Name property
		/// </summary>
		String^ _Name;

		/// <summary>
		/// The underlying field for the Port property
		/// </summary>
		String^ _Port;

		/// <summary>
		/// The underlying field for the Protocol property
		/// </summary>
		ConnectionProtocol _Protocol;

		/// <summary>
		/// The underlying field for the Iso property
		/// </summary>
		IsoSpeed _Iso;

		/// <summary>
		/// The underlying field for the Exposure property
		/// </summary>
		ExposureTime _Exposure;

		/// <summary>
		/// The underlying field for the PicturesRemaining property
		/// </summary>
		int _PicturesRemaining;

		/// <summary>
		/// The underlying field for the SupportedIsoSpeeds property
		/// </summary>
		List<IsoSpeed>^ _SupportedIsoSpeeds;

		/// <summary>
		/// The underlying field for the SupportedExposureTimes property
		/// </summary>
		List<ExposureTime>^ _SupportedExposureTimes;

		/// <summary>
		/// The EDSDK object for the camera
		/// </summary>
		EdsCameraRef CameraHandle;

		/// <summary>
		/// The EDSDK object for the host's LiveView memory stream
		/// </summary>
		EdsStreamRef LiveViewStream;

		/// <summary>
		/// The EDSDK object for the LiveView image buffer
		/// </summary>
		EdsEvfImageRef LiveViewImage;

		/// <summary>
		/// The camera that's currently connected and open to the SDK
		/// </summary>
		static Camera^ CurrentCamera;

		Task^ LiveViewReadTask;

		bool LiveViewClosing;

		Object^ LiveViewClosingLock;

		void LiveViewReadLoop();

		/// <summary>
		/// Refreshes the list of ISO speeds that the camera currently supports.
		/// </summary>
		void RefreshSupportedIsoSpeeds();

		/// <summary>
		/// Refreshes the list of exposure times that the camera currently supports.
		/// This changes based on certain settings like the mode and LiveView's status.
		/// </summary>
		void RefreshSupportedExposureTimes();

		/// <summary>
		/// A managed version of the EdsPropertyEventHandler function signature.
		/// </summary>
		/// <param name="EventType">The type of event (this will either be 
		/// kEdsPropertyEvent_PropertyChanged or kEdsPropertyEvent_PropertyDescChanged)</param>
		/// <param name="PropertyID">The ID of the property that changed</param>
		/// <param name="Param">Some useful detail about the parameter being changed,
		/// this is kind of a nebulous thing that depends on the parameter. I'm not really
		/// sure what it does yet.</param>
		/// <param name="Context">This is a custom object that we pass to
		/// EdsSetPropertyEventHandler when we first register a function with the API.
		/// Whatever we use will get pushed to the handler in this parameter.</param>
		/// <returns>EDS_ERR_OK if everything worked, or some other EDS error in the
		/// event of a failure.</returns>
		delegate EdsError PropertyEventHandlerDelegate(EdsPropertyEvent EventType,
			EdsPropertyID PropertyID, EdsUInt32 Param, EdsVoid* Context);

		/// <summary>
		/// A handle to the property event handler delegate that gets created and registered
		/// with the API. We have to keep a handle to the delegate open, otherwise the GC
		/// will collect it and the function pointer won't be valid anymore. Since the EDSDK
		/// runs in unmanaged space, it will still try to call the function and throw a
		/// segfault if that happens.
		/// </summary>
		PropertyEventHandlerDelegate^ Handler;

		/// <summary>
		/// This is a handler for the various events that get fired when a property on the
		/// camera changes.
		/// </summary>
		/// <param name="EventType">The type of event (this will either be 
		/// kEdsPropertyEvent_PropertyChanged or kEdsPropertyEvent_PropertyDescChanged)</param>
		/// <param name="PropertyID">The ID of the property that changed</param>
		/// <param name="Param">Some useful detail about the parameter being changed,
		/// this is kind of a nebulous thing that depends on the parameter. I'm not really
		/// sure what it does yet.</param>
		/// <param name="Context">This doesn't currently get used.</param>
		/// <returns>EDS_ERR_OK</returns>
		EdsError OnPropertyEvent(EdsPropertyEvent EventType,
			EdsPropertyID PropertyID, EdsUInt32 Param, EdsVoid* Context);

		/// <summary>
		/// This handles the logic for dealing with a camera property change.
		/// </summary>
		/// <param name="PropertyID">The ID of the property that changed</param>
		/// <param name="Param">Some useful detail about the parameter being changed,
		/// this is kind of a nebulous thing that depends on the parameter. I'm not really
		/// sure what it does yet.</param>
		void OnPropertyValueChanged(EdsPropertyID PropertyID, EdsUInt32 Param);

		/// <summary>
		/// This handles the logic when a particular property's list of valid options has changed.
		/// </summary>
		/// <param name="PropertyID">The ID of the property options that changed</param>
		/// <param name="Param">Some useful detail about the property being updated,
		/// this is kind of a nebulous thing that depends on the property. I'm not really
		/// sure what it does yet.</param>
		void OnPropertyOptionsChanged(EdsPropertyID PropertyID, EdsUInt32 Param);
	};

}
