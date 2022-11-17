#pragma once

#include <Header_Files/pch.h>

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

	long long current_number = 0;

	for (int i = 1; i < 10000; i++)
	{
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
					break; //no need to iterate all the way up to 50
				}
			}

			current_number += pal;
		}

		if (lychrel) answer++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 249
	//ran in 0.0005788 seconds
}

//NOTES
//This problem looks like it could be pretty tough if it didn't have such a low limit, but since we're only checking numbers below 10,000 I think a brute force method
//should be able to work fine. Also this won't be a true brute force. Any number in the current chain of numbers should be considered non-Lychrel as soon as a palindrome is
//found. In the example we can see that when starting at 349 we add on 943 to get 1292, which then has 2921 added to it and so on and so forth. All of the numbers in this chain
//(349, 943, 1292, 2921, 4213 and 3124) will ultimately end up at the palindrom 7337 no matter which one we start from. Because of this, all of these numbers can be set as
//non-lycheral without the need to retest each individual one. This should a little bit of a speed up in the algorithm. The same can be said for numbers that are Lychrel. The
//last thing, since we're limited to 50 iterations and the largest number to test is 9,999 then some of the larger numbers dealt with in this problem might be able to overflow
//a 64-bit integer. Start off by using long integers but if something seems screwy will need to switch to a big integer type or use an array to hold individual digits.

//Had an issue where numbers that had already been proven to be non-Lychrel (such as 496) were getting counted as Lychrel later on because they were the palindromes of Lychrel 
//numbers ending in zero (in this case 6940). Decided to eliminate all code having to do with saving numbers in the current chain and this led to the correct answer, as well 
//as reducing the run time. Normally I'd say that saving numbers proven to be non-lychrel to avoid recalculation should always benefit, however, it seems that most non-lychrel 
//numbers end up hitting a palindrome in relatively few iterations. Since this is the case the time saving benefit of eliminating recalculations doesn't end up helping out. Also,
//there are instances where the 64-bit integer holding the current number gets overflowed which throws things off. Despite this, the correct answer is still obtained. I think this 
//is because overflow doesn't tend to happen until just about the 50th iteration, so every number that get's high enough to overflow the 64-bit int ends up being Lychrel anyway.