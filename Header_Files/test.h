#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>

//test
std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0; //answer could be initialized to no-pivot as there's only one case of that
	//long long permutationsOfPartitionsOfSeventy = 0;
	int totalTopDice = 10;
	int totalDice = 10;
	int topDiceGoal = 70;
	int maxTopDieValue = 12;

	std::vector<std::vector<std::vector<int> > > partitions;
	getPartitions(topDiceGoal, partitions, maxTopDieValue, totalTopDice);
	long long maximumFactorial = factorial(totalDice); //this number will get used a lot so only calculate it once

	//std::vector<long long> poly = { 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
	//std::vector<long long> poly2 = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
	//std::vector<long long> poly3 = { 0, 1, 1 };
	//std::vector<long long> result = polynomial_multiply(poly, poly2, 25);
	//result = polynomial_multiply(result, poly3, 30);

	////for (int i = 0; i < 2; i++) result = polynomial_multiply(poly, result, 20);
	//for (int i = 0; i < result.size(); i++) std::cout << i << ", " << result[i] << std::endl;

	for (int i = 0; i < partitions[totalTopDice].size(); i++)
	{
		//vprint(partitions[totalTopDice][i]);
		int minimalTopDice = 0;
		int minimalTopDieValue = partitions[totalTopDice][i].back();

		//First count the number of minimal top dice
		for (int j = totalTopDice - 1; j >= 0; j--)
		{
			if (partitions[totalTopDice][i][j] > minimalTopDieValue) break;
			minimalTopDice++;
		}

		//Then calculate the number of permutations of non-minimal top dice
		long long maximalTopDicePermutations = factorial(totalTopDice - minimalTopDice);
		int currentNumber = partitions[totalTopDice][i][0], sameNumberCount = 0;
		for (int j = 0; j < (totalTopDice - minimalTopDice); j++)
		{
			if (partitions[totalTopDice][i][j] == currentNumber) sameNumberCount++;
			else
			{
				maximalTopDicePermutations /= factorial(sameNumberCount);
				sameNumberCount = 1;
				currentNumber = partitions[totalTopDice][i][j];
			}
		}
		/*for (int j = 0; j < neededPartitionLength; j++)
		{
			if (partitions[neededPartitionLength][i][j] == currentNumber) sameNumberCount++;
			else
			{
				maximalTopDicePermutations /= factorial(sameNumberCount);
				sameNumberCount = 1;
				currentNumber = partitions[neededPartitionLength][i][j];
			}
		}*/
		maximalTopDicePermutations /= factorial(sameNumberCount); //make sure to divide out the last number
		//permutationsOfPartitionsOfSeventy += maximalTopDicePermutations;

		long long totalDicePermutations = 0;
		long long commonFact = factorial(totalTopDice - minimalTopDice);

		//Now that we have the permutations for the Maximal Top Dice, loop through the 11 different possibilities
		//for total number of additional minimal Top Dice we can add
		for (int j = 0; j < (totalDice - totalTopDice + 1); j++)
			totalDicePermutations += (maximumFactorial / (factorial(minimalTopDice + j) * factorial(totalDice - totalTopDice - j) * commonFact) * pow(minimalTopDieValue - 1, totalDice - totalTopDice - j));
		
		answer += totalDicePermutations * maximalTopDicePermutations;
	}
	//ran in 0.0053879 seconds

	std::cout << "There are " << permutationsOfPartitionsOfSeventy << " permutations of partitions of 70 where the max number is 12 and 10 numbers are used" << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7448717393364181966
	//ran in 0.0053879 seconds
}
