#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes vprint()
#include <vector>
#include <map>

//Pandigital products
void constructPandigitals(int length, int current_level, std::vector<bool>& nums, std::vector<int>& bin, std::vector<int>& bin_values, int used = 0, int current_number = 0, int ten_power = 1)
{
	//A recursive function to create pandigital numbers, seemed quicker to do it this way than to just loop through everynumber to test if it's pandigital
	//also keeps track of the binary representation of each pandigital numbers, so it can be seen at a glance which digits it's made up of
	//i.e. the pandigital number 156 would have a binary value of [0, 1, 0, 0, 0, 1, 1] (98). 651 would also have a binary value of 98 (don't care about the order of the digits)
	if (current_level == length)
	{
		nums[current_number] = 1;
		bin[current_number] = used;
	}
	else
	{
	    for (int i = 1; i < 10; i++)
	    {
			if (bin_values[i] & used) continue;
			constructPandigitals(length, current_level + 1, nums, bin, bin_values, used + bin_values[i], current_number + ten_power * i, ten_power * 10);
	    }
	}
}

std::pair<std::string, double> q32()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, target_binary = 1022; //target binary is 1022 because that's the binary value of 2^1 - 2^9, which represents a number having all digits 1-9
	std::vector<bool> p_nums;
	std::vector<int> binary_values = { 1 }, binary; //represents 2^index power, so iniitialize with 2^0 = 1
	std::map<int, bool> answers; //map used to ensure no duplicates

	for (int i = 0; i < 10000; i++)
	{
		p_nums.push_back(0);
		binary.push_back(0);
	}

	int bin = 2;
	for (int i = 1; i <= 9; i++)
	{
		binary_values.push_back(bin);
		bin *= 2;
	}

	//loop to construct pandigitals between 1 and 4 digits
	for (int digits = 1; digits < 5; digits++) constructPandigitals(digits, 0, p_nums, binary, binary_values);

	for (int i = 0; i < p_nums.size(); i++)
	{
		if (!p_nums[i]) continue; //skip if i isn't a pandigital number
		if (i * i >= 10000) break;
		for (int j = i + 1; j < p_nums.size(); j++)
		{
			if (i * j >= 10000) break;//break if i and j yield a 5 digit number or larger
			if (!p_nums[j]) continue; //skip if j isn't a pandigital number
			if (binary[i] & binary[j]) continue; //skip if i and j share a digit
			

			int numb = i * j;
			if (p_nums[numb])
			{
				if (((binary[i] | binary[j]) ^ binary[numb]) == target_binary) //exclusive or is used to make sure digits 1-9 all appear, and only appear once
				{
					if (answers[numb]) continue; //even though this instance passes the test skip it because the product has already been hit by two other numbers
					answers[numb] = true;
					answer += numb;
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 45228
	//ran in 0.0004912 seconds
}

//NOTES
//Decided to create every possible pandigital number in a loop, each pandigital number also has a 10 digit binary number attached
//to it which represents the chosen numbers in the pandigital. When going to multiply two numbers together, a binary & is called
//on the binary representations of the number and the multiplication will only happen if the value of that & is 0. Finally, the
//product will be checked to make sure it a) has the appropriate number of digits b) is only comprised of digits which haven't been
//used yet and then c) is itself a pandigital number. Was able to find the numbers almost instantly, however, had a very hard time
//breaking out of main search loop once it was no longer possible to find any answers so loop keeps running for a long time after
//all of the answers have been found. Took a long time figuring out the best way to break out of multi-loop, ended up adding the
//number of digits of the first number, number of digits of the second number and the minimum amount of digits when multiplying an
//i digit number by a j digit number. For example, if i was 3 and j was also 3, 111 * 111 = 12,321 which is a 5 digit number. Well,
//3 + 3 + 5 = 11 digits, so it's not possible for this to be pandigital, therefore break out of that level of the loop.

//UPDATE - After thinking about it a little more, I realized that you don't need to create any pandigital numbers with more than 4
//digits. A 1 digit number times a 4 digit number should also give 4 digits (at a minimum), which in total is (1 + 4 + 4 = 9) digits. Not creating
//pandigitals or having to use function calls to figure out when to break out of loop yielded a 5x speedup in the program. Since 4 digit
//numbers are the maximum, all pandigitals can now be tracked in a simple boolean vector. There's no more reason to test if the product
//of two pandigitals is pandigital itself, as all pandigitals are stored in a vector. Removing the pandigital test for the product and
//a few other things led to another 20x speed up. So the final program is now 100x faster than the original!