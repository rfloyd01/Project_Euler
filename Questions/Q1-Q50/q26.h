#pragma once

#include <Header_Files/pch.h>

//Reciprocal cycles
std::pair<std::string, double> q26()
{
	auto run_time = std::chrono::steady_clock::now();
	int length = 0, answer = 0;

	for (int n = 1000; n >= 2; n--)
	{
		if (n <= length) break;

		int len = 0, numerator = 1;
		int* used_numerators = new int[n]();

		while (true)
		{
			while (numerator < n)
			{
				numerator *= 10;
				len++;
			}

			numerator %= n;
			if (numerator == 0)
			{
				len = 0;
				break;
			}

			if (used_numerators[numerator])
			{
				len -= used_numerators[numerator];
				break;
			}
			else used_numerators[numerator] = len;
		}
		delete[] used_numerators;

		if (len > length)
		{
			length = len;
			answer = n;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
	//the answer is 983
	//ran in 0.0000331 seconds
}

//NOTES
//To get the appropriate decimal digits, multily the numerator by ten and then modulus divide the numerator by n. If the numerator is smaller
//than n a zero is the next decimal digit. The main theory here is that as soon as you get a numerator that has been seen before, the pattern
//will repeat itself so that is the end of the chain. This worked, however, it was including non-repeating digits in the overall chain
//i.e. 1/6 = .16666 with the 6 repeating forever, but the 1 digit was being counted in the chain. This was fixed by creating a vector called
//used_numerators. At every iteration the current numerator is appended to it. Whatever numerator is found that ends the chain, it's current
//location in used_numerators is where the chain starts. The RecurenceLength function would be shorter, but I included some printinf 
//functionality which shows the recurrence chain.

//12/29 UPDATE
/*
Looking back at my original algorithm I wasn't happy with the fact that it used both a map and a vecvtor to store values. I changed the map
from a int and bool to an int and an int. Every time a new numerator is found, the integer value of it's current spot in the overal number is
added as the key value. This way, the first time a repeat numerator is encountered we can just subtract the current length from the length
stored with the map key to see how long the chain is. This cut the run time down by roughly half (went from 0.0232 seconds to 0.014 seconds).
I decided to remove the map entirely and just create an array from 1 - n to store the numerator values. This isn't as efficient from a memory
standpoint, however, it reduced the runtime all the way down to 0.0005931 which is a 24x imporvement from the updated map approach (and an ~50x
improvement from the original approach).
I then had the realization that it would be much quicker to work from 1000 down towards 0 instead of the other way around. The reason for this is
that the maximum value for the chain length for a number n is n-1. This means that working downwards, as soon as we hit a value of n that's less than
or equal to the current chain length we can break out of the loop as no other numbers will be able to match the current chain length. Doing this led
to another 17x speedup for an overall improvement of about 420x over the original algorithm.
*/