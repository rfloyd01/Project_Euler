#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //include MyPow()
#include <vector>

//Double-base palindromes

int reverseBinary(int n)
{
	int answer = 0, digit;
	while (n > 0)
	{
		digit = n & 1;
		answer = (answer << 1) | digit;
		n >>= 1;
	}
	return answer;
}

bool palindromeTest(int n)
{
	int length;
	if (n < 10) length = 1;
	else if (n < 100) length = 10;
	else if (n < 1000) length = 100;
	else if (n < 10000) length = 1000;
	else if (n < 100000) length = 10000;
	else if (n < 1000000) length = 100000;
	while (n > 0)
	{
		if (n % 10 != n / length) return 0;
		n %= length;
		n /= 10;
		length /= 100;
	}
	return 1;
}

std::pair<std::string, double> q36()
{
	auto run_time = std::chrono::steady_clock::now();

	//1 won't get counted so include it in answer to start off
	int num_base, num_back, limit = 1, answer = 1;
	for (int digit = 0; digit <= 9; digit++)
	{
		int num_start = 1 << digit;
		for (int i = 0; i < limit; i++)
		{
			num_base = (num_start | i);
			num_back = reverseBinary(num_base);
			int num1 = num_base << (digit + 1) | num_back;
			int num2 = num_base << (digit + 2) | num_back;
			int num3 = ((num_base << (digit + 2)) | (1 << digit + 1)) | num_back;
			if (num1 < 1000000) if (palindromeTest(num1)) answer += num1;
			if (num2 < 1000000) if (palindromeTest(num2)) answer += num2;
			if (num3 < 1000000) if (palindromeTest(num3)) answer += num3;
		}
		limit *= 2;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 872187
	//ran in 0.000017 seconds
}

//NOTES
//Orignal thought was to create two separate arrays between 0-1000000, one to hold binary palindromes and
//the other for base 10 palindromes. It took about .1 seconds to create each vector so decided against that
//and just made an array where binary palindromes were found and appended to list. Each palindrome in that
//list was then checked for base 10 palindromeness.

//HACKERRANK UPDATE
//The HackerRank version allows the limit to change, and the base for palindromes, so it makes more sense to
//just create all palindromes in base 10, then convert to the desired base and check for palindromeness again.
//Here's the code
/*
unsigned long long convertToBaseK(int num, int k)
{
	//converts the given number to a decimal representation
	//of its base k equivalent, i.e. given num = 515 and k = 2
	//this function would return 1000000011.

	int current_num = MyPow(k, log(num) / log(k));
	unsigned long long answer = 0;

	while (current_num)
	{
		answer *= 10;
		int val = num / current_num;

		answer += val;
		num -= val * current_num;
		current_num /= k;
	}
	return answer;
}

std::vector<int> palindromes;
for (int i = 1; i < 1000000; i++)
{
	if (palindromeTest(i)) palindromes.push_back(i);
}

int limit = 1000000, base = 2, answer = 0;

for (int i = 0; i < palindromes.size(); i++)
{
	if (palindromes[i] >= limit) break;

	unsigned long long base_number = convertToBaseK(palindromes[i], base);
	if (palindromeTest(base_number)) answer += palindromes[i];

}
*/