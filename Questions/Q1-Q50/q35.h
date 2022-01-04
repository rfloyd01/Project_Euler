#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes p_sieve()
#include <Header_Files/print.h> //includes vprint()

//Circular primes
std::pair<std::string, double> q35()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int digit_length = 1, num_digits = 1;

	std::vector<bool> prims = p_sieve(1000000);
	for (int i = 2; i < 1000000; i++)
	{
		if (!prims[i]) continue; //only want to check prime numbers
		int num = i;
		std::vector<int> circular_primes = { i };
		if (i < 10)
		{
			digit_length = 1;
			num_digits = 1;
		}
		else if (i < 100)
		{
			digit_length = 10;
			num_digits = 2;
		}
		else if (i < 1000)
		{
			digit_length = 100;
			num_digits = 3;
		}
		else if (i < 10000)
		{
			digit_length = 1000;
			num_digits = 4;
		}
		else if (i < 100000)
		{
			digit_length = 10000;
			num_digits = 5;
		}
		else
		{
			digit_length = 100000;
			num_digits = 6;
		}

		bool circular = 1;
		for (int j = 0; j < num_digits - 1; j++)
		{
			int last_digit = num % 10;
			if (last_digit % 2 == 0) //any number with an even digit can't be a circular prime, as one of it's rotations will end in an even digit, i.e. be divisible by 2
			{
				circular = 0;
				break;
			}
			num /= 10;
			num += (last_digit * digit_length);
			if (num == i) break; //had to add this line in because 11 was appearing as two separate numbers

			if (!prims[num])
			{
				circular = 0;
				break;
			}
			else circular_primes.push_back(num);
		}

		if (circular)
		{
			answer += circular_primes.size();
			for (int j = 0; j < circular_primes.size(); j++)
			{
				prims[circular_primes[j]] = 0;
				//std::cout << circular_primes[j] << std::endl;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 55
	//ran in 0.0427132 seconds
}

//NOTES
//Start with a boolean list from 0-1000000 where every prime is marked with a 1. Iterate through list and when a prime
//is found, cycle through all rotatation of it with a combination of taking the last digit by modulus division by 10 and
//then multiplying that by 10 ^ (length of the number) to move it to the front of the number. Also, keep a separate list
//of the circular primes for each number so that if a prime is truly circular, the other primes can be added right to the
//list as well. Lastly, go through all primes in the circular list and set them to 0 in the overall prime list so that
//no double counting takes place.