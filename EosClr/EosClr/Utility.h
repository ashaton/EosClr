
#pragma once
#include "Stdafx.h"
#include "EosException.h"

namespace EosClr
{
	void ErrorCheck(EdsError Error)
	{
		if (Error != EDS_ERR_OK)
		{
			throw gcnew EosException(Error);
		}
	}
}