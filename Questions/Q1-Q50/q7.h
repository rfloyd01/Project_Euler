#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes primes()

//10001st prime
std::pair<std::string, double> q7()
{
	//putting an upper bound of 200,000 assuming that prime numbers occur roughly every 20 numbers
	auto run_time = std::chrono::steady_clock::now();
	std::vector<int> prims = primes(200000);

	//the list starts at index 0 so the 10,001st prime is at index 10,000
	return {std::to_string(prims[10000]), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 104743
	//ran in 0.001208 seconds
}

//NOTES:
//To solve this, I looked at a decent amount of prime numbers to see if there's a rough pattern as to how often they pop up. Short answer, there isn't a set pattern
//however, they seem to pop up at least every 10 numbers or so when looking at relatively small numbers. To be on the safe side, assume that there will only be 1 prime
//every 20 numbers, therefore to get the 10,001st prime we'll need to generate the primes under 200,000. The Sieve of Erasthones can do this without breaking a sweat.
//It turns out that the 10,001st prime was 104,743 so if we had assumed a prime one in every ten numbers we would've come up a little bit short, so I'm glad I was on the
//safe side here.

//HACKERRANK UPDATE:
//I didn't change my code at all and I still got a 100. It seems that having an effective prime number algorithm is the only real important part of this problem.

//HackerRank Score = 100.0 / 100.0