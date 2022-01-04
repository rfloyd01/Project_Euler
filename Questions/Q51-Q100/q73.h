#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Counting Fractions in a Range
void generateFaraySequence(int &ans, fraction low = { 1, 3 }, fraction high = { 1, 2 }, bool new_pair = false)
{
	if (new_pair) ans++;

	if (low.denominator + high.denominator <= 12000)
	{
		generateFaraySequence(ans, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true);
		generateFaraySequence(ans, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false);
	}
}

std::pair<std::string, double> q73()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	generateFaraySequence(answer);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7295372
	//ran in 0.0580458 seconds
}

//NOTES
//This problem should be nothing more than doing a Faray sequence with a limit of 12,000 where the
//starting values or 1/2 and 1/3 instead of 0/1 and 1/1. This yields the correct answer in about
//half a second, which I'm not really that thrilled with. The fact that I need to hold each fraction
//in a vector really slows things down.

//Another thought that I had was to just count the total number of fractions between 0 and 1 with a
//denominator of 12,000 or less using the totient sum method from problem 72. Whatever that number is,
//just divide it by 6 as 1/2 - 1/3 = 1/6 (in other words we only care about 1/6 of the total number
//of fractions). After doing that subtract 2 because the fractions 1/2 and 1/3 shouldn't be included in
//the final tally. Doing this yielded an answer of 7,295,374 which remarkably is only 2 away from the 
//correct answer of 7,295,372 found using the Faray sequence method. Obviously we care about the exact
//number, but if for whatever reason we only needed an approximtion then this would be a really quick
//way to do it. It runs 832 times faster than the Faray sequence and has an error of only 0.0000274%!
//I'll need to apply some more thought to this at some point to try and get the exact answer and use
//it to vastly improve the run time of this problem.

//UPDATE
//After solving problem 75 I found a recursive way to produce the Faray sequence that doesn't require
//holding any values in a vector. Using this new algorithm offered an ~8.5x speedup from the vector
//method and doesn't require nearly as much space.