#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h> //includes coprime

#include <iostream>
#include <cmath>
#include <vector>

//Pentagonal Puzzle

int isPentagonal(long long n)
{
	//returns -1 for non-pentagonal numbers
	double en = (1 + sqrt(24 * n + 1)) / 6;
	if (en - (int)en != 0) return -1;
	return (int)en - 1; //convert to 0 index for array
}

std::pair<std::string, double> q799()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	int totalNumbers = 100000;

	long long* pentagonalNumbers = new long long[totalNumbers]();
	int* combinations = new int[totalNumbers]();
	long long current = 0, adder = -2;

	for (int i = 0; i < totalNumbers; i++) pentagonalNumbers[i] = (current += (adder += 3));

	//long long maxPentagonal = pentagonalNumbers[totalNumbers - 1];

	for (int i = 0; i < totalNumbers; i++)
	{
		for (int j = i + 1; j < totalNumbers - 1; j++)
		{
			long long add = pentagonalNumbers[i] + pentagonalNumbers[j];
			
			if ((add < pentagonalNumbers[j + 1])) break; //pentagonal[i] will never be high enough to reach other pentagonal numbers

			int pent = isPentagonal(pentagonalNumbers[i] + pentagonalNumbers[j]);
			if (pent >= totalNumbers) break;

			if (pent != -1)
			{
				combinations[pent]++;
				/*if (combinations[pent] == 16)
				{
					std::cout << pent << std::endl;
					std::cout << pentagonalNumbers[pent] << std::endl;
				}*/
				if (pent == 98573) std::cout << "P" << j << " + P" << i << " = P98573" << std::endl;
			}
		}
	}

	delete[] pentagonalNumbers;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES