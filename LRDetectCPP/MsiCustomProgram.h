#pragma once

#include "MsiProgram.h"


class MsiCustomProgram : public MsiProgram
{
	public:
		MsiCustomProgram(std::string u_code, std::string p_key_path = "");
		std::string get_NameVersionDate();
		std::string get_InstalledPatchesString();
		std::string FormatDate(std::string date);
		std::string get_CustomComponents();
};
