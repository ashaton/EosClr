
#pragma once

#include "Stdafx.h"
#include "Exceptions/EosException.h"

namespace EosClr
{
	/// <summary>
	/// Checks the return value of an EDSDK function. If it's an error, this
	/// will throw an exception with the error code.
	/// </summary>
	/// <param name="Error">The return value from the function this call wrapped</param>
	static void ErrorCheck(EdsError Error)
	{
		if (Error != EDS_ERR_OK)
		{
			throw gcnew EosException(Error);
		}
	}
}