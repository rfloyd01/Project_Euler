#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/int_64x.h>

//Powers of Two
std::pair<std::string, double> q686()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	double diff = (log10(124) - log10(123)) / log10(2);
	int k = 0, hitNumber = 0;

	while (true)
	{
		double upper = ( k++ + log10(124)) / log10(2);
		if ((long long)upper > (long long)(upper - diff))
		{
			if (++hitNumber == 678910)
			{
				answer = (long long)upper;
				break;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 193060223
	//ran in 0.0929607 seconds
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

1545
485, 196, 289, 196, 289, 196, 289, 196,
3681
485, 196, 289, 196, 289, 196, 289, 196,
5817
485, 196, 289, 196, 289, 196, 289, 196,
7953
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
681, 485, 485, 485

So it's definitely changing quite a bit, furthermore, the code I wrote to generate this hits does so by moving forward by the original pattern. If the pattern changes
and I'm only moving forward by the original pattern it's possible that I'm skipping over certain values. To confirm this I want to iterate over every number in the area
where I see the pattern changing above, however, it becomes painstakingly slow to check every single number. If I knew the pattern was going to change every set number of 
terms and by a set amount it would be one thing, but that doesn't appear to be the case. Assuming that I haven't missed any terms then the pattern lasts for 24, 49, 36 and 30
terms from what I can see, but then only lasts for 4 before changing again which takes me to the end of my data. So I'm a little stumped here, considering that this problem
is only supposed to be 5% difficulty there must be something major that I'm missing so it may be time to approach this from a different angle. Maybe it has something to do 
with the prime factorization of the exponents? That seems unlikely though because the pattern is additive and not multiplicative. Looking at the binary forms of the numbers
probably wont help either, since they're all powers of 2 it'll just be a 1 followed by a bunch of zeros.

If I'm not able to solve this with a pattern, maybe I can come up with some kind of equation to help. We're looking for integer powers of 2 that will be less than 124 * 10 ^ x and 
less than 123 * 10 ^ x for all values of x. Using logarithms we can see that when x = 10, 2^x = 1,240,000,000,000 ---> xlog(2) = log(124 * 10,000,000,000) ---> xlog(2) = log(124) + log(10^10)
---> xlog(2) = log(124) + 10 ---> x = (log(124) + 10) / log(2) ---> x = 40.173 and (log(123) + 10) / log(2) = 40.162. There aren't any whole numbers that come between 40.173 and 40.162
so we know that there is no power of 2 that will get a value of 123 followed by 10 zeros. We can extend this to any power of 10 to easily see if there are any powers of 2 that will
be a whole number. Our first hit found with the brute force algorithm was 2^90. 2^90 = 10^27  so applying this to the other equations (log(124) + (27-2)) / log(2) = 90.002 and
(log(123) + (27-2)) / log(2) = 89.9907. In this scenario we see that the non-decimal part of the number extends from 89 to 90 which means that 2^90 will give a value that starts with 123.

Basically what we get are two different linear equations:
y1 = (log(123) + x) / log(2)
y2 = (log(124) + x) / log(2)

y2 - y1 = ((log(124) + x) - (log(123) + x)) / log(2)
y2 - y1 = (log(124) - log(123)) / log(2) = 0.01168

So for any given power of 10, the value of the exponent that 2 is raised to to get the number 124(0) will always be greater than the exponent to get a number of the form 123(0)
by a value of 0.01168. What this boils down to, is that as long as the decimal portion of the equation y = (log(124) + x) / log(2) is less than 0.01168, then we'll have a hit.

I implemented a brute force-ish approach where I calculate all values of our upper range and then subtract the difference to the lower range (0.01168...) I then cast both of 
these doubles to integers and if the upper integer is greater than the lower integer we have a hit. I didn't think this approach would be that effective but surprisingly it works
in under 100 milliseconds. Just goes to show that what I consider to be brute force solutions aren't truly brute force these days.
*/