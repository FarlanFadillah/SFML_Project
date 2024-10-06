#pragma once
#include "Date.h"
#include <fstream>
#include <iostream>
class DateSystem
{
	Date m_date;
public:
	void update(uint64_t currentFrame);
	void saveDate();
	void loadDate(std::string path);
	const Date getDate();
	const std::string getDaysName();
	DateSystem();
};

