#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file
#include <Functions/functions.h> //includes primeNumberTest(), []powers_of_two

//Pandigital Prime
void checkAllPandigitals(int number_of_digits, long long& answer, int current_level, long long current_number, uint16_t used_numbers, const int *two_powers, bool &cont)
{
	//checks to see if any pandigital primes exist for the given number of digits. This problem asks for the largest pandigital so we start with the largest
	//number and break out of the function as soon as an answer is found
	//need to feed this function with current_level = 0, current_number = 0, used_numbers = 0 and cont = true

	if (current_level == number_of_digits)
	{
		if (primeNumberTest(current_number))
		{
			answer = current_number;
			cont = false;
		}
		return;
	}
	for (int i = number_of_digits; i > 0; i--) //a 9 digit pandigital should check numbers 1-9, a 7 digit should check 1 -7, etc.
	{
		if (!cont) return; //break out of function if the answer has been found
		if (used_numbers & *(two_powers + i)) continue; //if the number has already been used then skip it
		if (current_level == (number_of_digits - 1))
		{
			if (i % 2 == 0 || i == 5) return; //don't use a number in the one's place that guarantees a non-prime number
		}
		checkAllPandigitals(number_of_digits, answer, current_level + 1, current_number * 10 + i, used_numbers | *(two_powers + i), two_powers, cont);
	}
}

std::pair<std::string, double> q41()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	bool cont = true;

	for (int i = 7; i > 0; i--)
	{
		checkAllPandigitals(i, answer, 0, 0, 0, powers_of_two, cont);
		if (answer > 0) break;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7652413
	//ran in 0.0000074 seconds
}

//NOTES
//The maximum size of a pandigital number per the definition of the probnlem is 9 digits because 0 isn't included. Starting with 9 digit numbers, there are 9! (362,880) pandigitals
//to check and see if they are prime. A lot of this numbers won't actually need a prime test run on them, however, because anything that ends with an even digit or a 5 can be instantly
//eliminated from contention. Only checking numbers that end in 1, 3, 7 or 9 cuts the total numbers to run a prime test on by more than half. If there aern't any 9 digit pandigital 
//primes then check 8 digits. Then 7, etc, etc. After completing the problem I can see the answer is a 7 digit pandigital. If I only check 7 digit numbers and skip 8 and 9 digit
//numbers then the code only takes 0.0004768 seconds to run. I feel like there isn't really a good way to know that without actually running the program so it doesn't seem fair to start
//my search loop at 7 instead of 9. For that reason I'm keeping the start of the search loop at 9 digits.

//HACKERRANK UPDATE
/*
* I realized that it would be much more efficient to remove the check at the bottom of the recursion to see if our number ends in 2, 4, 5, 6 or 8 and instead place it at the top
* of the recursion (i.e. just don't build any numbers where these are the first digit). Doing this prevents us from creating thousands of numbers and ends up saving a bunch of time.
* This simple change reduced the runtime from 0.292167 seconds to 0.0457186 seconds. I then realized that testing 8 and 9 digit pandigitals isn't necessary as the sum of these 
* numbers are 36 and 45 respectively, meaning they're all divisible by 3. If only testing 7-digit numbers then the runtime drops to a mere 0.0000074 seconds
*/