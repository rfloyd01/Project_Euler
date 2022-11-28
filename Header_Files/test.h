#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <fstream>

long long getPalindromeT(long long n)
{
	long long pal = 0;
	while (n > 0)
	{
		pal *= 10;
		pal += n % 10;
		n /= 10;
	}
	return pal;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	uint8_t* lychrel_numbers = new uint8_t[10000](); //each index can hold a value of 0, 1 or 2. 0 = untested, 1 = non-lychrel, 2 = lychrel

	long long current_number = 0;

	for (int i = 1; i < 10000; i++)
	{
		//if (lychrel_numbers[i]) continue; //skip numbers that have already been tested
		std::vector<long long> current_chain; //create vector for new chain
		bool lychrel = true;
		current_number = i;

		for (int j = 0; j < 50; j++)
		{
			if (current_number < 10000 && current_number > 0 && lychrel_numbers[current_number] == 1)
			{
				//we've already encountered this number
				lychrel = false;
				for (int k = 0; k < current_chain.size(); k++)
				{
					//if (current_chain[k] >= 10000) break; //no need to keep track of numbers outside of current limit
					lychrel_numbers[current_chain[k]] = 1; //set every number in current chain to be non-lycheral
				}
				break; //no need to iterate all the way up to 50
			}
			long long pal = getPalindromeT(current_number);
			if (current_number == pal) //test current number for palindromeness
			{
				if (j > 0) //starting on a palindrome doesn't count, as can be seen in example text
				{
					lychrel = false;
					for (int k = 0; k < current_chain.size(); k++)
					{
						if (current_chain[k] >= 10000) break; //no need to keep track of numbers outside of current limit
						lychrel_numbers[current_chain[k]] = 1; //set every number in current chain to be non-lycheral
					}
					break; //no need to iterate all the way up to 50
				}
			}

			if (current_number < 10000 && current_number > 0)
			{
				if (!lychrel_numbers[current_number]) current_chain.push_back(current_number);
				if (current_number % 10 && !lychrel_numbers[pal] && pal != current_number)
				{
					current_chain.push_back(pal); //make sure palindrome doesn't have a lead 0 before adding
				}
			}
			current_number += pal;
		}

		

		if (lychrel)
		{
			for (int k = 0; k < current_chain.size(); k++)
			{
				lychrel_numbers[current_chain[k]] = 2; //set every number in current chain to be lycheral
				answer++; //add each lychrel number under 10000 towards the answer
			}
		}
	}

	//delete lychrel numbers array from heap memory
	delete[] lychrel_numbers;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}


//current best ratio for N=250 is 133.00456453