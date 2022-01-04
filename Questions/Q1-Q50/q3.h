#pragma once

#include <Header_Files/pch.h>

//Largest prime factor
std::pair <std::string, double> q3()
{
	auto run_time = std::chrono::steady_clock::now();

	long long num = 600851475143, prim = 2, answer = 2; //set answer to 2, if the answer isn't 2 then it will be discovered in while loop
	long long stop_point = sqrt(num);

	while (num % 2 == 0) num /= prim;
	if (num != 1)
	{
		prim++;
		while (prim <= stop_point)
		{
			if (num == 1)
			{
				answer = prim - 2;
				break;
			}
			if (num % prim == 0) num /= prim;
			else prim += 2;
		}

		if (num == 1) answer = prim;
		else answer = num;
	}
	//ran in 0.0000305 seconds
	//HackerRank Score: 100.00 / 100.00

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 6857
	//ran in 0.0000305 seconds
}

//NOTES:
//Nothing fancy here, just a brute force approach. Starting with the prime number 2, keep dividing the goal number by the current prime number as many times as
//possible. As soon as the prime number doesn't evenly go into the goal number then increment it to the next number. It will increment to numbers that aren't prime,
//however, it will be impossible for the number to divide the goal number if it isn't prime because the current number will be made up of primes less than itself and
//all primes less than itself have already been divided out of goal.

//HACKERRANK UPDATE:
//After what happened with question 2 I decided to just try my code as it is. It passed every test except the last one as it exceeded the time limit. It seems that in
//the problem write-up they use an actual prime number as one of the test cases so most likely what's happening is that they've chosen a really large prime which is
//taking a long time to calculate. Maybe to get around this fringe case I'll just copy and paste my current Prime Number test to first see if a number is prime, if it
//isn't then I'll carry out the normal search loop. Adding the prime number test surprisingly didn't help the runtime for the last test case so there's something else
//going on here. Well in my original code I check all of the numbers from 1 up to n, and after doing lot's of Project Euler problems I know that there's no need to check
//any numbers greater than sqrt(n), so if I implement this limit to the check it'll probably help a lot. Only checking up to the square root won't be quite enough as it's
//possible that the greatest factor will be a prime number greater than the square root. Basically there will be two different outcomes, after dividing n by all numbers
//up to the square root n has a value of 1, then the greatest prime factor was less than the square root, however, if the final value is something other than 1 then the 
//final value itself will be the greatest prime factor. Here are two examples:
//n = 100
//limit = sqrt(100) = 10
//100 / 2 = 50, 50 / 2 = 25, 2 !/ 25, 3 !/ 25, 4 !/ 25, 25 / 5 = 5, 5 / 5 = 1, done. Largest prime factor is 5.
//
//n = 102
//limit = sqrt(102) = 10.09 = 10
//102 / 2 = 51, 2 !/ 51, 51 / 3 = 17, 3 !/ 17, 4 !/ 17, 5 !/ 17, 6 !/ 17, 7 !/ 17, 8 !/ 17, 9 !/ 17, 10 !/ 17, done. Largest prime factor is 17.
//
//To speed things up even a touch more, the number 2 can be handled first and seprately so that way when looping through the other numbers you can go up by 2 at a time
//instead of having to check each individual number. THe run time isn't really that different for the Project Euler example because the answer was less than the square root
//of n, for the HackerRank examples though it made a world of difference.

//HackerRank Score = 100.00 / 100.00

//Original Euler Code
/*
long long num = 600851475143, prim = 2;

	while (num > 1)
	{
		if (num % prim == 0) num /= prim;
		else prim++;
	}

	run_time = 0.0000485 seconds
*/
