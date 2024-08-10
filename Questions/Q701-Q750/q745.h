#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Sum of Squares II
int mobius(long long n)
{
	int p = 0;
	if (n == 1) return 1;
	if (n == 2) return -1;

	//Handle 2 separately
	if (n % 2 == 0)
	{
		n /= 2;
		p++;

		if (n % 2 == 0) return 0;
	}

	//check other prime factors
	for (int i = 3; i <= sqrt(n); i += 2)
	{
		if (n % i == 0)
		{
			n /= i;
			p++;

			if (n % i == 0) return 0;
		}
	}

	return (p % 2 == 0) ? -1 : 1;
}

std::pair<std::string, double> q745()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0, limit = 100, mod = 1000000007; //100000000000000

	//first create an array for the Mobius function and squares all the way up to sqrt(limit).
	//Maximal square value is 10^7 so no need for modulus division here
	long long* mob = new long long[sqrt(limit) + 1], *squares = new long long[sqrt(limit) + 1];
	for (long long i = 1; i <= sqrt(limit); i++)
	{
		mob[i] = mobius(i);
		squares[i] = i * i;
	}


	for (int i = 1; i <= sqrt(limit); i++)
	{
		long long numerator = limit / squares[i];
		for (int j = 1; j <= sqrt(limit / squares[i]); j++)
			answer = (((answer + ModMult(squares[i], numerator / squares[j] * mob[j], mod)) % mod) + mod) % mod;
	}
	delete[] mob;
	delete[] squares;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 94586478
	//ran in 21.2857 seconds
}

//NOTES
//To get the mobius function up to 10,000,000 takes 14.3 seconds