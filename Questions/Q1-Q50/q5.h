#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes()
#include <cmath>

//Smallest Multiple
std::pair<std::string, double> q5()
{
	auto run_time = std::chrono::steady_clock::now();

	unsigned long long answer = 1; // start at 1 so we can multiply the answer accordingly
	int n = 20;
	
	std::vector<int> prims = primes(n);
	for (int i = 0; i < prims.size(); i++) answer *= pow(prims[i], (int)(log(n) / log(prims[i])));
	//ran in 0.0000235 seconds
	//HackerRank score = 100.00 / 100.00

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 232792560
	//ran in 0.0000235 seconds
}

//NOTES:
//This problem has everything to do with prime factorization. I didn't actually write notes down when I first solved this problem, but it looks like what I was trying to do was to
//look at the prime factorization for every number less than 20. Whichever instance had the most prime factors for a certain number would become that numbers contribution to the 
//final answer (for example, the prime factorizations of the following numbers: 3, 6, 9, 12, 15, 18 are [3], [2, 3], [3, 3], [2, 2, 3], [3, 5] and [2, 3, 3] respectively. The 
//instance that features the most 3 is 9 (and 18) with two 3's. This means that if our answer has 3 * 3 in its own prime factorization then anything else divisible by 3 will be
//able to evenly divide it. In this manner it's actually really easy to calculate the answer by hand. Answer = 2^4 * 3^2 * 5 * 7 * 11 * 13 * 17 * 19 = 232,792,560. I wanted to
//actually solve this problem with code though so I created the loop seen above.

//HACKERRANK UPDATE:
//Now that I'm revisiting this problem the logic is going to stay the exact same, however, instead of calculating the prime factorization for each number underneath our goal, all
//we need to do is look at all of the prime numbers under our goal and see what the largest exponent possible is that we can put on each of these primes. Looking at the original
//Euler problem, the largest exponent you could put on the number 2 and have it be less than 20 is 4 (2 ^ 4 = 16 while 2 ^ 5 = 32). This means that the maximum number of 2's possible
//is 4 and that our answer should have exactly 4 2's in its prime factorization. To quickly find the maximum value of the exponent for each prime we can just use logarithms. 
//log(20) / log(2) = 4.322 which gets rounded down to 4. So to find our answer we just loop through all of the primes that are less than n and multiply our answer by pow(i, log(n) / log(i))
//Using logs gave a 22x speedup over my initial algorithm which is nice, as well as making the code much more concise and readable.

//HackerRank Score = 100.00 / 100.00

//Original Euler Code
/*
int maximum = 20, answer = 1;

	std::vector<int> prims = primes(maximum);
	std::vector<int> factors;
	for (int i = 0; i < prims.size(); i++) factors.push_back(0);
	for (int i = 2; i <= 20; i++)
	{
		std::vector<int> facts = p_factors(i);
		for (int j = 0; j < facts.size(); j++)
		{
			if (facts[j] > factors[j]) factors[j] = facts[j];
		}
	}

	for (int i = 0; i < factors.size(); i++)
		answer *= pow(prims[i], factors[i]);

	run_time = 0.0005283 seconds
*/
