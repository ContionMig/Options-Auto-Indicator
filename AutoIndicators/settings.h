#pragma once
#include "includes.h"

namespace Settings {
	extern std::string root_path;
	extern std::string best_input;
	extern std::string prices;
	
	extern float trade_price;
	extern float profit_margin;

	extern int min_trades;
	extern float min_whole_per;
	extern float min_second_per;
	extern float min_first_per;

	extern int first_time_length;
	extern int second_time_length;
	extern int whole_time_length;

	extern int no_threads;
}