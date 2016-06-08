#include <Windows.h>
#include "..\MsiProgram.h"
#include "..\MsiCustomProgram.h"
#include "..\RegistryWrapper.h"
#include "..\Html.h"


MsiCustomProgram::MsiCustomProgram(std::string u_id, std::string p_key_path) : MsiProgram(u_id, p_key_path)
{
}

std::string MsiCustomProgram::get_NameVersionDate() 
{
	auto out = NO;

	if (is_installed)
	{
		out = YES;

		auto val = get_ProgramProperty("DisplayName");
		if (val != "")
			out.append(", " + val);

		val = get_ProgramProperty("DisplayVersion");
		if (val != "")
			out.append(" " + val);

		val = get_ProgramProperty("InstallDate");
		if (val != "")
			out.append(" installed on " + FormatDate(val));
	}
	return out; 
}

std::string MsiCustomProgram::get_InstalledPatchesString()
{
	auto patch_ids = get_InstalledPatchIds();
	int size = (int)patch_ids.size();
	std::string out;

	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			auto regPath = program_patches_key_path + "\\" + patch_ids[i];
			auto name = RegWrapper::get_reg_value(HKLM, regPath, "DisplayName", ""); 
			auto date = RegWrapper::get_reg_value(HKLM, regPath, "Installed", "");
			out.append(Html::b(name) + " installed on " + FormatDate(date));
		}
		return out;
	}
	return "None";
}


std::string MsiCustomProgram::FormatDate(std::string date)
{
	return date.substr(6, 2) + "-" + MONTH[std::stoi(date.substr(4, 2)) - 1] + "-" + date.substr(0, 4);
}

std::string MsiCustomProgram::get_CustomComponents()
{
	auto keyPath = program_key_path + "\\CustComponent\\";
	auto componentNames = RegWrapper::enumerateKeys(HKLM, keyPath, KEY32);
	int size = (int)componentNames.size();
	std::string out;

	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			out.append(Html::b(componentNames[i]) + " ");

			auto regPath = keyPath + componentNames[i] + "\\CurrentVersion";
			auto major = RegWrapper::regGetValue32(HKLM, regPath, "Major", ""); 
			auto minor = RegWrapper::regGetValue32(HKLM, regPath, "Minor", "");
			out.append(major + "." + minor + Html::br);
		}
		return out;
	}
	return "None";
}