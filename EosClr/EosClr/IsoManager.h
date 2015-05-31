
#pragma once
#include "IsoSpeed.h"

namespace EosClr
{
	private ref class IsoManager abstract sealed
	{
	public:
		static IsoManager();
		static IsoSpeed GetIsoSpeed(EdsUInt32 EdsIsoValue);
		static EdsUInt32 GetEdsIsoValue(IsoSpeed Iso);

	private:
		static Dictionary<EdsUInt32, IsoSpeed>^ IsoSpeedLookup;
		static Dictionary<IsoSpeed, EdsUInt32>^ EdsValueLookup;
	};
}