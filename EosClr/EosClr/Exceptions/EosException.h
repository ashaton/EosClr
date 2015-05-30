
#pragma once

namespace EosClr
{
	public ref class EosException : Exception
	{
	public:
		initonly EdsError Error;
		EosException(EdsError Error);
	};
}

