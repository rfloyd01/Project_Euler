#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>

#include <iostream>
#include <vector>
#include <map>
#include <cmath> //includes pow()

int total = 0, brute_total = 0;
int number_of_digits = 10; //initially used 10 everywhere but became cumbersome to change numbers when testing smaller numbers so made this variable
std::map<long long, long long> used;

//Double Pandigital Number Divisible by 11
void brute_force_test(int level, int current_number, std::vector<int> currently_used)
{
	int start = 0;
	if (level == 0) start = 1;

	if (level == (2 * currently_used.size()))
	{
		if (current_number % 11 == 0)
		{
			//std::cout << current_number << std::endl;
			brute_total++;
		}
		return;
	}
	else
	{
		for (int i = start; i < currently_used.size(); i++)
		{
			if (currently_used[i] < 2)
			{
				currently_used[i]++;
				brute_force_test(level + 1, current_number * 10 + i, currently_used);
				currently_used[i]--;
			}
		}
	}
}
void find_pairs(int level, int start, int number_sum, long long &maximum, std::vector<int> &currently_used, std::vector<int> &desired_nums)
{
	if (level == currently_used.size())
	{
		for (int i = 0; i < desired_nums.size(); i++)
		{
			if (number_sum == desired_nums[i])
			{
				long long yo = 0;
				for (int j = 0; j < currently_used.size(); j++)
				{
					yo *= 10;
					yo += currently_used[j];
				}

				if (!used[yo])
				{
					used[maximum - yo] = 9; //this will make it so pairs aren't checked twice, use a positive number that isn't 1 or 2 here
					used[yo] = maximum - yo; //put this line second for the case of 1111111111 which is its own pair (don't want that value to be 9)
					//std::cout << "Found hit: " << yo << " and pair: " << used[yo] << std::endl;
					total++; //this line counts how many pairs there will be to test
				}
			}
		}
		
		return;
	}
	else
	{
		for (int i = start; i < currently_used.size(); i++)
		{
			if (currently_used[i] < 2)
			{
				currently_used[i]++;
				find_pairs(level + 1, i, number_sum + i, maximum, currently_used, desired_nums);
				currently_used[i]--;
			}
		}
	}
}
long long digit_shuffle(long long odd_set, long long even_set, long long digit_factorial)
{
	//to make things easier just assume the number in the 10^19 place is digit 1 and the digit in the 10^0 place is digit 20
	//this means the odd_set starts with the first digit of the number and the even set ends with the last digit of the number
	//std::cout << odd_set << ", " << even_set << std::endl;
	long long odd_copy = odd_set, even_copy = even_set, odd_factorial = digit_factorial, even_factorial = digit_factorial;

	//find the number of ways to shuffle the odd set and the even set. There are 10 digits in each set so the number of ways to shuffle the digits would
	//be 10! / (number of each number in the set)!, for example the ways to shuffle to be the set [1, 1, 2, 2, 4, 4, 5, 5, 6, 9] would be 10! / (2! * 2! * 2! * 2! * 1! * 1!)
	//1! = 1 and 2! = 2 so no need to keep track of factorials when dividing. Iterate 10 times to represent each digit, if a set doesn't have the digit then a zero will be yielded
	//to that iteration needs to be skipped to avoid division by zero
	for (int i = 0; i < number_of_digits; i++)
	{
		if (odd_copy % 10) //make sure the current digit isn't a zero
			odd_factorial /= (odd_copy % 10); //divide by the last digit which represents how many of digit i are represented in the number
		if (even_copy % 10) //make sure the current digit isn't a zero
			even_factorial /= (even_copy % 10); //divide by the last digit which represents how many of digit i are represented in the number

		odd_copy /= 10; //remove the last digit to move on to the next
		even_copy /= 10; //remove the last digit to move on to the next
	}

	//check to see if the odd set has any zeros in it, if so subtract any way to shuffle them which would put a zero as the first digit. Using a decimal representation
	//of ternary numbers, 1000000000 represents 1 zero when using digits 0-9, so if there are no zeros in the set then subtracting by this number will yield a negative value
	if (odd_set - pow(10, number_of_digits - 1) > 0)
	{
		odd_set -= pow(10, number_of_digits - 1); //remove one of the zeros and find total number of ways to shuffle the 9 remaining digits
		//std::cout << odd_set << std::endl;
		long long less_digit_factorial = digit_factorial / number_of_digits;
		for (int i = 0; i < 9; i++)
		{
			if (odd_set % 10) //make sure the current digit isn't a zero
			    less_digit_factorial /= (odd_set % 10); //divide by the last digit which represents how many of digit i are represented in the number
			odd_set /= 10; //remove the last digit to move on to the next
		}
		odd_factorial -= less_digit_factorial; //subtract the ways so that odd_factorial represents all the ways without a 0 as the first digit
	}

	//std::cout << odd_factorial << ", " << even_factorial << std::endl << std::endl;
	return odd_factorial * even_factorial; //multiply the number of ways to shuffle each set to get the total ways to shuffle the 20 digit set
}

std::pair<std::string, double> q491()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0, maximum = 0;
	long long digit_factorial = 1;
	for (int i = 2; i <= number_of_digits; i++) digit_factorial *= i;

	std::vector<int> nums;
	for (int i = 0; i < number_of_digits; i++)
	{
		nums.push_back(0);
		maximum *= 10;
		maximum += 2;
	}

	//create a vector holding desired digit sums for 11 divisibility
	//had initially done this by hand for the case of 10 distinct digits but didn't want to hand calculate for other small base cases while testing
	int digit_sum = number_of_digits * (number_of_digits - 1), difference = 22; //2 times the triangle number for number of digits - 1
	std::vector<int> desired_nums = { digit_sum / 2 }; //digit sum will be an even number unless number of digits is 1, however we need at least 2 digits to be divisible by 11 anyway
	while (true)
	{
		int one, two;
		one = (digit_sum - difference) / 2;
		if (one <= 0) break;
		two = digit_sum - one;
		desired_nums.push_back(one);
		desired_nums.push_back(two);
		difference += 22;
	}

	//Get all usable sets for even and odd digits, store in the map <used>
	find_pairs(0, 0, 0, maximum, nums, desired_nums);

	//iterate through all pairs in the map and figure out how many ways there are to shuffle the digits
	for (auto i = used.begin(); i != used.end(); i++)
	{
		if (i->second == 9) continue; //put in some dummy values earlier to make sure we weren't double counting, ignore those now
		answer += digit_shuffle(i->first, i->second, digit_factorial);
		if (i->first != i->second) answer += digit_shuffle(i->second, i->first, digit_factorial);
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 194505988824000
	//ran in 0.0006428 seconds
}

//NOTES
//A quick Google search shows that for a number to be divisible by 11 the sum of the even digits must equal the sum of the odd digits, or the sum of the even digits and the sum of
//the odd digits must differ by a factor of 11. For example the number 12375 is divisible by 11 because the sum of the odd digits (1 + 3 + 5 = 9) equals the sum of the even digits
//(2 + 7 = 9). 12375 / 11 = 1125. In the case of this problem where there are two of every digit then the digit sum will always be even. Because of this we only care about digit sums
//with a difference of a factor of 11 that's divisible by 2, aka numbers like 22, 44, 66, etc. as can be seen in the problem example, 40561817703823564929 because (4 + 5 + 1 + 1 + 7 + 3 + 2 + 5 + 4 + 2 = 34) and
//(0 + 6 + 8 + 7 + 0 + 8 + 3 + 6 + 9 + 9 = 56) and (56 - 34 = 22 % 11 = 0). With this knowledge the problem becomes a lot easier, we know that the sum of digits for a double
//pandigital number will always equal 90, so splitting up the even and odd digits we need digits sums of (45, 45), (34, 56) or (23, 67) where only 10 numbers are used and no number is
//used more than twice [pairs of (12, 78) and (1, 99) don't work because the smallest sum possible would be (0 + 0 + 1 + 1 + 2 + 2 + 3 + 3 + 4 + 4 = 20). The function above named
//find_pairs() creates all of the possible pairs and stores them in a map of long longs. Each of these long longs is actually a ternary representation of each set of digits. For example,
//the number 2120001121 represents the set of digits [0, 0, 1, 2, 2, 6, 7, 8, 8, 9]. Each pair can be found by subtracting from 2222222222: 2222222222 - 2120001121 = 102221101 which 
//represents the set of digits [1, 3, 3, 4, 4, 5, 5, 6, 7, 9] and can be seen to compelemnt the first set because each digits appears twice. From this point, the number of ways to shuffle
//each set of digits is easy to find using combinatorics. Multiplying the number of combinations for each set in the pair by eachother and then switching the even set with the odd set and
//recalculating should yield the total ways to get a 20 digit number divisible by 11. Care needs to be taken though to make sure none of the numbers start with a 0,
//as that would only be a 19 digit number. The one exception is where the digits sets are identical as in the case of two sets of [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] it is possible to double
//count as switching the even and odd set won't change anything for identical sets. I figured that there may be some other random places where double counting might happen which I'm not
//thinking about so I wrote a quick brute force function to test my main algorithm against for smaller numbers. This ended up being cumbersome as I had to rewrite a bunch of code to be able
//to easily test smaller numbers (i.e. couldn't just write out 2222222222) but it did end up helping me debug in the end so it was worth it.