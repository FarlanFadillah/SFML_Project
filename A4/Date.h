#pragma once
#include "istream"
#include <sstream>
#include <iostream>
#include <vector>
enum Max
{
	second	= 60,
	minute	= 60,
	hour	= 24,
	day		= 31,
	month	= 12,
};


class Date
{
	
public:
	std::vector<std::string> daysName = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" }; 
	int daysIndex = 0;
	float second = 0;
	int minute = 0, hour = 0, day = 1, month = 1, year = 2006;
	std::string str();
	Date();
	Date(float _s, int _min, int _h, int _d, int _m, int _y);
};

