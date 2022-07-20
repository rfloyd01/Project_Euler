#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>

//test
std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0; //answer could be initialized to no-pivot as there's only one case of that

	std::vector<std::vector<std::vector<int> > > partitions;
	getPartitions(70, partitions, 12, 10);
	long long twentyFact = factorial(20); //this number will get used a lot so only calculate it once

	for (int i = 0; i < partitions[10].size(); i++)
	{
		int minimalTopDice = 0;
		int minimalTopDieValue = partitions[10][i].back();

		//First count the number of minimal top dice
		for (int j = 9; j >= 0; j--)
		{
			if (partitions[10][i][j] > minimalTopDieValue) break;
			minimalTopDice++;
		}

		//Then calculate the number of permutations of non-minimal top dice
		long long maximalTopDicePermutations = factorial(10 - minimalTopDice);
		int currentNumber = partitions[10][i][0], sameNumberCount = 0;
		for (int j = 0; j < (10 - minimalTopDice); j++)
		{
			if (partitions[10][i][j] == currentNumber) sameNumberCount++;
			else
			{
				maximalTopDicePermutations /= factorial(sameNumberCount);
				sameNumberCount = 1;
				currentNumber = partitions[10][i][j];
			}
		}
		maximalTopDicePermutations /= factorial(sameNumberCount); //make sure to divide out the last number

		long long totalDicePermutations = 0;
		long long commonFact = factorial(10 - minimalTopDice);

		//Now that we have the permutations for the Maximal Top Dice, loop through the 11 different possibilities
		//for total number of additional minimal Top Dice we can add
		for (int j = 0; j < 11; j++)
			totalDicePermutations += (twentyFact / (factorial(minimalTopDice + j) * factorial(10 - j) * commonFact) * pow(minimalTopDieValue - 1, 10 - j));
		
		answer += totalDicePermutations * maximalTopDicePermutations;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7448163872691144859
	//ran in 0.0053879 seconds

	//7448163872691144859
	//7448717393364181966
}
