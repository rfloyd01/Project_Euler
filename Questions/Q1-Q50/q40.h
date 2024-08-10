#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Champernowne's Constant
std::pair<std::string, double> q40()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1;

	int needed_digits[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };
	int maximum_digit = 1000000, num_length = 1;
	std::vector<int> number_groups = { 0 };

	while (number_groups.back() < maximum_digit)
	{
		number_groups.push_back((powers_of_ten[num_length] - powers_of_ten[num_length - 1]) * num_length + number_groups[num_length - 1]);
		num_length++;
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 1; j < number_groups.size(); j++)
		{
			if (number_groups[j] >= needed_digits[i])
			{
				//the digit we want is in this number group
				
				//first figure out which number in the group we want, and which digit of that number we need
				int number = (needed_digits[i] - number_groups[j - 1] - 1) / j + powers_of_ten[j - 1];
				int digit_number = (needed_digits[i] - number_groups[j - 1]) % j;

				//The modular division has a weird quirk of giving us the digits we need in order, unless the value 
				//of the modular division is 0 (which would really mean we need the last digit of the number and not 
				//the first digit of the number) so we need to handle 0 separately
				if (digit_number > 0)
				{
					for (int k = 0; k < j - digit_number; k++) number /= 10;
				}
				answer *= (number % 10);
				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 210
	//ran in 0.0000049 seconds
}

//NOTES
//The first thing that I do is figure out how many indiviual digits there are in x digit numbers. The 1 digit number group has 9 total digits (1 digit x 9 numbers in group), the 2 digit
//number group has 180 total digits (2 digits x 90 numbers in group), etc. I saved the ranges for each digit number group in a vector (i.e. digit group 1 is comprised of digits 1 - 9, 
//digit group 2 is comprised of digits 10 - 190, etc.). From this point you just look at the digit that you need and do a little modulus division to figure out what it is.