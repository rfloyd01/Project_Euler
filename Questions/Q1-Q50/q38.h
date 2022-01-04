#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file
#include <vector>

std::vector<int> pandigitals;

//Pandigital Multiples
void numberTest(short num)
{
	//maximum number should be ~18,000 so no need to use integers
	//short has max size of ~32,000 so just use shorts
	short used_digits[9] = { 0 };
	short pandigital_length = 0;
	short current_mult = 1;
	std::string pandigital_string;

	while (pandigital_length < 9)
	{
		short product = num * current_mult++;
		std::string word = std::to_string(product);
		pandigital_string += word;
		//std::cout << product << ", " << pandigital_string << std::endl;
		while (product > 0)
		{
			short num = product % 10;
			if (num == 0) return;
			if (used_digits[num - 1]) return;
			product /= 10;
			used_digits[num - 1] = 1;
			pandigital_length++;
		}
	}
	//std::cout << num << " results in a pandigital " << pandigital_string << std::endl;
	pandigitals.push_back(std::stoi(pandigital_string));
}

std::pair<std::string, double> q38()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	for (short i = 1; i < 10; i++) numberTest(i); //don't bother optimizing for 1 digit numbers
	for (short i = 25; i < 34; i++) numberTest(i); //test only pertinent 2 digit numbers (nothing else will result in 9 digits for pandigital
	for (short i = 100; i < 334; i++) numberTest(i); //test only pertinent 3 digit numbers (nothing else will result in 9 digits for pandigital
	for (short i = 5000; i < 10000; i++) numberTest(i); //test only pertinent 4 digit numbers (nothing else will result in 9 digits for pandigital

	for (int i = 0; i < pandigitals.size(); i++)
		if (pandigitals[i] > answer) answer = pandigitals[i]; //loop through all found pandigitals to see which is the biggest

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