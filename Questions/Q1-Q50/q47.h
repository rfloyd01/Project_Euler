#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Distinct Primes Factors
std::pair<std::string, double> q47()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int limit = 150000;

	std::vector<int> prims, distinct_prime_factors;
	for (int i = 0; i < limit; i++) distinct_prime_factors.push_back(1);
	distinct_prime_factors[0] = 0; distinct_prime_factors[1] = 0;

	//first create prime sieve
	for (int i = 2; i < limit; i++)
	{
		if (distinct_prime_factors[i])
		{
			prims.push_back(i);
			for (int j = i * i; j < limit; j += i)
			{
				if (j < 0) break; //prevent integer overflow situations
				distinct_prime_factors[j] = 0;
			}
		}
	}

	//now that primes are stored in a vector, sieve through sieve again and add 1 to every value that a prime goes into
	for (int i = 0; i < prims.size(); i++)
	{
		for (int j = prims[i] + prims[i]; j < limit; j += prims[i]) distinct_prime_factors[j]++;
	}

	//now that distinct prime factors table is complete, cycle through until 4 consecutive values of 4 are found
	int k = 2;
	while (true)
	{
		if (k >= limit) break; //exit strategy if limit is too small to avoid infinite loop
		if (distinct_prime_factors[k] == 4)
		{
			bool cont = false;
			for (int j = 1; j < 4; j++)
			{
				if (distinct_prime_factors[k + j] != 4)
				{
					cont = true;
					k += j;
					break;
				}
			}
			if (!cont)
			{
				answer = k;
				break;
			}
		}
		k++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 134043
	//ran in 0.0013706 seconds
}

//NOTES
//Create a prime factors table via a sieve like method for all numbers under 1000000 that will count distinct primes. Start on a prime, and keep advancing in table by that prime
//and add 1 to current value. At the end, the number of distinct primes should be listed for each number under 1000000. Once again the limit ended up being a good deal lower than I
//thought. After aolving the problem once and then adjusting the limit the run time is pretty good, not as fast as I thought it would be but definitely good enough to keep as is.