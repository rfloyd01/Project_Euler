#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes char_to_int()

//Largest product in a series
std::pair<std::string, double> q8()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0, current_answer = 1;
	int consecutive_digits = 13, current_location = 0, chain_start = 0;
	bool new_num = true;

	std::string number = "73167176531330624919225119674426574742355349194934"
		"96983520312774506326239578318016984801869478851843"
		"85861560789112949495459501737958331952853208805511"
		"12540698747158523863050715693290963295227443043557"
		"66896648950445244523161731856403098711121722383113"
		"62229893423380308135336276614282806444486645238749"
		"30358907296290491560440772390713810515859307960866"
		"70172427121883998797908792274921901699720888093776"
		"65727333001053367881220235421809751254540594752243"
		"52584907711670556013604839586446706324415722155397"
		"53697817977846174064955149290862569321978468622482"
		"83972241375657056057490261407972968652414535100474"
		"82166370484403199890008895243450658541227588666881"
		"16427171479924442928230863465674813919123162824586"
		"17866458359124566529476545682848912883142607690042"
		"24219022671055626321111109370544217506941658960408"
		"07198403850962455444362981230987879927244284909188"
		"84580156166097919133875499200524063689912560717606"
		"05886116467109405077541002256983155200055935729725"
		"71636269561882670428252483600823257530420752963450";

	while (current_location < number.length())
	{
		if (new_num)
		{
			//we're either just starting or we hit a 0 and need fresh digits
			current_answer = 1;
			new_num = false;
			chain_start = current_location;
			
			for (int i = 0; i < consecutive_digits; i++)
			{
				if (current_location >= number.length())
				{
					new_num = true;
					break;
				}
				if (number[current_location] - '0') current_answer *= (number[current_location++] - '0');
				else
				{
					new_num = true;
					current_location++;
					break;
				}
			}
			if (new_num) continue;
		}

		if (current_answer > answer) answer = current_answer; //check to see if the current chain is better than our current answer
		current_answer /= (number[chain_start++] - '0'); //drop the number at the start of current chain
		current_answer *= (number[current_location++] - '0'); //add the next number
		
		if (!current_answer) new_num = true; //if we multiplied by zero then we're going to need a fresh set of digits
	}
	//ran in 0.0000074 seconds
	//HackerRank score = 100.00 / 100.00

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 23514624000
	//ran in 0.0000074 seconds
}

//NOTES:
//Nothing fancy with this problem, start with a value that's equal to the product of the first 13 numbers. From this point iterate through the number string one at a time
//and the biggest thing is to just avoid zeros. When I initialliy solved this problem (I'm writing these notes after the fact) it seems like I had a counter
//that would increment every time a new zero was found and then decrement every time a zero dropped off of the front. Every time I'd iterate to the next digit of the number
//I would multiply the current product by the new digit (unless it was a zero in which case the product would stay the same but the zero counter would increment) and then I'd
//divid the current product by whatever number dropped off the front. If the zero counter was at a value of zero I'd then compare the current product to see if it was larger than
//the current answer, but if the zero counter was greater than 0 the current product wasn't eligible as an answer.

//HACKERRANK UPDATE:
//This question doesn't really seem like it's any harder for HackerRank, however, I'm not 100% with my original code. With the original code, if a zero was found I would continue to
//iterate through the numbers 1 by 1 until the 0 dropped off. It would be much more efficient to just skip ahead the appropriate amount of digits so that the zero drops of right then
//and there. After changing the logic to skip to the next valid number the run time increased by a factor of 80x which is a nice little speed increase.

//HackerRank Score = 100.00 / 100.00

//Original Project Euler Code
/*
long long answer = 0, current_answer = 1;
	int number_of_zeros = 0, consecutive_digits = 13;

	//initialize answer to product of first 13 digits
	for (int i = 0; i < 13; i++)
	{
		int num = char_to_int(number[i]);

		if (num == 0) number_of_zeros++;
		else answer *= num;
	}

	if (number_of_zeros == 0) current_answer = answer;

	for (int i = 13; i < number.length(); i++)
	{
		//remove first number from chain
		int num = char_to_int(number[i - 13]);
		if (num != 0) current_answer /= num;
		else number_of_zeros--;

		//add next number to chain
		num = char_to_int(number[i]);
		if (num != 0) current_answer *= num;
		else number_of_zeros++;

		if (number_of_zeros == 0)
			if (current_answer > answer) answer = current_answer;
	}

	run_time = 0.0005939 seconds
*/
