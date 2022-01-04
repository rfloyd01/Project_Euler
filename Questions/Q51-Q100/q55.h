#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/BigInt_128.h>
#include <vector>

long long getPalindrome(long long n)
{
	long long pal = 0;
	while (n > 0)
	{
		pal *= 10;
		pal += n % 10;
		n /= 10;
	}
	return pal;
}

//Lychrel Numbers
std::pair<std::string, double> q55()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	uint8_t* lychrel_numbers = new uint8_t[10000]; //each index can hold a value of 0, 1 or 2. 0 = untested, 1 = non-lychrel, 2 = lychrel
	for (int i = 0; i < 10000; i++) lychrel_numbers[i] = 0; //set everything to 0 to start off
	long long current_number = 0;

	for (int i = 1; i < 10000; i++)
	{
		if (lychrel_numbers[i]) continue; //skip numbers that have already been tested
		std::vector<long long> current_chain; //create vector for new chain
		bool lychrel = true;
		current_number = i;

		for (int j = 0; j < 50; j++)
		{
			long long pal = getPalindrome(current_number);
			if (current_number == pal) //test current number for palindromeness
			{
				if (j > 0) //starting on a palindrome doesn't count, as can be seen in example text
				{
					lychrel = false;
					for (int k = 0; k < current_chain.size(); k++)
					{
						if (current_chain[k] >= 10000) break; //no need to keep track of numbers outside of current limit
						lychrel_numbers[current_chain[k]] = 1; //set every number in current chain to be non-lycheral
					}
					break; //no need to iterate all the way up to 50
				}
			}
			current_chain.push_back(current_number);
			current_chain.push_back(pal);
			current_number += pal;
		}

		//vprint(current_chain);

		if (lychrel)
		{
			for (int k = 0; k < current_chain.size(); k++)
			{
				if (current_chain[k] >= 10000) break; //no need to keep track of numbers outside of current limit
				lychrel_numbers[current_chain[k]] = 2; //set every number in current chain to be lycheral
				answer++; //add each lychrel number under 10000 towards the answer
			}
		}
	}

	//delete lychrel numbers array from heap memory
	delete[] lychrel_numbers;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
//This problem looks like it could be pretty tough if it didn't have such a low limit, but since we're only checking numbers below 10,000 I think a brute force method
//should be able to work fine. Also this won't be a true brute force. Any number in the current chain of numbers should be considered non-Lychrel as soon as a palindrome is
//found. In the example we can see that when starting at 349 we add on 943 to get 1292, which then has 2921 added to it and so on and so forth. All of the numbers in this chain
//(349, 943, 1292, 2921, 4213 and 3124) will ultimately end up at the palindrom 7337 no matter which one we start from. Because of this, all of these numbers can be set as
//non-lycheral without the need to retest each individual one. This should a little bit of a speed up in the algorithm. The same can be said for numbers that are Lychrel. The
//last thing, since we're limited to 50 iterations and the largest number to test is 9,999 then some of the larger numbers dealt with in this problem might be able to overflow
//a 64-bit integer. Start off by using long integers but if something seems screwy will need to switch to a big integer type or use an array to hold individual digits.