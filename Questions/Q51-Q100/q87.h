#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes()
#include <Header_Files/print.h>
#include <vector>
#include <cmath>
#include <set>

//Prime Power Triples
std::pair<std::string, double> q87()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 50000000;

	std::vector<int> squares, cubes, fourths;
	std::set<int> checked;
	std::vector<int> prims = primes(sqrt(limit));

	bool trey = true, quat = true;

	for (int i = 0; i < prims.size(); i++) //populate list of squares, cubes and fourth powers up to 50,000,000
	{
		if (prims[i] * prims[i] >= limit) break;
		squares.push_back(prims[i] * prims[i]);
		if (trey)
		{
			if (squares.back() * prims[i] < limit) cubes.push_back(squares.back() * prims[i]);
			else trey = false;
		}
		if (quat)
		{
			if (cubes.back() * prims[i] < limit) fourths.push_back(cubes.back() * prims[i]);
			else quat = false;
		}
	}

	for (int i = 0; i < fourths.size(); i++)
	{
		for (int j = 0; j < cubes.size(); j++)
		{
			int k = 0, current_num = fourths[i] + cubes[j];
			if (current_num >= limit) break;

			while (true)
			{
				if (k >= squares.size()) break;
				if (squares[k] + current_num >= limit) break;
				/*
				if (!checked[squares[k] + current_num])
				{
					checked[squares[k] + current_num] = true;
					answer++;
				}
				*/
				checked.insert(squares[k] + current_num);
				k++;
			}
		}
	}

	answer = checked.size();
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1097343
	//ran in 0.456986 seconds
}

//NOTES
//Seems like a simple enough problem. Just generate all of the primes up to sqrt(50,000,000), create three
//separate vectors to hold squares, cubes and fourth powers, and then iterate over the three vectors and make
//sure that the sums are less than 50,000,000. There will most likely be a lot of duplicates so any time a 
//new value is found store it in a set. Can't seem to get this one to run in under a second and reading the
//forums doesn't seem like anyone else was able to either. Will need to revisit this problem at some point
//and see if there's anything else I can do to decrease the run time.

//MORE NOTES
//After running the code on my PC instead of my laptop the code now runs in roughly half a second. Still could
//be much better though.