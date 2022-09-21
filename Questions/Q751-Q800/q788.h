#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <cmath>

//Dominating Numbers
std::pair<std::string, double> q788()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0, mod = 1000000007;
	const long long N = 2022;
	long long facts[N + 2]; facts[0] = 1;

	//precalculate factorials % mod from 0 to 2022
	long long fact = 1;
	for (int i = 1; i <= N + 1; i++)
	{
		fact = (fact * i) % mod;
		facts[i] = fact;
	}

	for (long long i = 1; i <= ceil(N / 2.0); i++)
		answer = (answer + ModMult(ModPow((long long)9, i, mod, 1), ((BinomialModLargePrime(N + 1, N + 1 - i, mod, facts) - BinomialModLargePrime(2 * i - 1, (2 * i - 1) / 2, mod, facts)) % mod) + mod, mod)) % mod;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 471745499
	//ran in 0.0105267 seconds
}

//NOTES
/*
Original solution

for (long long i = 1; i <= N; i++) //i represents the length of the current number
{
	//first calculate the ways where 1 - 9 are the dominant digit
	long long ways = 0; //start with 1 way, represents all digits being the dominant digit
	for (long long j = i / 2 + 1; j <= i; j++) //j represents the amount of the dominant digit
		ways = (ways + ModMult(BinomialModLargePrime(i, j, mod, facts), ModPow((long long)9, i - j + 1, mod, 1), mod)) % mod;

	answer = (answer + ways) % mod;
}
*/