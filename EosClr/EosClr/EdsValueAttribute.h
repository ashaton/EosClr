
#pragma once

namespace EosClr
{
	/// <summary>
	/// This attribute is used on some enums defined in this assembly.
	/// Certain settings in the Canon SDK (like ISO speed) don't have
	/// a clean mapping from their human-readable names to the underlying
	/// values the SDK uses for them. This attribute is used to hold the
	/// SDK's internal value for each of those settings.
	/// </summary>
	[AttributeUsage(AttributeTargets::Field, AllowMultiple = false)]
	private ref class EdsValueAttribute : Attribute
	{
	public:
		/// <summary>
		/// Creates a new EdsValueAttribute used to map a human-readable
		/// enum value to its corresponding internal EDSDK value.
		/// </summary>
		/// <param name="Value">The EDSDK value corresponding to this
		/// setting</param>
		EdsValueAttribute(EdsUInt32 Value);

		/// <summary>
		/// The EDSDK value corresponding to this setting
		/// </summary>
		initonly EdsUInt32 Value;
	};
}