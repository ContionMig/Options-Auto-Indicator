#pragma once
#include "includes.h"

namespace Threads {

	extern std::map<std::string, SavedData> BestPrices;

	extern inline void Setup();

	extern inline void GuessCheck(LPVOID _price);
	extern inline Results Readings(std::string price, Inputs inputs, rapidjson::Document& d);
	extern inline void SaveReading();

	extern inline Inputs GetOldInputs(std::string price);
}