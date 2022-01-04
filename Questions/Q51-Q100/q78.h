#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Coin Partitions
std::pair<std::string, double> q78()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	std::vector<int> partitions, pentagonal, polarity;
	pentagonal.push_back(1); partitions.push_back(1);
	int counter = 1, temp;
	bool cont = true;

	//Get as many new partitions as possible with current pentagonal number before calculating the next
	while (cont)
	{
		if (counter % 2 == 1) pentagonal.push_back(pentagonal.back() + (counter + 1) / 2);
		else
		{
			temp = counter / 2 + 1;
			pentagonal.push_back((3 * temp * temp - temp) / 2);
		}
		counter++;

		//now loop from second to last pentagonal number to last number
		for (int i = pentagonal[pentagonal.size() - 2]; i < pentagonal.back(); i++)
		{
			partitions.push_back(0);
			for (int j = 0; j <= pentagonal.size() - 2; j++)
			{
				if (j % 4 < 2) partitions.back() += partitions[i - pentagonal[j]];
				else partitions.back() -= partitions[i - pentagonal[j]];
			}
			partitions.back() %= 1000000;

			if (partitions.back() == 0)
			{
				cont = false;
				answer = i;
				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 55374
	//ran in 0.0298627 seconds
}

//NOTES
//Another partition problem, and this one actually references coins so it seems like I was pretty spot on in comparing these problems to question 31. This problem
//will probably have the largest ceiling of all of these problems as it wants us to find the first numnber with a number of partitions divisible by 1,000,000. The 
//total number of partitions for n don't take that long to physically get over 1,000,000 but to be evenly divisible by it might take awhile. My gut says that a long
//long integer probably won't be enough to contain this problem so I'll probably need to use bigint unless I can find a more clever way to do this.

//The algorithm used for questions 31, 76 and 77 overflows a long integer before even finding the partitions of 500 (none of which was divisible by 1,000,000). I'm not 
//sure how high up the answer will be, but even if the bigint class is used this dynamic programming approach might still prove to take quite a long time. After doing
//some googling, it looks like Euler (what a surprise) found a generating function for the partitions of any integer n. P(n) = P(n - 1) + P(n - 2) - P(n - 5) - P(n - 7)...
//where the numbers 1, 2, 5, 7 ... are the pentagonal number sequence and the polarity switches every two terms (i.e. +, +, -, -, +, +...). Kind of an interesting
//equation but I'm not gonna dive into the theory behind it. I'm not actually positive if this function will help for this problem. Since we're looking for a number that's
//divisible by 1,000,000 but this function uses an additive approach and not a multiplicative one. Decided to use the Euler method anyway and it did get the correct answer,
//however, it takes almost 8 seconds to run which is by far the worst time I have for any of these problems. Reading the forums showed that most other people took the same
//approach that I did, however, they kept track of partitions % 1,000,000 so that no overflow ever happened. This is a much more efficient way to do it as the bigint class
//which is inherently slow is no longer needed. Since we're only keeping track of the last 6 digits, the problem is done when we first find a partition that equals 0 %
//1,000,000. The run time went from 8 seconds down to .03 seconds just by using modular arithmetic and integers for a ~260x speedup.