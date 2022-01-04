#pragma once

#include <Header_Files/pch.h>

//Permuted Multiples
int ten_base[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

long long getDigits(int n)
{
	//returns a ten digit base 10 number which represents how many of each digit 0-9 is in the number n
	long long digit_mask = 0;
	while (n > 0)
	{
		digit_mask += ten_base[n % 10];
		n /= 10;
	}

	return digit_mask;
}

std::pair<std::string, double> q52()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;

	int search_start = 100, search_end; //start by looking at two digit numbers, then three digit, and so on until the answer is found
	long long digit_mask;
	bool answer_found = false, cont = true;

	while (true)
	{
		search_end = (5.0 / 3.0) * (float)search_start;
		for (int i = search_start; i < search_end + 1; i++)
		{
			digit_mask = getDigits(i);
			answer_found = true;
			for (int j = 2; j <= 6; j++)
			{
				if (getDigits(j * i) != digit_mask)
				{
					answer_found = false;
					break;
				}
			}
			if (answer_found)
			{
				cont = false;
				answer = i;
				break;
			}
		}
		search_start *= 10;
		if (!cont) break;
	}
	//ran in 0.0005932 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 142857
	//ran in 0.0005932 seconds
}

//NOTES
//Since we need to go all the way up to 6 times the original number X and keep the same digits, we know that X can never be larger than (5/3) x 10^(number_of_digits - 1).
//For example, if we're checking 3 digit numbers, then X can't be larger than (5/3) * 10^2 = 166. 6 x 166 = 996 which is a three digit number, but 6 x 167 = 1002 which is a four
//digit number. If the number of digits don't match then the digits themselves won't match either. With this ceiling to our searches it should actually be fairly trivial to find
//the correct answer as long as the answer isn't more than 10 digits. Keep track of digits in each number by using a base 10 digit base 10 number. If this isn't sufficient then
//a 10 digit hex number can be used as well. In case the number ends up being larger I'm sure there are some other tricks that can be used but I feel confident this algorithm
//should be fine.