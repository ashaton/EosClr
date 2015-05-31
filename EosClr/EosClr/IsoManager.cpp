#include "stdafx.h"
#include "IsoManager.h"

namespace EosClr
{
	IsoManager::IsoManager()
	{
		IsoSpeedLookup = gcnew Dictionary<EdsUInt32, IsoSpeed>();
		EdsValueLookup = gcnew Dictionary<IsoSpeed, EdsUInt32>();
		Type^ isoType = IsoSpeed::typeid;
		array<String^>^ isoFields = Enum::GetNames(isoType);
		for each(String^ isoFieldName in isoFields)
		{
			IsoSpeed iso = static_cast<IsoSpeed>(Enum::Parse(isoType, isoFieldName));
			FieldInfo^ fieldInfo = isoType->GetField(isoFieldName);

			array<Object^>^ edsValueAttrs = fieldInfo->GetCustomAttributes(EdsValueAttribute::typeid, false);
			if (edsValueAttrs->Length != 1)
			{
				throw gcnew Exception("ISO Speed " + isoFieldName + " doesn't have an EDS value attribute.");
			}
			EdsValueAttribute^ edsValueAttr = (EdsValueAttribute^)edsValueAttrs[0];
			EdsInt32 edsValue = edsValueAttr->Value;
			IsoSpeedLookup->Add(edsValue, iso);
			EdsValueLookup->Add(iso, edsValue);
		}
	}

	IsoSpeed IsoManager::GetIsoSpeed(EdsUInt32 EdsIsoValue)
	{
		IsoSpeed speed;
		if (!IsoSpeedLookup->TryGetValue(EdsIsoValue, speed))
		{
			throw gcnew Exception("Camera supports ISO speed with value " + EdsIsoValue.ToString("X")
				+ " but EosClr currently doesn't support it.");
		}
		return speed;
	}

	EdsUInt32 IsoManager::GetEdsIsoValue(IsoSpeed Iso)
	{
		EdsUInt32 edsValue;
		if (!EdsValueLookup->TryGetValue(Iso, edsValue))
		{
			throw gcnew Exception("Requested an EDS ISO Value for " + Iso.ToString()
				+ " but that value was never cached.");
		}
		return edsValue;
	}
}
