#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes PrimeFactors() and <vector>
#include <Header_Files/print.h> //includes vprint()
#include <map>

//Digit cancelling fractions
std::pair<std::string, double> q33()
{
	auto run_time = std::chrono::steady_clock::now();
	
	fraction final_fraction = { 1, 1 };
	std::vector<fraction> coprime_pairs;
	FaraySequence(9, coprime_pairs); //generate coprime pairs with 1-9

	for (int i = 0; i < coprime_pairs.size(); i++)
	{
		int k = 0;
		while (true)
		{
			k++;
			if (coprime_pairs[i].numerator * k > 9 || coprime_pairs[i].denominator * k > 9) break;

			//Only one form will give us distinct working answers X#/#X. Plug in all values of 
			//k * coprime pair to find the ones that work
			if ((9 * k * coprime_pairs[i].numerator * coprime_pairs[i].denominator) % (10 * coprime_pairs[i].numerator - coprime_pairs[i].denominator) == 0)
			{
				int x = (9 * k * coprime_pairs[i].numerator * coprime_pairs[i].denominator) / (10 * coprime_pairs[i].numerator - coprime_pairs[i].denominator);
				if (x > 0 && x < 10)
				{
					final_fraction.numerator *= 10 * coprime_pairs[i].numerator * k + x;
					final_fraction.denominator *= 10 * x + coprime_pairs[i].denominator * k;
				}
			}
		}
	}

	int answer = final_fraction.denominator / gcd(final_fraction.numerator, final_fraction.denominator);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 100
	//ran in 0.0000182 seconds
}

//NOTES
//Really wanted to avoid any potential floating point division errors (i.e. 1/2 = .500012 instead of .5) so turned
//every number into a vector composed of it's prime factors. There should only be around 5000 numerator /denominator
//combinations that need to be checked against eachother so brute force checking of each number possibility shouldn't
//be an issue. Any number with a zero in it is skipped so that trivial solutions are ignored.

//HACKERRANK UPDATE
//Lots of changes were made for the HackerRank version of the problem. The code now accomdates numbers that are either 2, 3 or 4 digits where we can remove anywhere
//from 1 to number_length - 1 digits from the number.