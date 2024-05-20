// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Utils/CDateUtils.h"

static constexpr const char M_MONTHS_IN_YEAR = 12;

FDateTime UCDateUtils::AdvanceMonth(FDateTime aInitialDateTime)
{
	int32 currentYear = aInitialDateTime.GetYear();
	int32 nextYear = aInitialDateTime.GetYear();

	int32 currentMonth = aInitialDateTime.GetMonth();
	int32 nextMonth = currentMonth + 1;

	// If last month of the year, reset the month and advance a year
	if (nextMonth > M_MONTHS_IN_YEAR)
	{
		nextMonth = 1;
		nextYear += 1;
	}

	int32 currentDay = aInitialDateTime.GetDay();
	int32 nextDay = currentDay;

	// If the next month has fewer days than the current one, just go to the last day
	int32 maxDaysNextMonth = FDateTime::DaysInMonth(nextYear, nextMonth);
	if (nextDay > maxDaysNextMonth)
	{
		nextDay = maxDaysNextMonth;
	}

	return FDateTime(nextYear, nextMonth, nextDay);
}
