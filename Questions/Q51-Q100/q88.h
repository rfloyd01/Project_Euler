#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <set>

//Product-sum Numbers
void recursiveMakeNumbers(int k, unsigned int& current_winner, int current_place = 1, int current_sum = 0, int current_product = 1, int current_minimum = 1)
{
	//Base Case occurs when we are in the second to last location of the sum
	if (current_place == (k - 1))
	{
		//calculate the highest possible value for the second to last number and then iterate down towards current
		//minimum looking for hits. The first hit found (if any) will be the lowest possible value of N for this 
		//configuration
		int max_x = (2 + sqrt(4 + 4 * current_product * current_sum)) / (2 * current_product);
		for (int i = max_x; i >= current_minimum; i--)
		{
			if ((i + current_sum) % (i * current_product - 1) == 0)
			{
				unsigned int potential_winner = current_product * i * (i + current_sum) / (i * current_product - 1);
				if (potential_winner < current_winner)
				{
					current_winner = potential_winner;
					break;
				}
			}
		}
		return;
	}

	//Calcualte the maximum number that could go in this location. Since all subsequent 
	//numbers need to be greater than or equal to the current one, if the number is too 
	//high then the product will get to large for the sum to catch it.
	int maximum = current_minimum;
	while (current_product * MyPow(maximum, k - current_place + 1) <= current_sum + maximum * (k - current_place + 1)) maximum++;

	for (int i = current_minimum; i < maximum; i++)
		recursiveMakeNumbers(k, current_winner, current_place + 1, current_sum + i, current_product * i, i);
}

std::pair<std::string, double> q88()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, max_twos = 2, next_calc = 5;
	std::set<int> answers; //store found answers in a set to avoid duplicates

	for (int k = 2; k <= 12000; k++)
	{
		unsigned int current_best = -1; //the negative will flip so this becomes the maximum possible integer

		//calculate the maximum number of 2's we could have when the length is k (i.e. number of twos = x,
		//which will greatly reduce the recursion levels reached
		if (k == next_calc)
		{
			max_twos++;
			next_calc = MyPow(2, max_twos + 1) - 2 * (max_twos + 1) + max_twos + 1;
		}

		//Find the lowest value N that can fit into k slots
		recursiveMakeNumbers(k, current_best, k - max_twos + 1, k - max_twos);
		answers.insert(current_best);
	}

	//Tally up the final answer
	for (auto it = answers.begin(); it != answers.end(); it++) answer += *it;
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7587457
	//ran in 0.0940948 seconds
}

//NOTES
/*
*/