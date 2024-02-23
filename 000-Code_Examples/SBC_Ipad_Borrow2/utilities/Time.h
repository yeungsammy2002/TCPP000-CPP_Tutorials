#pragma once

#include <string>
#include <ctime>
#include <sstream>

using std::string;

namespace regulus {
namespace utilities {

class Time
{
public:
	static Time now();

	static Time get_time(long long time);

	long long m_unixtime;
	int m_year;
	int m_month;
	int m_day;

	int m_hour;
	int m_minute;
	int m_second;

	int m_12hr_hour;
	string str_12hr_hour;
	string m_12hr_format;
	string m_chi_12hr_format;

	string str_weekday;
	string str_chi_weekday;
	string str_hour;
	string str_minute;
	string str_second;

	string str_time;
	string str_chi_time;

	static const char * s_weekday[7];
};

}
}

