#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file
#include <Functions/functions.h> //includes powers_of_ten[]
#include <vector>

//Pandigital Multiples
void numberTest(short num, int& answer)
{
	int concatenated_product = num, used = 0, current_mult = 2;

	while (concatenated_product < 100000000)
	{
		int prod = num * current_mult++;
		concatenated_product = concatenated_product * powers_of_ten[(int)log10(prod) + 1] + prod;
	}

	int copy = concatenated_product;
	while (copy != 0)
	{
		int dig = copy % 10;
		
		if (!dig) return; //can't have any 0's
		if (used & powers_of_two[dig]) return; //not pandigital

		copy /= 10;
		used |= powers_of_two[dig];
	}
	if (concatenated_product > answer) answer = concatenated_product;
}

std::pair<std::string, double> q38()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//only test numbers starting with 9 that will result in a 
	//9-digit concatenated product
	//numberTest(9, answer);
	for (short i = 2; i < 10000; i++) numberTest(i, answer);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 932718654
	//ran in 0.0002079 seconds
}

//NOTES
//Considering the original number needs to be multiplied by 1, we know that the base number can't have any repeat digits. As far as a limit goes, I think 4 digits would be the maximum
//test number as 5 digits would results in a minimum of 10 digits for the pandigital. As for 1 - 4 digit numbers there should be a suitable test range. 4 digit numbers alone can't make a 9 digit
//pandigital as 4 + 4 = 8 and 4 + 4 + 4 = 12. Need a 4 digit large enough that when multiplied by 2 it gives a 5 digit number, i.e., 5000 is the minimal 4 digit number to test. 3 digit numbers
//have the opposite problem. As soon as the three digit number gets too big it will turn into a 4 digit when multiplied by 3. This limits the test range between 100 and 333. Between all 1-4 digit 
//testable numbers there shouldn't be more than 5,300 test numbers so a brute force approach should be able to run in the blink of an eye. Even testing all numbers less than 10,000 should run in 
//the blink of an eye but will add these speedups just for completeness sake. Added a line in the number test function that quits as soon as it sees a zero as we're only looking for a 1-9
//pandigital