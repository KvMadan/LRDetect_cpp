#pragma once

#include <string>
#include <winreg.h>
#include <vector>

const HKEY HKLM = HKEY_LOCAL_MACHINE;
const HKEY HKCU = HKEY_CURRENT_USER;
const HKEY HKCR = HKEY_CLASSES_ROOT;
const REGSAM KEY32 = KEY_WOW64_32KEY;
const REGSAM KEY64 = KEY_WOW64_64KEY;

namespace RegWrapper
{
	std::string regGetValue(HKEY hKey, const std::string& key_name, LPCTSTR lpValueName, const std::string& defaultValue, DWORD dwFlags = KEY64);
	std::string regGetDwdValue(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, const std::string& defaultValue, DWORD dwFlags = KEY64);
	DWORD regGetRegDwd(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, DWORD defaultValue, DWORD dwFlags = KEY64);

	std::string regGetValue32(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, std::string defaultValue);
	std::string get_reg_value(HKEY hKey, std::string key_name, LPCTSTR lpValueName, std::string defaultValue, DWORD dwFlags = KEY64);
	std::string find_reg_value(HKEY hKey, std::string key_name, LPCTSTR lpValueName, std::string defaultValue);
	std::vector<std::string> enumerateValueNames(HKEY hKey, std::string key_name, DWORD dwFlags = KEY64);
	std::string get_first_value_name(HKEY hKey, std::string key_name, DWORD dwFlags = KEY64);
	std::vector<std::string> enumerateKeys(HKEY hKey, const std::string& lpSubKey, DWORD dwFlags = KEY64);
}