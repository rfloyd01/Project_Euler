#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/int_64x.h>

//Powers of Two
std::pair<std::string, double> q686()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 90, hits = 1; //start at first found exponent
	long long differences[3] = { 196, 289, 485  }; //these were found by examaning patterns

	while (hits < 678910)
	{
		for (int i = 0; i < 3; i++)
		{
			if ((int)pow(10, (answer + differences[i]) * log10(2) - (int)((answer + differences[i]) * log10(2)) + 2) == 123)
			{
				answer += differences[i];
				hits++;
				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 193060223
	//ran in 0.0301722 seconds
}

//NOTES
/*
My guess here is that there's going to be some kind of pattern here where every x-th number or so will start with 123. My first order of busines will be to use the 
int_64x class to try and get the first few values and see if anything stands out. Before looking for patterns beginning with '123' I started with '12', any number 
that starts with '123' will be a subset of the '12' pattern so it seemed like a good place to start looking.

An interesting pattern does occur here. It seems that there will be a span with no hits, then there will be either three or four hits that are 10 apart from each other, another span 
of no hits and then three or four more all 10 away from eachother. Doing a search to find hits under 2^1000 gets the following results:
[7, 80, 90, 100, 110, 183, 193, 203, 276, 286, 296, 369, 379, 389, 399, 472, 482, 492, 565, 575, 585, 595, 668, 678, 688, 761, 771, 781, 854, 864, 874, 884, 957, 967, 977]

excluding 2^7 which is on its own, here's the pattern for number of consecutive hits that are off by 10 [4, 3, 3, 4, 3, 4, 3, 3, 4, 3]. And here's the length of the gap 
between these groups of hits: [73, 73, 73, 73, 73, 73, 73, 73, 73, 73] well that's interesting. The first term in a group of hits always occurs 73 powers later. Interesting,
so there's definitely something going on here. Now to take a look at hits that start with '123'. The hits are [90, 379, 575, 864]. This means that the power of two will start 
with '123' when it's the second value in one of the groups of 4 for powers of 2 that start with '12'. Searching for hits under 5000 gives:
[90, 379, 575, 864, 1060, 1545, 1741, 2030, 2226, 2515, 2711, 3000, 3196, 3681, 3877, 4166, 4362, 4651, 4847]. I want to see the distance between hits as I'm sure there will
be a similar pattern that was seen in the numbers start with '12'. I've taken the hits out to 2^8000 this time.
[90, 289, 196, 289, 196, 485, 196, 289, 196, 289, 196, 289, 196, 485, 196, 289, 196, 289, 196, 289, 196, 485, 196, 289, 196, 289, 196, 289, 196, 485]. This is exactly what 
I'm looking for. This pattern is very repeatable, with the exception of the first set, it seems that we always have [485, 196, 289, 196, 289, 196, 289, 196] and the pattern 
repeats itself every 8 terms so with this it should be fairly trivial to find out answer. This pattern starts on the 6th term with a value of 2^1060, the 45th term would then 
be 4.875 patterns away (or in other words, 4 full patterns and then 7/8 of the fifth pattern). In this case it would be easier to go 5 full paterns away and then subtract the 
last term which is 196. The sum of a full pattern is 2136 so the 45th term in the sequence should be 2^(1060 + 5*21336 = 11740). Hmm, that doesn't match the question prompt
though, the 45th term should be 12,710 which means that the pattern maybe doesn't repeat out into infinty. Here's what the difference pattern looks like for hits 25 - 50:


[196, 289, 196, 289, 196, 485, 485, 196, 289, 196, 289, 196, 485, 485, 196, 289, 196, 289, 196, 485, 485, 196, 289, 196, 289, 196]

So it looks like there are two 485 terms next to eachother here at terms 30 and 31, then again at 37 and 38. This would make the value of the 37th and 45th terms 10089 and
12710 respectively. This complicates things, if the sequence changes over time then I won't be able to just use a simple formula to get to the answer like I thought I would,
although if the differences still only include the numbers 196, 289 and 485 (which is simply 196 + 289) then all isn't lost. After carying out the sequence for the first 150 
hits (starting with the 6th hit) this is what the pattern looks:

485, 196, 289, 196, 289, 196, 289, 196,
485, 196, 289, 196, 289, 196, 289, 196,
485, 196, 289, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 196, 289, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 196, 289, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485, 196,
485, 485, 485, 485,

So it's definitely changing quite a bit, however, it seems that even though the pattern changes only the same three numbers are used. Instead of iterating over the 
pattern I had initially found it may be enough to just try adding any of the above three numbers to the current exponent. 485 is the sum of 196 and 289 so it should 
be the last number tested, otherwise numbers may get skipped.

The last thing to do is to find a good way to tell if the first three digits of our number are 123. During my brute force testing above I used an int_64x, converted 
it into a string and then just tested whether the first three characters were "123", this is obviously very inefficient. What would be better would be to use 
logarithms to figure it out. If we want the first three digits of a number to equal 123 then the number would look something like this:

10^x = 123yy

In this scenario x doesn't have to be an integer, it can feature a decimal amount. Every time we subtract 1 from whatever this number is it has the effect of dividing
the number on the right side of the equation by 10.

10^x = 123yy
10^(x - 1) = 123y.y
10^(x - 2) = 123.yy
10^(x - (int)x) = 1.23yy

As can be seen, if we remove the integer part of the exponent entirely then we'll be left with a number less than 10 on the right side of the equation. So to see if the 
first three digits of our power of 2 are equal to 123 we can use the below equation:

2^exp = 10^x
exp*log(2) = x
exp*log(2) - (int)(exp*log(2)) = x_decimal_only
(int)10^(x_decimal_only + 2) = three digit number

If we raise 10 to the decimal portion of our known exponent log(2) and add 2 to it then we'll be guaranteed to get a three digit number. All we need to do from here is cast
this three digit number to an integer to remove the decimals and see whether or not the number is 123. Starting from the first known exponent that was found (because this is 
the only exponent I found which doesn't follow the set pattern) and only adding one of the numbers [196, 289, 485] allows us to find the answer in about 30 milliseconds.
*/