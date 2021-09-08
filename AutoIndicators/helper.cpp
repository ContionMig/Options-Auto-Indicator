#include "includes.h"

#include <olectl.h>
#include <memory>
#include <stdexcept>

namespace Helper {

    inline int RandomNumber(int min, int max) {
        return int(min + (std::rand() % (max - min + 1)));
    }


    inline bool CrossOver(std::vector<double> a, std::vector<double> b) {
        int n_a = a.size();
        int n_b = b.size();

        if (a[n_a - 1] > b[n_b - 1] && a[n_a - 2] < b[n_b - 2])
            return true;
        return false;
    }

    inline int Normalize(int number, int min, int max) {

        while (number > max)
            number -= max;

        while (number < min)
            number += max;

        return number;
    }

    inline std::string CurrentPath()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }

    inline std::wstring String2WString(const std::string s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    inline bool CreateFolder(std::string path)
    {
        std::wstring wPath = String2WString(path);
        return CreateDirectory(wPath.c_str(), NULL);
    }

    inline std::string ShortenPath(std::string path) {
        return path.substr(path.find_last_of("/\\") + 1, 6);
    }

    inline std::string Round(float var) {
        std::string num_text = std::to_string(var);
        std::string rounded = num_text.substr(0, num_text.find(".") + 3);
        return rounded;
    }

    inline rapidjson::Value JsonString(std::string text, rapidjson::Document::AllocatorType& allocator) {
        rapidjson::Value index(text.c_str(), text.size(), allocator);
        return index;
    }

    inline void SaveFile(const char* stuff, const char* file) {

        std::ofstream out(file);
        out << stuff;
        out.close();
    }

    inline std::string ReadFile(const char* file) {

        std::ifstream ifs(file);
        std::string content((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

        return content;
    }

    inline std::string Timestamp(int timestamp) {
        std::uint32_t time_date_stamp = timestamp;
        std::time_t temp = time_date_stamp;
        std::tm* t = std::gmtime(&temp);

        std::stringstream ss;
        ss << std::put_time(t, "%Y-%m-%d %I:%M:%S %p");
        std::string output = ss.str();

        return output;
    }

    inline std::string ConvertNumbers(std::vector<int> numbers) {

        std::string result = "";

        for (int i = 0; i < numbers.size(); i++) {
            result += std::to_string(numbers[i]);
        }

        return result;
    }

    double AddPrice(std::vector<double> _source, int _len) {

        int n = _source.size();
        double price = 0.0;

        for (int i = 0; i < _len; i++) {
            price += _source[n - i - 1];
        }

        return price;
    }
}