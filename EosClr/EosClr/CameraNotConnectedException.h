
#pragma once

namespace EosClr
{
	/// <summary>
	/// This is thrown when the user tries to access a camera that 
	/// isn't currently connected. Call Camera.Connect() to connect it.
	/// </summary>
	public ref class CameraNotConnectedException : Exception
	{
	public:
		/// <summary>
		/// Creates a new CameraNotConnectedException.
		/// </summary>
		CameraNotConnectedException();
	};
}