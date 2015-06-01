
#pragma once

#include "ExposureTime.h"

namespace EosClr
{
	private ref class ExposureTimeManager abstract sealed
	{
	public:
		/// <summary>
		/// Returns the exposure time that matches the given EDSDK value
		/// </summary>
		/// <param name="EdsExposureValue">An EDSDK value obtained by using the
		/// kEdsPropID_Tv property identifier</param>
		/// <returns>The exposure time matching the EDSDK value</returns>
		static ExposureTime GetExposureTime(EdsUInt32 EdsExposureValue);

		/// <summary>
		/// Returns the EDSDK value that matches the given exposure time
		/// </summary>
		/// <param name="Exposure">An exposure time enum value</param>
		/// <returns>The EDSDK value corresponding to the given enum value</returns>
		static EdsUInt32 GetEdsExposureValue(ExposureTime Exposure);

	private:
		/// <summary>
		/// Static constructor that initializes all of the lookup logic
		/// </summary>
		static ExposureTimeManager();

		/// <summary>
		/// The lookup from EDSDK values to exposure times
		/// </summary>
		static Dictionary<EdsUInt32, ExposureTime>^ ExposureTimeLookup;

		/// <summary>
		/// The lookup from exposure times to EDSDK values
		/// </summary>
		static Dictionary<ExposureTime, EdsUInt32>^ EdsValueLookup;
	};
}