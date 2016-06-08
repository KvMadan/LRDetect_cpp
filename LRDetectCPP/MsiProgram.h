#pragma once

#include <string>
#include <vector>


class MsiProgram
{
	public:
		MsiProgram(void);
		MsiProgram(std::string u_code, std::string p_key = "");
		virtual ~MsiProgram(void);

		bool is_installed;
		std::string get_ProgramProperty(const char *property_name);
		std::vector<std::string> get_InstalledPatchIds();

	private:

		std::string program_properties_key_path;
		std::string get_product_id(const std::string& uprgade_code);

	protected:
		const std::string u_code;
		std::string product_code;
		std::string program_key_path;
		std::string program_patches_key_path;
		const std::string installer_key;
};

