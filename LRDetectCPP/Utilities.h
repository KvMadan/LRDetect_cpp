#pragma once
#include <string>
#include <Windows.h>

namespace Utilities
{
	std::string ExecCmd( LPCSTR pCmdArg);
	std::string exec(const std::string& command);
}