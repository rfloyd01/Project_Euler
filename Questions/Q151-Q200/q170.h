#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes []powers_of_ten
#include <Header_Files/print.h>
#include <vector>
#include <cmath>

//Find the largest 0 to 9 pandigital that can be formed by concatenating products
void createPossibleNumbers(int lowest, int highest, int digits, int length, std::vector<int>& winners, int current_number = 0, int level = 0)
{
	//a function that creates pandigital numbers utilizing the digits allowed by "digits" and inside the given range
	//must be greater than or equal to lowest, and lower than highest
	
	if (level == length)
	{
		winners.push_back(current_number);
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		//cycle through all numbers, the if statements below will handle when to skip
		if (!level && !i) continue; //0 can't be the first digit
		if (powers_of_two[i] & digits) continue; //needs to be a digit that hasn't been used yet

		//we need to stay above (or equal to) lowest and lower than highest. This will require a few checks.
		if (!lowest && !highest)
		{
			//we're currently free to use whatever numbers we want
			createPossibleNumbers(lowest, highest, digits | powers_of_two[i], length, winners, 10 * current_number + i, level + 1);
		}
		else if (lowest && highest)
		{
			//we need to fulfill the high and low requirements at the same time
			int high = (int)(highest / pow(10, length - level - 1)) % 10;
			int low = (int)(lowest / pow(10, length - level - 1)) % 10;
			if ((i >= low) && (i < high)) createPossibleNumbers(0, 0, digits | powers_of_two[i], length, winners, 10 * current_number + i, level + 1);
		}
		else if (highest)
		{
			//we need to be lower than highest, however, don't need to worry about floor. If i < the current digit of highest then set highest to 0
			if (i <= (int)(highest / pow(10, length - level - 1)) % 10) createPossibleNumbers(lowest, 0, digits | powers_of_two[i], length, winners, 10 * current_number + i, level + 1);
		}
		else
		{
			//we need to be greater than or equal to lowest, however, don't need to worry about a ceiling. If i >= the current digit of lowest then set lowest to 0
			if (i >= (int)(lowest / pow(10, length - level - 1)) % 10) createPossibleNumbers(0, highest, digits | powers_of_two[i], length, winners, 10 * current_number + i, level + 1);
		}
	}
	return;
}
bool pandigital(std::string n)
{
	//returns true if n doesn't have any repeat digits, otherwise returns false
	int used = 0, current = 0;
	for (int i = 0; i < n.length(); i++)
	{
		if (n[i] == ' ')
		{
			continue; //ignore spaces in the string
		}
		current = powers_of_two[n[i] - '0'];
		if (current & used) return false;
		used |= current;
	}
	return true;
}
int convertToBinary(int n)
{
	//takes the integer n and converts it into a binary representation which shows us the digits that make up n.
	//For example 158 would be 0b0100100010
	int used = 0;
	while (n > 0)
	{
		used |= powers_of_two[n % 10];
		n /= 10;
	}
	return used;
}
void singleRecursiveFunction(int repeated, std::vector<int>& equation, std::vector<int>& growth_limit, std::string left_current, std::string right_current, 
	int left_used, int right_used, bool growth, long long& answer, int current_largest = 0, int level = 0)
{
	//looks at all valid possibilites of equations where the repeated integer is only 1-digit long and equal to "repeated"
	//utilizing equations of the structure dictated by "equation". "used" represent which digits are allowed. "growth" represents
	//whether or not the equation which is allowed to grow has been used yet. "growth_limit" represents the first number which can
	//be utilized to make the equation on the right hand side grow by 1 and is equal to ceiling(10^equation[i] / repeated).
	if (level == equation.size())
	{
		//if we've made it this far than we have a hit! Now we need to see if the equations can be rearranged to increase the size of
		//our pandigital
		
		//first split the string up by number and put the numbers into separate parts of a vector
		std::vector<std::string> numbers;
		std::string current_number = "";
		for (int i = 1; i < right_current.length(); i++) //start at 1 because of leading space
		{
			if (right_current[i] == ' ')
			{
				//place the current number into the vector in numerical position
				bool placed = false;
				for (int j = 0; j < numbers.size(); j++)
				{
					if ((current_number[0] - '0') > (numbers[j][0] - '0'))
					{
						numbers.insert(numbers.begin() + j, current_number);
						placed = true;
						break;
					}
				}
				if (!placed) numbers.push_back(current_number);
				current_number = "";
			}
			else current_number += right_current[i];
		}
		//place the final number into the vector in numerical position
		bool placed = false;
		for (int j = 0; j < numbers.size(); j++)
		{
			if ((current_number[0] - '0') > (numbers[j][0] - '0'))
			{
				numbers.insert(numbers.begin() + j, current_number);
				placed = true;
				break;
			}
		}
		if (!placed) numbers.push_back(current_number);

		//now reconstruct the number into current_number
		current_number = "";
		for (int i = 0; i < numbers.size(); i++) current_number += numbers[i];

		if (stoll(current_number) > answer)
		{
			answer = stoll(current_number);
			/*vprint(equation);
			std::cout << repeated << ":" << left_current << ", " << current_number << std::endl;*/
		}
		return;
	}

	//if we've moved onto an equation with more digits we're no longer restricted as to what digit our new number can start with
	//(with the exception it needs to be at least as big as the necessary digits
	if (level > 0)
	{
		if (equation[level] > equation[level - 1]) current_largest = 0; //TODO: can change this to some variation of pow(10, x)
	}

	//if we aren't on the last level of the function we can choose whether or not the current number will cause growth or not
	if (!growth)
	{
		//growth number hasn't been picked, cycle through all possible growth numbers
		std::vector<int> possible = {};
		createPossibleNumbers(growth_limit[level], 0, left_used, equation[level], possible);

		int current;
		for (int i = 0; i < possible.size(); i++)
		{
			current = repeated * possible[i];
			
			if (!pandigital(right_current + std::to_string(current))) continue; //if product isn't pandigital with current numbers continue

			//at this point we have a hit and need to go to the next level
			singleRecursiveFunction(repeated, equation, growth_limit, left_current + " " + std::to_string(possible[i]), right_current + " " + std::to_string(current), 
				left_used | convertToBinary(possible[i]), right_used | convertToBinary(current), true, answer, possible[i], level + 1);
		}
		//if this is the last part of the equation and no other numbers are growth numbers then no need to check more non-growth numbers below
		if (level == (equation.size() - 1)) return;
	}
	
	//now cycle through the non-growth numbers
	std::vector<int> possible;
	createPossibleNumbers(current_largest, growth_limit[level], left_used, equation[level], possible);
	int current;
	for (int i = 0; i < possible.size(); i++)
	{
		current = repeated * possible[i];
		if (!pandigital(right_current + std::to_string(current))) continue; //if product isn't pandigital with current numbers continue

		//at this point we have a hit and need to go to the next level
		singleRecursiveFunction(repeated, equation, growth_limit, left_current + " " + std::to_string(possible[i]), right_current + " " + std::to_string(current), 
			left_used | convertToBinary(possible[i]), right_used | convertToBinary(current), growth, answer, possible[i], level + 1);
	}
	return;
}
void doubleRecursiveFunction(int repeated, std::vector<int>& equation, std::vector<int>& growth_limit, std::string left_current, std::string right_current,
	int left_used, int right_used, long long& answer, int current_largest = 0, int level = 0)
{
	//same as the singleRecursiveFunction, however, repeated will always be a 2-digit number and only a growth of 1 is allowed
	//the below block is the same as in the single version of the recursive function
	if (level == equation.size())
	{
		//if we've made it this far than we have a hit! Now we need to see if the equations can be rearranged to increase the size of
		//our pandigital

		//first split the string up by number and put the numbers into separate parts of a vector
		std::vector<std::string> numbers;
		std::string current_number = "";
		for (int i = 1; i < right_current.length(); i++) //start at 1 because of leading space
		{
			if (right_current[i] == ' ')
			{
				//place the current number into the vector in numerical position
				bool placed = false;
				for (int j = 0; j < numbers.size(); j++)
				{
					if ((current_number[0] - '0') > (numbers[j][0] - '0'))
					{
						numbers.insert(numbers.begin() + j, current_number);
						placed = true;
						break;
					}
				}
				if (!placed) numbers.push_back(current_number);
				current_number = "";
			}
			else current_number += right_current[i];
		} 
		//place the final number into the vector in numerical position
		bool placed = false;
		for (int j = 0; j < numbers.size(); j++)
		{
			if ((current_number[0] - '0') > (numbers[j][0] - '0'))
			{
				numbers.insert(numbers.begin() + j, current_number);
				placed = true;
				break;
			}
		}
		if (!placed) numbers.push_back(current_number);

		//now reconstruct the number into current_number
		current_number = "";
		for (int i = 0; i < numbers.size(); i++) current_number += numbers[i];

		if (stoll(current_number) > answer)
		{
			answer = stoll(current_number);
			//the below two lines are there for curiosity's sake, I wanted to see what equation yields the answer
			/*vprint(equation);
			std::cout << repeated << ":" << left_current << ", " << current_number << std::endl;*/
		}
		return;
	}//this block is the same as in the single recursive function

	//if we've moved onto an equation with more digits we're no longer restricted as to what digit our new number can start with
	//(with the exception it needs to be at least as big as the necessary digits
	if (level > 0)
	{
		if (equation[level] > equation[level - 1]) current_largest = 0; //TODO: can change this to some variation of pow(10, x)
	}

	//we can only cycle through numbers that will yield a growth of 1
	std::vector<int> possible;
	createPossibleNumbers(current_largest, growth_limit[level], left_used, equation[level], possible);
	int current;

	//if (repeated == 18) vprint(possible);
	for (int i = 0; i < possible.size(); i++)
	{
		current = repeated * possible[i];
		if (!pandigital(right_current + std::to_string(current))) continue; //if product isn't pandigital with current numbers continue

		//at this point we have a hit and need to go to the next level
		doubleRecursiveFunction(repeated, equation, growth_limit, left_current + std::to_string(possible[i]), right_current + " " + std::to_string(current),
			left_used | convertToBinary(possible[i]), right_used | convertToBinary(current), answer, possible[i], level + 1);
	}
	return;
}

std::pair<std::string, double> q170()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0;
	
	//for each vector in "acceptable_equations" the length of the vector represents the number of equations and each number represents the length of the non
	//repeated integer. For example the equation {1, 3, 5} could be represented by 4 * 2, 4 * 135, 4 * 67890 as 2, 135 and 67890 have lenghts of 1, 3 and 5
	//respectively. Since these are integer partitions they must go in numerical order (i.e. {1, 6, 3} wouldn't be valid). The single equations are partitions
	//of 9 while the double equations are partitions of 8 (because 1 + 9 = 10 and 2 + 8 = 10, there are 10 total digits needed). The single equations can have
	//lengths of 2, 3 or 4 depending on what the repeated integer is but the double equations must have a length of 2.
	std::vector<std::vector<int> > single_acceptable_equations = { {1, 8}, {2, 7}, {3, 6}, {4, 5}, {1, 1, 7}, {1, 2, 6}, {1, 3, 5}, {1, 4, 4}, {2, 2, 5}, {2, 3, 4}, {3, 3, 3},
																   {1, 1, 1, 6}, {1, 1, 2, 5} , {1, 1, 3, 4} , {1, 2, 2, 4}, {1, 2, 3, 3}, {2, 2, 2, 3} };
	std::vector<std::vector<int> > double_acceptable_equations = { {1, 7}, {2, 6}, {3, 5}, {4, 4} };
	int final_equation[10] = { 0, 0, 16, 10, 10, 3, 3, 3, 3, 3 }; //represents the element of the final equation in "single_acceptable_equations" that each number can utilize

	//Since the single and double numbers are inherently different, two functions have been created to handle them separately
	for (int i = 2; i < 10; i++)
	{
		for (int j = 0; j <= final_equation[i]; j++)
		{
			std::vector<int> growth_limit; //represents the first number that allows for growth for each level of the equation when the repeated integer is i
			for (int k = 0; k < single_acceptable_equations[j].size(); k++) growth_limit.push_back(ceil(pow(10, single_acceptable_equations[j][k]) / i));

			singleRecursiveFunction(i, single_acceptable_equations[j], growth_limit, "", "", powers_of_two[i], 0, false, answer);
		}
	}

	//with single numbers out of the way, now to check double numbers
	for (int i = 10; i < 100; i++)
	{
		if (!pandigital(std::to_string(i))) continue; //the repeated integer itself must be pandigital
		for (int j = 0; j < double_acceptable_equations.size(); j++)
		{
			std::vector<int> growth_limit; //represents the first number that allows for growth of 2 digits, we must stay below this number
			for (int k = 0; k < double_acceptable_equations[j].size(); k++) growth_limit.push_back(floor(pow(10, double_acceptable_equations[j][k] + 1) / i));

			doubleRecursiveFunction(i, double_acceptable_equations[j], growth_limit, "", "", convertToBinary(i), 0, answer);
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 9857164023
	//ran in 0.258385 seconds
}

//NOTES
//I have much more detailed notes on my website (URL to come). Currently the highest hit that I've found so far is the following:
/*
9 x 1        = 9
9 x 87345026 = 786,105,234
pandigital = 9786105234
*/
//The highest pandigital product I've found with a two digit repeated number is:
/*
12 x 793   = 9,516
12 x 60854 = 730,248
pandigital = 9516730248
*/
//Frustratingly enough neither of these is the correct answer even though they both meet the requirements. I had mentioned how I thought
//it would be possible to shift the equations around to get a higher answer, there must be some shenigans like that going on. The good news
//is that currently I check all options in only 0.153506 seconds. Having to rearrange things will definitely take up more time, however, I think
//most of the time is in weeding out options that won't work, so manipulating ones that do work shouldn't add a whole lot of time

//Found out what the issue was, I was using a < symbol instead of a <= symbol in one section of code and thus was discrediting a good amount of
//options. After fixing this I got the correct answer immediately which is from the following equation:
/*
27 x 36508 = 985,716
27 x 149   = 4,023
27(36508, 149)
*/