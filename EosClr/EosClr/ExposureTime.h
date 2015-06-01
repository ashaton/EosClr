
#pragma once

#include "EdsValueAttribute.h"

namespace EosClr
{
	/// <summary>
	/// Defines the standard exposure times that Canon cameras support.
	/// </summary>
	public enum class ExposureTime
	{
		/// <summary>
		/// Keep the shutter in sync with the flash bulb.
		/// </summary>
		[EdsValue(0x0C)]
		[Description("Bulb")]
		Bulb,

		/// <summary>
		/// 30 second exposure
		/// </summary>
		[EdsValue(0x10)]
		[Description("30\"")]
		_30,

		/// <summary>
		/// 25 second exposure
		/// </summary>
		[EdsValue(0x13)]
		[Description("25\"")]
		_25,

		/// <summary>
		/// 20 second exposure
		/// </summary>
		[EdsValue(0x14)]
		[Description("20\"")]
		_20,

		/// <summary>
		/// 20 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x15)]
		[Description("20\" (1/3 Step Set)")]
		_20_OneThirdStepSize,

		/// <summary>
		/// 15 second exposure
		/// </summary>
		[EdsValue(0x18)]
		[Description("15\"")]
		_15,

		/// <summary>
		/// 13 second exposure
		/// </summary>
		[EdsValue(0x1B)]
		[Description("13\"")]
		_13,

		/// <summary>
		/// 10 second exposure
		/// </summary>
		[EdsValue(0x1C)]
		[Description("10\"")]
		_10,

		/// <summary>
		/// 10 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x1D)]
		[Description("10\" (1/3 Step Set)")]
		_10_OneThirdStepSize,

		/// <summary>
		/// 8 second exposure
		/// </summary>
		[EdsValue(0x20)]
		[Description("8\"")]
		_8,

		/// <summary>
		/// 6 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x23)]
		[Description("6\" (1/3 Step Set)")]
		_6_OneThirdStepSize,

		/// <summary>
		/// 6 second exposure
		/// </summary>
		[EdsValue(0x24)]
		[Description("6\"")]
		_6,

		/// <summary>
		/// 5 second exposure
		/// </summary>
		[EdsValue(0x25)]
		[Description("5\"")]
		_5,

		/// <summary>
		/// 4 second exposure
		/// </summary>
		[EdsValue(0x28)]
		[Description("4\"")]
		_4,

		/// <summary>
		/// 3.2 second exposure
		/// </summary>
		[EdsValue(0x2B)]
		[Description("3.2\"")]
		_3_2,

		/// <summary>
		/// 3 second exposure
		/// </summary>
		[EdsValue(0x2C)]
		[Description("3\"")]
		_3,

		/// <summary>
		/// 2.5 second exposure
		/// </summary>
		[EdsValue(0x2D)]
		[Description("2.5\"")]
		_2_5,

		/// <summary>
		/// 2 second exposure
		/// </summary>
		[EdsValue(0x30)]
		[Description("2\"")]
		_2,

		/// <summary>
		/// 1.6 second exposure
		/// </summary>
		[EdsValue(0x33)]
		[Description("1.6\"")]
		_1_6,

		/// <summary>
		/// 1.5 second exposure
		/// </summary>
		[EdsValue(0x34)]
		[Description("1.5\"")]
		_1_5,

		/// <summary>
		/// 1.3 second exposure
		/// </summary>
		[EdsValue(0x35)]
		[Description("1.3\"")]
		_1_3,

		/// <summary>
		/// 1 second exposure
		/// </summary>
		[EdsValue(0x38)]
		[Description("1\"")]
		_1,

		/// <summary>
		/// 0.8 second exposure
		/// </summary>
		[EdsValue(0x3B)]
		[Description("0.8\"")]
		_0_8,

		/// <summary>
		/// 0.7 second exposure
		/// </summary>
		[EdsValue(0x3C)]
		[Description("0.7\"")]
		_0_7,

		/// <summary>
		/// 0.6 second exposure
		/// </summary>
		[EdsValue(0x3D)]
		[Description("0.6\"")]
		_0_6,

		/// <summary>
		/// 0.5 second exposure
		/// </summary>
		[EdsValue(0x40)]
		[Description("0.5\"")]
		_0_5,

		/// <summary>
		/// 0.4 second exposure
		/// </summary>
		[EdsValue(0x43)]
		[Description("0.4\"")]
		_0_4,

		/// <summary>
		/// 0.3 second exposure
		/// </summary>
		[EdsValue(0x44)]
		[Description("0.3\"")]
		_0_3,

		/// <summary>
		/// 0.3 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x45)]
		[Description("0.3\" (1/3 Step Set)")]
		_0_3_OneThirdStepSize,

		/// <summary>
		/// 1/4 second exposure
		/// </summary>
		[EdsValue(0x48)]
		[Description("1/4")]
		_1_4th,

		/// <summary>
		/// 1/5 second exposure
		/// </summary>
		[EdsValue(0x4B)]
		[Description("1/5")]
		_1_5th,

		/// <summary>
		/// 1/6 second exposure
		/// </summary>
		[EdsValue(0x4C)]
		[Description("1/6")]
		_1_6th,

		/// <summary>
		/// 1/6 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x4D)]
		[Description("1/6 (1/3 Step Set)")]
		_1_6th_OneThirdStepSize,

		/// <summary>
		/// 1/8 second exposure
		/// </summary>
		[EdsValue(0x50)]
		[Description("1/8")]
		_1_8th,

		/// <summary>
		/// 1/10 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x53)]
		[Description("1/10 (1/3 Step Set)")]
		_1_10th_OneThirdStepSize,

		/// <summary>
		/// 1/10 second exposure
		/// </summary>
		[EdsValue(0x54)]
		[Description("1/10")]
		_1_10th,

		/// <summary>
		/// 1/13 second exposure
		/// </summary>
		[EdsValue(0x55)]
		[Description("1/13")]
		_1_13th,

		/// <summary>
		/// 1/15 second exposure
		/// </summary>
		[EdsValue(0x58)]
		[Description("1/15")]
		_1_15th,

		/// <summary>
		/// 1/20 second exposure (use when the custom step is set to 1/3)
		/// </summary>
		[EdsValue(0x5B)]
		[Description("1/20 (1/3 Step Set)")]
		_1_20th_OneThirdStepSize,

		/// <summary>
		/// 1/20 second exposure
		/// </summary>
		[EdsValue(0x5C)]
		[Description("1/20")]
		_1_20th,

		/// <summary>
		/// 1/25 second exposure
		/// </summary>
		[EdsValue(0x5D)]
		[Description("1/25")]
		_1_25th,

		/// <summary>
		/// 1/30 second exposure
		/// </summary>
		[EdsValue(0x60)]
		[Description("1/30")]
		_1_30th,

		/// <summary>
		/// 1/40 second exposure
		/// </summary>
		[EdsValue(0x63)]
		[Description("1/40")]
		_1_40th,

		/// <summary>
		/// 1/45 second exposure
		/// </summary>
		[EdsValue(0x64)]
		[Description("1/45")]
		_1_45th,

		/// <summary>
		/// 1/50 second exposure
		/// </summary>
		[EdsValue(0x65)]
		[Description("1/50")]
		_1_50th,

		/// <summary>
		/// 1/60 second exposure
		/// </summary>
		[EdsValue(0x68)]
		[Description("1/60")]
		_1_60th,

		/// <summary>
		/// 1/80 second exposure
		/// </summary>
		[EdsValue(0x6B)]
		[Description("1/80")]
		_1_80th,

		/// <summary>
		/// 1/90 second exposure
		/// </summary>
		[EdsValue(0x6C)]
		[Description("1/90")]
		_1_90th,

		/// <summary>
		/// 1/100 second exposure
		/// </summary>
		[EdsValue(0x6D)]
		[Description("1/100")]
		_1_100th,

		/// <summary>
		/// 1/125 second exposure
		/// </summary>
		[EdsValue(0x70)]
		[Description("1/125")]
		_1_125th,

		/// <summary>
		/// 1/160 second exposure
		/// </summary>
		[EdsValue(0x73)]
		[Description("1/160")]
		_1_160th,

		/// <summary>
		/// 1/180 second exposure
		/// </summary>
		[EdsValue(0x74)]
		[Description("1/180")]
		_1_180th,

		/// <summary>
		/// 1/200 second exposure
		/// </summary>
		[EdsValue(0x75)]
		[Description("1/200")]
		_1_200th,

		/// <summary>
		/// 1/250 second exposure
		/// </summary>
		[EdsValue(0x78)]
		[Description("1/250")]
		_1_250th,

		/// <summary>
		/// 1/320 second exposure
		/// </summary>
		[EdsValue(0x7B)]
		[Description("1/320")]
		_1_320th,

		/// <summary>
		/// 1/350 second exposure
		/// </summary>
		[EdsValue(0x7C)]
		[Description("1/350")]
		_1_350th,

		/// <summary>
		/// 1/400 second exposure
		/// </summary>
		[EdsValue(0x7D)]
		[Description("1/400")]
		_1_400th,

		/// <summary>
		/// 1/500 second exposure
		/// </summary>
		[EdsValue(0x80)]
		[Description("1/500")]
		_1_500th,

		/// <summary>
		/// 1/640 second exposure
		/// </summary>
		[EdsValue(0x83)]
		[Description("1/640")]
		_1_640th,

		/// <summary>
		/// 1/750 second exposure
		/// </summary>
		[EdsValue(0x84)]
		[Description("1/750")]
		_1_750th,

		/// <summary>
		/// 1/800 second exposure
		/// </summary>
		[EdsValue(0x85)]
		[Description("1/800")]
		_1_800th,

		/// <summary>
		/// 1/1000 second exposure
		/// </summary>
		[EdsValue(0x88)]
		[Description("1/1000")]
		_1_1000th,

		/// <summary>
		/// 1/1250 second exposure
		/// </summary>
		[EdsValue(0x8B)]
		[Description("1/1250")]
		_1_1250th,

		/// <summary>
		/// 1/1500 second exposure
		/// </summary>
		[EdsValue(0x8C)]
		[Description("1/1500")]
		_1_1500th,

		/// <summary>
		/// 1/1600 second exposure
		/// </summary>
		[EdsValue(0x8D)]
		[Description("1/1600")]
		_1_1600th,

		/// <summary>
		/// 1/2000 second exposure
		/// </summary>
		[EdsValue(0x90)]
		[Description("1/2000")]
		_1_2000th,

		/// <summary>
		/// 1/2500 second exposure
		/// </summary>
		[EdsValue(0x93)]
		[Description("1/2500")]
		_1_2500th,

		/// <summary>
		/// 1/3000 second exposure
		/// </summary>
		[EdsValue(0x94)]
		[Description("1/3000")]
		_1_3000th,

		/// <summary>
		/// 1/3200 second exposure
		/// </summary>
		[EdsValue(0x95)]
		[Description("1/3200")]
		_1_3200th,

		/// <summary>
		/// 1/4000 second exposure
		/// </summary>
		[EdsValue(0x98)]
		[Description("1/4000")]
		_1_4000th,

		/// <summary>
		/// 1/5000 second exposure
		/// </summary>
		[EdsValue(0x9B)]
		[Description("1/5000")]
		_1_5000th,

		/// <summary>
		/// 1/6000 second exposure
		/// </summary>
		[EdsValue(0x9C)]
		[Description("1/6000")]
		_1_6000th,

		/// <summary>
		/// 1/6400 second exposure
		/// </summary>
		[EdsValue(0x9D)]
		[Description("1/6400")]
		_1_6400th,

		/// <summary>
		/// 1/8000 second exposure
		/// </summary>
		[EdsValue(0xA0)]
		[Description("1/8000")]
		_1_8000th,

		/// <summary>
		/// Not valid or none
		/// </summary>
		[EdsValue(0xFFFFFFFF)]
		[Description("Invalid/Unavailable")]
		Invalid
	};
}