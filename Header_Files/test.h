#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes permutationsOfPartitions()
#include <iostream>

long long permutationsOfPartitionsTest(int n, int l, int maximum, int minimum)
{
	//returns the number of distinct permutations for the partitions of n that have a length of l, 
	//where the maximum integer allowed is maximum and the least integer allowed is minimum. Maximum must be
	//greater than or equal to minimum, and less than or equal to n while minimum must be greater than or
	//equal to 1 and less than or equal to maximum.
	if (maximum == 0) maximum = n;
	else if (maximum > n) maximum = n;

	if (minimum == 0) minimum = 1;
	else if (minimum < 1) minimum = 1;
	else if (minimum > maximum) minimum = maximum;

	//This function utilizes generating functions to find the partitions.
	int target = n - minimum * l, primary = maximum - minimum + 1, flip = -1;
	int secondaryDenominator = target + primary, secondaryNumerator = l + secondaryDenominator - 1;
	long long answer = 0;

	//we get an equation of the form: [x^target]: (1 - x^primary)^l * (1 - x)^-l
	//for which we solve using binomial expansion
	for (int i = 0; i <= target / primary; i++)
	{
		answer += choose(l, i) * choose((secondaryNumerator -= primary), (secondaryDenominator -= primary)) * (flip *= -1);
		//std::cout << choose(l, i) % 1000000007 << " * " << choose(secondaryNumerator, secondaryDenominator) % 1000000007 << " % " << 1000000007 << " = ";
		//std::cout << (choose(l, i) * choose(secondaryNumerator, secondaryDenominator)) % 1000000007 << std::endl;
		//std::cout << "binomal(" << secondaryNumerator << ", " << secondaryDenominator << ") % 1000000007 = " << choose(secondaryNumerator, secondaryDenominator) % 1000000007 << std::endl;
	}

	return answer;
}

std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int dice_sides = 6, number_top_dice = 5, total_dice = 10;
	long long answer;

	std::cout << "Dice sides = " << dice_sides << std::endl;
	std::cout << "Top dice = " << number_top_dice << std::endl;
	std::cout << "Total dice = " << total_dice << std::endl << std::endl;

	//initialize an array for factorials % 1,000,000,007
	long long* factorials = new long long[total_dice + 1]();
	factorials[0] = 1;
	for (int i = 1; i <= total_dice; i++) factorials[i] = factorials[i - 1] * i;//% modulus;

	//Memoize ways to split dice into two sections
	long long* ways_to_choose = new long long[number_top_dice + 1]();
	for (int i = 0; i <= number_top_dice; i++) ways_to_choose[i] = (factorials[total_dice] / (factorials[i] * factorials[total_dice - i]));

	//for (int goal = number_top_dice; goal <= number_top_dice * dice_sides; goal++)
	for (int goal = 17; goal <= 27; goal++)
	{
		answer = 0; //re-zero the answer

		//int goal = 25, dice_sides = 6, number_top_dice = 5, total_dice = 10;
		int larger_than_lowest, lowest, lower_than_lowest;
		long long modulus = 1000000007;

		//minimum_minimum represents the smallest possible value for the minimum top die
		int minimum_minimum = goal - (number_top_dice - 1) * dice_sides;
		if (minimum_minimum <= 0) minimum_minimum = 1;
		//maximum_minimum represents the largest possible value for the minimum top die
		int maximum_minimum = goal / number_top_dice;

		if (minimum_minimum <= dice_sides)
		{
			//if the lowest possiblevalue for our minimum value top die is greater than the number of sides 
			//on the dice, then it won't be possible to reach the goal so we skip all of the below logic
			for (int least_top_die_value = minimum_minimum; least_top_die_value <= maximum_minimum; least_top_die_value++)
				//for (int least_top_die_value = minimum_minimum; least_top_die_value < minimum_minimum + 1; least_top_die_value++)
			{
				//calculate both the most and least number of least valued top die we can have
				//for the current least top die value
				int fewest;
				int most;

				if (least_top_die_value * number_top_dice == goal)
				{
					//the equation for setting the fewest and most possible top dice for the current least top die value
					//breaks down if our goal number can only be reached by having all the top dice be the maximum number.
					//For example if the goal was 120, we could only get there if every Top Die was 12.
					fewest = number_top_dice;
					most = number_top_dice;
				}
				else
				{
					//these equations should work in all other cases
					fewest = (number_top_dice * (least_top_die_value + 1)) - goal;
					most = ((number_top_dice * dice_sides) - goal) / (dice_sides - least_top_die_value);
					if (fewest <= 0) fewest = 1;
					if (most > dice_sides) most = number_top_dice;
				}
				for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice <= most; amount_of_least_top_dice++)
					//for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice < fewest + 1; amount_of_least_top_dice++)
				{
					//First we calculate the total ways to shuffle the dice that are less than or equal to the least valued top die
					int number_lower_dice = total_dice - number_top_dice + amount_of_least_top_dice;
					long long small_shuffle = MyPow(least_top_die_value, number_lower_dice) - MyPow(least_top_die_value - 1, number_lower_dice);

					for (int i = 1; i < amount_of_least_top_dice; i++)
						small_shuffle -= factorials[number_lower_dice] / (factorials[i] * factorials[number_lower_dice - i]) * (long long)pow(least_top_die_value - 1, number_lower_dice - i);

					//std::cout << "small shuffle = " << small_shuffle << std::endl;

					//Next we calculate the total ways to shuffle the top dice that are greater than the least valued top die
					long long large_shuffle = permutationsOfPartitionsTest(goal - amount_of_least_top_dice * least_top_die_value, number_top_dice - amount_of_least_top_dice, dice_sides, least_top_die_value + 1);

					//std::cout << "large shuffle = " << large_shuffle << std::endl;
					//We multiply the ways to individually shuffle the upper and lower dice with the overall number of ways to shuffle
					//the dice together

					//std::cout << "overall shuffle = " << ways_to_choose[number_top_dice - amount_of_least_top_dice] << std::endl;
					answer += ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle;
					/*std::cout << "MTD = " << least_top_die_value;
					std::cout << ", # of MTD = " << amount_of_least_top_dice;
					std::cout << ", Ways % MOD = " << (ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle) % 1000000007 << std::endl << std::endl;*/
				}
			}
		}

		std::cout << "Top dice goal = " << goal << std::endl;
		std::cout << "Ways to shuffle MOD 1,000,000,007 = " << answer % 1000000007 << std::endl << std::endl;
	}

	//ran in 0.000304 seconds

	//First attempt: 29/51 failed
	//Second attempt:

	delete[] factorials;
	delete[] ways_to_choose;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}