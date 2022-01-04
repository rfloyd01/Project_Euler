#pragma once

#include <Header_Files/pch.h>

//Multiples of 3 and 5
std::pair<std::string, double> q1()
{
	auto run_time = std::chrono::steady_clock::now();

	//unsigned longs are needed for the HackerRank version of the problem
	unsigned long long answer = 0, n = (1000 - 1);

	unsigned long long div_3  = (n / 3)  * (n / 3 + 1) / 2 * 3;
	unsigned long long div_5  = (n / 5)  * (n / 5 + 1) / 2 * 5;
	unsigned long long div_15 = (n / 15) * (n / 15 + 1) / 2 * 15;

	answer = div_3 + div_5 - div_15;
	//runs in 0.0000005 seconds
	//HackerRank Score: 100.00 / 100.00

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 233168
	//runs in 0.0000005 seconds
}

//NOTES
//The potential problem with this question is double counting numbers that are divisible by both 3 and 5 like 15. By using an or statement
//and looping through each number only once should get rid of that possibility. Another way to do this would be just to sum all numbers
//divisible by 3, all divisible by 5 and subtract everything divisible by 15 which is double counted. Sum of numbers 1-1000 divisible by three
//should be 3 * n * (n+1) / 2 where n is 333 so sum(3) = 166,833. sum(5) = 5 * 199 * 200 / 2 = 99,500. sum(15) = 15 * 66 * 67 / 2 = 33,165
//sum(3) + sum(5) - sum(15) = 233,168 so same answer as gotten by the program. This would make much more sense to do for larger numbers, but in
//this case both options basically run instantly

//HACKERRANK UPDATE:
//I recently discovered a website called HackerRank which has harder versions of all (well a lot of) the Project Euler problems so
//naturally I was drawn to give them a try. In the HackerRank versions of the Project Euler problems there are a certain number of
//cases that you have to pass, and each case may have multiple passes of the problem. For example, in this first problem they give you
//a test case that has two iterations in it. For the first iteration you need to find the sum of all numbers divisible by 3 and 5 that
//are less than 100, and in the second iteration the sum of all numbers less than 1000. The logic doesn't change between the different
//cases, only the maximum number. We're given that the maximum number of iterations for a single test is 100,000 and the largest value
//of N is 1,000,000,000! What an incredible step up from the oiriginal Project Euler version! So not only have we gone from needing to
//solve a single test to 100,000 tests, but the maximum value has gone from 1,000 all the way up to a billion. The final kicker is that
//your code needs to run in under 2 seconds. Running under 2 seconds is a joke (in C++ anyway) for the Euler version of the problem,
//however, there's no way that my current code would pass the HackerRank version (I'm not positive currently if 2 seconds is the time
//limit for all of the test cases combined, or if each test case has its own 2 second time limit).

//With all of that said, this problem should still be very straightforward. As I state in the original notes but opted not too do as the
//brute force case was quick enough, we can use triangle numbers and some multiplication to get the answer almost instantly, even for really
//high test cases. If we were to write all of the numbers less than 20 that are divisible by 3 we'd get (3, 6, 9, 12, 15, 18). Well if
//you were to divide all of those numbers by 3 then it would look like this: 3 * (1, 2, 3, 4, 5, 6). So instead of summing all the numbers
//less than 20 that are divisible by 3, we can instead just divide everything by 3 and sum all of the numbers that are divisible by 1
//and less than floor(20 / 3) = 6 and then just multiply that final sum by 3. Well the sum of numbers from 1 to n is given by the 
//following formula: sum from 1 to n = n * (n + 1) / 2 [this formula gives us what's known as a triangular number]. Plugging in 6 for n
//we get a sum for 1-6 = 21 and then we can multiply that value by 3 to find the sum of all of the numbers under 20 that are divisible
//by 3. 21 * 3 = 63 = 3 + 6 + 9 + 12 + 15 + 18. We can do the exact same thing for 5 ---> 20 / 5 = 4 --> triangle(4) * 5 = 50 --->
//5 + 10 + 15 + 20 = 50. We actually need to be careful here to not include n as the problem text states that it wants the sum of the
//numbers LESS than n, so really we want to do triangle(floor((20 - 1) / 5)) = 6 * 5 = 30 = 5 + 10 + 15. The final note here is that we
//will end up double counting numbers that are divisible by both 3 and 5 if we use this method (this was addressed automatically in my
//original code for the Project Euler version of the problem). All we need to do is subtract out any numbers that are divisible by 15
//from the final total.

//HackerRank Score = 100.0 / 100.0

//Original Euler Code
/*
int answer = 0;

	for (int i = 3; i < 1000; i++)
		if (i % 3 == 0 || i % 5 == 0) answer += i;

	Runtime = 0.0004699 seconds
*/