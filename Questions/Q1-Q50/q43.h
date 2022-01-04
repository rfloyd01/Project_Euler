#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes []powers_of_two, []powers_of_ten
#include <Vector>

//Sub-string Divisibility
int prims[9] = { 23, 19, 17, 13, 11, 7, 5, 3, 2 }; //add three extra primes in beginning so 13 is at index 3

void CreateNumbers(int current_three_digit, int level, long long current_number, long long& answer, int used_digits)
{
	if (level == 9)
	{
		for (int i = 0; i < 10; i++)
		{
			if ((powers_of_two[i] & used_digits) == 0)
			{
				if (i > 0) //pandigital number can't start with 0
				{
					answer += (current_number + i * powers_of_ten[level]);
					//std::cout << (current_number + i * powers_of_ten[level]) << std::endl; //Uncomment this line to see which values have the sub-string divisibility
					return;
				}
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if ((powers_of_two[i] & used_digits) == powers_of_two[i]) continue; //skip if number's already been used
		if (((current_three_digit / 10 + 100 * i) % prims[level]) == 0) CreateNumbers((current_three_digit / 10 + 100 * i), level + 1, current_number + i * powers_of_ten[level], answer, used_digits | powers_of_two[i]);
	}
}

std::pair<std::string, double> q43()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	std::vector<std::pair<int, int> > seventeen_substrings;
	for (int i = 17; i < 1000; i += 17)
	{
		int used_digits = 0, i_copy = i;
		bool add = true;
		for (int j = 0; j < 3; j++)
		{
			if (powers_of_two[i_copy % 10] & used_digits)
			{
				add = false;
				break; //see if digit has been used yet
			}
			used_digits += powers_of_two[i_copy % 10];
			i_copy /= 10;
		}
		if (add)
		{
			std::pair<int, int> new_pair = { i, used_digits };
			seventeen_substrings.push_back(new_pair);
		}
	}

	for (int i = 0; i < seventeen_substrings.size(); i++)
	{
		CreateNumbers(seventeen_substrings[i].first, 3, seventeen_substrings[i].first, answer, seventeen_substrings[i].second);
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 16695334890
	//ran in .0000206 seconds
}

//NOTES
//This is a pretty interesting problem. Iterating through all of the 0-9 pandigitals (of which there are 10! * .9 = 3,265,920 of) shouldn't be necessary. Instead I think it'll
//be more efficient to try and craft numbers with the appropriate properties starting from the final three digits. We're only looking at numbers under 1000 which are divisible by 17
//so that means only 1000 floor 17 = 58 numbers can be used as a starting point. From this point use a recursive algorithm to keep adding unused digits onto the front of the
//number making sure it's divisible by the next lowest prime. In hindsight it might have been easier to program this using strings instead of ints and long longs but I think the code runs
//quicker this way. Basically runs instantaneously.