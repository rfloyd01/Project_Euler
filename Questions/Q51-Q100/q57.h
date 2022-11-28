#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/BigInt.h>

//Square Root Convergents
int digitNumber(bigint n)
{
	//returns the number of digits in n
	int number_of_digits = 0;
	while (n > 0)
	{
		number_of_digits++;
		n /= 10;
	}
	return number_of_digits;
}

std::pair<std::string, double> q57()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0, limit = 1000;
	long double fraction[2] = { 3.0, 2.0 };
	bool numerator = 0, denominator = 1;

	for (int i = 1; i <= limit; i++)
	{
		if (fraction[numerator] >= 10.0)
		{
			if (fraction[denominator] < 10.0) answer++;

			fraction[numerator] /= 10.0;
			fraction[denominator] /= 10.0;
		}

		fraction[numerator] += fraction[denominator];
		fraction[denominator] += fraction[numerator];

		numerator = !numerator;
		denominator = !denominator;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 153
	//ran in 0.0000082 seconds
}

//NOTES
//After messing around a little bit with paper and pencil you can see that each iteration can be found by taking the 
//current fraction, adding one, flipping the numerator and denominator, and then adding one again. It's a pretty simple
//algorithm and doing 1000 iterations shouldn't take long. Also coded up a down and dirty function to see how many digits
//a number has. The numerator and denominator ended up getting much bigger than I thought they would so I ended up 
//needing to use the BigInt class to handle this problem.