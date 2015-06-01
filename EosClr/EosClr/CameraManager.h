// EosClr.h

#pragma once
#include "Camera.h"

namespace EosClr
{
	/// <summary>
	/// CameraManager is the entry point into the EOS remote control system. Use this
	/// to get the list of currently connected Canon devices.
	/// </summary>
	public ref class CameraManager abstract sealed
	{
	public:
		/// <summary>
		/// Initializes the EOS API so it can be used to interact with cameras. This must
		/// be called before anything else.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Closes the EOS API. Call this when your application is shutting down, or when
		/// you no longer need to use the API.
		/// </summary>
		static void Close();

		/// <summary>
		/// Returns a collection of Canon devices that are currently connected to the host.
		/// </summary>
		/// <returns>A collection of connected Canon cameras</returns>
		static IEnumerable<Camera^>^ GetCameraList();
	};
}
