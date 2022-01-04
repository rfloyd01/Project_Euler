#pragma once

#include <Header_Files/pch.h>
#include <cmath>

//Summation of primes
std::pair <std::string, double> q10()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 2; //a regular integer overflows so use a long integer

    const int n = 2000000;
    int* primez = new int[n](), stop = sqrt(n);
    for (int i = 3; i < n; i += 2) primez[i] = i;

    //first create the sieve
    long long i = 3; //use long long to prevent overflow upon multiplication
    for (; i < stop; i += 2)
    {
        if (primez[i])
        {
            answer += i; //add primes as they're found
            for (long long j = i * i; j < n; j += i) primez[j] = 0;
        }
    }

    //count all primes greater than the square root of n
    for (; i < n; i += 2)
        if (primez[i]) answer += i;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 142913828922
	//ran in 0.0039177 seconds
}

//NOTES:
//Nothing fancy here, just use the Sieve of Erasthonese and make sure to store the answer in a long long so there isn't any overflow.

//HACKERRANK UPDATE (10/20/21):
//The HackerRank version of the problem features a smaller limit, however, there's a maximum of 10,000 trials per single test. The Sieve of Erasthonese can be sieved over
//after completion to add each element to the element before it. This gives a running tally of the sum of primes that are less than or equal to each number in the sieve.
//Here's what that code looks like:
/*
//first create the sieve
    const int n = 2000000;
    long long* primez = new long long[n](), stop = sqrt(n);
    primez[2] = 2;
    for (int i = 3; i < n; i += 2) primez[i] = i;
    for (long long i = 3; i < stop; i += 2)
    {
        if (primez[i])
        {
            for (long long j = i * i; j < n; j += i) primez[j] = 0;
        }
    }

    //next sieve through the sieve and add successive elements to get sum of primes
    //less than or equal to n for all n under 2,000,000
    for (int i = 3; i < n; i++) primez[i] += primez[i - 1];

    long long answer = primez[n - 1];
    delete[] primez;
    //ran in 0.0143283 seconds
*/
//I had rewritten my original euler code to not call my primes() function and to instead just sieve in an array which sped up the code by a factor of 3 or so. Since the Euler code is
//faster than the HackerRank code I'm keeping the Euler Code as my most current update.

//Original Euler Code
/*
    long long answer = 0; //a regular integer overflows so use a long integer

    std::vector<int> prims = primes(2000000);
    for (int i = 0; i < prims.size(); i++) answer += prims[i];
    //ran in 0.0178745 seconds
*/