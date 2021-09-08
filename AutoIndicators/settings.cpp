#include "includes.h"

namespace Settings {
	std::string root_path = ".\\data\\";
	std::string best_input = root_path + "data.json";
	std::string prices = root_path + "prices";

	float trade_price = 1.0;
	float profit_margin = 0.7;

	int min_trades = 60;
	float min_whole_per = 40;
	float min_second_per = 40;
	float min_first_per = 40;

	int first_time_length = 1 WEEKS;
	int second_time_length = 2 WEEKS;
	int whole_time_length = 4 WEEKS;

	int no_threads = 1;
}