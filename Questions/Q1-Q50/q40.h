#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file
#include <vector>
#include <cmath> //pow()

std::vector<std::pair<int, int> > findDigitRange(int limit)
{
	int digit_start = 0, digit_end = 0, number_of_digits = 0, number_in_range = 0;
	std::pair<int, int> range = { 0, 0 };
	std::vector<std::pair<int, int> > ranges; ranges.push_back(range);

	while (digit_end <= limit)
	{
		digit_start = digit_end + 1;
		number_of_digits++;
		number_in_range = (int)pow(10, number_of_digits) - (int)pow(10, number_of_digits - 1);
		digit_end = digit_start + (number_of_digits * number_in_range) - 1;
		range.first = digit_start; range.second = digit_end;
		ranges.push_back(range);
	}

	return ranges;
}

//Champernowne's Constant
std::pair<std::string, double> q40()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1;

	std::vector<std::pair<int, int> > digit_ranges = findDigitRange(1000000);
	int needed_digits[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < digit_ranges.size(); j++)
		{
			if (needed_digits[i] <= digit_ranges[j].second)
			{
				//do stuff then break out
				int numbers_from_start = (needed_digits[i] - digit_ranges[j].first) / j;
				int digits_into_number = (needed_digits[i] - digit_ranges[j].first) % j;
				int needed_digit = numbers_from_start + (int)pow(10, j -1);
				for (int k = 0; k < j - digits_into_number - 1; k++) needed_digit /= 10;
				answer *= (needed_digit % 10);

				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 210
	//ran in 0.0000198 seconds
}

//NOTES
//The first thing that I do is figure out how many indiviual digits there are in x digit numbers. The 1 digit number group has 9 total digits (1 digit x 9 numbers in group), the 2 digit
//number group has 180 total digits (2 digits x 90 numbers in group), etc. I saved the ranges for each digit number group in a vector (i.e. digit group 1 is comprised of digits 1 - 9, 
//digit group 2 is comprised of digits 10 - 190, etc.). From this point you just look at the digit that you need and do a little modulus division to figure out what it is.