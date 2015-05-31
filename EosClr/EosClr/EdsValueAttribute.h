
#pragma once

namespace EosClr
{
	[AttributeUsage(AttributeTargets::Field, AllowMultiple = false)]
	private ref class EdsValueAttribute : Attribute
	{
	public:
		EdsValueAttribute(EdsInt32 Value);
		initonly EdsInt32 Value;
	};
}