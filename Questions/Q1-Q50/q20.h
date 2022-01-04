#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/int_64x.h>
#include <string>

//Factorial digit sum
std::pair<std::string, double> q20()
{
	//May be a better way to do this, but the question is trivial with my BigNum class. Runs in under .1 seconds and only took a minute or so to code
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int_64x ans(1);

	for (int i = 2; i <= 100; i++) ans *= i;

	std::string digits = ans.getNumberString();
	for (int i = 0; i < digits.length(); i++) answer += (digits[i] - 48); //subtracting 48 converts a number character to its integer value

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 648
	//ran in 0.000442 seconds
}

//NOTES
//UPDATE 9/22/21
//I initially solved this problem over a year ago but decided to come back to it because I've written a much more efficient big integer class. The new class makes
//absolute mincemeat out of this problem, it even puts my previous run time to shame. When I first solved this my run time was 0.032 seconds and now it's 0.000442
//which is 120x faster! If I can find a quicker way to convert int_64x to a string then I bet I could get the runtime down to the 10e-5 range on this problem.