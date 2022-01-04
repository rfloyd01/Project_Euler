#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes PrimeFactors() and <vector>
#include <Header_Files/print.h> //includes vprint()

//Digit cancelling fractions
std::pair<int, int> reduceFraction(std::vector<int> numerator, std::vector<int> denominator)
{
	int num = 1, den = 1;
	for (int i = 0; i < denominator.size(); i++)
	{
		for (int j = 0; j < numerator.size(); j++)
		{
			if (numerator[j] == denominator[i])
			{
				numerator[j] = 1;
				denominator[i] = 1;
				break;
			}
		}
	}

	for (int i = 0; i < denominator.size(); i++) den *= denominator[i];
	for (int i = 0; i < numerator.size(); i++) num *= numerator[i];

	return { num, den };
}

std::pair<std::string, double> q33()
{
	auto run_time = std::chrono::steady_clock::now();
	std::vector<std::vector<int> > prime_factorizations = { {0}, {1} };
	int num, den, nnum, dden;
	int final_numerator = 1, final_denominator = 1;

	for (int i = 2; i < 100; i++) prime_factorizations.push_back(PrimeFactors(i));

	for (int i = 11; i < 100; i++)
	{
		if (i % 10 == 0) continue;
		for (int j = i + 1; j < 100; j++)
		{
			if (j % 10 == 0) continue;
			bool overlapping_digit = false;

			if (i % 10 == j % 10)
			{
				overlapping_digit = 1;
				nnum = i / 10;
				dden = j / 10;
			}
			else if (i % 10 == j / 10)
			{
				overlapping_digit = 1;
				nnum = i / 10;
				dden = j % 10;
			}
			else if (i / 10 == j % 10)
			{
				overlapping_digit = 1;
				nnum = i % 10;
				dden = j / 10;
			}
			else if (i / 10 == j / 10)
			{
				overlapping_digit = 1;
				nnum = i % 10;
				dden = j % 10;
			}

			if (overlapping_digit)
			{
				std::pair<int, int> original_fraction = reduceFraction(prime_factorizations[i], prime_factorizations[j]);
				std::pair<int, int> new_fraction = reduceFraction(prime_factorizations[nnum], prime_factorizations[dden]);

				if ((original_fraction.first == new_fraction.first) & (original_fraction.second == new_fraction.second))
				{
					//uncomment this code to see the actual fractions
					//std::cout << i << '/' << j << std::endl;
					//std::cout << nnum << '/' << dden << std::endl;
					//std::cout << std::endl;
					final_numerator *= i;
					final_denominator *= j;
				}
			}
		}
	}

	std::pair<int, int> answer = reduceFraction(PrimeFactors(final_numerator), PrimeFactors(final_denominator));
	return { std::to_string(answer.second), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 100
	//ran in 0.0010425 seconds
}

//NOTES
//Really wanted to avoid any potential floating point division errors (i.e. 1/2 = .500012 instead of .5) so turned
//every number into a vector composed of it's prime factors. There should only be around 5000 numerator /denominator
//combinations that need to be checked against eachother so brute force checking of each number possibility shouldn't
//be an issue. Any number with a zero in it is skipped so that trivial solutions are ignored.
