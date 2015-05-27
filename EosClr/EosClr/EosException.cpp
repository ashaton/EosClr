#include "stdafx.h"
#include "EosException.h"

namespace EosClr
{
	EosException::EosException(EdsError Error)
		: Exception()
	{
		this->Error = Error;
	}
}
