#include "includes.h"

namespace Logging
{
	bool initialize = false;
	std::vector<std::string> SavedLogs = {	};

	inline std::string getCurrentDateTime(std::string s)
	{
		using namespace std;
		time_t now = time(0);
		struct tm  tstruct;
		char  buf[80];
		tstruct = *localtime(&now);
		if (s == "now")
			strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		else if (s == "date")
			strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		else if (s == "at")
			strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);

		return string(buf);
	}

	inline void LogFiles(const char* fmt, ...)
	{
		char text[256];
		va_list ap;

		va_start(ap, fmt);
		vsprintf_s(text, fmt, ap);
		va_end(ap);

		Helper::CreateFolder((Helper::CurrentPath().c_str() + (std::string)"\\data\\logs").c_str());
		std::string filePath = Helper::CurrentPath().c_str() + (std::string)"\\data\\logs\\log_" + getCurrentDateTime("date") + ".txt";
		std::string now = getCurrentDateTime("now");

		static std::ofstream myfile;
		if (!myfile.is_open())
			myfile.open(filePath, std::ios_base::out | std::ios_base::app);
		else 
			myfile << now.c_str() << '\t' << text << " " << GetLastError() << '\n';
		
	}
}