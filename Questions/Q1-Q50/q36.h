#pragma once

#include <Header_Files/pch.h>
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
	int answer = 0;
	std::vector<int> bin_palindromes = { 1, 3, 5, 7 };
	//numbers 1-7 are palindromes in binary but algorithm starts at 8, add these in manually for ease

	int num_base, num_back, limit = 2;
	for (int digit = 1; digit <= 10; digit++) //digit max should be 10
	{
		int num_start = 1 << digit;
		for (int i = 0; i < limit; i++)
		{
			num_base = (num_start | i);
			num_back = reverseBinary(num_base);
			int num1 = num_base << (digit + 1) | num_back;
			int num2 = num_base << (digit + 2) | num_back;
			int num3 = ((num_base << (digit + 2)) | (1 << digit + 1)) | num_back;
			if (num1 < 1000000) bin_palindromes.push_back(num1);
			if (num2 < 1000000) bin_palindromes.push_back(num2);
			if (num3 < 1000000) bin_palindromes.push_back(num3);
		}
		limit *= 2;
	}

	for (int i = 0; i < bin_palindromes.size(); i++)
	{
		if (palindromeTest(bin_palindromes[i])) answer += bin_palindromes[i];
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 872187
	//ran in 0.0001225 seconds
}

//NOTES
//Orignal thought was to create two separate arrays between 0-1000000, one to hold binary palindromes and
//the other for base 10 palindromes. It took about .1 seconds to create each vector so decided against that
//and just made an array where binary palindromes were found and appended to list. Each palindrome in that
//list was then checked for base 10 palindromeness.