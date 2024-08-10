#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h> //includes coprime
#include <algorithm>

//Digit Sum Numbers
std::pair<std::string, double> q725()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0, mod = 1E16;
	int max_length = 2020;

	for (long long i = 1; i < 10; i++)
		answer = (answer + ModMult(i, (((ModMult((long long)2, binomialMod(max_length + i - 2, i, mod), mod)) - (max_length - 1)) + mod) % mod, mod)) % mod;

	answer = ModMult(answer, ModMult(((ModPow((long long)10, (long long)max_length, mod, 1) - 1) + mod) % mod, ModularMultiplicativeInverse((long long)9, mod, 0), mod), mod);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4598797036650685
	//ran in 0.0098699 seconds
}

//NOTES
