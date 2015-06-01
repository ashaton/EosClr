#include "stdafx.h"
#include "IsoManager.h"

namespace EosClr
{
	IsoManager::IsoManager()
	{
		IsoSpeedLookup = gcnew Dictionary<EdsUInt32, IsoSpeed>();
		EdsValueLookup = gcnew Dictionary<IsoSpeed, EdsUInt32>();

		// Iterate through all of the IsoSpeed values and process the attributes on each one.
		Type^ isoType = IsoSpeed::typeid;
		array<String^>^ isoFields = Enum::GetNames(isoType);
		for each(String^ isoFieldName in isoFields)
		{
			// Get the field information for this value
			IsoSpeed iso = static_cast<IsoSpeed>(Enum::Parse(isoType, isoFieldName));
			FieldInfo^ fieldInfo = isoType->GetField(isoFieldName);

			// Get the EdsValue attribute
			array<Object^>^ edsValueAttrs = fieldInfo->GetCustomAttributes(EdsValueAttribute::typeid, false);
			if (edsValueAttrs->Length != 1)
			{
				throw gcnew Exception("ISO Speed " + isoFieldName + " doesn't have an EDS value attribute.");
			}
			EdsValueAttribute^ edsValueAttr = (EdsValueAttribute^)edsValueAttrs[0];
			EdsInt32 edsValue = edsValueAttr->Value;

			// Cache the lookup correlations from EDSDK value to this enum value
			IsoSpeedLookup->Add(edsValue, iso);
			EdsValueLookup->Add(iso, edsValue);
		}
	}

	IsoSpeed IsoManager::GetIsoSpeed(EdsUInt32 EdsIsoValue)
	{
		IsoSpeed speed;
		if (!IsoSpeedLookup->TryGetValue(EdsIsoValue, speed))
		{
			// Future-proofing in case a new camera comes out with an ISO setting that doesn't exist
			// in the EDSDK yet.
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
			// This should never happen unless someone messes with the enum via reflection,
			// in which case all bets are off. Still, it doesn't hurt.
			throw gcnew Exception("Requested an EDS ISO Value for " + Iso.ToString()
				+ " but that value was never cached.");
		}
		return edsValue;
	}
}
