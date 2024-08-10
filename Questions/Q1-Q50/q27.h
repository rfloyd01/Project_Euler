#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <vector>
#include <cmath>

//Quadratic primes
bool IsPrime(int n)
{
	if (n < 2) return 0;

	int limit = sqrt(n) + 1;
	for (int i = 2; i < limit; i++)
		if (n % i == 0) return 0;

	return 1;
}

std::pair<std::string, double> q27()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0, longest = 0;
	const int limit = 1000;
	std::vector<int> prims; prims.push_back(0);
	std::vector<int> p = primes(limit);

	for (int b = p.size() - 1; b >= 0; b--)
	{
		if (b < longest) break;
		for (int a = -b + 1; a < limit; a++)
		{
			int n = 0, counter = 1;
			int num = (n * n) + (a * n) + p[b];
			while (n < b)
			{
				if (num < 0)
				{
					if ((n + 1) > longest)
					{
						longest = n + 1;
						answer = a * p[b];
					}
					break;
				}
				if (num > prims.size())
				{
					int add = num - prims.size();
					for (int i = 0; i <= add; i++) prims.push_back(0);
				}
				if (prims[num] == 0)
				{
					if (IsPrime(num)) prims[num] = 1;
					else prims[num] = -1;
				}

				if (prims[num] == 1)
				{
					n++;
					num += (a + counter);
					counter += 2;
				}
				else
				{
					if ((n + 1) > longest)
					{
						longest = n + 1;
						answer = a * p[b];
					}
					break;
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is -59231
	//ran in 0.0104571 seconds
}

//NOTES
//Initial thought was that since |a| and |b| are both < 1000 there are 2000 * 2000 = 4,000,000 total combinations which shouldn't be too bad
//to brute force. Had to create a function which checks if a number is prime. Initial brute force approach ran with no problem and a runtime
//of .17 seconds. I added a hash table so that I don't have to check the same numbers over and over for being prime and run time improved to
//.033 seconds which was about a 5x speedup. There are probably better prime number testing functions out there, but this one was fine for this q.

//Update 12/31/21
/*
After looking at the math a little bit I've found a few things that will help speed up the algorithm. Maybe not a whole lot, but they'll definitely help.

First and foremost, 'b' MUST be a prime number. This is because we're told the chain must start with n = 0. Well when n is 0 the only thing going to our final
value is 'b', so if b isn't prime then we can't even start our chain with n = 0. Limiting 'b' to be a prime number both takes all negative values for 'b' out of the
search space, but also all positive non-prime numbers. 

We can also limit our search of what 'a' can be. Since prime numbers are positive, we can rearange the governing equation into an inequality:

n^2 + an + b > 1 (because 2 is the first prime number)
a > 1 - b/n - n

Since we already know that we get a prime when n is 0 (because we force 'b' to be a prime number) we substitute the smallest value of n into the equation
which is 1. 

a > 1 - b - 1
a > -b

A final note here, is that the maximum value for n will be b - 1. If we substitue 'b' in for n the governing equation becomes b^2 + ab + b = b * (b + a + 1). The
final number will be divisible by b and therefore can't be a prime number. Knowing this fact, it now makes more sense to start our search from the highest value of
b and work our way downwards. As soon as we reach a value of b that's smaller than our current longest chain we can break out of the algorithm because it won't be
possible to find a longer answer.

Making this changes offered an ~10x speedup. Not as much of a speedup as I was hoping but still, not too shabby. If I could find out some way to limit the maximum value
of a based on the value of b I could do better here, however, I'm currently not sure of a good way to limit the upper bound of a (other than capping it at 1000 as the problem
suggets). I played around with changing the limits of a and b from 1,000 all the way up to 100,000 and found that the maximal chains always seem to come when 'a' is a negative
number, however, it's still possible to get decently long chains when a is positive. For now I'm forced to leave the upper bound on 'a' as it is.

I also tried to speed up the algorithm by using the Miller-Rabin primality test, however, this actually made things go slower which seems a bit counter-intuitive to me. Maybe
it's really only suitable for really large numbers.
*/
