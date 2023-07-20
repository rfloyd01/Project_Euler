#pragma once

#include <Header_Files/pch.h>
#include <cmath> //includes log10f()

//Powerful Digit Counts
std::pair<std::string, double> q63()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	for (int b = 1; b < 10; b++) answer += 1.0 / (1 - log10f(b));

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 49
	//ran in 0.0000059 seconds
}

//NOTES
//You can find the number of digits in a number by taking the log base 10 of that number and rounding up. For example ceiling(log10(563)) = ceiling(2.7505) = 3 and
//563 has 3 digits. The same can be done for large numbers represented by exponents. The number of digits in 16^25 (which would be too big to be represented by any
//of the built in c++ types) can be found by ceiling(log10(16^25)) = ceiling(25*log10(16)) = ceiling(30.103) = 31 digits. Using this fact it should be possible to solve
//this problem without the need to use very large numbers. We should only need two loops, one that cycles through base numbers and one that cycles through exponents.
//Move to the next base number when current exponent makes the number of digits smaller than the exponent itself and break out of both loops if the number of digits
//becomes greater than the exponent itself as the descrepancy will only get worse as larger numbers are used.

//After running the problem I realized that this property can only apply to single digit numbers. As soon as you raise a double digit number to any power there will be more
//digits in the number than the exponent, and the effect gets worse the higher the exponent gets. The code currently runs in the blink of an eye so changing the first while
//loop to a for loop that ranges from 1-9 shouldn't really effect the time that much so I'll just leave the code as is.

//UPDATE
//Knowing that the maximum value for the base is 10, just iterate the base from 1 to 9 and figure out the value of the exponent that will first make the number of digits 
//less then the actual exponent. This leads to a maximum exponent value of e = 1 / (1 - log(b)). This number will always have a decimal since b < 10 so floor this value
//and add it to the answer after each iteration.