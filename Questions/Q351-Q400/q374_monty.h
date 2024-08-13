#include <Header_Files/pch.h>
#include "Functions/montgomery_space.h"

std::pair<std::string, double> q374_monty()
{
	auto run_time = std::chrono::steady_clock::now();

	long long goal = 100000000000000, mod = 982451653;
	long long max_length = (-3 + sqrt(9 + 8 * (goal - 1))) / 2.0;
	MontgomeryNumber answer = { 1 }, final_value = { 0 }, current_num = { 0 };

	//Define a Montgomery Space which will allow us to carry out calculations without the need
	//of modulus division.
	MontgomerySpace space(mod);

	//start number is the second number in the final group (aka the number with the longest partition and highest pivot point). calculated from triangular number formula
	long long start_number = (max_length + 1) * (max_length + 2) / 2;
	MontgomeryNumber current_factorial = { space.Transform(1) };
	for (uint64_t i = 2; i <= max_length + 1; i++) current_factorial = space.Multiply(current_factorial, { i });

	//when pivot reaches this location the maximum possible length decreases by 1 (don't mod this number)
	long long pivot_stop = max_length - 1 - (goal - start_number) - 1;
	long long double_pivot_start = max_length;
	if (max_length + start_number > goal) double_pivot_start--;

	//Memoize modular multiplicative inverses up to max_length + 2 for quicker calculations
	//space.calculateInverses(max_length + 2);

	//TODO: the pp variable should become a Montgomery Number. Memoize the Montgomery space versions
	//of 1, 2 and 3 for quicker calculations of things like pp + 3 since the the 3 in this case will 
	//also need to be a Montgomery number
	for (uint64_t pp = max_length - 1; pp != -1; pp--) //pp stands for pivot point you pervs
	{
		//before changing the "current_num" variable we need to check and see if the pivot location will cause the allowable maximum
		//length to change. This "shrink" effect will only every need to happen one time
		//when the pivot point get's lower than max_pivot - (goal_number - start_number). In the case of goal = 100, our start number
		//is 91 as this is the value when pivot is as far forwards as possibe. The max pivot location in this example is at index 11
		//of our array. 11 - 9 = 2, so as soon as the pivot becomes less than 2, i.e. the first time it hits 1 we need to subtract the 
		//final value of our cumulative multiplication. Keep track of our final value each iteration and when the time comes just
		//subtract it from the "current_num" variable.
		if (pp == pivot_stop) current_num = space.Subtract(current_num, final_value);

		current_num = space.Multiply(current_num, { pp + 3 });
		current_num = space.Divide(current_num, { pp + 2 });

		//add new values
		current_factorial = space.Divide(current_factorial, { pp + 2 });
		MontgomeryNumber new_values = space.Multiply(space.Multiply(current_factorial, { pp + 3 }), { pp + 1 }); //calculating the factorial each iteration is slow, keep a running tally of the current factorial instead
		current_num = space.Add(current_num, new_values);

		//set final row value
		if (pp == (max_length - 1)) final_value = current_num; //set the first time
		else
		{
			final_value = space.Multiply(final_value, { pp + 3 });
			final_value = space.Divide(final_value, { pp + 2 });
		}

		//calculate no pivot value by multiplying case where pivot is at the very end
		answer = space.Add(answer, (space.Divide(space.Multiply(new_values, { pp + 2 }), { pp + 3 })));

		//calculate double pivot by cascading every number up by 1. This value should only be
		//calculated in the first iteration of loop if start_number + max_length < goal which is
		//why there's the double_pivot_start variable
		if (pp < double_pivot_start) answer = space.Add(answer, (space.Divide(space.Multiply(space.Divide(space.Multiply(new_values, { pp + 2 }), { 2 }), { pp + 4 }), { pp + 3 })));

		answer = space.Add(answer, current_num);
	}

	return { std::to_string(answer.value), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}