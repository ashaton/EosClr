#include "stdafx.h"
#include "ExposureTimeManager.h"

namespace EosClr
{
	ExposureTimeManager::ExposureTimeManager()
	{
		ExposureTimeLookup = gcnew Dictionary<EdsUInt32, ExposureTime>();
		EdsValueLookup = gcnew Dictionary<ExposureTime, EdsUInt32>();

		// Iterate through all of the IsoSpeed values and process the attributes on each one.
		Type^ exposureType = ExposureTime::typeid;
		array<String^>^ exposureFields = Enum::GetNames(exposureType);
		for each(String^ exposureFieldName in exposureFields)
		{
			// Get the field information for this value
			ExposureTime exposure = static_cast<ExposureTime>(Enum::Parse(exposureType, exposureFieldName));
			FieldInfo^ fieldInfo = exposureType->GetField(exposureFieldName);

			// Get the EdsValue attribute
			array<Object^>^ edsValueAttrs = fieldInfo->GetCustomAttributes(EdsValueAttribute::typeid, false);
			if (edsValueAttrs->Length != 1)
			{
				throw gcnew Exception("Exposure Time " + exposureFieldName + " doesn't have an EDS value attribute.");
			}
			EdsValueAttribute^ edsValueAttr = (EdsValueAttribute^)edsValueAttrs[0];
			EdsInt32 edsValue = edsValueAttr->Value;

			// Cache the lookup correlations from EDSDK value to this enum value
			ExposureTimeLookup->Add(edsValue, exposure);
			EdsValueLookup->Add(exposure, edsValue);
		}
	}

	ExposureTime ExposureTimeManager::GetExposureTime(EdsUInt32 EdsExposureValue)
	{
		ExposureTime exposure;
		if (!ExposureTimeLookup->TryGetValue(EdsExposureValue, exposure))
		{
			// Future-proofing in case a new camera comes out with an exposure that doesn't exist
			// in the EDSDK yet.
			throw gcnew Exception("Camera supports exposure with value " + EdsExposureValue.ToString("X")
				+ " but EosClr currently doesn't support it.");
		}
		return exposure;
	}

	EdsUInt32 ExposureTimeManager::GetEdsExposureValue(ExposureTime Exposure)
	{
		EdsUInt32 edsValue;
		if (!EdsValueLookup->TryGetValue(Exposure, edsValue))
		{
			// This should never happen unless someone messes with the enum via reflection,
			// in which case all bets are off. Still, it doesn't hurt.
			throw gcnew Exception("Requested an EDS Exposure value for " + Exposure.ToString()
				+ " but that value was never cached.");
		}
		return edsValue;
	}
}
