#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes choose()

//Lattice paths
std::pair<std::string, double> q15()
{
	auto run_time = std::chrono::steady_clock::now();


	////In the HackerRank version, a loop would go at this part that would print out grid[m][n] for all of the given inputs.
	////The ProjectEuler version only cares about the one answer though so the loop has been ommited.
	//long long answer = grid[20][20];
	long long answer = recursiveChoose(40, 20);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 137846528820
	//ran in 0.0000009 seconds
}

//NOTES
//I have more in depth notes up on my website, but essentially, the answer to this problem can be found simply by taking the binomial coefficient of the number midway through the 2*Nth
//row of Pascal's Triangle. That means the answer to this problem is nothing more than Choose(40, 20). As neat as that code looks, since this is a relatively small case I've found that it's
//actually over 10x quicker to just make a 20x20 grid and fill out the values of Pascal's Triangle using a dynamic approach as opposed to just calculating Choose(40, 20) which is a rather
//slow function.

//Original algorithm
/*
long long answer = choose(40, 20); //this function is in my functions.h file
//ran in 0.000032 seconds

long long answer = binomial(40, 20); //a touch faster than the above function, implementation below
//ran in 0.0000242 seconds

long long binomial(int n, int k)
{
	//n will be larger than both k and n - k, so generate all primes that are
	//less than or equal to n
	std::vector<int> prims = primes(n);

	//arrays to store number of each prime
	int* top_primes = new int[n + 1]();

	for (int i = 0; i < prims.size(); i++)
	{
		int current_num = prims[i], instancesk = 0, instancesk_minus = 0;
		while (current_num <= n)
		{
			top_primes[prims[i]] += n / current_num;
			instancesk = k / current_num;
			instancesk_minus = (n - k) / current_num;

			if (instancesk <= k) top_primes[prims[i]] -= instancesk;
			if (instancesk_minus <= (n - k)) top_primes[prims[i]] -= instancesk_minus;

			current_num *= prims[i];
		}
	}

	long long ans = 1;
	for (int i = 2; i <= n; i++)
		if (top_primes[i]) ans *= pow(i, top_primes[i]);

	delete[] top_primes;

	return ans;
}
*/

/*
* Update 8/10/22. Decided to use the recent recursive choose function I found, makes this question
* cleaner and faster. Here's the code from before this update
* 	const int maximum = 20; //maximum value is 500 in HackerRank version

	//create grid and initialize first row and column to have values of 1.
	long long grid[maximum + 1][maximum + 1] = { {0} };
	for (int i = 0; i <= maximum; i++)
	{
		grid[0][i] = 1;
		grid[i][0] = 1;
	}

	//Traverse through the grid and dynamically build answer
	for (int i = 1; i <= maximum; i++)
	{
		for (int j = 1; j <= maximum; j++)
		{
			grid[i][j] = grid[i - 1][j] + grid[i][j - 1]; // % 1000000007; modulo needed for HackerRank version
		}
	}
*/