#include <Windows.h>  
#include <iostream>  
#include <WbemCli.h> 
#pragma comment(lib, "wbemuuid.lib")

namespace WmiWrapper {

	std::wstring GetWmiProperty(LPCWSTR property_name, std::wstring class_name);

}