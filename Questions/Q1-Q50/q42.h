#pragma once

#include <Header_Files/pch.h>
#include <Questions/Q1-Q50/q22.h> //includes CapitalLetterValue()
#include <vector>
#include <fstream>

//Coded Triangle Numbers
std::pair<std::string, double> q42()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//create a list of the first 1000 numbers and initialize everything to 0
	//iterate through and turn every triangle number into a 1
	bool triangle_numbers[1001] = { 0 };

	int current_number = 1, sum = 1;
	while (sum <= 1000)
	{
		triangle_numbers[sum] = true;
		current_number++;
		sum += current_number;
	}
	char letter = 'a'; //just use a random character to initialize the variable

	//first step is to copy the words
	std::ifstream inFile;
	inFile.open("Resources/q42.txt");

	//read all of the names from the text file into the words vector alphabetically
	//wasn't sure how to break the loop when EOF is reached so I just put an '@' character at the very end of the file to signify when to stop
	while (letter != '@')
	{
		inFile >> letter;
		if (letter == '"')
		{
			//signifies the start of a new name
			int word_sum = 0;
			inFile >> letter;
			while (letter != '"') //record letters until the next '"' is found
			{
				word_sum += (letter + 1 - 'A');
				inFile >> letter;
			}

			if (triangle_numbers[word_sum]) answer++;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 162
	//ran in 0.000717 seconds
}

//NOTES
//Was able to reuse some code from question #22 which also dealt with giving numeric values to individual letters. Instead of writing a function to test whether or not a number is a 
//triangular number, I just created an array of the numbers 0 - 1000 where the value is 1 on all triangular numbers and 0 on non-triangular numbers. I figured that none of the word scores
//would be over 1000 as that would require at least 39 letters (all of which are Z). This took all of 5 minutes to write up.

/*
1/5/22 Update
Replaced a function called CapitalLetterValue() with (letter + 1 - 'A') on line 43
*/