#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <vector>
#include <algorithm>

//Goldbach's Other Conjecture

bool findPrime(std::vector<int>& prims, int p)
{
	//use a binary search to see if prime p exists in the vector of primes
	int i = 0, j = prims.size() - 1;

	while (i > j)
	{
		int search_index = (i + j) / 2;

		if (prims[search_index] == p) return true;
		else if (prims[search_index] < p) i = search_index;
		else j = search_index;
	}
}

std::pair<std::string, double> q46()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	const int limit = 100000;

	std::vector<int> prims;
	bool sieve[limit], double_squares[limit]; 
	sieve[0] = 0; sieve[1] = 0;

	//initialize both sieves
	for (int i = 2; i < limit; i++)
	{
		sieve[i] = true;
		double_squares[i] = false;
	}

	//create sieve for prime numbers under limit. true means index is prime and false means it isn't
	for (int i = 2; i < limit; i++)
	{
		if (sieve[i])
		{
			prims.push_back(i);
			for (int j = 2 * i; j < limit; j += i)
			{
				sieve[j] = false;
			}
		}
	}

	//create sieve for double square numbers. true means the index is a double prime and false means it isn't
	for (int i = 1; i < limit; i++)
	{
		int num = i * i * 2;
		if (num >= limit) break;
		double_squares[num] = true;
	}

	bool cont = true;

	for (int i = 9; i < limit; i += 2)
	{
		//cycle through odd composite numbers and test if there is any combo of prime and double square that will sum to i
		if (sieve[i]) continue; //if number is prime don't bother testing
		int j = 0; 
		while (true)
		{
			if (prims[j] >= i)
			{
				//if none of the primes lower than i can be added to a double square to get i then we have our answer
				cont = false;
				answer = i;
				break;
			}
			if (double_squares[i - prims[j]]) break;
			j++;
		}

		if (!cont) break;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 5777
	//ran in 0.0005788 seconds
}

//NOTES
//Not sure how small the answer is, so start by making a seive of primes under 100,000. Do the same thing with double square numbers under 100,000. Cycle through odd numbers of the
//prime sieve and if it has a value of 0 then test all primes and double squares underneath it until a hit is found. If no hits are found then we have our answer. The answer ended
//up being much lower than I thought. After lowering the limit to 6,000 the program runs basically instantaneously. A little cheap to lower the limiar after getting the answer but
//what are you gonna do?

//Here's a method that doesn't require the use of a limit. Iterate up through the odd composite numbers and then keep
//subtracting double square amounts. Test for primality after each subtraction. If the subtraction get's to 1 or lower
//then we've found our answer.
/*
int answer = 9, max_prime = 0;
std::vector<int> prims = { 2, 3, 5, 7 }; //initialize with primes less than 9
while (true)
{
	int subtraction = 0, square = 0, adder = 1;
	bool cont = false;
	while (subtraction >= 0)
	{
		subtraction = answer - 2 * square;
		if (std::find(prims.begin(), prims.end(), subtraction) != prims.end())
		{
			cont = true;
			break;
		}
		square += adder;
		adder += 2;
	}
	if (!cont) break;
	answer += 2;

	//see if the new number is itself a prime
	while (primeNumberTest(answer))
	{
		prims.push_back(answer);
		max_prime = answer;
		answer += 2;
	}
}
*/