
#pragma once
#include "Stdafx.h"
#include "Exceptions/EosException.h"

namespace EosClr
{
	static void ErrorCheck(EdsError Error)
	{
		if (Error != EDS_ERR_OK)
		{
			throw gcnew EosException(Error);
		}
	}
}