
#pragma once

namespace EosClr
{
	[AttributeUsage(AttributeTargets::Field, AllowMultiple = false)]
	private ref class EdsValueAttribute : Attribute
	{
	public:
		EdsValueAttribute(EdsUInt32 Value);
		initonly EdsUInt32 Value;
	};
}