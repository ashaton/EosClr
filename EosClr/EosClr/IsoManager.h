
#pragma once

#include "IsoSpeed.h"

namespace EosClr
{
	/// <summary>
	/// This is a utility class that stores lookups between ISO
	/// settings and their matching EDSDK values.
	/// </summary>
	private ref class IsoManager abstract sealed
	{
	public:
		/// <summary>
		/// Returns the ISO speed enum that matches the given EDSDK value
		/// </summary>
		/// <param name="EdsIsoValue">An EDSDK value obtained by using the
		/// kEdsPropID_ISOSpeed property identifier</param>
		/// <returns>The ISO speed enum matching the EDSDK value</returns>
		static IsoSpeed GetIsoSpeed(EdsUInt32 EdsIsoValue);

		/// <summary>
		/// Returns the EDSDK value that matches the given ISO speed enum
		/// </summary>
		/// <param name="Iso">An ISO speed enum value</param>
		/// <returns>The EDSDK value corresponding to the given enum value</returns>
		static EdsUInt32 GetEdsIsoValue(IsoSpeed Iso);

	private:
		/// <summary>
		/// Static constructor that initializes all of the lookup logic
		/// </summary>
		static IsoManager();

		/// <summary>
		/// The lookup from EDSDK values to ISO speeds
		/// </summary>
		static Dictionary<EdsUInt32, IsoSpeed>^ IsoSpeedLookup;

		/// <summary>
		/// The lookup from ISO speeds to EDSDK values
		/// </summary>
		static Dictionary<IsoSpeed, EdsUInt32>^ EdsValueLookup;
	};
}