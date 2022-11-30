#pragma once

#include <Header_Files/pch.h>
#include <vector>
#include <cmath>

//Digit fifth powers
std::pair<std::string, double> q30()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, power = 5, power_differences[10] = { 0 };

	for (int i = 0; i < 9; i++)
	{
		int next_pow = 1;
		for (int j = 0; j < power; j++) next_pow *= (i + 1);
		power_differences[i] = next_pow - power_differences[i];
		power_differences[i + 1] = next_pow;
	}

	int limit = power_differences[9], nines = 9;

	//calculate the upper limit of our search
	while (limit >= nines)
	{
		nines = nines * 10 + 9;
		limit += power_differences[9];
	}

	//once we're done using 9^5 for caclulations flip it to a negative number
	//because going from 9^5 to 0^5 causes a subtraction instead of an addition
	power_differences[9] *= -1;
	int maximum_digits = log10(limit) + 1, n = 10, current_sum = 1; //our starting number and starting fifth power sum
	int* digits = new int[maximum_digits]();
	digits[1] = 1; //set our digits array to [0, 1, ...] to represent 10

	//Increase the current number, n, one at a time until it exceeds the limit.
	//This in turn causes the running sum of powers to increase by the amount
	//kept in the power_differences array.
	while (n < limit)
	{
		//see if n equals current_sum before incrementing it
		if (current_sum == n++) answer += (n-1);
		for (int i = 0; i < maximum_digits; i++)
		{
			current_sum += power_differences[digits[i]];
			if (digits[i] == 9) digits[i] = 0;
			else
			{
				digits[i]++;
				break; //since there's no 9, higher places won't change
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 443839
	//ran in 0.0005228 seconds
}

//NOTES
//The first step was to see what the theoretical limit is. 9 ^ 5 ~= 59,000 so eventually a number will get so many digits that even
//if every digit is a 9, the sum of each digit raised to the 5th power won't be able to reach the actual number. In this case that limit
//seems to be about 350,000. This number isn't that that high so just check every number from 1 - 350,000 to see if it satisfies what
//the question is asking for.

//HACKERRANK UPDATE
/*
The HackerRank version really isn't any different, although looking back over my code I realized I could make a slight speedup. My current 
way for getting the fifth power sum is to just keep dividing the current number by 10 and then adding the fifth power of the digit in the one's
place. This requires n additions and n-1 subtractions, where n is the number of digits in the number. Instead of doing this, since we know the 
one's digit always increases by 1, we can memoize the differences between successive powers of 5 and just add this number to a running tally.
For example the 5th power sum when going from 123,456 to 123,457 will increase by the same amount as going from 36 to 37. In both cases the 5th
power sum increases by 9,031 because only the last digit is changing, going from 6 to 7. When a digit goes from 9 to zero we just subtract 9^5
from the current sum. Doing it like this, there are 0 divisions to make and at most there will be n additions/subtractions.
*/