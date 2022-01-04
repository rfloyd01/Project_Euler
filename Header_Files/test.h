#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <vector>
#include <algorithm>

//Non-abundant sums
std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1; //answer could be initialized to no-pivot as there's only one case of that

	long long goal = 100;

	//maximal partition length found using triangle number equation and quadratic formula
	long long max_length = (-3 + sqrt(9 + 8 * (goal - 1))) / 2.0, final_value, current_num = 0;

	//start number is the second number in the final group (aka the number with the longest
	//partition and highest pivot point). calculated from triangular number formula
	long long start_number = (max_length + 1) * (max_length + 2) / 2;
	long long current_factorial = factorial(max_length + 1); //used for faster calculation of factorial
	long long pivot_stop = max_length - 1 - (goal - start_number) - 1; //when pivot reaches this location the maximum possible length decreases by 1
	long long double_pivot_start = max_length;
	if (max_length + start_number > goal) double_pivot_start--;

	for (long long pp = max_length - 1; pp >= 0; pp--) //pp stands for pivot point you pervs
	{
		//reduce partition length when pivot point reaches appropriate spot
		if (pp == pivot_stop) current_num -= final_value;

		//separate multiplication and division into two steps to avoid floating point errors
		current_num *= (pp + 3);
		current_num /= (pp + 2);

		//add new values
		current_factorial /= (pp + 2);

		//calculating the factorial each iteration is slow, keep a running tally of the current factorial instead
		long long new_values = current_factorial * (pp + 3) * (pp + 1);
		current_num += new_values;

		//set final row value
		if (pp == (max_length - 1)) final_value = current_num; //set the first time
		else
		{
			final_value *= (pp + 3);
			final_value /= (pp + 2);
		}

		//calculate no pivot value by multiplying case where pivot is at the very end
		answer += new_values * (pp + 2) / (pp + 3);

		//calculate double pivot by cascading every number up by 1. This value should only be
		//calculated in the first iteration of loop if start_number + max_length < goal which is
		//why there's the double_pivot_start variable
		if (pp < double_pivot_start) answer += new_values * (pp + 2) / 2 * (pp + 4) / (pp + 3);

		answer += current_num;
	}
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)
}
