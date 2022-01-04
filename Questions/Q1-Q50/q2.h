#pragma once

#include <Header_Files/pch.h>
#include <iomanip>
#include <vector>

//Even Fibonacci numbers
std::pair<std::string, double> q2()
{
	auto run_time = std::chrono::steady_clock::now();

	unsigned long long answer = 2; //include the first positive number
	unsigned long long fib[2] = { 0, 2 };
	unsigned long long maximum = 4000000;

	while (true)
	{
		fib[0] += 4 * fib[1];
		if (fib[0] > maximum) break;
		answer += fib[0];
		fib[1] += 4 * fib[0];
		if (fib[1] > maximum) break;
		answer += fib[1];
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4613732
	//ran in 0.0000006 seconds
}

//NOTES:
//Adding up the fibonacci numbers is easy enough, and it can be seen that only every third number in the sequence is even. Made an array that
//holds every three numbers in the sequence where fib[0] is always the even number, so only fib[0] is summed.

//HACKERRANK UPDATE:
//Once again the ante has been upped immensley in the HackerRank version of the problem. The original ceiling to this problem in the
//Project Euler version is 4,000,000 whereas in the HackerRank version it's 40,000,000,000,000,000. My original code just looped up to
//the original limit but when the limit is 4e16 this is not really feasible anymore (when you consider that there can be multiple
//test cases for every single test) to be under the 2 second time limit. As I mentioned above, only every third number in the sequence
//is actually even which can be used to our advantage. Since this sequence is a recurrence relation, then there's a good chance that a
//different recurrence relation can be made to only give every third number in the sequence. Like the original Fibonacci sequence, let's
//try and make this sequence by utilizing the two numbers before it (basically we want to try and get F15 in terms of F12 and F9, F12 in
//terms of F9 and F6...).
//
//F15 = F14 + F13
//F14 = F13 + F12
//F13 = F12 + F11
//---------------
//F15 = F13 + 2*F12 + F11
//F15 = 3*F12 + 2*F11
//---------------
//F11 = F10 + F9
//--------------
//F15 = 3*F12 + F11 + F10 + F9
//--------------
//F11 + F10 = F12
//--------------
//F15 = 4*F12 + F9
//
//Ok so that took a little work but we can see that the F15 = 4*F12 + F9 (610 = 4*144 + 34). Well what about F12 in terms of F9 and F6?
//
//F12 = F11 + F11
//F11 = F10 + F9
//F10 = F9 + F8
//---------------
//F12 = F10 + 2*F9 + F8
//--------------
//I can already see that the pattern should end up identical so let's just go ahead and test F12 = 4*F9 + F6 (144 = 4*34 + 8). So the
//same equation still works, which is perfect. Since the recurrence relation is the same for the whole Fibonnaci sequence it's safe to
//say that this relationship for every 3rd Fibonacci number should also be the same over the course of the sequence. Going in this manner,
//the sequence grows really large really fast, so even having to calculate each iteration of the relationship should be quick when going up
//to 4e16. In my original code, I knew that the end point was 4,000,000 so I used that as the ending point for a while loop. In the
//HackerRank version there will be different goal numbers. Because of this, we don't know if it will be the first or second number in fib[]
//that first exceeds the limit which means two if statements will be needed to check for overflow. An unsigned long long is good up until
//1.8e19 so there shouldn't be any integer overflow.

//This new method works lightning fast, even for values up to 4e16. I decided to try my old code with the new HackerRank limits and surprisingly
//enough the run time was about identical as with the new method. I guess the Fibonacci sequence ends up growing fast enough that having a large
//upper limit doesn't really do all that much. Well, it was still a good exercise to employ this other method.

//HackerRank Score = 100.0 / 100.0

//Orignal Euler Code
/*
int answer = 0;

	int fib[3] = { 2, 3, 5 };

	while (fib[0] < 4000000)
	{
		answer += fib[0];
		fib[0] = fib[1] + fib[2]; fib[1] = fib[0] + fib[2]; fib[2] = fib[0] + fib[1];
	}
	run_time = 0.0000003 seconds
*/