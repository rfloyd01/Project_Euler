#pragma once

#include <Header_Files/pch.h>
#include <vector>
#include <Header_Files/print.h>

//Distinct Primes Factors
std::pair<std::string, double> q47()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0, limit = 150000;
	std::vector<int> factors(limit + 1, 0);
	
	//calculate the number of distinct prime factors up to the limit by using a modified prime sieve
	for (int i = 2; i <= limit; i++)
	{
		if (factors[i] == 0)
		{
			//if a prime is found, add 1 to everything divisible by this number in the sieve
			for (int j = i; j <= limit; j += i) factors[j]++;
		}
		else if (factors[i] == 4)
		{
			//see if the next three numbers also have 4 factors, if so we have our answer
			if (factors[i + 1] == 4 && factors[i + 2] == 4 && factors[i + 3] == 4)
			{
				answer = i;
				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 134043
	//ran in 0.0008031 seconds
}

//NOTES
//Create a prime factors table via a sieve like method for all numbers under 1000000 that will count distinct primes. Start on a prime, and keep advancing in table by that prime
//and add 1 to current value. At the end, the number of distinct primes should be listed for each number under 1000000. Once again the limit ended up being a good deal lower than I
//thought. After solving the problem once and then adjusting the limit the run time is pretty good, not as fast as I thought it would be but definitely good enough to keep as is.