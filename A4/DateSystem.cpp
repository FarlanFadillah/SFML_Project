#include "DateSystem.h"

DateSystem::DateSystem()
{
}
void DateSystem::update(uint64_t currentFrame)
{
	if (currentFrame % 60 == 0)
	{
		m_date.second += 10;
	}
	if (m_date.second >= Max::second)
	{
		m_date.second = 0;
		m_date.minute += 10;
	}

	if (m_date.minute >= Max::minute)
	{
		m_date.minute = 0;
		m_date.hour += 3;
	}

	if (m_date.hour >= Max::hour)
	{
		m_date.hour = 0;
		m_date.day += 1;
		m_date.daysIndex++;
		
	}
	if (m_date.daysIndex > 6)
	{
		m_date.daysIndex = 0;
	}

	if (m_date.day > Max::day)
	{
		
		m_date.day = 1;
		m_date.month += 1;
	}
	if (m_date.month > Max::month)
	{
		m_date.month = 1;
		m_date.year += 1;
	}

}
void DateSystem::saveDate()
{
	std::ofstream save;
	save.open("res/config/date.ini");

	save << "Date " << m_date.second << " " << m_date.minute << " " << m_date.hour << " " << m_date.day << " " << m_date.month << " " << m_date.year << " " << m_date.daysIndex;

	save.close(); 
}

void DateSystem::loadDate(std::string path)
{
	std::ifstream in(path);
	std::string token;
	while (in.good())
	{
		in >> token;
		if (token == "Date")
		{
			in >> m_date.second >> m_date.minute >> m_date.hour >> m_date.day >> m_date.month >> m_date.year >> m_date.daysIndex;
		}
	}
}

const Date DateSystem::getDate()
{
	return m_date;
}

const std::string DateSystem::getDaysName()
{
	return m_date.daysName[m_date.daysIndex];
}

