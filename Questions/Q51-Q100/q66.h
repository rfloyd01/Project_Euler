#pragma once

#include <Header_Files/pch.h>
#include <cmath>
#include <vector>
#include <Header_Files/bigint.h>
#include <Header_Files/print.h>
#include <map>

//Diophantine Equation
std::pair<std::string, double> q66()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int* square_root_integers = new int[1001];
	square_root_integers[0] = 0;
	long long triple[3] = { 0 };

	for (int i = 1; i < (int)sqrt(1000); i++)
		for (int j = i * i; j < ((i + 1) * (i + 1)); j++) square_root_integers[j] = i;

	for (int i = 31 * 31; i < 1001; i++) square_root_integers[i] = 31;
	bigint Maximum(0); //this variable will be used to keep track of the largest x value

	for (int n = 2; n < 1001; n++)
	{
		long long n_root = square_root_integers[n];
		if (n == (n_root * n_root)) continue; //skip numbers that are perfect squares

		std::vector<int> repeating_fraction;

		triple[0] = n_root;
		triple[1] = 0;
		triple[2] = 1;

		while (true)
		{
			repeating_fraction.push_back(triple[0]); //build repeating fraction one element at a time

			//check to see if current iteration of repeating fraction yields an answer
			bigint numerator(1), denominator(repeating_fraction.back()), temp, D(n);
			for (int i = repeating_fraction.size() - 2; i >= 0; i--)
			{
				numerator += denominator * repeating_fraction[i];
				temp = denominator;
				denominator = numerator;
				numerator = temp;
			}

			if ((denominator * denominator - D * numerator * numerator) == 1)
			{
				if (denominator > Maximum)
				{
					Maximum = denominator;
					answer = n;
				}
				break;
			}

			//if answer wasn't found then go to the next level of the repeated fraction
			triple[1] = triple[0] * triple[2] - triple[1];
			triple[2] = (n - (triple[1] * triple[1])) / triple[2];
			triple[0] = (n_root + triple[1]) / triple[2];
		}
	}

	/*
	for (int n = 2; n < 1001; n++)
	{
		long long n_root = square_root_integers[n];
		if (n == (n_root * n_root)) continue; //skip numbers that are perfect squares

		std::map<std::vector<long long>, bool> used_triplets;
		std::vector<long long> triple;
		std::vector<bigint> A; //use a bigint vector for later math

		//numerator and denominator are used to iteratively calculate the current convergent of sqrt(n)
		bigint numerator[3], denominator[3], current_iteration(0), D(n);
		numerator[0] = 0; numerator[1] = 1; numerator[2] = n_root;
		denominator[0] = 1; denominator[1] = 0; denominator[2] = 1;

		triple.push_back(n_root);
		triple.push_back(0);
		triple.push_back(1);

		used_triplets[triple] = true;
		bool cont = true;

		while (true)
		{
			//check to see if answer to diophantine is found before finding full continued fraction
			if (numerator[(current_iteration + 2) % 3] * numerator[(current_iteration + 2) % 3] - D * denominator[(current_iteration + 2) % 3] * denominator[(current_iteration + 2) % 3] == 1)
			{
				if (numerator[(current_iteration + 2) % 3] > Maximum)
				{
					Maximum = numerator[(current_iteration + 2) % 3];
					answer = n;
				}
				cont = false;
				break;
			}

			//if no answer to the diophantine equation is found then keep expanding out the continued fraction
			triple[1] = triple[0] * triple[2] - triple[1];
			triple[2] = (n - (triple[1] * triple[1])) / triple[2];
			triple[0] = (n_root + triple[1]) / triple[2];

			bigint An(triple[0]);

			numerator[current_iteration % 3] = An * numerator[(current_iteration + 2) % 3] + numerator[(current_iteration + 1) % 3];
			denominator[current_iteration % 3] = An * denominator[(current_iteration + 2) % 3] + denominator[(current_iteration + 1) % 3];
			current_iteration += 1;

			if (used_triplets[triple] == true) break;
			used_triplets[triple] = true;
			A.push_back(An);
		}

		if (!cont) continue; //go to next number if solution for current number has already been found

		//once the continued fraction representation has been found, iteratively calculated the nth convergent until an answer is found
		while (true)
		{
			if (numerator[(current_iteration + 2) % 3] * numerator[(current_iteration + 2) % 3] - D * denominator[(current_iteration + 2) % 3] * denominator[(current_iteration + 2) % 3] == 1)
			{
				if (numerator[(current_iteration + 2) % 3] > Maximum)
				{
					Maximum = numerator[(current_iteration + 2) % 3];
					answer = n;
				}
				break;
			}
			numerator[current_iteration % 3] = A[current_iteration % A.size()] * numerator[(current_iteration + 2) % 3] + numerator[(current_iteration + 1) % 3];
			denominator[current_iteration % 3] = A[current_iteration % A.size()] * denominator[(current_iteration + 2) % 3] + denominator[(current_iteration + 1) % 3];
			current_iteration += 1;
		}
	}
	*/

	delete[] square_root_integers;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 661
	//ran in 0.0758241 seconds
}

//NOTES
//I know from experience that working with Diophantine equations is a pain in the ass so I elected to try and find some existing info on this particular one.
//A little research shows that x^2 - Dy^2 = 1 is what's known as Pell's equation and the base solution for any positive integer of D (other than when D is a 
//perfect square) can be generated by looking at succesive elements for the repeated fraction representation of sqrt(D). This seems fitting as the last few
//questions were also about repeating fractions. Essentially all that needs to be done here is tweak some of the repeated fraction code from the last few problems
//and use it to expand a repeated fraction until we find our first solution to x^2 - Dy^2 = 1. Do this for all non-square numbers less than 1000 and the answer
//should be found pretty quickly. The only snag I see with this approach is that each iteration of the repeated fraction needs to be calculated from the bottom up,
//so if hypothetically the first solution isn't obtained until the 100th iteration then 100 * 101 / 2 = 5,050 divisions would need to be done to find the base solution
//for a single number. If there was a better way to iteratively get the next fraction in a repeated fraction sequence (i.e. a top down approach) the code would be much
//more efficient

//The original code runs well enough, however, I know it would be a lot faster if I had an iterative approach to calculating the nth convergent for sqrt(n). Did
//a little reading on the Wikipedia page for continued fractions and found an iterative solution to find successive convergents for a given continued fraction of the form
//[a[0], a[1], a[2], a[3] ... a[n]]. The convergent (h[n] / k[n]) is given by: (a[n] * h[n-1] + h[k-2]) / (a[n] * k[n-1] + k[n-2]) which is actually a pretty straightforward
//formula. So all that's needed to do is to calculate the repeated a[n] represenatation in it's entirety and then just keep iterating over each element until the first
//solution to Pell's equation is found. Dissapointingly enough, after taking a decent amount of time to code this iterative implementation the program run time actually
//increased from 0.0758 seconds to 0.997 seconds. Since the iterative solution requires the entirety of the repeating sequence for the repeated fraction to be found, for 
//any number that has a very long sequence but has a very early convergent that solves the Diophantine equation, there are a lot of unnecessary calculations that happen.

//A third approach is to combine 1 and 2, i.e. use the iterative approach to finding each convergent but do it at each stage of the calculation for the next A in the overall
//repeated fraction sequence. If the answer to the Diophantine equation is found before the full repeating fraction sequence is found then great. If not then continue approach
//3 in the same manner is approach 2 and just keep cycling through the repeating digits iteratively until the answer is found. Annoyingly enough, this method ended up taking
//even more time than the second approach (by.008 seconds) It seems that enough of the Diophantine answers are small enough that an iterative approach isn't necessary and ends
//up slowing down the overall problem. In a perfect world I think the iterative approach should still be faster, however, since it requires the use of a map of vectors to known
//when the continued fraction is complete and uses the bigint class which inherently is slower than c++ built in types it just doesn't work out. I've decided to keep the original
//algorithm but have left version three commented in place just as a reference.