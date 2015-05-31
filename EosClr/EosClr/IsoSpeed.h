
#pragma once

#include "EdsValueAttribute.h"

namespace EosClr
{
	public enum class IsoSpeed
	{
		[EdsValue(0x0)]
		[Description("Auto")]
		Auto,

		[EdsValue(0x40)]
		[Description("50")]
		_50,

		[EdsValue(0x48)]
		[Description("100")]
		_100,

		[EdsValue(0x4b)]
		[Description("125")]
		_125,

		[EdsValue(0x4d)]
		[Description("160")]
		_160,

		[EdsValue(0x50)]
		[Description("200")]
		_200,

		[EdsValue(0x53)]
		[Description("250")]
		_250,

		[EdsValue(0x55)]
		[Description("320")]
		_320,

		[EdsValue(0x58)]
		[Description("400")]
		_400,

		[EdsValue(0x5b)]
		[Description("500")]
		_500,

		[EdsValue(0x5d)]
		[Description("640")]
		_640,

		[EdsValue(0x60)]
		[Description("800")]
		_800,

		[EdsValue(0x63)]
		[Description("1000")]
		_1000,

		[EdsValue(0x65)]
		[Description("1250")]
		_1250,

		[EdsValue(0x68)]
		[Description("1600")]
		_1600,

		[EdsValue(0x6b)]
		[Description("2000")]
		_2000,

		[EdsValue(0x6d)]
		[Description("2500")]
		_2500,

		[EdsValue(0x70)]
		[Description("3200")]
		_3200,

		[EdsValue(0x73)]
		[Description("4000")]
		_4000,

		[EdsValue(0x75)]
		[Description("5000")]
		_5000,

		[EdsValue(0x78)]
		[Description("6400")]
		_6400,

		[EdsValue(0x7b)]
		[Description("8000")]
		_8000,

		[EdsValue(0x7d)]
		[Description("10000")]
		_10000,

		[EdsValue(0x80)]
		[Description("12800")]
		_12800,

		[EdsValue(0x88)]
		[Description("25600")]
		_25600,

		[EdsValue(0x90)]
		[Description("51200")]
		_51200,

		[EdsValue(0x98)]
		[Description("102400")]
		_102400,

		[EdsValue(0xffffffff)]
		[Description("Invalid")]
		Invalid,
	};
}