#pragma once
#include "settings.h"

#define WEEKS * 7 * 24 * 60
#define MONTHS WEEKS * 4


struct SMAData {
	float short_len = 4;
	float long_len = 16;
	float base_len = 50;

	SMAData(float _short_len = 4, float _long_len = 16, float _base_len = 50) {
		short_len = _short_len;
		long_len = _long_len;
		base_len = _base_len;
	}

	int biggest() {
		int biggest = std::max<float>({ short_len, long_len, base_len });
		return biggest;
	}
};

struct ErrorLogs {
	std::string time;
	std::string error;

	ErrorLogs(std::string _time, std::string _error) {
		time = _time;
		error = _error;
	}
};

struct RightWrong {
	int right = 1;
	int wrong = 1;

	int total_trades() {
		return wrong + right;
	}

	float profit() {
		float win = (float)right / (float)total_trades();
		float lose = 1.0 - win;

		float Earnings = ((float)total_trades() * win * Settings::profit_margin) - ((float)total_trades() * lose * Settings::trade_price);
		return Earnings;
	}

	float percentage() {
		return (float)right / (float)total_trades() * 100.0;
	}

};

struct Results {
	int time_stamp = 0;
	int period = 0;

	RightWrong first_time = {};
	RightWrong second_time = {};
	RightWrong whole_time = {};

	int total_trades() {
		return whole_time.total_trades() + second_time.total_trades() + first_time.total_trades();
	}

	int total_time() {
		return Settings::first_time_length + Settings::second_time_length + Settings::whole_time_length;
	}

	float profit() {
		float win = (float)first_time.right / (float)first_time.total_trades();
		float lose = 1.0 - win;

		float Earnings = ((float)first_time.total_trades() * win * Settings::profit_margin * Settings::trade_price) - ((float)first_time.total_trades() * lose * Settings::trade_price);
		return Earnings;
	}

	float percentage() {
		float lowest_ = std::min<float>({ whole_time.percentage(),second_time.percentage(), first_time.percentage() });
		return lowest_;
	}
};

struct Inputs {

	int E = 0;

	// SMA
	int short_len = 0;
	int long_len = 0;
	int base_len = 0;

	bool inversed = true;

	Inputs(
		int _E = 2, 
		int _short_len = 6, 
		int _long_len = 14, 
		int _base_len = 50,
		bool _inversed = true) {

		E = _E;

		short_len = _short_len;
		long_len = _long_len;
		base_len = _base_len;

		inversed = _inversed;
	}

	Inputs inverse() {
		return Inputs(E, short_len, long_len, base_len, false);
	}

	int biggest() {
		int biggest = std::max<int>({ short_len, long_len, base_len });
		return biggest;
	}
};

struct ThreadData {
	int variations = 0;
};

struct SavedData {
	std::string price = "";
	float highest_per = 0;

	Results results = {};
	Inputs inputs = {};
	ThreadData thread = {};
	

	SavedData(std::string _price = "NONE", Results _results = {}, Inputs _inputs = {}, ThreadData _thread = {}, float _highest_per = 0) {
		price = _price;
		results = _results;
		inputs = _inputs;
		thread = _thread;
		highest_per = _highest_per;
	}
};

struct Data {
	std::vector<double> candle_list = {};

	std::vector<double> shortSMA = {};
	std::vector<double> longSMA = {};
	std::vector<double> baseSMA = {};

	std::vector<bool> CALL = {};
	std::vector<bool> PUT = {};
	std::vector<bool> crossed = {};
	std::vector<bool> base_below = {};
	std::vector<bool> base_above = {};

};
