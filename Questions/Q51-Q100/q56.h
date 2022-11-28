#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/BigInt.h>

//Powerful Digit Sum
std::pair<std::string, double> q56()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	for (int a = 2; a < 100; a++)
	{
		bigint A(a);
		for (int b = 2; b < 100; b++)
		{
			//total brute force here, don't have an exponent function in the BigNum class so multiply
			//by the number of times the exponent is
			A *= a;
			int current_sum = 0;
			bigint C = A;

			//std::cout << a << "^" << b << " = " << A << std::endl;

			while (C > 0)
			{
				current_sum += (C % 10);
				C /= 10;
			}
			//std::cout << "Digit sum = " << current_sum << std::endl << std::endl;
			if (current_sum > answer) answer = current_sum;
		}
	}
	//ran in 0.0281492 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 972
	//ran in 0.0281492 seconds
}

//NOTES
//I wrote a big integer class for a reason, should be able to brute force this one without too much effort.

//The brute force approach ended up taking much much longer than I thought it would until I realized that I was 
//doing way too many multiplictions. For each increment of b I was starting the multiplication over. For example,
//when A was 5 and B was 6 I calculated out 5 * 5 * 5 * 5 * 5 * 5. This wasn't necessary though because I had already calculated
//5^5 in the step before, and 5^4 in the step before that. Basically each time b increments I just need to multiply
//A by A again and not start over from scratch. Doing the initial method would yield about 4,500 multiplications
//for each iteration of A. Another thing this problem taught me is that my BigNum class is woefully inefficient.
//This code runs in about 10 seconds whereas the same exact code written in Python only takes like .3 seconds (would
//expect to run in roughly .007 seconds in c++). This is because python has a much better method of handling really
//large numbers. I'm going to take some time to refine my BigNum class and see how it improves my run time for this problem.

//Ended up just copying a big integer library from online, maybe I should've just done this awhile ago... Using the same
//algorithm the code now runs in under .1 seconds. I could probably improve the run time even more by ignoring certain
//numbers but I think this is fast enough for now.