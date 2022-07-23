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
	long long answer = 0;

	int goal = 70, dice_sides = 12, number_top_dice = 10, total_dice = 20;
	int larger_than_lowest, lowest, lower_than_lowest;
	long long modulus = 1000000007;

	//initialize an array for factorials % 1,000,000,007
	long long* factorials = new long long[total_dice + 1]();
	factorials[0] = 1;
	for (int i = 1; i <= total_dice; i++) factorials[i] = factorials[i - 1] * i;//% modulus;

	//minimum_minimum represents the smallest possible value for the minimum top die
	int minimum_minimum = goal - (number_top_dice - 1) * dice_sides;
	if (minimum_minimum <= 0) minimum_minimum = 1;
	//maximum_minimum represents the largest possible value for the minimum top die
	int maximum_minimum = goal / number_top_dice;

	//Memoize ways to split dice into two sections
	long long* ways_to_choose = new long long[number_top_dice + 1]();
	for (int i = 0; i <= number_top_dice; i++) ways_to_choose[i] = (factorials[total_dice] / (factorials[i] * factorials[total_dice - i]));

	//for (int least_top_die_value = minimum_minimum; least_top_die_value <= maximum_minimum; least_top_die_value++)
	for (int least_top_die_value = minimum_minimum; least_top_die_value < minimum_minimum + 1; least_top_die_value++)
	{
		//calculate both the most and least number of least valued top die we can have
		//for the current least top die value
		int fewest = (number_top_dice * (least_top_die_value + 1)) - goal;
		int most = ((number_top_dice * dice_sides) - goal) / (dice_sides - least_top_die_value);
		if (fewest <= 0) fewest = 1;
		if (most > dice_sides) most = number_top_dice;
		//for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice <= most; amount_of_least_top_dice++)
		for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice < fewest + 1; amount_of_least_top_dice++)
		{
			//First we calculate the total ways to shuffle the dice that are less than or equal to the least valued top die
			int number_lower_dice = total_dice - number_top_dice + amount_of_least_top_dice;
			long long small_shuffle = MyPow(least_top_die_value, number_lower_dice) - MyPow(least_top_die_value - 1, number_lower_dice);

			for (int i = 1; i < amount_of_least_top_dice; i++)
				small_shuffle -= factorials[number_lower_dice] / (factorials[i] * factorials[number_lower_dice - i]) * (long long)pow(least_top_die_value - 1, number_lower_dice - i);

			std::cout << "small shuffle = " << small_shuffle << std::endl;

			//Next we calculate the total ways to shuffle the top dice that are greater than the least valued top die
			long long large_shuffle = permutationsOfPartitionsTest(goal - amount_of_least_top_dice * least_top_die_value, number_top_dice - amount_of_least_top_dice, dice_sides, least_top_die_value + 1);

			std::cout << "large shuffle = " << large_shuffle << std::endl;
			//We multiply the ways to individually shuffle the upper and lower dice with the overall number of ways to shuffle
			//the dice together

			std::cout << "overall shuffle = " << ways_to_choose[number_top_dice - amount_of_least_top_dice] << std::endl;
			answer += ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle;
			std::cout << "MTD = " << least_top_die_value; 
			std::cout << ", # of MTD = " << amount_of_least_top_dice;
			std::cout << ", Ways % MOD = " << (ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle) % 1000000007 << std::endl << std::endl;
		}
	}
	//ran in 0.000304 seconds

	delete[] factorials;
	delete[] ways_to_choose;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}