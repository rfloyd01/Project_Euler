#pragma once

#include <Header_Files/pch.h>
#include <cmath> //includes log10f()

//Powerful Digit Counts
std::pair<std::string, double> q63()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1, base = 2, exponent; //make answer equal to one to account for the case of 1^1 being a 1 digit number
	bool cont = true;

	while (true)
	{
		exponent = 1;
		float log = log10f(base), current_num = log;
		while (true)
		{
			if ((int)current_num + 1 < exponent) break; //the number of digits will never catch back up to the exponent
			else if ((int)current_num + 1 > exponent)
			{
				//once we hit a point where the number of digits is greater than the exponent, no more numbers will work
				cont = false;
				break;
			}
			else answer++;
			exponent++;
			current_num += log; //instead of calculating the log (x^y) each iteration, just keep adding log(x)
		}
		if (!cont) break;
		base++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 49
	//ran in .0000073 seconds
}

//NOTES
//You can find the number of digits in a number by taking the log base 10 of that number and rounding up. For example ceiling(log10(563)) = ceiling(2.7505) = 3 and
//563 has 3 digits. The same can be done for large numbers represented by exponents. The number of digits in 16^25 (which would be too big to be represented by any
//of the built in c++ types) can be found by ceiling(log10(16^25)) = ceiling(25*log10(16)) = ceiling(30.103) = 31 digits. Using this fact it should be possible to solve
//this problem without the need to use very large numbers. We should only need two loops, one that cycles through base numbers and one that cycles through exponents.
//Move to the next base number when current exponent makes the number of digits smaller than the exponent itself and break out of both loops if the number of digits
//becomes greater than the exponent itself as the descepancy will only get worse as larger numbers are used.

//After running the problem I realized that this property can only apply to single digit numbers. As soon as you raise a double digit number to any power there will be more
//digits in the number than the exponent, and the effect gets worse the higher the exponent gets. The code currently runs in the blink of an eye so changing the first while
//loop to a for loop that ranges from 1-9 shouldn't really effect the time that much so I'll just leave the code as is.