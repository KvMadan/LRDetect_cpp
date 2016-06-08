#include <string>
#include "Html.h"
#include <Windows.h>
#include <vector>
#include "RegistryWrapper.h"
#include <stdlib.h>
#include "Utilities.h"

using namespace std;

namespace OsInfo {

	string computerName()
	{
		TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(computerName) / sizeof(computerName[0]);
		GetComputerName(computerName, &size);
		return computerName;
	}



	std::string get_os_env_vars()
	{
		string path = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\";
	  string values;
		vector<string> var_names = RegWrapper::enumerateValueNames(HKLM, path);
		int size = (int)var_names.size();

		for (int i = 0; i < size; i++)
		{
			string val = RegWrapper::regGetValue(HKLM, path, var_names[i].c_str(), "");
			values.append(Html::b(var_names[i]) + "=" + val + Html::br);
		}
		return Html::linkToHiddenContent(values);
	}

	std::string UserEnvVars()
	{
		const int MAX_SIZE = 1024;
		TCHAR tcPath[MAX_SIZE];
		DWORD size = GetEnvironmentVariable("TEMP", tcPath, MAX_SIZE );
		std::string value = string(Html::b("TEMP") + "=" + tcPath + Html::br);

		size = GetEnvironmentVariable("TMP", tcPath, MAX_SIZE );
		value.append(string(Html::b("TMP") + "=" + tcPath + Html::br));

		//size = GetEnvironmentVariable("PATH", tcPath, MAX_SIZE );
		//value.append(string(Html::b("PATH") + "=" + tcPath));

		return Html::linkToHiddenContent(value);
	}


	int osBits = 0;
	bool is64os = false;


	BOOL is64 = 0;

	int getOsBits()
	{
		if (IsWow64Process(GetCurrentProcess(), &is64) != ERROR_SUCCESS)
			return is64 ? 64 : 32;
		return 0;
	}

	std::string osBitString = "";
	std::string OsBitString()
	{
		if(osBitString == "")
		{
			int bits = getOsBits();
			return "x" + to_string(bits);
		}
	}
	
	string osName()
	{
		string out = "";
		string val = RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "ProductName", "Microsoft Windows"); 
		out.append(val + " ");
		val = RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CSDVersion", ""); 
		out.append(val + " " + OsBitString() + " (");
		val = RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CurrentVersion", ""); 
		out.append(val + ".");
		val = RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CurrentBuild", ""); 
		out.append(val + ")");

		return out;
	}

	std::string osRootDir()
	{
		return RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "SystemRoot", ""); 
	}


	std::string OsLanguage()
	{
		/*LANGID langid = GetSystemDefaultUILanguage();
		PULONG  pulNumLanguages;
		PZZWSTR pwszLanguagesBuffer;
		PULONG  pcchLanguagesBuffer;
		BOOL res = GetSystemPreferredUILanguages(MUI_LANGUAGE_NAME, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
		*/
		return Html::error("ERROR: " + GetLastError());
	}

	std::string getLocale()
	{
		wchar_t lpLocaleName[255];
		if (GetSystemDefaultLocaleName(lpLocaleName, 255) != 0)
		{
			wstring ws(lpLocaleName);
			return string(ws.begin(), ws.end());
		}
		return Html::error("ERROR: " + GetLastError());
	}

	std::string UAC()
	{
		int val = RegWrapper::regGetRegDwd(HKLM, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "EnableLUA", -1);
		string status[3] = {"UAC is not supported for this OS", "Disabled", Html::error("Enabled")};
		return status[val + 1];
	}

	std::string WindowsFirewall()
	{
		int firewallFlag = RegWrapper::regGetRegDwd(HKLM, "SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\", "EnableFirewall", -1);
		if (firewallFlag == -1)
		{
			//Implement netsh 	
		}
		 return Html::intToYesNo(firewallFlag);
	}

	BOOL IsUserAdmin(VOID)
	{
		SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
		PSID AdministratorsGroup; 
		BOOL b = AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup);

		if(b) 
		{
			if (!CheckTokenMembership( NULL, AdministratorsGroup, &b)) 
				b = FALSE;
			FreeSid(AdministratorsGroup); 
		}

		return(b);
	}

	std::string get_AppInit_DLLs()
	{
		std::string out;
		std::string val = RegWrapper::regGetValue(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\", "AppInit_DLLs", "notfound");
		if (val != "" && val != "notfound")
      out.append("x64 entries: " + val + Html::br);

		if (getOsBits() == 64)
		{
			val = RegWrapper::regGetValue32(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\", "AppInit_DLLs", "notfound");
			if (val != "" && val != "notfound")
				out.append("x32 entries: " + val + Html::br);
		}

		return out.size() > 0 ? out : "No dll entries found";
	}

	std::string get_LoadAppInit_DLLs()
	{
		std::string out;
		int val = RegWrapper::regGetRegDwd(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", "LoadAppInit_DLLs", -1);
		if (val > -1)
      out.append("x64 entries: " + std::to_string(val) + Html::br);

		if (getOsBits() == 64)
		{
			val = RegWrapper::regGetRegDwd(HKLM, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", "LoadAppInit_DLLs", -1, KEY32);
			if (val > -1)
				out.append("x32 entries: " + std::to_string(val) + Html::br);
		}

		return out.size() > 0 ? out : Html::error("Unable to detect!");
	}

	std::string DepInfo()
	{
		std::string dep = Utilities::exec("wmic OS Get DataExecutionPrevention_SupportPolicy");


		return "";
	}

	void collectData(string& content)
	{
		content.append(Html::h2("Operating System"));
		content.append(Html::table_open());
		content.append(Html::tr(Html::td2("Operating system information")));

		content.append(Html::tr(Html::td("Machine name") + Html::td(computerName())));
		content.append(Html::tr(Html::td("Full name") + Html::td(osName())));
		content.append(Html::tr(Html::td("Root directory") + Html::td(osRootDir())));
		content.append(Html::tr(Html::td("Language") + Html::td(OsLanguage())));
		content.append(Html::tr(Html::td("Locale") + Html::td(getLocale())));
		content.append(Html::tr(Html::td("Is OS Virtualized?") + Html::td("")));
		content.append(Html::tr(Html::td("Is 3GB switch enabled?") + Html::td("")));
		content.append(Html::tr(Html::td("Data Execution Prevention") + Html::td(DepInfo())));
		content.append(Html::tr(Html::td("User Account Control") + Html::td(UAC())));
		content.append(Html::tr(Html::td("Is user Admin?") + Html::td(Html::boolToYesNo(IsUserAdmin()))));
		content.append(Html::tr(Html::td("Is user connected remotely?") + Html::td("")));
		content.append(Html::tr(Html::td("Is Windows firewall enabled?") + Html::td(WindowsFirewall())));
		content.append(Html::tr(Html::td("Is secondary logon enabled?") + Html::td("")));

		content.append(Html::tr(Html::td2("Environment information")));
		content.append(Html::tr(Html::td("System environment variables") + Html::td(get_os_env_vars())));
		content.append(Html::tr(Html::td("User environment variables") + Html::td(UserEnvVars())));
		content.append(Html::tr(Html::td("Kerberos configuration") + Html::td("")));
		content.append(Html::tr(Html::td("Layered Service Providers") + Html::td("")));
		content.append(Html::tr(Html::td("AppInit_DLLs registry value") + Html::td(get_AppInit_DLLs())));
		content.append(Html::tr(Html::td("LoadAppInit_DLLs registry value") + Html::td(get_LoadAppInit_DLLs())));

		content.append(Html::table_close);
	}


}