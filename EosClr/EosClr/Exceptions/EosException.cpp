#include "stdafx.h"
#include "EosException.h"

namespace EosClr
{
	EosException::EosException(EdsError Error)
		: Exception("ERROR CODE " + Error.ToString("X"))
	{
		this->Error = Error;
	}
}
