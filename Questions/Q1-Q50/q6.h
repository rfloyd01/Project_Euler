#pragma once

#include <Header_Files/pch.h>

//Sum square difference
std::pair<std::string, double> q6()
{
	auto run_time = std::chrono::steady_clock::now();

	unsigned long long n = 100;
	unsigned long long num1 = n * n * (n * n + 2 * n + 1) / 4, num2 = n * (n + 1) * (2 * n + 1) / 6;

	return { std::to_string(num1 - num2), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 25164150
	//ran in 0.0000006 seconds
}

//NOTES:
//I'm once again writing notes much after I coded up the solution, but it's pretty easy to see what I did here. To square the sum of 1 - 100 I just took the 100th traingle number
//and squared it. I then just manually take the square of each number 1 - 100 and then take the difference between the two numbers. This problem was easily brute forciable so I
//jumped on that opportunity.

//HACKERRANK Update:
//A big improvement on my initial algorithm (although I only did the Euler solution the way I did because I knew it would be easy to brute force) is to sum the squares using a very
//similar equation to that of the triangle numbers. Sum of numbers 1 - n = n * (n + 1) / 2, sum of square numbers 1 - n^2 = n * (n + 1) * (2 * n + 1) / 6. Using this fact this problem
//can be solved with just a few easy multiplcations, divisions and the final subtraction, regardless of what size n is. Understandably so, the new program runs 755x faster than the
//original one.

//HackerRank Score = 100.00 / 100.00

//Original Euler Code
/*
int num1 = 5050 * 5050, num2 = 0;

	for (int i = 1; i <= 100; i++)
		num2 += i * i;

	run_time = 0.0006041 seconds 
*/
