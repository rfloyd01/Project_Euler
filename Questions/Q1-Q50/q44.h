#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Pentagon Numbers
std::pair<std::string, double> q44()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int limit = 10000000;

	std::vector<int> pentagonal_check, pentagonal_numbers;
	for (int i = 0; i < limit; i++) pentagonal_check.push_back(0);

	int i = 1;
	while (true)
	{
		int hex = i * (3 * i - 1) / 2;
		if (hex > limit) break;
		pentagonal_check[hex] = 1;
		pentagonal_numbers.push_back(hex);
		i++;
	}

	for (int first_num = 0; first_num < pentagonal_numbers.size(); first_num++)
	{
		bool cont = true;
		for (int second_num = first_num + 1; second_num < pentagonal_numbers.size(); second_num++)
		{
			if (pentagonal_numbers[second_num] + pentagonal_numbers[first_num] >= limit) break;
			if (pentagonal_check[pentagonal_numbers[second_num] + pentagonal_numbers[first_num]])
			{
				if (pentagonal_check[pentagonal_numbers[second_num] - pentagonal_numbers[first_num]])
				{
					answer = pentagonal_numbers[second_num] - pentagonal_numbers[first_num];
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 5482660
	//ran in 0.10292 seconds
}

//NOTES
//There's probably an easy way to do this mathematically, but we're programing here! Not sure how small the answer is gonna
//be but I'm setting a ceiling at 50,000 and will go from there. Just generate all pentagon numbers below 50,000
//and see how many hits I get. Will have one vector which acts as a hash table for quick lookup of whether or not a
//number is pentagonal and another vector which holds only pentagonal numbers which I'll pull from to test. Given two
//pentagonal numbers, iterate through list starting directly after the second pentagonal number until their sum is less
//than the next pentagonal number in the list. If you get no hits then you know the lower pentagonal number can no longer
//reach higher pentagonal numbers and can be discarded (i.e. 1 + 5 is 6 which is less than the next pentagonal number
//of 12. Therefore there isn't any reason to try adding 1 to other numbers as the distance just grows larger.). Not
//really proud of this one as it was more or less brute force but it gets the job done. Ended up having to increase the
//limit all the way to 10,000,000 in order to get an answer.