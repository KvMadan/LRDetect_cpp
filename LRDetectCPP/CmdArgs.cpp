#pragma once

#include <Windows.h>
#include <string>
#include "CmdArgs.h"

using namespace std;

namespace CmdArgs 
{
	bool hideReport = false;
	bool collect = false;

	void GetArgs()
	{
		LPTSTR args = GetCommandLine();
		std::string args_string = std::string(args);
		size_t found = args_string.find("-hideReport");
		if (found != string::npos)
			hideReport = true;

		found = args_string.find("-collect");
		if (found != string::npos)
			collect = true;
	}
}