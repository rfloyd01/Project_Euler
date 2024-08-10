#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes vprint()
#include <vector>
#include <map>

//Pandigital products
void constructPandigitals(int length, int N, int current_level, std::vector<bool>& nums, std::vector<int>& bin, std::vector<int>& bin_values, int used = 0, int current_number = 0, int ten_power = 1)
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
		for (int i = 1; i <= N; i++)
		{
			if (bin_values[i] & used) continue;
			constructPandigitals(length, N, current_level + 1, nums, bin, bin_values, used + bin_values[i], current_number + ten_power * i, ten_power * 10);
		}
	}
}

void testNumbers(bool growth, int digits, int start, int end, int N, int target_binary, bool& stop, std::vector<bool>& p_nums,
	std::vector<int>& binary, std::map<int, bool>& answers, int& answer)
{
	if (growth)
	{
		//since we need growth by X, the maximum value for 'j' will be the same 
		//regardless of what 'i' is. Calculate it now.
		int multiplicand_length = N / 2 - digits;
		if (multiplicand_length < digits)
		{
			//digits should be the smaller number, otherwise we'd would end up double 
			//counting unnecessarily
			stop = true;
			return;
		}
		int maximum = MyPow(10, multiplicand_length), goal = MyPow(10, N - digits - multiplicand_length - 1);
		for (int i = start; i < end; i++)
		{
			if (!p_nums[i]) continue; //skip if i isn't a non-repeat number
			//the minimum value of j will depend on the value of i
			//std::cout << i << ": minimum = " << goal / i << ", maximum = " << maximum << std::endl;
			for (int j = goal / i; j < maximum; j++)
			{
				if (!p_nums[j]) continue; //skip if i isn't a non-repeat number
				if (binary[i] & binary[j]) continue; //skip if i and j share a digit

				int numb = i * j;
				if (p_nums[numb])
				{
					if (((binary[i] | binary[j]) ^ binary[numb]) == target_binary) //exclusive or is used to make sure digits 1-9 all appear, and only appear once
					{
						if (answers[numb]) continue; //even though this instance passes the test skip it because the product has already been hit by two other numbers
						//std::cout << i << " * " << j << " = " << i * j << std::endl;
						answers[numb] = true;
						answer += numb;
					}
				}
			}
		}
	}
	else
	{
		//since we need growth by X-1, the minimum value for 'j' will be the same 
		//regardless of what 'i' is. Calculate it now.
		int multiplicand_length = (N + 1) / 2 - digits;
		if (multiplicand_length < digits)
		{
			//digits should be the smaller number, otherwise we'd would end up double 
			//counting unnecessarily
			stop = true;
			return;
		}
		int minimum = MyPow(10, multiplicand_length - 1), goal = MyPow(10, N - digits - multiplicand_length);
		for (int i = start; i < end; i++)
		{

			if (!p_nums[i]) continue; //skip if i isn't a non-repeat number
			//the maximum value of j will depend on the value of i
			//std::cout << i << ": minimum = " << minimum << ", maximum = " << goal / i << std::endl;
			for (int j = minimum; j < goal / i; j++)
			{
				if (!p_nums[j]) continue; //skip if j isn't a non-repeat number
				if (binary[i] & binary[j]) continue; //skip if i and j share a digit

				int numb = i * j;
				//std::cout << i << " * " << j << " = " << numb << std::endl;
				if (p_nums[numb])
				{

					if (((binary[i] | binary[j]) ^ binary[numb]) == target_binary) //exclusive or is used to make sure digits 1-9 all appear, and only appear once
					{
						if (answers[numb]) continue; //even though this instance passes the test skip it because the product has already been hit by two other numbers
						//std::cout << i << " * " << j << " = " << i * j << std::endl;
						answers[numb] = true;
						answer += numb;
					}
				}
			}
		}
	}
}

std::pair<std::string, double> q32()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, N = 9;
	bool growth = !(N % 2);

	int target_binary = 0; //target binary is 1022 because that's the binary value of 2^1 - 2^9, which represents a number having all digits 1-9
	std::vector<bool> p_nums;
	std::vector<int> binary_values, binary; //represents 2^index power, so iniitialize with 2^0 = 1
	std::map<int, bool> answers; //map used to ensure no duplicates

	for (int i = 0; i < 10000; i++)
	{
		p_nums.push_back(0);
		binary.push_back(0);
	}

	int bin = 1;
	for (int i = 0; i <= N; i++)
	{
		target_binary |= bin;
		binary_values.push_back(bin);
		bin *= 2;
	}
	target_binary--; //can't use the number 0 so remove 2^0 from target

	/*std::cout << target_binary << std::endl;
	vprint(binary_values);*/

	//loop to construct pandigitals between 1 and 4 digits
	for (int digits = 1; digits <= N / 2; digits++) constructPandigitals(digits, N, 0, p_nums, binary, binary_values);

	bool stop = false;
	int digits = 1, start = 1, end = 10;
	while (!stop)
	{
		testNumbers(growth, digits, start, end, N, target_binary, stop, p_nums, binary, answers, answer);
		digits++;
		start *= 10;
		end *= 10;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 45228
	//ran in 0.000337 seconds
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

//HACKERRANK UPDATE
//Made changes to pass the HackerRank trials and altered the start and stop variables for loop search. This led to a very slight
//speedup