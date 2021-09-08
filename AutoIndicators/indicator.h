#pragma once
#include "includes.h"

namespace Indicator {
	extern inline float SMA(std::vector<double> _source, int _len);
	extern inline SMAData SMA(std::vector<double> _source, SMAData lengths);
	
}