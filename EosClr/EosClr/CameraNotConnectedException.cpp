#include "stdafx.h"
#include "CameraNotConnectedException.h"

namespace EosClr
{
	CameraNotConnectedException::CameraNotConnectedException()
		: Exception("This camera is not currently connected.")
	{

	}
}