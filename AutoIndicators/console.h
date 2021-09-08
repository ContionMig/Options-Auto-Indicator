#pragma once
#include "includes.h"

namespace Console {
	extern std::vector<ErrorLogs> Errors;
	extern inline void Setup();

	extern inline void Print();
	extern inline void WaterMark();
}