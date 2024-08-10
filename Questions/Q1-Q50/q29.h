#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes gcd()
#include <Header_Files/print.h>
#include <map>

//Distinct powers	
std::pair<std::string, double> q29()
{
	auto run_time = std::chrono::steady_clock::now();

	for (int test = 512; test < 513; test++)
	{
		long long answer = 0;
		int limit = test; //maximum allowable exponent for problem
		int baseTwoLimit = log(limit) / log(2); //represents highest exponent where 2^baseLimit <= limit

		//Calculate the total number of numbers below or equal to the limit that can be 
		//described as a different number raised to a power. For example, if the limit was 
		//30 then these numbers would be (4, 8, 9, 16, 25, 27). When a number can be expressed
		//as multiple powers (i.e. 16 could be 2^4 aor 4^2) we put it in the category with the 
		//higher exponent.
		std::vector<long long> powerNumbers(baseTwoLimit + 1, 0);
		powerNumbers[1] = limit - 1; //can't raise 1 to any power so remove it

		for (int i = baseTwoLimit; i > 1; i--)
		{
			double correctRoot = pow(limit, 1.0 / i) + 0.00001; //need the addition for floating point errors
			powerNumbers[i] = (int)correctRoot - 1; //remove 1 as an option

			//remove any numbers of the current power that could be described by a different
			//number of a higher power
			for (int j = i + i; j <= baseTwoLimit; j += i) powerNumbers[i] -= powerNumbers[j];
			powerNumbers[1] -= powerNumbers[i];
		}


		for (int i = 1; i <= baseTwoLimit; i++)
		{
			//use a modified sieve to remove powers that overlap
			int* sieve = new int[limit + 1]();

			for (int j = 1; j < limit + 1; j++) sieve[j] = j;

			for (int j = i - 1; j > 0; j--)
				for (int k = 0; k <= j * limit / i; k += j / gcd(j, i)) sieve[k] = 0;

			long long total = 0;
			for (int j = 0; j < limit + 1; j++)
				if (sieve[j]) total++;

			answer += total * powerNumbers[i];
			delete[] sieve;
		}
		answer -= powerNumbers[1]; //remove all powers of 1 that have been raised to the first power only (i.e. 6^1)

		/*int testanswer = 0;
		std::map<std::vector<int>, bool> used;
		std::vector<int> primes; for (int i = 0; i <= limit; i++) primes.push_back(0);

		for (int a = 2; a <= limit; a++)
		{
			std::vector<int> prime_factors = PrimeFactors(a);
			std::vector<int> primes; for (int i = 0; i <= limit; i++) primes.push_back(0);
			for (int i = 0; i < prime_factors.size(); i++) primes[prime_factors[i]]++;

			for (int b = 2; b <= limit; b++)
			{
				for (int i = 0; i < prime_factors.size(); i++) primes[prime_factors[i]]++;
				if (!used[primes]) used[primes] = 1;
			}
		}

		if (used.size() != answer)
		{
			std::cout << "Issue when limit = " << limit << std::endl;
			std::cout << "Brute force method = " << used.size() << ", Fast method = " << answer << std::endl;
		}*/
	}
	
	int answer = 0;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 9183
	//ran in 0.0000175 seconds
}

//NOTES
//The numbers in this question can get large, but instead of storing the individual numbers I create a vector that will hold the 
//prime factorization of each number found. All primes from 2-100 will be included in each numbers vector,
//i.e. 10 = [0, 0 1, 0, 1, 0, 0, 0.... 0] where the ones represent 2^1 and 5^1. 25 = [0, 0, 0, 0, 2, 0, 0...0] represents 5^2.
//A map is used to store each of these prime factor vectors to make sure that no duplicates are kept track off.

/*
My guess as to the original approach I took in 2014
int answer = 0;
std::vector<int_64x> distinctValues;

for (int a = 2; a <= 100; a++)
{
	for (int b = 2; b <= 100; b++)
	{
		int_64x num = BigPow(a, b);
		if (!std::count(distinctValues.begin(), distinctValues.end(), num)) distinctValues.push_back(num);
	}
}

answer = distinctValues.size();
//ran in 0.0607721 seconds
*/

/*
Original Euler Code

int answer = 0;
std::map<std::vector<int>, bool> used;
std::vector<int> primes; for (int i = 0; i <= 100; i++) primes.push_back(0);

for (int a = 2; a <= 100; a++)
{
	std::vector<int> prime_factors = PrimeFactors(a);
	std::vector<int> primes; for (int i = 0; i <= 100; i++) primes.push_back(0);
	for (int i = 0; i < prime_factors.size(); i++) primes[prime_factors[i]]++;
	//std::cout << a << ", "; vprint(prime_factors);
	for (int b = 2; b <= 100; b++)
	{
		for (int i = 0; i < prime_factors.size(); i++) primes[prime_factors[i]]++;
		if (!used[primes]) used[primes] = 1;
	}
}

answer = used.size();
ran in 0.0082711 seconds
*/