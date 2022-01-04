#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes factorial(), int_to_char()
#include <vector>
#include <cmath> //includes ceil()

//Lexicographic permutations
std::pair<std::string, double> q24()
{
	auto run_time = std::chrono::steady_clock::now();
	std::string answer = "";
	std::vector<bool> digit_used = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int to_target = 1000000;

	for (int i = 0; i < 10; i++) //one iteration for each digit that needs to be found
	{
		long long fact = (double)factorial(9 - i);
		int next_digit = ceil((double) to_target / fact);
		int count = 0;
		for (int j = 0; j < digit_used.size(); j++)
		{
			if (!digit_used[j]) count++;
			if (count == next_digit)
			{
				digit_used[j] = 1;
				answer += int_to_char(j);
				to_target -= (fact * (count - 1));
				break;
			}
		}
	}

	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 2783915460
	//ran in .0000081 seconds
}
//NOTES
//Since order doesn't matter for the permutations, there should be the same number of permutations that start with 0, 1, 2, etc. Taking this a step further there should be the same number of
//permutations of 01xxx as there are 02xxx, 03xxx, 13xxx, etc. The number of permutations that start with any given digit (i.e. 1xxxxxxxxx) should be 9! as that's how many ways there are to
//shuffle the last 9 digits. We're looking for the 1,000,000th iteration so the first digit should be ceiling(1000000 / 9!) = 3. Since 0 is used, the 3rd digit isn't actually 3, but 2.
//The first permutation that starts with 2 (2013456789) should bethe (9! x 2) + 1 = 725,761th permutation (times 2 + 1 because the first 9! * 2 numbers are the permutations starting with 
//0 and 1). The rest of the digits should be found in a similar fashion. The below code runs very quickly, although at some point I'd like to code a function that actually can quickly
//go through the actual permutations.