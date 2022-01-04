#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Largest palindrome product
bool palindromeTestSixDigits(int num)
{
	//tests whether or not a 6 digit number is a palindrome
	int digits[3];
	for (int i = 2; i >= 0; i--)
	{
		digits[i] = num % 10;
		num /= 10;
	}

	for (int i = 0; i < 3; i++)
	{
		if ((num % 10) != digits[i]) return false;
		num /= 10;
	}
	return true;
}

std::pair <std::string, double> q4()
{
	auto run_time = std::chrono::steady_clock::now();

	int maximum = 0, n = 1000000;
	n--; //decrement n so that we find the first palindrome that's LESS than n

	for (int i = 999; i > 99; i--)
	{
		if ((i * i) < maximum) break; //as soon as i^2 is less than current best we won't find any palindromes that can beat the current best
		int j = n / i < 1000 ? n / i : 999; //j starts at the highest value that will keep i * j less than or equal to n
		for (; j > 99; j--)
		{
			if ((i * j) < maximum) break; //break here. Since j increments downwards there's no chance of breaking current best
			int num = i * j; //create all 6-digit numbers that are a multiplication of two 3-digit numbers, then check if it's a palindrome
			if (palindromeTestSixDigits(num))
			{
				if (num > maximum)
				{
					maximum = num;
				}
				break;
			}
		}
	}
	//ran in 0.0000443 seconds
	//HackerRank score = 100.00 / 100.00

	return { std::to_string(maximum), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 906609
	//ran in 0.0000443 seconds
}

//NOTES
//First thought was to have two loops that cycle through all 3 digits numbers and multiply them by eachother, testing for palindrome-ness along the way. This code worked fine
//but ran in .24 seconds which I think can be imrpoved upon. The issue is that I started multiplying numbers starting at a hundred and working up to 999, but doing it this way
//there's no gaurentee that you've found the maximum and you must cycle through each possibility. If you start at 999 and work your way down it's much easier to tell when the
//maximum has been found. As soon as the first three digit number squared can no longer reach the current highest palindrome, then all other palindromes to be discovered will
//be smaller than the current max, so break out of the loop. Taking a top down approach with the exact same algorithm led to a 5x speed increase. Looking at my palindrome
//calculation algorithm I realized that could use some work as well. Instead of storing digits in a vector and comparing at the end, just use modular arithmetic to determine
//if digit 1 = digit 4, digit 2 = digit 5 and digit 3 = digit 6. Making this change yielded a 312x speed up on top of the original 5x speedup. In total this problem is now solved
//1527 times faster than the original algorithm!

//HACKERRANK UPDATE:
//This is the first HackerRank version of a Project Euler problem that doesn't actually seem that much more difficult than the original. We're still looking at a maximum of 6 digits
//here, however, there's a maximum of 100 trials per test. Considering that the current run time for my code is 0.0001571 seconds, doing 100 trials should theoretically only take
//0.01571 seconds which is well beneath the two second time limit. The only difference to make to the existing code is to set a limit on what value j can start at. We were looking at all
//6 digit numbers in the Euler version, however, with this one we only want to look at 6 digit numbers under a certain limit. So we always start i at 999 and then set j at (n / i) to make
//sure that multiplying by j won't give us a value larger than n. Surprisingly adding this little check caused my code to get an around 3x speedup which was nice.

//HackerRank Score = 100.00 / 100.00

//Original Euler Code
/*
    for (int i = 999; i > 99; i--)
	{
		if ((i * i) < maximum) break; //if i^2 can no longer reached current maximum then all other discovered palindromes will be smaller so algorithm is done
		for (int j = i; j > 99; j--)
		{
			int num = i * j;
			if (palindromeTestSixDigits(num))
			{
				if (num > maximum) maximum = num;
				break; //as soon as a palindrom is found break out of inner loop, only smaller palindromes can be found if continued
			}
		}
	}
	run_time = 0.0001571 seconds
*/