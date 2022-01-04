#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes []powers_of_two
#include <vector>

//Cube Digit Pairs
void WaysToChoose(int digits, int choose, int start, int current_number, int* possibilities, int& found, int level)
{
	if (level == choose)
	{
		*(possibilities + found) = current_number;
		found++;
		return;
	}

	for (int i = start; i <= digits; i++)
	{
		if (digits - i > digits - level) return;
		WaysToChoose(digits, choose, i + 1, current_number | powers_of_two[i], possibilities, found, level + 1);
	}
}

std::pair<std::string, double> q90()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, num = 0, counter = 0;
	int dice[210]; //there are choose(10, 6) = 210 ways to create different die combinations

	WaysToChoose(9, 6, 0, num, dice, counter, 0); //recursively create all die possibilities and store in dice array as binary representations

	//with all ways to make dice set up, go through all ways to select two dice and run tests
	for (int i = 0; i < 210; i++)
	{
		if (!(dice[i] & powers_of_two[0])) break; //there won't be any more zeros from this point on so break out of loop
		for (int j = i; j < 210; j++)
		{
			//check for 01
			if (!((dice[i] & powers_of_two[0]) && (dice[j] & powers_of_two[1])))
				if (!((dice[j] & powers_of_two[0]) && (dice[i] & powers_of_two[1]))) continue; //01 can't be made

			//check for 04
			if (!((dice[i] & powers_of_two[0]) && (dice[j] & powers_of_two[4])))
				if (!((dice[j] & powers_of_two[0]) && (dice[i] & powers_of_two[4]))) continue; //04 can't be made

			//check for 09 (or 06)
			if (!((dice[i] & powers_of_two[0]) && (dice[j] & powers_of_two[6] || dice[j] & powers_of_two[9])))
				if (!((dice[j] & powers_of_two[0]) && (dice[i] & powers_of_two[6] || dice[i] & powers_of_two[9]))) continue; //09 can't be made

			//check for 16 (or 19)
			if (!((dice[i] & powers_of_two[1]) && (dice[j] & powers_of_two[6] || dice[j] & powers_of_two[9])))
				if (!((dice[j] & powers_of_two[1]) && (dice[i] & powers_of_two[6] || dice[i] & powers_of_two[9]))) continue; //16 can't be made

			//check for 25
			if (!((dice[i] & powers_of_two[2]) && (dice[j] & powers_of_two[5])))
				if (!((dice[j] & powers_of_two[2]) && (dice[i] & powers_of_two[5]))) continue; //25 can't be made

			//check for 36 (or 39)
			if (!((dice[i] & powers_of_two[3]) && (dice[j] & powers_of_two[6] || dice[j] & powers_of_two[9])))
				if (!((dice[j] & powers_of_two[3]) && (dice[i] & powers_of_two[6] || dice[i] & powers_of_two[9]))) continue; //36 can't be made

			//check for 49 (or 46)
			if (!((dice[i] & powers_of_two[4]) && (dice[j] & powers_of_two[6] || dice[j] & powers_of_two[9])))
				if (!((dice[j] & powers_of_two[4]) && (dice[i] & powers_of_two[6] || dice[i] & powers_of_two[9]))) continue; //49 can't be made

			//check for 64 (or 94)
			if (!((dice[i] & powers_of_two[6] || dice[i] & powers_of_two[9]) && (dice[j] & powers_of_two[4])))
				if (!((dice[j] & powers_of_two[6] || dice[j] & powers_of_two[9]) && (dice[i] & powers_of_two[4]))) continue; //64 can't be made

			//check for 81
			if (!((dice[i] & powers_of_two[8]) && (dice[j] & powers_of_two[1])))
				if (!((dice[j] & powers_of_two[8]) && (dice[i] & powers_of_two[1]))) continue; //81 can't be made

			answer++; //all squares can be made so dice combo works
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1217
	//ran in 0.0001302 seconds
}

//NOTES
//There may be a more clever way to do this problem but it's able to be brute forced. Since there can't be any
//repeat digits on an individual die then that means there are choose(10, 6) = 210 different ways to distinctly create
//a 6 sided die using the digits 0-9. There are then choose_with_repetition(210, 2) = choose(211, 2) = 22,155 ways to
//select two dice. Since there are only about 22,000 possible sets of dice then each one should be able to be checked
//very quickly, so there isn't really a need to try and make too fancy of an algorithm. A simple recursive function is
//created which creates all the different combinations for a single die. Each die combo is saved as a binary representation
//(i.e. a die with the numbers 0, 2, 4, 5, 7 and 8 would be saved in binary as 0110110101 = 437[base 10]). This makes it
//easy to check what digits are stored in the die by use of bitwise AND and OR functions. The run time can actually be
//reduced a little further by ignoring any combination of dice that don't feature the number 0. The combinations from the
//recursive function will be in numerical order so once the first combo without a 0 occurs, then there won't be anymore
//combos that feature a 0, and at least one 0 is necessary to make 01, 04 and 09. This takes the total number of combos to
//check from 22,155 to 18,575.