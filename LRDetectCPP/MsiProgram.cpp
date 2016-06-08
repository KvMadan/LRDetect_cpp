#include <windows.h>
#include "MsiProgram.h"
#include <string>
#include <vector>
#include "RegistryWrapper.h"
#include "Constants.h"
#include "Html.h"

MsiProgram::MsiProgram() : 
	is_installed(false), 
	installer_key("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Products\\")
{
	if (u_code != "")
		product_code = get_product_id(u_code);

	if (product_code != "")
		{
		is_installed = true;
		program_properties_key_path = installer_key  + product_code + "\\InstallProperties";
		program_patches_key_path = installer_key + product_code + "\\Patches";
		}
}

MsiProgram::MsiProgram(std::string u_code, std::string p_key) :
	is_installed(false), 
	installer_key("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Products\\")
{
	product_code = get_product_id(u_code);
	if (product_code != "")
	{
		is_installed = true;
		program_properties_key_path = installer_key  + product_code + "\\InstallProperties";
		program_patches_key_path = installer_key + product_code + "\\Patches";
		program_key_path = "SOFTWARE\\" + p_key;
	}
}

MsiProgram::~MsiProgram() {}

std::string MsiProgram::get_product_id(const std::string& uprgade_code)
{
	return RegWrapper::get_first_value_name(HKCR, "Installer\\UpgradeCodes\\" + uprgade_code);
}

std::string MsiProgram::get_ProgramProperty(const char *property_name)
{
	return RegWrapper::regGetValue(HKLM, program_properties_key_path, property_name, "");
}

//TODO return false if operation fails
std::vector<std::string> MsiProgram::get_InstalledPatchIds()
{
	return RegWrapper::enumerateKeys(HKLM, program_patches_key_path);
}

