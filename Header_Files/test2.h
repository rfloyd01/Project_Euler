#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes permutationsOfPartitions()
#include <iostream>
#include <Header_Files/int_64x.h>

int_64x BigPermutationsOfPartitions(int n, int l, int maximum, int minimum)
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
	int_64x answer = 0;
	long long mod = 1000000007;

	//we get an equation of the form: [x^target]: (1 - x^primary)^l * (1 - x)^-l
	//for which we solve using binomial expansion
	for (int i = 0; i <= target / primary; i++)
	{

		if ((flip *= -1) == 1) answer = answer + (BigChoose(l, i) * BigChoose((secondaryNumerator -= primary), (secondaryDenominator -= primary)));
		else answer -= BigChoose(l, i) * BigChoose((secondaryNumerator -= primary), (secondaryDenominator -= primary));
	}

	return answer;
}

std::vector<long long> test2(long long dice_sides, long long number_top_dice, long long total_dice)
{
	auto run_time = std::chrono::steady_clock::now();
	//int dice_sides = 15, number_top_dice = 10, total_dice = 12;
	int_64x answer = 0;
	std::vector<long long> all_answers;

	//std::cout << "Dice sides = " << dice_sides << std::endl;
	//std::cout << "Top dice = " << number_top_dice << std::endl;
	//std::cout << "Total dice = " << total_dice << std::endl << std::endl;

	//initialize an array for factorials % 1,000,000,007
	//the maximum factorial we will need depends on which number is bigger, total_dice or
	//top_dice_sum. is equal to n + the number of top dice
	int max_factorial = (total_dice > number_top_dice * dice_sides) ? total_dice : number_top_dice * dice_sides;
	long long mod = 1000000007;
	int_64x* factorials = new int_64x[max_factorial + 1]();
	factorials[0] = 1;
	for (int i = 1; i <= max_factorial; i++) factorials[i] = factorials[i - 1] * i;

	//Memoize ways to split total dice into two sections dice into two sections (top_dice = 0 through
	//top_dice = number_top_dice)
	int_64x* ways_to_choose = new int_64x[number_top_dice + 1]();
	for (int i = 0; i <= number_top_dice; i++) ways_to_choose[i] = BigChoose(total_dice, i);

	for (int goal = number_top_dice; goal <= number_top_dice * dice_sides; goal++)
	//for (int goal = 72; goal <= 72; goal++)
	{
		//std::cout << "Goal: " << goal << std::endl;
		answer = 0; //re-zero the answer
		std::vector<std::vector<std::vector<int > > > partitions;
		//std::pair<int, int>* minimumTopDieCounter = new std::pair<int, int>[dice_sides + 1]();
		
		getPartitions(goal, partitions, dice_sides, number_top_dice);

		for (int i = 0; i < partitions[number_top_dice].size(); i++)
		{
			//std::cout << "Partition " << i + 1 << " of " << partitions[number_top_dice].size() << std::endl;
			int minimalTopDice = 0;
			int minimalTopDieValue = partitions[number_top_dice][i].back();
			//vprint(partitions[number_top_dice][i]);

			//First count the number of minimal top dice
			for (int j = number_top_dice - 1; j >= 0; j--)
			{
				if (partitions[number_top_dice][i][j] > minimalTopDieValue) break;
				minimalTopDice++;
			}

			//FOR DEBUGGING:
			/*if (minimumTopDieCounter[minimalTopDieValue].first == 0 || minimalTopDice < minimumTopDieCounter[minimalTopDieValue].first)
				minimumTopDieCounter[minimalTopDieValue].first = minimalTopDice;
			if (minimumTopDieCounter[minimalTopDieValue].second == 0 || minimalTopDice > minimumTopDieCounter[minimalTopDieValue].second)
				minimumTopDieCounter[minimalTopDieValue].second = minimalTopDice;
			continue;*/

			//Then calculate the number of permutations of non-minimal top dice
			int_64x maximalTopDicePermutations = factorials[number_top_dice - minimalTopDice];
			int currentNumber = partitions[number_top_dice][i][0], sameNumberCount = 0;
			for (int j = 0; j < (number_top_dice - minimalTopDice); j++)
			{
				if (partitions[number_top_dice][i][j] == currentNumber) sameNumberCount++;
				else
				{
					maximalTopDicePermutations /= factorials[sameNumberCount];
					sameNumberCount = 1;
					currentNumber = partitions[number_top_dice][i][j];
				}
			}
			maximalTopDicePermutations /= factorials[sameNumberCount]; //make sure to divide out the last number

			int_64x minimalDicePermutations = BigPow(minimalTopDieValue, total_dice - number_top_dice + minimalTopDice) - BigPow(minimalTopDieValue - 1, total_dice - number_top_dice + minimalTopDice);
			for (long long i = 1; i < minimalTopDice; i++)
				minimalDicePermutations -= BigChoose(total_dice - number_top_dice + minimalTopDice, i) * BigPow(minimalTopDieValue - 1, total_dice - number_top_dice + minimalTopDice - i);

			answer += maximalTopDicePermutations * minimalDicePermutations * ways_to_choose[number_top_dice - minimalTopDice];
		}

		/*for (int i = 0; i <= dice_sides; i++)
			if (minimumTopDieCounter[i].first) std::cout << i << ": [" << minimumTopDieCounter[i].first << ", " << minimumTopDieCounter[i].second << "]" << std::endl;*/

		//std::cout << answer % 1000000007 << std::endl << std::endl;
		//std::cout << answer << std::endl;
		answer %= mod;
		all_answers.push_back(answer.digits[0]);

		//delete[] minimumTopDieCounter;
	}

	//ran in 0.000304 seconds

	//First attempt: 29/51 failed
	//Second attempt:

	delete[] factorials;
	//delete[] ways_to_choose;

	return all_answers;
	//return { answer.getNumberString(), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}

//Generating Function Version:
//for (int goal = number_top_dice; goal <= number_top_dice * dice_sides; goal++)
	////for (int goal = 1250; goal <= 1250; goal++)
	//{
	//	std::cout << "Goal: " << goal << std::endl;
	//	answer = 0; //re-zero the answer

	//	int larger_than_lowest, lowest, lower_than_lowest;

	//	//minimum_minimum represents the smallest possible value for the minimum top die
	//	int minimum_minimum = goal - (number_top_dice - 1) * dice_sides;
	//	if (minimum_minimum <= 0) minimum_minimum = 1;

	//	//maximum_minimum represents the largest possible value for the minimum top die
	//	int maximum_minimum = goal / number_top_dice;

	//	if (minimum_minimum <= dice_sides)
	//	{
	//		//if the lowest possible value for our minimum value top die is greater than the number of sides 
	//		//on the dice, then it won't be possible to reach the goal so we skip all of the below logic
	//		for (int least_top_die_value = minimum_minimum; least_top_die_value <= maximum_minimum; least_top_die_value++)
	//		{
	//			//calculate both the most and least number of least valued top die we can have
	//			//for the current least top die value
	//			int fewest;
	//			int most;

	//			if (least_top_die_value * number_top_dice == goal)
	//			{
	//				//the equation for setting the fewest and most possible top dice for the current least top die value
	//				//breaks down if our goal number can only be reached by having all the top dice be the maximum number.
	//				//For example if the goal was 120, we could only get there if every Top Die was 12.
	//				fewest = number_top_dice;
	//				most = number_top_dice;
	//			}
	//			else
	//			{
	//				//these equations should work in all other cases
	//				fewest = (number_top_dice * (least_top_die_value + 1)) - goal;
	//				most = ((number_top_dice * dice_sides) - goal) / (dice_sides - least_top_die_value);
	//				if (fewest <= 0) fewest = 1;
	//				if (most > number_top_dice) most = number_top_dice;
	//			}

	//			for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice <= most; amount_of_least_top_dice++)
	//			{
	//				//First we calculate the total ways to shuffle the dice that are less than or equal to the least valued top die
	//				int number_lower_dice = total_dice - number_top_dice + amount_of_least_top_dice;
	//				int_64x small_shuffle = BigPow(least_top_die_value, number_lower_dice) - BigPow(least_top_die_value - 1, number_lower_dice);

	//				for (int i = 1; i < amount_of_least_top_dice; i++)
	//					small_shuffle -= BigChoose(number_lower_dice, i) * BigPow(least_top_die_value - 1, number_lower_dice - i);

	//				//std::cout << "small shuffle = " << small_shuffle << std::endl;

	//				//Next we calculate the total ways to shuffle the top dice that are greater than the least valued top die
	//				int_64x large_shuffle = BigPermutationsOfPartitions(goal - amount_of_least_top_dice * least_top_die_value, number_top_dice - amount_of_least_top_dice, dice_sides, least_top_die_value + 1);

	//				//std::cout << "large shuffle = " << large_shuffle << std::endl;
	//				//We multiply the ways to individually shuffle the upper and lower dice with the overall number of ways to shuffle
	//				//the dice together

	//				//std::cout << "overall shuffle = " << ways_to_choose[number_top_dice - amount_of_least_top_dice] << std::endl;
	//				answer += ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle;
	//				//std::cout << ways_to_choose[number_top_dice - amount_of_least_top_dice] % mod << ", " << large_shuffle % mod << ", " << small_shuffle % mod << std::endl;

	//				std::cout << "MTD = " << least_top_die_value;
	//				std::cout << ", # of MTD = " << amount_of_least_top_dice << std::endl;
	//				//std::cout << ", Ways % MOD = " << (ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle * small_shuffle) % 1000000007 << std::endl << std::endl;
	//			}
	//		}
	//	}

	//	std::cout << answer % 1000000007 << std::endl;
	//}