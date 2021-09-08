#pragma once
#include "includes.h"

namespace Helper {
	extern inline int RandomNumber(int min, int max);
	extern inline bool CrossOver(std::vector<double> a, std::vector<double> b);
	extern inline int Normalize(int number, int min, int max);
	
	extern inline  std::string Round(float var);

	extern inline std::string CurrentPath();
	extern inline bool CreateFolder(std::string Path);
	extern inline std::wstring String2WString(const std::string s);
	extern inline std::string ShortenPath(std::string path);
	extern inline rapidjson::Value JsonString(std::string text, rapidjson::Document::AllocatorType& allocator);

	extern inline void SaveFile(const char* stuff, const char* file);

	extern inline std::string ReadFile(const char* file);

	extern inline std::string Timestamp(int timestamp);

	extern inline std::string ConvertNumbers(std::vector<int> numbers);

	extern double AddPrice(std::vector<double> _source, int _len);
}