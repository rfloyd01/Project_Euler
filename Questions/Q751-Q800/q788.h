#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Dominating Numbers
std::pair<std::string, double> q788()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0, mod = 1000000007;
	const int N = 2022;
	long long facts[N + 1]; facts[0] = 1;

	//precalculate factorials % mod from 0 to 2022
	long long fact = 1;
	for (int i = 1; i <= N; i++)
	{
		fact = (fact * i) % mod;
		facts[i] = fact;
	}

	for (long long i = 1; i <= N; i++) //i represents the length of the current number
	{
		//first calculate the ways where 1 - 9 are the dominant digit
		long long ways = 1; //start with 1 way, represents all digits being the dominant digit
		for (long long j = i / 2 + 1; j < i; j++) //j represents the amount of the dominant digit
		{
			//first add all possible ways
			ways = (ways + ModMult(BinomialModLargePrime(i, j, mod, facts), ModPow((long long)9, i - j, mod, 1), mod)) % mod;

			//then subtract out ways where we can have a leading 0
			ways = ((ways - ModMult(BinomialModLargePrime(i - 1, j, mod, facts), ModPow((long long)9, i - j - 1, mod, 1), mod)) % mod) + mod;
		}

		//multiply ways by 9 now to count all digits 1-9
		ways = ModMult(ways, (long long)9, mod);

		//Now we need to calculate the number of ways where 0 is the dominant digit, this should be nothing 
		//more than the number of ways where 1-9 can be the dominant digit for n - 1
		for (long long j = i / 2 + 1; j < i; j++)
			ways = (ways + ModMult(BinomialModLargePrime(i - 1, j, mod, facts), ModPow((long long)9, i - j, mod, 1), mod)) % mod;

		answer = (answer + ways) % mod;
	}
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 471745499
	//ran in 17.1918 seconds
}

//NOTES
/*
* Notes are on project-floyd.com
*/