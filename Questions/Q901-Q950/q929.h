#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <Header_Files/print.h>

//Odd-Run Compositions
std::vector<long long> correct_nums = { 1, 1, 1, 4, 4, 10, 19, 33, 59, 113, 210, 379, 704, 1296, 2380, 4410, 8104, 14932, 27521, 50706, 93426 };

long long firstMethod(int limit, long long mod)
{
	std::vector<std::vector<std::pair<long long, long long> > > oddEndingRuns = { {{0, 1}} };
	std::vector<std::vector<long long> > evenEndingRuns = { {0} };
	long long cumulative_sum = 0;

	for (int n = 1; n <= limit; n++)
	{
		//First calculate compositions of n that end in an even run
		//by looking at all compositions less than n which end in an odd
		//run, and add the same digit to the back.
		evenEndingRuns.push_back({ 0 });
		for (int i = 1; i <= n / 2; i++) evenEndingRuns.back().push_back(oddEndingRuns[n - i][i].first);

		//Then calculate compositions of n that end in an odd run
		//by looking at two things. Compositions less than n which 
		//end in an odd run and will have a 'different' number added to 
		//it. Compositions less than n which end in an even run and
		//will have the 'same' number added to it.
		oddEndingRuns.push_back({ {0, 0} });
		cumulative_sum = 0;
		for (int i = 1; i <= n; i++)
		{
			if (n - i < i)
			{
				oddEndingRuns.back().push_back({ oddEndingRuns[n - i][0].second, 0 });
			}
			else
			{
				oddEndingRuns.back().push_back({ oddEndingRuns[n - i][i].second, 0 });
			}

			if ((n - i) / 2 >= i) oddEndingRuns.back().back().first = (oddEndingRuns.back().back().first + evenEndingRuns[n - i][i]) % mod;
			cumulative_sum = (cumulative_sum + oddEndingRuns.back().back().first) % mod;
		}

		//Add the cumulative number of ways to make the current number to the second 
		//part of each pair, subtracting the first part of the pair. This will tell 
		//us in future steps the number of ways to add i to compositions of n - i 
		//that end in different numbers.
		oddEndingRuns.back()[0].second = cumulative_sum;
		for (int i = 1; i <= n; i++) oddEndingRuns.back()[i].second = (cumulative_sum - oddEndingRuns.back()[i].first);
	}

	return cumulative_sum;
}

long long secondMethod(int limit, long long mod)
{
	std::vector<std::vector<long long> > runs = { {1} };

	for (int n = 1; n <= limit; n++)
	{
		//Create a vector of size n+1 to hold the compositions for n
		std::vector<long long> compositions(n + 1, 0);

		//Iterate through all numbers x such that 2x <= n, adding double digits onto the 
		//back of compositions of y ending in x where n - 2x = y. For example, if n = 6
		//then a new composition of 6 can be created by adding double 1's onto the back of 
		//compositions of 4 ending in 1, i.e. (3,1 | 1,1) and (1,2,1 | 1,1)
		int x = 2, y = 1;
		for (; x <= n; x += 2)
		{
			if (n - x < y) break; //There will be no runs ending in y so break out of loop
			compositions[y++] = runs[n - x][y]; //increment y, which represents x/2
		}

		//Compositions of n are calculated by iterating values of x from 1 to n
		//and adding a single digit x onto the back of compositions of n - x that 
		//don't end in x (otherwise compositions with even runs would be created).
		//The total number of compositions of n is stored at index 0 of the given array,
		//and compositions of n ending in x are stored at index x of the given array.
		for (int x = 1; x <= n; x++)
		{
			compositions[x] = (compositions[x] + runs[n - x][0]) % mod;
			if (x <= (n - x)) compositions[x] = (compositions[x] - runs[n - x][x]) % mod; //subtract out compositions starting with x

			//add compositions of n ending with x to cumulative total
			compositions[0] = (compositions[0] + compositions[x]) % mod;
		}

		//Add compositions of n to the final list and add the cumulative number of compositions of 
		//n to the running total
		runs.push_back(compositions);
	}

	for (int i = 0; i <= limit; i++)
	{
		/*std::cout << '[';
		for (int j = 0; j < runs[i].size(); j++)
		{
			std::cout << runs[i][j] << "(" << runs[i][j] - correct_nums[i - j] << "), ";
		}
		std::cout << ']' << std::endl;*/
		vprint(runs[i]);
	}

	return runs[limit][0];
}

std::pair<std::string, long double> q929 ()
{
	auto run_time = std::chrono::steady_clock::now();
	int limit = 5;
	long long answer = 0, mod = 1111124111;

	//answer = firstMethod(limit, mod);
	answer = secondMethod(limit, mod);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 351367844 (n = 10000)
	//ran in xxx seconds
}

//NOTES
//The answer is 984660863
//Program ran in 0.065374 seconds