#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Totient Maximum
std::pair<std::string, double> q69()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1;
	
	std::vector<int> prims = primes(18);
	for (int i = 0; i < prims.size(); i++) answer *= prims[i];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 510510
	//ran in 0.0000126 seconds
}

//NOTES
//I actually already know a little bit about the totient function going into this question. To maximize the value of n/totient(n) you need to minimize the number
//of reltaively prime numbers there are to n while maximixing n itself. This is done by not repeating any prime factors, and only having distinct primes that make
//up a number. In the writing prompt they say that 6 produces a maximum in numbers below 10, this is because 6 is the lowest number that is the product of two distinct
//primes (2 and 3). The lowest number that's the product of three distinct primes is 30 (2, 3 and 5). Basically all we need to do is find the highest number below
//1,000,000 that is the product of succesive primes starting with 2 and we should have our answer. This can easily be done with pencil and paper to show that multiplying
//primes 2-17 yields the correct answer of 510,510.