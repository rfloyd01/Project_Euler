#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes()
#include <vector>
#include <map>

//Distinct powers	
std::pair<std::string, double> q29()
{
	auto run_time = std::chrono::steady_clock::now();
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
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 9183
	//ran in 0.0082711 seconds
}

//NOTES
//The numbers in this question can get large, but instead of storing the individual numbers I create a vector that will hold the 
//prime factorization of each number found. All primes from 2-100 will be included in each numbers vector,
//i.e. 10 = [0, 0 1, 0, 1, 0, 0, 0.... 0] where the ones represent 2^1 and 5^1. 25 = [0, 0, 0, 0, 2, 0, 0...0] represents 5^2.
//A map is used to store each of these prime factor vectors to make sure that no duplicates are kept track off.
