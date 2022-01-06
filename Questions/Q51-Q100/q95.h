#pragma once

#include <Header_Files/pch.h>
#include <cmath>

//Amicable chains
int calculateChainLength(int* chain_values, int* chain_lengths, int current_length, int current_index, int& starting_index, const int& limit)
{
	if (current_index > limit)
	{
	    //we've hit a value that's larger than our limit, nothing in this chain can be considered
	    return -1;
	}
	else if (*(chain_lengths + current_index) == 1)
	{
	    //we've encountered a value that's in our current chain. If the current number is equal to the starting number than we've
		//succesfully found a chain and everything is good. If we're NOT at the starting number we've found a lollipop chain. Set
		//the current node to 0 so that as we work back, we'll know that when we hit a 0 we shouldn't alter it. This will allow for
		//the chain to be discovered again at a later time.
		if (current_index == starting_index) return current_length;
		else
		{
			*(chain_lengths + current_index) = 0;
			return 0;
		}
	}
	else if (*(chain_lengths + current_index) > 1)
	{
		//we've encountered a value that's in a different chain, this means none of the numbers in our current chain
		//will be able to get back to themselves. Return -1 to show that we've hit a dud
		return -1;
	}
	else if (*(chain_lengths + current_index) < 0)
	{
		//we've hit a number that's known to fail so everything in the current chain will also fail
		return -1;
	}
	else
	{
		//we've hit an unseen number add it to the current chain and carry on, set the current value to 1 to indicate
		//that it's part of the current chain then do the next recursive call
		*(chain_lengths + current_index) = 1;
		int new_length = calculateChainLength(chain_values, chain_lengths, current_length + 1, *(chain_values + current_index), starting_index, limit);
		if (*(chain_lengths + current_index) == 0)
		{
			//this number is part of an actual chain but was discovered as part of a lollipop chain. Keep it's value at 0 so it can
			//be discovered later. Everything in the chain before this can be set to -1 as it's the "lollipop" portion of the chain.
			return -1;
		}
		else
		{
			*(chain_lengths + current_index) = new_length;
			return *(chain_lengths + current_index);
		}
	}
}

std::pair<std::string, double> q95()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	const int limit = 1000000;
	int* divisor_sums = new int[limit + 1]();
	int* chain_lengths = new int[limit + 1]();
	int maximum_chain_length = 0;

	for (int i = 1; i < limit; i++)
	{
		int j = i + i;
		while (true)
		{
			if (j > limit) break;
			*(divisor_sums + j) += i;
			j += i;
		}
	}

	for (int i = 0; i <= limit; i++)
	{
		if (chain_lengths[i] == 0)
		{
			int length = calculateChainLength(divisor_sums, chain_lengths, 0, i, i, limit);
			if (length > maximum_chain_length) maximum_chain_length = length;
		}
	}

	//iterate throguh the array until we hit our first value equal to maximum_chain_length
	for (int i = 0; i <= limit; i++)
	{
		if (chain_lengths[i] == maximum_chain_length)
		{
			answer = i;
			break;
		}
	}

	delete[] divisor_sums;
	delete[] chain_lengths;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 14316 (with a chain length of 28)
	//ran in 0.0193472 seconds
}

//NOTES
/*
Since no element in the chain can exceed 1,000,000 I think it'd be best to just create an array with 1,000,001 elements that will hold the sum of proper
divisor sums for each possible number. These sums can be calculated pretty easily with a sieve via pure addition and iteration. From here it's just a 
matter of keeping track of which numbers are a part of long chains where no member exceeds 1,000,000. A second array will be used to hold chain lengths
for individual numbers to remove the need for recalculating. Any number that's part of a chain that goes over 1,000,000 will get a value of -1 to indicate
that the chain will have no value.

The biggest thing I'm not currently sure of is if there are "lollipop" type chains. For example, looking at the problem text the number 12,496 starts
a chain of length 5. If there are numbers other than 14,264 whose proper divisors sum to 12,496 it won't actually be able to get back to it's own value.
This mystery number would hit 12,496 right off the bat but then the chain of length 5 would keep repeating without hitting the starting number ever again.
Now that I type this, I think I've answered my own question, if I number isn't capable of getting back to itself then it isn't part of a chain.
*/