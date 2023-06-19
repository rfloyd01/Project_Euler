#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Counting block combinations II
unsigned long long twoThresholdSolver(long double m, unsigned long long goal)
{
	//To make sure no overflow happens we solve for (n - m) and call this variable distance
	//once the value for distance has been found, we get the value for n by adding m to distance
	long double distance = sqrt(9 - 4 * (4 - 2 * goal)) / 2 - 1.5;
	return (unsigned long long)ceil(distance) + (unsigned long long)m;
}

unsigned long long threeThresholdSolver(long double m, unsigned long long goal)
{
	//This equation uses triangle and pentatope numbers, which are dependent on a 4th power. The quadratic 
	//equation solver for fourth order polynomials is much more complex than for order 2 polynomials so 
	//instead of solving like that, what we do instead is just input different values for N until we find the 
	//first one greater than or equal to the goal. This search can happen in O(log m) time by using a binary search
	//as opposed to just incrementing N by 1 digit at a time.
	unsigned long long i = 2 * m + 1, j = 3 * m + 1;
	bool overflow_avoided = false;

	while (true)
	{
		long double k = (i + j) / 2;
		long double x = k - 2 * m;

		//Just because this equation can be used doesn't mean there won't be overflow while using this equation. Before doing any 
		//calculations, see if there will be overflow by testing the fourth order part of the equation which will be disproportionally
		//larger than the rest of the equation.
		if (!overflow_avoided)
		{
			bool cont = true;
			unsigned long long big = -1; //set to the largest 64-bit integer
			long double test = big;
			for (int t = 0; t < 4; t++)
			{
				test /= (x + t);
				if (test < 1.0 / 24.0)
				{
					cont = false;
					break;
				}
			}

			if (!cont)
			{
				//overflow will happen if we test this value so it isn't the answer we're looking for. Update j and move on
				j = k;
				continue;
			}

			overflow_avoided = true;
		}
		
		unsigned long long potential_answer = 2 + k - m + (k - m) * (k - m + 1) / 2.0 + (x / 24.0 ) * (x + 1) * (x + 2) * (x + 3);

		if (potential_answer < goal)
		{
			//the number is too small so continue the search
			if (k == i)
			{
				//this will only happen when the answer is on index j
				return j;
			}
			i = k;
		}
		else
		{
			//We've exceeded the goal. Check to see if the value directly before this is less than the goal to confirm that 
			//this is the first value.
			long double l = k - 1;
			unsigned long long answer_check = 2 + l - m + (l - m) * (l - m + 1) / 2.0 + ((x - 1) / 24.0) * (x) * (x + 1) * (x + 2);

			if (potential_answer == goal || answer_check < goal) return k;
			else if (answer_check == goal) return l;
			else
			{
				//the number is too large so continue the search
				if (l <= i)
				{
					//this will only happen when the answer is on index i
					return i;
				}
				j = l;
			}
		}
	}
}

std::pair<std::string, double> CB2()
{
	auto run_time = std::chrono::steady_clock::now();

	/*
	* I found two closed form equations that give us the tilings for walls of size N from the range 0 to
	* 3m + 1 (where m is the size of the smallest tile we can use). Since the problem doesn't ask us to use
	* modulus division it can be assumed that the answer will not overflow a 64-bit integer. Given this,
	* two values have been calculated, three_m_threshold and two_m_threshold that let us know which of the 
	* closed form equations to use given the size of m.
	* If the smallest tile is larger than three_m_threshold then a wall of size (3m + 1) or larger will 
	* have its total tilings be greater than (2^64 - 1) and therefore
	* will have it's answer found from one of the two equations. Anything smaller than three_m_threshold can exceed 
	* a wall size of 3m + 1 without overflowing a 64-bit integer and needs to be solved manually
	* (since I haven't found the equation for larger wall sizes). If the value for m is larger than two_m_threshold 
	* it can only be solved with the first of the two closed form equations.
	*/

	unsigned long long three_m_threshold = 145054, two_m_threshold = 6074000998; //these values were calculated by hand

	long double m = 50; //use a long double so closed form equations can be used
	unsigned long long goal = 1000000;
	unsigned long long answer = 0;
	
	if (goal == 1) std::cout << m << std::endl; //edge case
	else if (goal == 2) std::cout << m + 1 << std::endl; //edge case
	else if (m > two_m_threshold)
	{
		//only the first of the two closed form equations can be used (i.e. the largets wall size we can have without 
		//overflowing a 64-bit integer is between m and 2m in length.
		std::cout << twoThresholdSolver(m, goal + 1) << std::endl;
	}
	else if (m > three_m_threshold)
	{
		//if m is smaller than the two_m_threshold but larger than the three_m_threshold this means that the answer
		//can be found with either of the two closed form equations. To figure out which equation to use, find the 
		//number of tilings for wall 2m using the first of the closed form equations. If this number is larger than or 
		//equal to goal then we find the actual answer with closed form equation 1, if it's smaller than the goal then we use
		//closed form equation 2
		unsigned long long two_m_tilings = 0;

		//To avoid overflow, divide by two before multiplying to figure out triangle(n - m). Since in this case n = 2m
		//both of the (n-m) terms in the equation simpaly transform into (2m - m) = m
		if ((unsigned long long)m % 2 == 0) two_m_tilings = m / 2 * (m + 1) + m + 2;
		else two_m_tilings = (m + 1)/2 * m + m + 2;

		if (two_m_tilings >= goal + 1) std::cout << twoThresholdSolver(m, goal + 1) << std::endl;
		else std::cout << threeThresholdSolver(m, goal + 1) << std::endl;
	}
	else
	{
		//if m is smaller than three_m_threshold it means we need to find the answer using the traditional method from the 
		//project euler version of the problem.
		long long wall_size = m + 1;
		std::vector<long long> ways(wall_size, 1);
		ways.back()++;

		while (ways.back() <= goal)
		{
			ways.push_back(1 + ways[wall_size - m - 1] + ways[wall_size - 1]);
			ways[wall_size - m] += ways[wall_size - m - 1];
			wall_size++;
		}

		//std::cout << ways.size() - 1 << std::endl;
	}
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 168
	//ran in 0.000083 seconds
}

//NOTES