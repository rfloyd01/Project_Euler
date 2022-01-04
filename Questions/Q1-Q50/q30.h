#pragma once

#include <Header_Files/pch.h>
#include <vector>

long long convertVectoLong(std::vector<int> vec)
{
	int multiplier = 1;
	long long answer = 0;

	for (int i = vec.size() - 1; i >= 0; i--)
	{
		answer += multiplier * vec[i];
		multiplier *= 10;
	}

	return answer;
}

//Digit fifth powers
std::pair<std::string, double> q30()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	std::vector<int> nines = { 9 }, fifth_powers;
	for (int i = 0; i < 10; i++)
	{
		int pow = i * i * i * i * i;
		fifth_powers.push_back(pow);
	}

	int limit = fifth_powers.back();

	while (limit >= convertVectoLong(nines))
	{
		nines.push_back(9);

		limit += fifth_powers.back();
	}

	for (int i = 10; i < limit; i++)
	{
		int power_sum = 0, num = i;
		while (num > 0)
		{
			power_sum += fifth_powers[num % 10];
			num /= 10;
		}

		if (power_sum == i)
		{
			answer += i;
			//std::cout << i << std::endl; //uncomment this line to see all of the numbers that meet the requirement
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 443839
	//ran in 0.0019529 seconds
}

//NOTES
// The first step was to see what the theoretical limit is. 9 ^ 5 ~= 59,000 so eventually a number will get so many digits that even
//if every digit is a 9, the sum of each digit raised to the 5th power won't be able to reach the actual number. In this case that limit
//seems to be about 350,000. This number isn't that that high so just check every number from 1 - 350,000 to see if it satisfies what
//the question is asking for.
