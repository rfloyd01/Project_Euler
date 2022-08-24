#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes
#include <map>
#include <vector>

bool contains200(long long sqube)
{
	std::string squbeString = std::to_string(sqube);
	for (int i = squbeString.length() - 3; i >= 0; i--)
	{
		if (squbeString.substr(i, 3) == "200") return true;
	}
	return false;
}

//Find the 200th prime-proof sqube containing the contiguous sub-string "200"
std::pair<std::string, double> q200()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0;
	int sqube_count = 0;

	auto prims = primes(1000);
	for (int i = 0; i < prims.size(); i++)
	{
		for (int j = 0; j < prims.size(); j++)
		{
			if (j == i) continue;
			long long sqube = long long(prims[i] * prims[i] * prims[i]) * long long(prims[j] * prims[j]);
			if (contains200(sqube))
			{
				std::cout << prims[j] << "^2 * " << prims[i] << "^3 = " << sqube << std::endl;
				sqube_count++;
			}
		}
	}

	std::cout << sqube_count << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 126461847755
	//ran in 0.0008542 seconds
}

//NOTES
//Initial Thoughts:
/*
Sqube is a funny word. But my first thought here is that since they don't ask us to mod the answer or give any kind of shortened version that we're gonna be looking
for something that fits into an unsigned long integer. This means that the biggest square number we can have is 2^3 * n^2 = 2^64 -> n = 1,518,500,250, the largest
cubed number we can have is 2^2 * n^3 = 2^64 = 397,336. I'm not sure if this information will have any bearing on how the problem is solved, but it's good to know
some rough limits. Apart from that there are two major questions I have, first, is there a good way to guarantee that our sqube will have a 200 in it somewhere or
do we need to manually check for this? My second question is, is there an efficient way to test if a number is prime-proof? Even if there's a good way to find the 
right squbes, if we need to manually go in and change each of the digits and check for primality each time then the run-time for this problem could stil be
pretty long. I think before I get too far into this problem I need to try and find answers to these two questions.
*/

//First Attempt:
/*
So the first question that I want to answer is whether or not there's a good way to guarantee that the squbes will have a 200 in there somewhere. I wrote a brute force
algorithm to make some of these squbes just so I can try and find a pattern, here are a handful of the first ones it found:

2 * 2 * 2 * 5 * 5 = 200
2 * 2 * 2 * 499 * 499 = 1,992,008
2 * 2 * 2 * 751 * 751 = 4,512,008
5 * 5 * 5 * 449 * 449 = 25,200,125
23 * 23 * 61 * 61 * 61 = 120,072,949
13 * 13 * 13 * 509 * 509 = 569,200,957

The first on the list is easy enough, 2 * 5 = 10 which appears twice and then you have the extra 2. The next one isn't as clear so let's take a look at some of the 
different multiplications to try and find where the 200 is coming from:

499 * 499 = 249,001
2 * 2 * 2 = 8

so what's happening here is that the 8 we get from cubing 2 is smalle enough that it doesn't cause the 1 in the ones place to overflow. Then when we multiply
the 9 in the thousands place by 8 the result is 72, which puts the 2 right next to the 0s in the ten and hundreds place. I think we have a similar situation
with 2^3*751^2. How about 23^2*61^3?

23 * 23 = 529
61 * 61 * 61 = 226,981
1,968,409

Here it isn't really cut and dry at all, I don't really see something after these first two stages of multiplication to suggest that we'll get a 200 towards
the front of the number. I feel like I could potentially dig further into this but there's a good chance that there's no underlying relationship and we just need
to manually scan through each multiplication digit byt digit. This usually tends to be the case with other Project Euler problems that have to do with digits
in numbers, like with pandigitals. There's not really something underlying causing these numbers to happens, we just need to check for them as they pop up.

Instead of multiplying primes and hoping to get a 200 in the resulting product, what if we did the opposite? That is, start out with numbers that have 200 in them
and then see if their prime factorization is of the form p^2q^3
*/