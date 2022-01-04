#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes ModPow()

//Self Powers
std::pair<std::string, double> q48()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	for (long long i = 1; i <= 1000; i++) answer += ModPow(i, i, (long long)10000000000, 1);
	answer %= 10000000000;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 9110846700
	//ran in 0.0020581 seconds
}

//NOTES
//Stealing shamelessly a little bit here, but I know that there exists a function which quickly returns a^b % c, call it ModPow(a, b, c). Since we only care about the last ten
//digits then we just need to sum ModPow(n, n, 10^10) for n = 1 - 1000. Take the answer mod 10^10 and that should be our answer. I had to modify my ModPow() function to include
//Modular Multiplication to avoid overflowing a 64-bit integer.