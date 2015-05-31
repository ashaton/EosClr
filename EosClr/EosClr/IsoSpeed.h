
#pragma once

#include "EdsValueAttribute.h"

namespace EosClr
{
	/// <summary>
	/// Defines the standard ISO levels that Canon cameras support.
	/// </summary>
	public enum class IsoSpeed
	{
		/// <summary>
		/// Set the ISO speed automatically
		/// </summary>
		[EdsValue(0x0)]
		[Description("Auto")]
		Auto,

		/// <summary>
		/// ISO 50
		/// </summary>
		[EdsValue(0x40)]
		[Description("50")]
		_50,

		/// <summary>
		/// ISO 100
		/// </summary>
		[EdsValue(0x48)]
		[Description("100")]
		_100,

		/// <summary>
		/// ISO 125
		/// </summary>
		[EdsValue(0x4b)]
		[Description("125")]
		_125,

		/// <summary>
		/// ISO 160
		/// </summary>
		[EdsValue(0x4d)]
		[Description("160")]
		_160,

		/// <summary>
		/// ISO 200
		/// </summary>
		[EdsValue(0x50)]
		[Description("200")]
		_200,

		/// <summary>
		/// ISO 250
		/// </summary>
		[EdsValue(0x53)]
		[Description("250")]
		_250,

		/// <summary>
		/// ISO 320
		/// </summary>
		[EdsValue(0x55)]
		[Description("320")]
		_320,

		/// <summary>
		/// ISO 400
		/// </summary>
		[EdsValue(0x58)]
		[Description("400")]
		_400,

		/// <summary>
		/// ISO 500
		/// </summary>
		[EdsValue(0x5b)]
		[Description("500")]
		_500,

		/// <summary>
		/// ISO 640
		/// </summary>
		[EdsValue(0x5d)]
		[Description("640")]
		_640,

		/// <summary>
		/// ISO 800
		/// </summary>
		[EdsValue(0x60)]
		[Description("800")]
		_800,

		/// <summary>
		/// ISO 1000
		/// </summary>
		[EdsValue(0x63)]
		[Description("1000")]
		_1000,

		/// <summary>
		/// ISO 1250
		/// </summary>
		[EdsValue(0x65)]
		[Description("1250")]
		_1250,

		/// <summary>
		/// ISO 1600
		/// </summary>
		[EdsValue(0x68)]
		[Description("1600")]
		_1600,

		/// <summary>
		/// ISO 2000
		/// </summary>
		[EdsValue(0x6b)]
		[Description("2000")]
		_2000,

		/// <summary>
		/// ISO 2500
		/// </summary>
		[EdsValue(0x6d)]
		[Description("2500")]
		_2500,

		/// <summary>
		/// ISO 3200
		/// </summary>
		[EdsValue(0x70)]
		[Description("3200")]
		_3200,

		/// <summary>
		/// ISO 4000
		/// </summary>
		[EdsValue(0x73)]
		[Description("4000")]
		_4000,

		/// <summary>
		/// ISO 5000
		/// </summary>
		[EdsValue(0x75)]
		[Description("5000")]
		_5000,

		/// <summary>
		/// ISO 6400
		/// </summary>
		[EdsValue(0x78)]
		[Description("6400")]
		_6400,

		/// <summary>
		/// ISO 8000
		/// </summary>
		[EdsValue(0x7b)]
		[Description("8000")]
		_8000,

		/// <summary>
		/// ISO 10000
		/// </summary>
		[EdsValue(0x7d)]
		[Description("10000")]
		_10000,

		/// <summary>
		/// ISO 12800
		/// </summary>
		[EdsValue(0x80)]
		[Description("12800")]
		_12800,

		/// <summary>
		/// ISO 25600
		/// </summary>
		[EdsValue(0x88)]
		[Description("25600")]
		_25600,

		/// <summary>
		/// ISO 51200
		/// </summary>
		[EdsValue(0x90)]
		[Description("51200")]
		_51200,

		/// <summary>
		/// ISO 102400
		/// </summary>
		[EdsValue(0x98)]
		[Description("102400")]
		_102400,

		/// <summary>
		/// Invalid / unknown ISO setting
		/// </summary>
		[EdsValue(0xffffffff)]
		[Description("Invalid")]
		Invalid,
	};
}