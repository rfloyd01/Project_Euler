#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Window into a Matrix
std::pair<std::string, double> q743()
{
	auto run_time = std::chrono::steady_clock::now();
	
	long long answer = 0, k = 100000000, n = 10000000000000000, mod = 1000000007;
	long long* inverse_i = new long long[k / 2 + 1];
	inverse_i[0] = 1; inverse_i[1] = 1;

	//compute successive modular inverses for values of i % mod
	for (long long i = 2; i <= k / 2; i++) inverse_i[i] = ModMult(i, inverse_i[i - 1], mod);
	inverse_i[k / 2] = ModularMultiplicativeInverse(inverse_i[k / 2], mod);

	for (long long i = k / 2 - 1; i >= 0; i--)
	{
		long long temp = inverse_i[i];
		inverse_i[i] = ModMult(inverse_i[i + 1], i + 1, mod);
		inverse_i[i + 1] = ModMult(inverse_i[i + 1], temp, mod);
	}

	long long f = ModPow((long long)2, n, mod, 1);
	long long divider = ModularMultiplicativeInverse(ModPow((long long)2, 2 * n / k, mod, 1), mod);
	answer = f;
	for (int i = 1; i <= k / 2; i++)
	{
		long long numerator = ModMult(k - 2 * i + 2, k - 2 * i + 1, mod);
		long long denominator = ModMult(ModMult(inverse_i[i], inverse_i[i], mod), divider, mod);
		f = ModMult(ModMult(f, numerator, mod), denominator, mod);
		answer = (answer + f) % mod;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 259158998
	//ran in 39.9036 seconds
}

//NOTES
//