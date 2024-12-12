#pragma once
#include "common.h"

const RZUF3_ConfigFileDef g_defaultConfigDef(
	{
		{"lang", RZUF3_ConfigEnumEntry("lang_en.txt", {"lang_en.txt", "lang_pl.txt"})},
		{"x_scale", RZUF3_ConfigEnumEntry("decimal", {"decimal", "log", "pi_based"})},
		{"y_scale", RZUF3_ConfigEnumEntry("decimal", {"decimal", "log", "pi_based"})},
		{"show_grid", RZUF3_ConfigBoolEntry(true)},
		{"show_point", RZUF3_ConfigBoolEntry(true)}
	},
	MAIN_CONFIG,
	true,
	true,
	true
);
