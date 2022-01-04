#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Counting Sundays
bool leap_year(long long year)
{
	bool leap = false;
	if (year % 4 == 0) leap = true;
	else return leap; //if not divisible by 4, then won't be by 100 or 400 either

	if (year % 100 == 0) leap = false;
	else return leap; //if not divisible by 100, then won't be by 400 either

	if (year % 400 == 0) leap = true;

	return leap;
}

int countSundays(int start_month, int end_month, long long& current_day, long long year, std::vector<int>& month_days, bool leap)
{
	int count = 0;
	for (int month = start_month; month <= end_month; month++)
	{
		current_day %= 7; //make sure to round to the current week
		if (current_day == 6) count++;
		current_day += month_days[month];
		if ((month == 2) && leap) current_day++;
	}

	return count;
}

std::pair<std::string, double> q19()
{
	//just a brute force approach where there are two loops. loop one goes from 1901 to 2000 for the year and loop two goes from 1 to 12 for the month
	//days of the week are 0 for Monday, 1 for Tuesday ... 6 for Sunday

	auto run_time = std::chrono::steady_clock::now();
    int answer = 0;
    int test_cases = 1;
    
    //days in the month for Jan. - Dec.
    std::vector<int> month_days = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for (int i = 0; i < test_cases; i++)
    {
        //for the HackerRank version, the below 6 variables are read from std::cin, but for the Euler
        //version I've just typed them in as there's only 1 test case
        long long Y1 = 1901, M1 = 1, D1 = 1, Y2 = 2000, M2 = 12, D2 = 31;

        answer = 0;
        long long current_day = 0;

        //First we need to calculate what day of the week our starting day is.
        //All that we're given is January 1, 1900 is a Monday so we need to work
        //off of that. Every year that passes January 1st will advance a day in the
        //week, except on a leap year it will advance 2 days forward. Therefore Our starting day
        //will be (0 + (Number of years from 1900 to Y1) + (Number of Leap years from 1900 to Y1) +
        //(days to reach M1 D1 from jan 1st of Y1)) % 7. The reason we add to 0 is because 0 represents
        //Monday and that's what day jan 1st of 1900 is.

        //since we're starting on the year 1900 every time, the below variables, four_leaps and hundred
        //leaps both will "hit" on that year. The vbariable Four_hundred_leaps is offset from it's
        //first hit by 100 years so calculate it slightly differently

        long long four_leaps = 1 + (Y1 - 1900 - 1) / 4; //the number of times the 4 year rule hits in the range
        long long hundred_leaps = 1 + (Y1 - 1900 - 1) / 100; //the number of time the 100 year rule hits in the range
        long long four_hundred_leaps = 0; //the number of times the 400 year rule hits in the range

        if (Y1 > 2000) four_hundred_leaps = 1 + (Y1 - 2000 - 1) / 400; //calculate from year 2000 instead of 1900

        long long total_leap_years = four_leaps - hundred_leaps + four_hundred_leaps;
        current_day = (Y1 - 1900 + total_leap_years) % 7;

        //current day is now equal to January 1st of Year 1. Per the problem text we
        //aren't guaranteed a start date of January 1 though so now we calculate the
        //start day based on the M1 and D1.
        for (int month = 1; month < M1; month++)
        {
            if (month == 2 && leap_year(Y1)) current_day += 29;
            else current_day += month_days[month];
        }

        //We are now on the first day of M1 in the year Y1, add the appropriate days
        //to get the start day
        current_day += (D1 - 1);
        current_day %= 7;

        //We're almost at our start date but there's one final check. We need to see if our start date is in the middle of a month.
        //If so, advance to the next month as we only care about the first days of the month
        if (D1 != 1)
        {
            //if currently in February and it's a leap year, need to add an extra day
            current_day += (month_days[M1] - D1 + 1);
            if (M1++ == 2 && leap_year(Y1)) current_day++;
        }

        //Ok, the hardest part of the problem is done, now that we know what day of the week we're starting
        //on we shouldn't need anymore than 12,000 loop iterations to count all of the days.
        //First count the Sundays in Y1. If Y1 == Y2 then only count up to M2 instead of December.
        if (Y2 > Y1) answer += countSundays(M1, 12, current_day, Y1, month_days, leap_year(Y1));
        else  answer += countSundays(M1, M2, current_day, Y1, month_days, leap_year(Y1));

        //Count Sundays between Y1 and Y2 (not inclusive)
        for (long long year = Y1 + 1; year < Y2; year++) answer += countSundays(1, 12, current_day, year, month_days, leap_year(year));

        //Count Sundays in Y2 (but only if Y2 isn't the same year as Y1)
        if (Y2 > Y1)  answer += countSundays(1, M2, current_day, Y2, month_days, leap_year(Y2));
    }

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 171
    //ran in 0.0000072 seconds
    //HackerRank Score = 100.00/100.00
}

//NOTES
//Old algorithm
/*
//int answer = 0;
    //int current_day = 0; //Jan. 1 1901 was a Monday so start at day 0
    //bool leap_year = 0;
    //std::vector<int> month_days = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //days in the month for Jan. - Dec.

    //for (int year = 1901; year < 2001; year++)
    //{
    //	//the 100 and 400 year rule would only take effect during the year 2000 but I've left them in the code just for completeness sake
    //	if (year % 400 == 0) leap_year = 1;
    //	else if (year % 100 == 0) leap_year = 0;
    //	else if (year % 4 == 0) leap_year = 1;
    //	else leap_year = 0;

    //	for (int month = 1; month <= 12; month++)
    //	{
    //		current_day %= 7; //make sure to round to the current week
    //		if (current_day == 6)
    //		{
    //			//std::cout << "Month = " << month << ", Year = " << year << std::endl;
    //			answer++;
    //		}

    //		current_day += month_days[month];
    //		if ((month == 2) & leap_year) current_day++;
    //	}
    //}
    //ran in 0.0000064 seconds
*/
