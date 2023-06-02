#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Paper sheets of standard sizes: an expected-value problem
int total_calcs = 0;
struct Fraction
{
	//new fractions get initialized to 0/1
	long long numerator = 0;
	long long denominator = 0;

	void reduce()
	{
		if (numerator == 0 || denominator == 0)
		{
			numerator = 0;
			denominator = 1;
			return;
		}

		long long x = gcd(this->numerator, this->denominator);
		if (x > 1)
		{
			this->numerator /= x;
			this->denominator /= x;
		}
	}

	Fraction& operator+=(const Fraction& num)
	{
		//first find the GCD for the denominators of the current fraction and the 
		//fraction being added
		if (this->denominator == 0)
		{
			//current fraction hasn't been initialized yet so set it equal to the incoming fraction
			this->numerator = num.numerator;
			this->denominator = num.denominator;
		}
		else
		{
			long long x = gcd(this->denominator, num.denominator);

			this->numerator *= (num.denominator / x);
			this->numerator += (num.numerator * (this->denominator / x));
			this->denominator *= (num.denominator / x);
		}
		//reduce the fraction after addition
		this->reduce();
		return *this;
	}
	friend Fraction operator+(const Fraction& num1, const Fraction& num2)
	{
		Fraction num1_copy = num1;
		num1_copy += num2;
		return num1_copy;
	}

	void print()
	{
		std::cout << numerator << "/" << denominator << std::endl;
	}
};

Fraction recursiveSheetSelectWithProbability(int* current_envelope, int current_sheets, Fraction* discovered_ways, int total_sheets)
{
	if (discovered_ways[current_sheets].numerator == 0 && discovered_ways[current_sheets].denominator == 0)
	{
		total_calcs++;
		//We haven't explored this route yet so we keep recursing downwards. The idea of the recursion is that at each 
		//level you can only take a single sheet of paper, so we take one sheet and do that whole recursion. After returning
		//we put that piece of paper back and go forwards in the envelope array, taking the next sheet and recursion downwards
		//again. All of the recursions initiated at this level will contribute to the number of hits and misses that make up 
		//the pair which will go in the discovered ways array.

		for (int i = 3; i >= 0; i--)
		{
			if (current_envelope[i])
			{
				//remove a sheet of paper from the current envelope column, and then add a sheet of paper to all 
				//subsequent columns. Then recurse down.
				current_envelope[i]--;
				current_sheets -= powers_of_ten[i];
				total_sheets--;

				for (int j = i - 1; j >= 0; j--)
				{
					current_envelope[j]++;
					current_sheets += powers_of_ten[j];
					total_sheets++;
				}

				Fraction recurse = recursiveSheetSelectWithProbability(current_envelope, current_sheets, discovered_ways, total_sheets);

				//After returning from the recursion we need to reset the envelope array before the next iteration, and then 
				//add the results from the recursion
				for (int j = i - 1; j >= 0; j--)
				{
					current_envelope[j]--;
					current_sheets -= powers_of_ten[j];
					total_sheets--;
				}

				current_envelope[i]++;
				current_sheets += powers_of_ten[i];
				total_sheets++;

				recurse.numerator *= current_envelope[i];
				recurse.denominator *= total_sheets;

				//When adding the number of paths and hits that were found, we multiply be the number of sheets of paper
				//that are in the column. In theory any of these sheets could've been picked and not changed the outcome
				//so we need to ensure that all possible paths are explored.
				discovered_ways[current_sheets] += recurse;
			}
		}

		//check to see if there's only one sheet in the envelope currently, if there is it means that all of the paths that were
		//just explored are guaranteed to give us a hit
		if (total_sheets == 1) discovered_ways[current_sheets].numerator += discovered_ways[current_sheets].denominator;
	}
	return discovered_ways[current_sheets];
}

//Fraction recursiveSheetSelect(int current_envelope, int total_sheets, int* sheet_subtraction, Fraction* discovered_ways)
//{
//	if (discovered_ways[current_sheets].numerator == 0 && discovered_ways[current_sheets].denominator == 0)
//	{
//		//total_calcs++;
//		//We haven't explored this route yet so we keep recursing downwards. The idea of the recursion is that at each 
//		//level you can only take a single sheet of paper, so we take one sheet and do that whole recursion. After returning
//		//we put that piece of paper back and go forwards in the envelope array, taking the next sheet and recursion downwards
//		//again. All of the recursions initiated at this level will contribute to the number of hits and misses that make up 
//		//the pair which will go in the discovered ways array.
//		unsigned int bit_match = 0xF000, bit_adder = 0x0111, bit_subtractor = 0x1000;
//		for (int i = 3; i >= 0; i--)
//		{
//			if (current_envelope && bit_match)
//			{
//				//remove a sheet of paper from the current envelope column, and then add a sheet of paper to all 
//				//subsequent columns. Then recurse down.
//				current_envelope = current_envelope - bit_subtractor + bit_adder;
//				current_sheets -= powers_of_ten[i];
//				total_sheets--;
//
//				for (int j = i - 1; j >= 0; j--)
//				{
//					current_envelope[j]++;
//					current_sheets += powers_of_ten[j];
//					total_sheets++;
//				}
//
//				Fraction recurse = recursiveSheetSelect(current_envelope, current_sheets, discovered_ways, total_sheets);
//
//				//After returning from the recursion we need to reset the envelope array before the next iteration, and then 
//				//add the results from the recursion
//				for (int j = i - 1; j >= 0; j--)
//				{
//					current_envelope[j]--;
//					current_sheets -= powers_of_ten[j];
//					total_sheets--;
//				}
//
//				current_envelope[i]++;
//				current_sheets += powers_of_ten[i];
//				total_sheets++;
//
//				recurse.numerator *= current_envelope[i];
//				recurse.denominator *= total_sheets;
//
//				//When adding the number of paths and hits that were found, we multiply be the number of sheets of paper
//				//that are in the column. In theory any of these sheets could've been picked and not changed the outcome
//				//so we need to ensure that all possible paths are explored.
//				discovered_ways[current_sheets] += recurse;
//			}
//			bit_match >>= 4;
//			bit_adder >= 1;
//		}
//
//		//check to see if there's only one sheet in the envelope currently, if there is it means that all of the paths that were
//		//just explored are guaranteed to give us a hit
//		if (total_sheets == 1) discovered_ways[current_sheets].numerator += discovered_ways[current_sheets].denominator;
//	}
//	return discovered_ways[current_sheets];
//}

std::pair<std::string, double> q151()
{
	auto run_time = std::chrono::steady_clock::now();

	const int goal_number = 1111; //must be 1111 or less
	int goal_copy = goal_number, total_sheets = 0;

	int current_envelope[4];
	for (int i = 0; i < 4; i++)
	{
		current_envelope[i] = goal_copy % 10;
		goal_copy /= 10;
		total_sheets += current_envelope[i];
	}

	Fraction weighted_hits[goal_number + 1];

	weighted_hits[1].numerator = 0;
	weighted_hits[1].denominator = 1;

	Fraction ans = recursiveSheetSelectWithProbability(current_envelope, goal_number, weighted_hits, total_sheets);
	std::cout << total_calcs << " calculations were made." << std::endl;

	return { std::to_string((double)ans.numerator / (double)ans.denominator), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 0.464399
	//ran in 0.0000343 seconds
}

//NOTES
/*
*/