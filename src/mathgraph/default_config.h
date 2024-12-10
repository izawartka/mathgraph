#pragma once
#include "common.h"

const RZUF3_ConfigFileDef g_defaultConfigDef(
	{
		{"lang", RZUF3_ConfigStringEntry("lang_en.txt")},
		{"x_scale", RZUF3_ConfigEnumEntry("decimal", {"decimal", "log", "pi_based"})},
		{"y_scale", RZUF3_ConfigEnumEntry("decimal", {"decimal", "log", "pi_based"})}
	},
	"config.txt",
	true,
	true
);
