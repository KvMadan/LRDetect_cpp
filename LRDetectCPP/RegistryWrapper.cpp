#pragma once
#include <windows.h>
#include <string>
#include <winreg.h>
#include "RegistryWrapper.h"


namespace RegWrapper
{
		std::string get_reg_value(HKEY hKey, std::string keyName, LPCTSTR lpValueName, std::string defaultValue, DWORD dwFlags)
		{
			const int TOTALBYTES = 512;
			DWORD dwType = REG_SZ; //default type
			DWORD size = TOTALBYTES;
			char  buffer[TOTALBYTES];
			
			std::string value;
			DWORD dwValue = 0;
			//LPCTSTR lpSubKey = keyName.;

			if (RegOpenKeyEx( hKey, keyName.c_str(), 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				// Call to get the type of the registry
				//auto res = RegQueryValueEx(hKey, lpValueName, 0, &dwType, (BYTE*)buffer, &size);
				auto res = RegQueryValueEx(hKey, lpValueName, 0, &dwType, (LPBYTE)buffer, &size);

				//If the initial size is not enough to hold the value i.e. in case of a Reg_Multi_Sz value
				//Reinitialize a temp buffer with the new size
				if (res == ERROR_MORE_DATA)
				{
					std::vector<char>temp(size);
					res = RegQueryValueEx(hKey, lpValueName, 0, &dwType, reinterpret_cast<LPBYTE>(&temp[0]), &size);
					if (res == ERROR_SUCCESS)
						 return std::string(temp.data(), temp.size());
				}

				if (res == ERROR_SUCCESS)
				{
					// If the type of the key is REG_SZ, process it
					if (dwType == REG_SZ)
						value = std::string(buffer);

					//if the reg type is REG_DWORD, make a new call to get the value correctly
					if (dwType == REG_DWORD)
					{
						res = RegQueryValueEx(hKey, lpValueName, 0, &dwType, (BYTE*)(DWORD)&dwValue, &size);
						if (res == ERROR_SUCCESS)
							value = std::to_string(dwValue);
					}		
					RegCloseKey(hKey);
					return value;
				}
			}
			return defaultValue;
		}



		//Search for a value first in 64bit regs then in 32bit regs
		std::string find_reg_value(HKEY hKey, std::string key_name, LPCTSTR lpValueName, std::string defaultValue)
		{
			auto res = get_reg_value(hKey, key_name, lpValueName, defaultValue);
			if (res == "")
				res = regGetValue32(hKey, key_name, lpValueName, defaultValue);

			return res;

		}
		std::vector<std::string> enumerateValueNames(HKEY hKey, std::string key_name, DWORD dwFlags)
		{
			std::vector<std::string> value_names;
			DWORD size = 255;
			TCHAR lpValueName[255];
			int dwIndex = 0;

			if (RegOpenKeyEx( hKey, key_name.c_str(), 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				while (RegEnumValue(hKey, dwIndex, lpValueName, &size, NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS)
				{
					dwIndex++;
					value_names.push_back(lpValueName);
					size = 255; //reset the size
				}
				RegCloseKey(hKey);
			}
			return value_names;
		}

		std::string get_first_value_name(HKEY hKey, std::string key_name, DWORD dwFlags)
		{
			std::string value = "";
			DWORD size = 255;
			TCHAR lpValueName[255];

			if (RegOpenKeyEx( hKey, key_name.c_str(), 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				auto res = RegEnumValue(hKey, 0, lpValueName, &size, NULL, NULL, NULL, NULL);
				value = lpValueName;
				RegCloseKey(hKey);
			}
			return value;
		}

		std::vector<std::string> enumerateKeys(HKEY hKey, const std::string& lpSubKey, DWORD dwFlags)
		{
			int dwIndex = 0;
			TCHAR lpValueName[255];
			DWORD size = 255;
			std::vector<std::string> key_names; 

			if (RegOpenKeyEx(hKey, lpSubKey.c_str(), 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				while (RegEnumKeyEx(hKey, dwIndex, lpValueName, &size, NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS)
				{
					dwIndex++;
					key_names.push_back(lpValueName);
					size = 255; //reset the size
				}
				RegCloseKey(hKey);
			}
			return key_names;
		}


		std::string regGetValue(HKEY hKey, const std::string& lpSubKey, LPCTSTR lpValueName, const std::string& defaultValue, DWORD dwFlags)
		{
			DWORD pdwType = REG_SZ;
			DWORD size = 512;
			char  buffer[512];

			if (RegOpenKeyEx( hKey, NULL, 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				auto res = RegGetValue(hKey, lpSubKey.c_str(), lpValueName, RRF_RT_ANY, &pdwType, (LPBYTE)buffer, &size);

				//If the initial size is not enough to hold the value i.e. in case of a Reg_Multi_Sz value
				//Reinitialize a temp buffer with the new size
				if (res == ERROR_MORE_DATA)
				{
					std::vector<char>temp(size);
					res = RegGetValue(hKey, lpSubKey.c_str(), lpValueName, RRF_RT_ANY, &pdwType, reinterpret_cast<LPBYTE>(&temp[0]), &size);
					if (res == ERROR_SUCCESS)
					{
						RegCloseKey(hKey);
						return std::string(temp.data());
					}
				}

				if (res == ERROR_SUCCESS)
				{
					// If the type of the key is REG_SZ, process it
					if (pdwType == REG_SZ)
					{
						RegCloseKey(hKey);
						return std::string(buffer);
					}
					//if the reg type is REG_DWORD, make a new call to get the value correctly
					if (pdwType == REG_DWORD)
					{
						DWORD dwValue = 0;
						res = RegGetValue(hKey, lpSubKey.c_str(), lpValueName, RRF_RT_ANY, &pdwType, (BYTE*)(DWORD)&dwValue, &size);
						if (res == ERROR_SUCCESS)
						{
							RegCloseKey(hKey);
							return std::to_string(dwValue);
						}		
					}
				}
			}
			return defaultValue;
		}

		std::string regGetValue32(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, std::string defaultValue)
		{
			return regGetValue(hKey, keyName.c_str(), lpValueName, defaultValue, KEY32);
		}

		std::string regGetDwdValue(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, const std::string& defaultValue, DWORD dwFlags)
		{
			DWORD pdwType = REG_DWORD;
			DWORD dwValue = 0;
			DWORD size = 512;

			if (RegOpenKeyEx( hKey, NULL, 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				if (RegGetValue(hKey, keyName.c_str(), lpValueName, RRF_RT_ANY, &pdwType, (BYTE*)(DWORD)&dwValue, &size) == ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return std::to_string(dwValue);
				}		
			}
			return defaultValue;
		}
		
		DWORD regGetRegDwd(HKEY hKey, const std::string& keyName, LPCTSTR lpValueName, DWORD defaultValue, DWORD dwFlags)
		{
			DWORD pdwType = REG_DWORD;
			DWORD dwValue = 0;
			DWORD size = 512;

			if (RegOpenKeyEx( hKey, NULL, 0, KEY_READ | dwFlags, &hKey ) == ERROR_SUCCESS)
			{
				if (RegGetValue(hKey, keyName.c_str(), lpValueName, RRF_RT_ANY, &pdwType, (BYTE*)(DWORD)&dwValue, &size) == ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return dwValue;
				}		
			}
			return defaultValue;
		}

}