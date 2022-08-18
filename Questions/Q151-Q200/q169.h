#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/int_64x.h>
#include <Header_Files/functions.h>

//Exploring the number of different ways a number can be expressed as a sum of powers of 2
std::pair<std::string, double> q169()
{
	auto run_time = std::chrono::steady_clock::now();

	//first get 10^25 in binary form, int_64x does this automatically
	int_64x goal = BigPow(10, 25);

	long long dynamicArray[2] = { 1, 0 };
	int leadWord = goal.digits.size() - 1;

	//if the number is odd, right shift it until it's even, trailing 1's have no bearing
	//on the final number of equations
	while (goal.digits[0] & 1) goal >>= 1;

	//next, find the number of trailing zero's as this will be lost during the 
	//below left shifting operations
	int finalZeros = 0;
	while (!(goal.digits[0] & 1))
	{
		goal >>= 1;
		finalZeros++;
	}

	//now, keep left shifting the number until the lead bit is a 1
	while (!(goal.digits[leadWord] & 0x8000000000000000)) goal <<= 1;

	while (goal.digits[leadWord])
	{
		//we keep left shifting our number until we've created a new binary group
		int numberOnes = 0, numberZeros = 0;
		bool zerosStarted = false;

		while (goal.digits[leadWord])
		{
			if (goal.digits[leadWord] & 0x8000000000000000)
			{
				if (zerosStarted) break;
				else numberOnes++;
				
			}
			else
			{
				zerosStarted = true;
				numberZeros++;
			}
			goal <<= 1;
		}

		//add final zeros if necessary
		if (!(goal.digits[leadWord])) numberZeros += finalZeros;
		
		//Now that we have the next binary group we can dynamically build next iteration of array
		long long temp = dynamicArray[0];
		dynamicArray[0] = temp * (1 + numberOnes * numberZeros) + dynamicArray[1] * numberZeros;
		dynamicArray[1] = temp * numberOnes + dynamicArray[1];
	}
	long long answer = dynamicArray[0];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 178653872807
	//ran in 0.0000306 seconds
}

//NOTES
/*
* I'm not sure what I did to originally solve this problem back in 2015, but this is just an easier version of problem 175. Use the algorithm I came up
* with to calculate this based on the binary representation of the number.
*/
