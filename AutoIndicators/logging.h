#pragma once
#include "includes.h"

namespace Logging
{
	extern bool initialize;
	extern std::vector<std::string> SavedLogs;

	extern inline std::string getCurrentDateTime(std::string s);
	extern inline void LogFiles(const char* fmt, ...);
}