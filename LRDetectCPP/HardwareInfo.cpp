#include <string>
#include <Windows.h>
#include "Html.h"
#include <winreg.h>
#include "HardwareInfo.h"
#include "RegistryWrapper.h"


namespace HardwareInfo {

	SYSTEM_INFO sysinfo;
	void init()
	{
		GetSystemInfo( &sysinfo );
	}


	ULONGLONG get_TotalMemoryInKilobytes()
	{
		ULONGLONG TotalMemoryInKilobytes;
		GetPhysicallyInstalledSystemMemory(&TotalMemoryInKilobytes);
		return TotalMemoryInKilobytes;
	}

	//TODO make compatible with pre vista versions of windows
	MEMORYSTATUSEX memStatus;
	std::string get_TotalMemory()
	{
			//memStatus.dwLength = sizeof(memStatus);
			//GlobalMemoryStatusEx(&memStatus);
			//return std::to_string(memStatus.ullTotalPhys / 1024 / 1024) + " Mb";
		return to_string(get_TotalMemoryInKilobytes() / 1024 ) + " Mb";
	}
	
	std::string get_CpuName()
	{
		std::string name = RegWrapper::get_reg_value(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", "");
		return name;
	}

	std::string get_DiskSpace()
	{
		ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes,lpTotalNumberOfFreeBytes;
		if (GetDiskFreeSpaceEx(NULL, &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes) != 0)
		{
			int free = lpTotalNumberOfFreeBytes.QuadPart / 1024 / 1024;
			int total = lpTotalNumberOfBytes.QuadPart / 1024 / 1024;
			int user_free = lpFreeBytesAvailableToCaller.QuadPart / 1024 / 1024;
		
			return "Total size of drive: " + to_string(total) + " Mb" + Html::br + 
				"Total available space: " + to_string(free) + " Mb" + Html::br + 
				"Available space for current user: " + to_string(user_free) + " Mb";
		}
		return Html::error("Unable to detect. Error " + to_string(GetLastError()));
	}

	std::string get_MonitorSize()
	{
		int n = GetSystemMetrics(SM_CMONITORS);
		std::string s = n > 1 ? "s" : "";
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		
		return to_string(n) + " monitor" + s + " detected." 
			+ Html::br + "Primary screen resolution: " + to_string(width) + "x" + to_string(height) + "pixels";
	}

	void collectData(std::string& content)
	{

		init();

		content.append(Html::h2("Hardware"));
		content.append(Html::table_open());
		content.append(Html::tr(Html::td2("Hardware information")));

		content.append(Html::tr(Html::td("CPU") + Html::td(HardwareInfo::get_CpuName())));
		content.append(Html::tr(Html::td("Processor Count") + Html::td(to_string(sysinfo.dwNumberOfProcessors))));
		content.append(Html::tr(Html::td("Total Memory") + Html::td(get_TotalMemory())));
		content.append(Html::tr(Html::td("Hard Drives") + Html::td(get_DiskSpace())));
		content.append(Html::tr(Html::td("Monitor information") + Html::td(get_MonitorSize())));

		content.append(Html::table_close);
	}


}