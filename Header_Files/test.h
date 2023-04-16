#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>

int VectorGDCFinderTest(std::vector<int>& v, int ignore = -1)
{
	//finds the GCD between all elements in v while ignoring the element at ignore
	int current_gcd;

	//set the current_gcd to the first non-zero term that isn't at element ignore
	for (int i = 0; i < v.size(); i++)
		if (i != ignore && v[i])
			current_gcd = v[i];

	for (int i = 0; i < v.size(); i++)
	{
		if (i == ignore || !v[i]) continue;

		for (int j = i + 1; j < v.size(); j++)
		{
			if (j == ignore || !v[j]) continue;

			int new_gcd = gcd(v[i], v[j]);
			if (new_gcd == 1) return 1;

			int combined_gcd = gcd(current_gcd, new_gcd);
			if (combined_gcd == 1) return 1;

			current_gcd = combined_gcd;
		}
	}

	return current_gcd;
}

bool StrongNumberTest(std::vector<int>& prime_factors)
{
	if (prime_factors.size() < 2) return false;

	int current_factor = prime_factors[0], factor_count = 1;
	for (int i = 1; i < prime_factors.size(); i++)
	{
		if (prime_factors[i] == prime_factors[i - 1]) factor_count++;
		else
		{
			if (factor_count == 1) return false;
			else
			{
				current_factor = prime_factors[i];
				factor_count = 1;
			}
		}
	}

	if (factor_count == 1) return false;
	return true;
}

bool PerfectPowerTest(std::vector<int>& prime_factors)
{
	std::vector<int> prime_count;

	int current_prime = prime_factors[0], current_count = 0;

	for (int i = 0; i < prime_factors.size(); i++)
	{
		if (prime_factors[i] == current_prime) current_count++;
		else
		{
			prime_count.push_back(current_count);
			current_count = 1;
			current_prime = prime_factors[i];
		}
	}
	prime_count.push_back(current_count);

	if (VectorGDCFinderTest(prime_count) == 1) return false;
	return true;
}

bool AchillesNumberTest(std::vector<int>& prime_factors)
{
	return (StrongNumberTest(prime_factors) && !PerfectPowerTest(prime_factors));
}

int totient(std::vector<int>& prime_factors)
{
	int current_prime = 0, n = 1;
	for (int i = 0; i < prime_factors.size(); i++) n *= prime_factors[i];

	for (int i = 0; i < prime_factors.size(); i++)
	{
		if (prime_factors[i] != current_prime)
		{
			n /= prime_factors[i];
			n *= (prime_factors[i] - 1);
			current_prime = prime_factors[i];
		}
	}

	return n;
}

bool StrongAchillesNumberTest(int n, std::vector<std::vector<int> >& prime_factors)
{
	if (AchillesNumberTest(prime_factors[n]))
	{
		if (AchillesNumberTest(prime_factors[totient(prime_factors[n])]))
		{
			return true;
		}
	}

	return false;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int limit = 100000000;
	bool* achilles_numbers = new bool[limit + 1]();

	std::vector<std::vector<int> > prime_factors = AllPrimeFactors(limit);

	for (int i = 2; i <= limit; i++)
	{
		if (AchillesNumberTest(prime_factors[i])) achilles_numbers[i] = true;
	}

	for (int i = 3; i <= limit; i++)
	{
		if (!achilles_numbers[i]) continue;

		if (achilles_numbers[totient(prime_factors[i])])
		{
			std::cout << i << std::endl;

			answer++;
		}
		//std::cout << i << " " << totient(prime_factors[i]) << std::endl;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}