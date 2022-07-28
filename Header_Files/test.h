#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes permutationsOfPartitions()
#include <iostream>

long long permutationsOfPartitionsTest(long long n, long long l, long long maximum, long long minimum, long long mod, long long* facts)
{
	//the same as my standard permutationsOfPartitions() function, but uses the BinomialModLargePrime() function
	//instead of the standard choose() function.
	if (maximum == 0) maximum = n;
	else if (maximum > n) maximum = n;

	if (minimum == 0) minimum = 1;
	else if (minimum < 1) minimum = 1;
	else if (minimum > maximum) minimum = maximum;

	//This function utilizes generating functions to find the partitions.
	long long target = n - minimum * l, primary = maximum - minimum + 1, flip = -1;
	long long secondaryDenominator = target + primary, secondaryNumerator = l + secondaryDenominator - 1;
	long long answer = 0;

	if (secondaryNumerator <= 0) return 1; //there's only one way to choose from 0

	//we get an equation of the form: [x^target]: (1 - x^primary)^l * (1 - x)^-l
	//for which we solve using binomial expansion
	for (long long i = 0; i <= target / primary; i++)
	{
		long long numb = (BinomialModLargePrime(l, i, mod, facts) * BinomialModLargePrime((secondaryNumerator -= primary), (secondaryDenominator -= primary), mod, facts)) % mod;
		if ((flip *= -1) == -1) answer = ((answer - numb) % mod + mod) % mod; //seems redundant to have all the mods but it's necessary for negative numbers
		else answer = (answer + numb) % mod;

		//std::cout << answer << ": ";
		//if (flip == -1) std::cout << "-(";
		//std::cout << "choose(" << l << ", " << i << ")[" << BinomialModLargePrime(l, i, mod, facts) << "] * choose(" << secondaryNumerator << ", " << secondaryDenominator << ")[" << BinomialModLargePrime(secondaryNumerator, secondaryDenominator, mod, facts) << "]";
		//if (flip == -1) std::cout << ")";
		//std::cout << std::endl;
	}

	return answer;
}

std::vector<long long> test(long long dice_sides, long long number_top_dice, long long total_dice)
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer; std::vector<long long> all_answers;
	//long long dice_sides = 15, number_top_dice = 10, total_dice = 12;
	long long modulus = 1000000007;

	std::cout << "Dice sides = " << dice_sides << std::endl;
	std::cout << "Top dice = " << number_top_dice << std::endl;
	std::cout << "Total dice = " << total_dice << std::endl << std::endl;

	//initialize an array for factorials % 1,000,000,007
	//the maximum factorial we will need depends on which number is bigger, total_dice or
	//top_dice_sum. is equal to n + the number of top dice
	long long max_factorial = (total_dice > number_top_dice * dice_sides) ? total_dice : number_top_dice * dice_sides;
	long long* factorials = new long long[max_factorial + 1]();
	factorials[0] = 1;
	for (int i = 1; i <= max_factorial; i++) factorials[i] = factorials[i - 1] * i % modulus;

	//std::cout << "max factorial needed is " << max_factorial << std::endl;

	//Memoize ways to split dice into two sections, unlike the max factorial, this only depends on the
	//total number of dice
	long long* ways_to_choose = new long long[total_dice + 1]();
	for (long long i = 0; i <= total_dice; i++) ways_to_choose[i] = BinomialModLargePrime(total_dice, i, modulus, factorials);

	for (int goal = number_top_dice; goal <= number_top_dice * dice_sides; goal++)
	//for (int goal = 72; goal <= 72; goal++)
	{
		//std::cout << "Goal: " << goal << std::endl;
		answer = 0; //re-zero the answer

		//minimum_minimum represents the smallest possible value for the minimum top die
		int minimum_minimum = goal - (number_top_dice - 1) * dice_sides;
		if (minimum_minimum <= 0) minimum_minimum = 1;

		//maximum_minimum represents the largest possible value for the minimum top die
		int maximum_minimum = goal / number_top_dice;

		for (long long least_top_die_value = minimum_minimum; least_top_die_value <= maximum_minimum; least_top_die_value++)
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
				if (most > number_top_dice) most = number_top_dice;
			}

			//std::cout << least_top_die_value << ": [" << fewest << ", " << most << "]" << std::endl;

			for (long long amount_of_least_top_dice = fewest; amount_of_least_top_dice <= most; amount_of_least_top_dice++)
			{
				//std::cout << "MTD = " << least_top_die_value;
				//std::cout << ", # of MTD = " << amount_of_least_top_dice << std::endl;
				//First we calculate the total ways to shuffle the dice that are less than or equal to the least valued top die
				long long number_lower_dice = total_dice - number_top_dice + amount_of_least_top_dice;
				long long small_shuffle = (((ModPow(least_top_die_value, number_lower_dice, modulus, 1) - ModPow(least_top_die_value - 1, number_lower_dice, modulus, 1)) % modulus) + modulus) % modulus;

				for (long long i = 1; i < amount_of_least_top_dice; i++)
					small_shuffle = ((small_shuffle - (BinomialModLargePrime(number_lower_dice, i, modulus, factorials) * ModPow(least_top_die_value - 1, number_lower_dice - i, modulus, 1) % modulus)) + modulus) % modulus;

				//std::cout << "small shuffle = " << small_shuffle << std::endl;
				//Next we calculate the total ways to shuffle the top dice that are greater than the least valued top die
				long long large_shuffle = permutationsOfPartitionsTest(goal - amount_of_least_top_dice * least_top_die_value, number_top_dice - amount_of_least_top_dice, dice_sides, least_top_die_value + 1, modulus, factorials);

				//We multiply the ways to individually shuffle the upper and lower dice with the overall number of ways to shuffle
				//the dice together
				//if (ways_to_choose[number_top_dice - amount_of_least_top_dice] < 0) std::cout << "overall shuffle = " << ways_to_choose[number_top_dice - amount_of_least_top_dice] << std::endl;
				//std::cout << ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus << " ways to order them.\n" << std::endl << std::endl;
				answer = (answer + ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus) % modulus;
				//std::cout << ways_to_choose[number_top_dice - amount_of_least_top_dice] << ", " << large_shuffle << ", " << small_shuffle << std::endl;

				//std::cout << ", Ways % MOD = " << ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus << std::endl << std::endl;
			}
		}

		//Code for partitions check
		/*std::vector<std::vector<std::vector<int> > > actual_top_dice_combos;
		getPartitions(goal, actual_top_dice_combos, (int)dice_sides, (int)number_top_dice);
		for (int z = 0; z < actual_top_dice_combos[number_top_dice].size(); z++) vprint(actual_top_dice_combos[number_top_dice][z]);
		std::cout << std::endl;*/

		//std::cout << "Top dice goal = " << goal << std::endl;
		//std::cout << "Ways to shuffle MOD 1,000,000,007 = " << answer % 1000000007 << std::endl << std::endl;
		//std::cout << answer << std::endl << std::endl;
		all_answers.push_back(answer);
	}

	//ran in 0.000304 seconds

	delete[] factorials;
	delete[] ways_to_choose;
	return all_answers;
	//return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

}