#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Pentagon Numbers
std::pair<std::string, double> q44()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, square_count = 0;

	long long x = 0, adder = 2;
	bool cont = true;
	while (cont)
	{
		square_count++;
		x += adder;
		adder += 6;
		long long max_i = (1 + sqrt(1 + 6 * x)) / 6.0;
		for (long long i = 1; i <= max_i; i++)
		{
			long long c = 3 * i * i - i - x;
			long long j = (1 + sqrt(1 - 12 * c)) / 6.0;
			long long potential_root = 3 * i * i - i + 3 * j * j - j;

			//confirm whether or not j had a decimal portion
			if (potential_root == x)
			{
				long long k = (1 + sqrt(1 + 12 * potential_root)) / 6;

				long long potential_m_one = (1 + sqrt(1 + 12 * (3 * i * i - i + 3 * k * k - k))) / 6;
				long long potential_m_two = (1 + sqrt(1 + 12 * (3 * j * j - j + 3 * k * k - k))) / 6;

				if (3 * (i * i + k * k - potential_m_one * potential_m_one) == i + k - potential_m_one)
				{
					answer = j * (3 * j - 1) / 2;
					cont = false;
					break;
				}
				else if (3 * (j * j + k * k - potential_m_two * potential_m_two) == j + k - potential_m_two)
				{
					answer = i * (3 * i - 1) / 2;
					cont = false;
					break;
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 5482660
	//ran in 0.0055078 seconds
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