#pragma once

#include <string>
#include <ctime>

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

	string str_weekday;
	string str_hour;
	string str_minute;
	string str_second;

	static const char * s_weekday[7];
};

}
}

