#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes []powers_of_ten

//Totient Permutation
bool permutationCheck(int a, int b)
{
	long long num1 = 0, num2 = 0;
	while (a > 0)
	{
		num1 += powers_of_ten[a % 10];
		a /= 10;
	}

	while (b > 0)
	{
		num2 += powers_of_ten[b % 10];
		b /= 10;
	}

	if (num1 == num2) return true;
	return false;
}

std::pair<std::string, double> q70()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	std::vector<int> prims = primes(5000000);
	float current_min = 100000.0; //since we're minimizing the answer start with some random high number

	for (int i = 0; i < prims.size(); i++)
	{
		if (prims[i] > 3200) break; //all options from this point on will be greater than the limit of 10,000,000
		for (int j = i + 1; j < prims.size(); j++)
		{
			long long hey = (long long)prims[i] * (long long)prims[j];
			long long yo = hey;
			if (hey >= 10000000) continue; //start out at next prime
			
			hey -= hey / prims[i];
			hey -= hey / prims[j];

			if (permutationCheck(hey, yo))
				if ((float)yo / (float)hey < current_min)
				{
					current_min = (float)yo / (float)hey;
					answer = yo;
				}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 8319823
	//ran in 0.198021 seconds
}

//NOTES
//Instead of checking whether or not every single number below a certain number (n) share any prime factors to get the totient of n, it's instead possible to just
//take n - 1 and subtract from it a floor division of all of it's prime factors to get its totient. For example, n = 6 which has prime factors [2, 3]: floor((6 - 1), 2)
// = 2 and (6 - 1) - 2 = 3. Now take that value left behind, and subtract from it a floor division by the next prime factor: floor(3, 3) = 1, 3 - 1 = 2. Therefore 2 is
//the value of totient(6). The same works from prime numbers, for example the prime factors of 13 are [13]. Floor((13 - 1), 13) = 0, 12 - 0 = 12 so totient(13) = 12. The 
//totient of every number from 2-10,000,000 can be found in a sieve like manner by simply taking a prime number and dividing the current totient of every number divisible
//by the prime by that prime itself. The prime numbers don't even need to be calculated beforehand, they can be sieved at the same time to speed things up a little bit if
//two arrays are used to keep track of everything. Roughly half of the runtime here is for getting the totient of each number and the other half is for checking what
//numbers are permutations of their own totient. In total it takes just under half a second to find the answer, however, I find the ratio for every number under 10,000,000
//in that time frame as opposed to only some of them so I actually feel pretty good about this.

//After reading the forums someone pointed out that in order to minimize the ratio of n/totient(n) you need to minimize the number of distinct prime factors. Ideally you
//would use a prime number (as those only have one distinct prime factor) however, since a prime number is always 1 away from it's totient it's never possible for the two
//values to be permutations of eachother. The next best option would be to look at numbers that only have two distinct prime factors. If the primes below 5,000,000 are sieved
//at the start (5,000,000 is chosen because 2 * 5,000,000 = 10,000,000) then only the values obtained from multiplying two of these primes together needs to be checked. Using
//my same sieve type method for calculating the totient function and only checking numbers that are the product of two distinct primes yielded a 2.5x speedup, which admittedly 
//is less than I thought it would be. By only checking numbers with 2 prime factors under 10,000,000 there are 1,903,878 numbers to be checked compared to the 9,999,999 
//that I was checking before (which is 5.25x the amount of numbers being checked). It might be taking longer simply because in this versino I'm using a sieve function which
//returns a pretty large vector whereas before I wasn't. Either way I'll take any kind of speedup.