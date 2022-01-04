#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>

#include <iostream>
#include <cmath> //includes ceiling()
#include <vector>
#include <algorithm> //includes std::prev_permutation

bool split(long long n, int goal)
{
	if (goal < 0) return false;
	long long mult = 1, first_num, last_num;

	while (true)
	{
		mult *= 10;
		first_num = n / mult;
	    last_num = n % mult;
		if (first_num == 0) return false;

		long long num = first_num + last_num;

		if (num == goal) return true;
		else if (num > goal)
		{
			if (split(first_num, goal - last_num)) return true;
			continue;
		}
	}
}

//Number Splitting
std::pair<std::string, double> q719()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0;
	for (long long i = 4; i <= 1000000; i++)
	{
		if (i % 9 > 1) continue; //not sure why this is the case but saw it in the forums, speed up program by about 4.5X!

		if (split(i * i, i)) answer += i * i;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 128088830547982
	//ran in 0.205519 seconds seconds
}

//NOTES
//Upon looking at the problem, you can see that since you're looking at square roots you really only have to check the numbers 1 - 1,000,000 instead of 1 - 10^12 which is a big help.
//My first thought was to look at every number up to a million, and based on how many digits it is place a '+' sign in every possible location inside the number. For example, the number
//1,234 could have plus signs in the following locations: (1 + 234, 12 + 34, 123 + 4, 1 + 2 + 34, 1 + 23 + 4, 12 + 3 + 4 and 1 + 2 + 3 + 4). In order to find out the possible number of
//ways to place the '+' signs, just look at the number of partitions that there are for an n digit number, and then use all possible permutations of each partition. Looking at the above
//example, 1234 is a four digit number and the partitions of 4 are (4, 3/1, 2/2, 2/1/1 and 1/1/1/1). Each of the numbers here will represent the number of digits before the next '+' sign.
//So 2/2 would translate to 12 + 34. As for permutations, 3/1 could also be written as 1/3, i.e. 123 + 4 and 1 + 234. The first permutation was left out as it wouldn't have any '+'signs
//associated with it. After looking at this for a little longer, realized that at least half of the permutations could be ignored. If you need the sum to equal a 6 digit number for example,
//then any permutation that didn't have at least a 5 digit number in it can't reach 6 digits when added. On the flip side, if a permutation had more digits than the target it could be ignored.
//Trying to get a 6 digit answer with 10 digits for example, 12345678 + 90 would equal more than 6 digits. Used two variables named max_digit and min_digit to weed out any permutations that 
//had digits greater than max_digit or that didn't have any digits at least as big as min_digit. All of the permuted partitions were found recursively using the getPartitions and 
//getUsefulPartitions functions. From this point just iterated through the numbers 1-1,000,000 and brute forced every possible partition to see if there was a succesful one (broke out of loop
//as soon as a hit was found to save time). Got the correct answer in this manor in ~92 seconds, which doesn't beat the 1 minute rule. I figured that if a number was really big for its number
//of digits (i.e. 100,000 is the smallest 6 digit number and 999,999 is the biggest) you could only use permutation where there was at least one n digit number. Added some lines of code that
//would ignore any permutations that didn't have at least n number of digits if the first digit of the square number was greater than 1, and the number of digits in the square number was even.
//My thinking was right and this reduced the run time of the program down to about 35 seconds so it now beats the 1 minute rule! After reading the forum posts, someone mentioned that a number
//only works if it = 1 one module divided by 9. Not sure why this is the case, but it sped up the program so that it now runs in only 7.77 seconds.

//UPDATE
//Found a recursive algorithm in the forums that works really well. Same concept as finding number partitions and trying all of them, except a lot of partitions get weeded out along the
//way. Therefore a lot of options that were being tested for have already been eliminated with this algorithm. Runs in only .21 seconds with the mod 9 speed up also found on the forum!
//Also, use only integers and division here. Not needing to convert to strings and back saves time as well. Old code is saved down below.

/*
void getPartitions(int n, int first_digit, int current_sum, std::vector<std::vector<int> > &parts, std::vector<int> &current_part)
{
	for (int i = n - current_sum; i > 0; i--)
	{
		if (i > current_part.back()) continue;
		int sum = current_sum + i;
		if (sum == n)
		{
			current_part.push_back(i);
			//parts.push_back(current_part); //add first permutation

			do {
				parts.push_back(current_part);
				//vprint(current_part); //delete this later, just add for now to make sure prev_permutation is working
			} while (std::prev_permutation(current_part.begin(), current_part.end()));
			current_part.pop_back();
		}
		else if (sum < n)
		{
			current_part.push_back(i);
			getPartitions(n, first_digit, current_sum + i, parts, current_part);
			current_part.pop_back();
		}
	}
	return;
}
std::vector<std::vector<std::vector<std::vector<int> > > > getUsefulPartitions(int n)
{
	//gets all of the partitions for numbers 1-n in all orders (i.e. for 6, (2, 2, 1, 1), (2, 1, 2, 1), (2, 1, 1, 2), (1, 2, 2, 1), (1, 2, 1, 2) and (1, 1, 2, 2) would be returned)
	//a partition is only useful 1f two condidions are met:
	//1 - No number in the partition can be larger than the number of digits of n^.5
	//    For example, if n is 7 or 8 (representing a 7 or 8 digit number), then no partitions with 5 or above will be useful.
	//    This is because the square root of all 7 and 8 digit numbers, is a 4 digit number (1,000,000^.5 = 1,000 and 99,999,999^.5 = 9,999(after removing decimals)).
	//    A variable called max_digit will store the maximum number allowed in a partition and weed out all answers with something higher. It can be proven that max_digit
	//    is equal to ceiling(n / 2).
	//2 - There must be a minimum number in the partition equal to the digits of the square root - 1.
	//    If looking at a 6 digit number for example, partitions of (4,2) or (5,1) would be found. The maximum value of (5,1) would be 99,999 + 9 = 100,008 (a 6 digit number).
	//    The maximal value for (4, 2) would be 9,999 + 99 = 10,098 (a 5 digit number). If there isn't at least one digit of (n-1) then it won't be possible to reach n. This
	//    minimal digit will be stored in a variable called min_digit.

	std::vector<std::vector<std::vector<std::vector<int> > > > useful_parts;

	for (int i = 0; i <= n; i++)
	{
		std::vector<std::vector<std::vector<int> > > parts_of_i;
		useful_parts.push_back(parts_of_i);

		int max_digit = ceil((float)i / 2.0);
		int min_digit = max_digit - 1;

		//calculate all partitions of n (where digit_1 <= max_digit and digit_1 >= min_digit)
		//partitions for each number will be stored in two vectors, vector 1 is where the first digit is max_digit, vector 2 is where the first digit is min_digit
		for (int j = max_digit; j >= min_digit; j--)
		{
			std::vector<std::vector<int> > parts;
			std::vector<int> current_part = { j };
			getPartitions(i, j, j, parts, current_part);

			useful_parts[i].push_back(parts);
			//for (int k = 0; k < parts.size(); k++) useful_parts[i].push_back(parts[k]);
		}
	}

	return useful_parts;
}

std::pair<std::string, double> q719_oldversion()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1000000000000; //there's only one 13 digit number to test and it's known that it works, no need to code permutations up to 13, 12 will be fine
	std::vector<std::vector<std::vector<std::vector<int> > > > partitions = getUsefulPartitions(12);

	//main loop
	for (int i = 1; i <= 1000000; i++)
	{
		if (i % 9 > 1) continue; //not sure why this is the case but saw it in the forums, speed up program by about 4.5X!

		long long square = (long long)i * (long long)i;

		int length;
		if (i < 4) length = 1; //3**2 = 9, 4**2 = 16
		else if (i < 10) length = 2; //9**2 = 81, 10**2 = 100
		else if (i < 32) length = 3; //31**2 = 961, 32**2 = 1024
		else if (i < 100) length = 4; //99**2 = 9,801, 100**2 = 10,000
		else if (i < 317) length = 5; //316**2 = 99,856, 317**2 = 100,489
		else if (i < 1000) length = 6; //999**2 = 998,001, 1000**2 = 1,000,000
		else if (i < 3163) length = 7; //3162**2 = 9,998,244, 3163**2 = 10,004,569
		else if (i < 10000) length = 8; //9999**2 = 99,980,001, 10000**2 = 100,000,000
		else if (i < 31623) length = 9; //31622**2 = 999,950,884, 31623**2 = 1,000,014,129
		else if (i < 100000) length = 10; //99999**2 = 9,999,800,001, 100000**2 = 10,000,000,000
		else if (i < 316228) length = 11; //316227**2 = 99,999,515,529, 316228**2 = 100,000,147,984
		else if (i < 1000000) length = 12;

		std::string number = std::to_string(square); //convert the square into a string so it's easier to manipulate

		//if the first digit of the square number is greater than 1 it makes certain smaller permutations impossible so they can be skipped down below
		bool cut_short = 0;
		if (std::stoi(std::to_string(number[0])) > 1 && length % 2 == 0) cut_short = 1;

		//Go through max_number partitions first
		for (int j = 0; j < partitions[length][0].size(); j++)
		{
			long long sum = 0;
			int position = 0;

			for (int k = 0; k < partitions[length][0][j].size(); k++)
			{
				std::string snum = number.substr(position, partitions[length][0][j][k]);
				//number_additions.push_back(snum);

				long long num = std::stoll(snum);
				sum += num;
				position += partitions[length][0][j][k];
			}

			if (sum == i)
			{
				answer += square;
				//std::cout << square << "^.5 = " << i << std::endl;
				cut_short = 1; //set this to 1 in case it wasn't already so the next set of partitions can be skipped
				//for (int k = 0; k < number_additions.size() - 1; k++) std::cout << number_additions[k] << " + ";
				//std::cout << number_additions.back() << std::endl;
				break;
			}
		}

		if (cut_short) continue;

		//Go through min_number partitions second
		for (int j = 0; j < partitions[length][1].size(); j++)
		{
			long long sum = 0;
			int position = 0;

			for (int k = 0; k < partitions[length][1][j].size(); k++)
			{
				std::string snum = number.substr(position, partitions[length][1][j][k]);
				//number_additions.push_back(snum);

				long long num = std::stoll(snum);
				sum += num;
				position += partitions[length][1][j][k];
			}

			if (sum == i)
			{
				answer += square;
				//std::cout << square << "^.5 = " << i << std::endl;
				//for (int k = 0; k < number_additions.size() - 1; k++) std::cout << number_additions[k] << " + ";
				//std::cout << number_additions.back() << std::endl;
				break;
			}
		}
	}
}
*/