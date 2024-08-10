#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes nonRotatedPermutations() and MyPow()

//Circular primes
std::pair<std::string, double> q35()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 3; //2, 5 and 11 wont get counted so add them manually to the answer

	//First create a vector containing all numbers less than 1,000,000 and composed only 
	//of the digits 1, 3, 7 and 9 which aren't rotations of eachother (i.e. 113 and 131
	//are rotations so only include 113)
	std::vector<int> odd_nums = nonRotatedPermutations({ 1, 3, 7, 9 }, 6);

	//For each of the numbers in odd_nums, see if it can be rotated digits amount of times
	for (int i = 0; i < odd_nums.size(); i++)
	{
		bool circular = true;
		int digits = log10(odd_nums[i]) + 1;
		int divider = powers_of_ten[digits - 1], modder = powers_of_ten[digits];

		for (int j = 0; j < digits; j++)
		{
			//test current number
			if (!primeNumberTest(odd_nums[i]))
			{
				circular = false;
				break;
			}

			//then shuffle to next iteration
			odd_nums[i] = (odd_nums[i] * 10 + odd_nums[i] / divider) % modder;
		}
		if (circular) answer += digits;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 55
	//ran in 0.0004048 seconds
}

//NOTES
//Start with a boolean list from 0-1000000 where every prime is marked with a 1. Iterate through list and when a prime
//is found, cycle through all rotatation of it with a combination of taking the last digit by modulus division by 10 and
//then multiplying that by 10 ^ (length of the number) to move it to the front of the number. Also, keep a separate list
//of the circular primes for each number so that if a prime is truly circular, the other primes can be added right to the
//list as well. Lastly, go through all primes in the circular list and set them to 0 in the overall prime list so that
//no double counting takes place.

//UPDATE
/*
Updated my approach so that I only check numbers which don't have any even digits or 5's in them. With this new approach 
a prime sieve was no longer necessary and ends up running about 80x quicker with only a quarter of the memory.
*/