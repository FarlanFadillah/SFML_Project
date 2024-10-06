#include "Date.h"

std::string Date::str()
{
	std::ostringstream oss;

	oss << second << " " << minute << " " << hour << " " << day << " " << month << " " << year;
	return oss.str();
}

Date::Date()
{

}
Date::Date(float _s, int _min, int _h, int _d, int _m, int _y)
	:second(_s)
	,minute(_min)
	,hour(_h)
	,day(_d)
	,month(_m)
	,year(_y)
{

}
