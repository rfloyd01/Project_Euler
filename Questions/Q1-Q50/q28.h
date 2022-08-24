#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Number spiral diagonals
std::pair<std::string, double> q28()
{
	auto run_time = std::chrono::steady_clock::now();
	

	int n = 500;
	int answer = 4 * n * (((2 * n + 2) * (2 * n + 1) / 3) + ((n + 1) / 2.0) + 1) + 1;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 669171001
	//ran in .00000067 seconds
}

//NOTES
//When looking at a 3x3 square you can see that each corner is only two numbers away from the corner before it. When looking at a
//5x5 square you can see that every corner is 4 numbers away from the corner before it, and so the pattern continues that in an 
//nxn square the corners will always be (n - 1) numbers away from eachother. In the above code, loop one designates the width of
//the square (which will always go up by two when created in the pattern the problem talks about) and loop two is from 1 to 4 to
//designate the numbers in each corner.

/*
Original Solution

int answer = 1;

	for (int N = 3; N <= 1001; N += 2)
	{
		int cornerValue = N * N;
		for (int j = 0; j < 4; j++)
		{
			answer += cornerValue;
			cornerValue -= (N - 1);
		}
	}
*/

/*
HackerRank Solution
long long N = 500, mod = 1000000007;
long long first = ModMult(ModMult((long long)2, N, mod) + 2, ModMult((long long)2, N, mod) + 1, mod);

first = ModMult((long long)2, first, mod);

long long second = ModMult((long long)3, N + 1, mod);

second = (first + second) % mod + 6;
first = ModMult((long long)4, N, mod);

long long answer = ((ModMult(first, second, mod) + 6) * ModularMultiplicativeInverse((long long)6, mod)) % mod;
*/