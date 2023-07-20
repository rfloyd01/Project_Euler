#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <cmath>

//Large Non-Mersenne Prime
std::pair<std::string, double> q97()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = ModPow((long long)2, (long long)7830457, 10000000000, 1);

	answer = (answer * 28433) % 10000000000 + 1;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 8739992577
	//ran in 0.0000066 seconds
}

//NOTES
/*
Probably a little cheap as I'm coming back to this problem already having a good ModPow function.
*/