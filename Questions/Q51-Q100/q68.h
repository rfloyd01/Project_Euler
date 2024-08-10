#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes []powers_of_two
#include <vector>
#include <string>

//Magic 5-gon Ring
int string_order[15] = { 0, 1, 2, 3, 2, 4, 5, 4, 6, 7, 6, 8, 9, 8, 1 }; //represents the order to print out the winning permutation
//int powers_of_two[10] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };

void checkPermutations(int* permutation, int used_numbers, int level, std::vector<long long>& az)
{
	if (level == 10)
	{
		int sum1 = *(permutation + 0) + *(permutation + 1) + *(permutation + 2);
		int sum2 = *(permutation + 3) + *(permutation + 2) + *(permutation + 4);
		int sum3 = *(permutation + 5) + *(permutation + 4) + *(permutation + 6);
		int sum4 = *(permutation + 7) + *(permutation + 6) + *(permutation + 8);
		int sum5 = *(permutation + 9) + *(permutation + 8) + *(permutation + 1);

		if (sum1 == sum2)
			if (sum2 == sum3)
				if (sum3 == sum4)
					if (sum4 == sum5)
					{
						//A winner is found, see which outer node is the smallest and print from there
						//the outer nodes (other than where 10 is which will be the biggest) are 3, 5, 7 and 9
						int current_position = 3;
						std::string ans = "";
						for (int j = 1; j <= 3; j++) current_position = (*(permutation + current_position) < *(permutation + current_position + 2 * j)) ? current_position : 3 + 2 * j;
						current_position += (current_position + 2) / 3; //need this to convert from position in permutation to position in string_order
						for (int j = 0; j < 15; j++) ans += std::to_string(*(permutation + string_order[(current_position + j) % 15]));
						az.push_back(std::stoll(ans));
					}
		return;
	}

	for (int i = 1; i < 10; i++)
	{
		if (powers_of_two[i] & used_numbers) continue; //don't use a number already in the current permutation
		else
		{
			*(permutation + level) = i;
			checkPermutations(permutation, used_numbers | powers_of_two[i], level + 1, az);
		}
	}

	return;
}

std::pair<std::string, double> q68()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	std::vector<long long> answers;

	int permutations[10] = { 0 }, used = 0;
	permutations[0] = 10; //10 will always be the first entry of the permutation array

	checkPermutations(permutations, used, 1, answers);

	for (int i = 0; i < answers.size(); i++)
		if (answers[i] > answer) answer = answers[i];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 6531031914842725
	//ran in 0.0103677 seconds
}

//NOTES
//The first thing that struck me about this problem is that it says the answer can be either a 16 OR 17 digit string but that they want the maximum 16 digit string. This
//means that the number 10 has to be in one of the outer most nodes so that it only gets utilized once instead of twice. I'm not entirely sure if this will help solve
//the problem but I'm sure it will at least help the run time somewhat. With that aside, there should only be 10! ways to fill out the n-gon ring. The ring has 5 way
//rotational symmetry, however, and we know that 10 has to go in one of the outermost rings so there should really only be 9! possible ways to fill out the ring distinctly
//which is only 362,880 options to brute force through. When all was said and done the brute force approach ran in about 10 milliseconds which I think is fine for this problem.