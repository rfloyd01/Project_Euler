#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Singular Integer Right Triangles
/*
struct fraction
{
	//an easy struct to keep track of coprime pairs as a fraction
	int numerator;
	int denominator;
	void print()
	{
		//a printing function for an easy way to view what the current coprime pair is
		std::cout << numerator << '/' << denominator << std::endl;
	}
};
*/

void generateFaraySequence(int faray_limit, int perimeter_limit, int* triangles, fraction low = { 0, 1 }, fraction high = { 1, 1 }, bool new_pair = false)
{
	if (new_pair) //if the low fraction is a new coprime pair
	{
		if (!((low.numerator % 2 == 1) && (low.denominator % 2 == 1))) //primitive pythagorean triples are only produced when both parts of the coprime pair aren't odd
		{
			int perimeter = 2 * low.denominator * (low.denominator + low.numerator);
			for (int i = perimeter; i <= perimeter_limit; i += perimeter) *(triangles + i) += 1;
		}
	}

	if (low.denominator + high.denominator <= faray_limit)
	{
		//go forwards in Faray sequence
		generateFaraySequence(faray_limit, perimeter_limit, triangles, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true);

		//go backwards in Faray sequence
		generateFaraySequence(faray_limit, perimeter_limit, triangles, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false);
	}
}

std::pair<std::string, double> q75()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	const int limit = 1500000;
	int faray_limit;
	int* triangles = new int[limit + 1];
	for (int i = 0; i <= limit; i++) triangles[i] = 0; //initialize triangle array to all zeros
	faray_limit = (-1 + sqrt(1 + 4 * limit / 2)) / 2; //maximum denominator for Faray Sequence calculated using quadratic formula

	generateFaraySequence(faray_limit, limit, triangles);
	for (int i = 0; i <= limit; i++)
		if (triangles[i] == 1) answer++;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 161667
	//ran in 0.0086603 seconds
}

//NOTES
//We've already encountered a problem where Pythagorean triplets were generated (#9), but this time we're interested in more than just the base cases. Generate all of
//the possible cases below 1,500,000 by multiplying the base cases by k = 2 through 1,500,000 / current_perimeter. With a little knowledge of the Faray sequnce from 
//earlier problems it should be pretty easy to generate the base cases by looking at coprime pairs. When n == 1 the largest m you can have and keep the perimeter of the
//triangle less than or equal to 1,500,000 is 865 so we need to generate all coprime pairs where the numbers are less than or equal to 865. The Faray sequence has popped
//up a few times and whenever it does I store every coprime pair in a vector. In the case of this problem and other problems done it hasn't been too bad to keep track of
//each pair, however, it is possible for the vector to get very large when the limit is big. To try and avoid this I want to come up with an algorithm where only two 
//variables are kept track of. 

//After messing around with a few ideas for a little I came up with a recursive function that can generate every fraction in a Faray sequence without needing more than
//two fractions. A possible pitfall I see with this approach is that if the limit is something very high then C++ might not be able to handle the recursion depth but for
//the purposes of this problem there shouldn't be an issue. Also, to avoid using pairs for everything I just made a struct called fraction which made coding this up a little
//bit easier.