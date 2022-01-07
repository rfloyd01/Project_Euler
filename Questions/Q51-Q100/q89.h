#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <sstream>
#include <string>

//Roman numerals
std::pair<std::string, double> q89()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//create an array that holds the values of the roman numerals
	int numeralValues[26] = { 0 };
	numeralValues['I' - 'A'] = 1;
	numeralValues['V' - 'A'] = 5;
	numeralValues['X' - 'A'] = 10;
	numeralValues['L' - 'A'] = 50;
	numeralValues['C' - 'A'] = 100;
	numeralValues['D' - 'A'] = 500;
	numeralValues['M' - 'A'] = 1000;

	//number of Roman Numerals needed to most efficiently make digits 1-9
	//this value is the same for the ones, tens and hundred place of a decimal number
	int mostEfficient[10] = { 0, 1, 2, 3, 2, 1, 2, 3, 4, 2 };

	//import numbers from text file
	std::ifstream myFile;
	myFile.open("Resources/q89.txt");

	std::string line;
	int line_total, current_min, current_letter;

	while (std::getline(myFile, line))
	{
		//reverse the string for easy checking of when to subtract a digit instead of add it
		std::reverse(line.begin(), line.end());
		line_total = 0, current_min = numeralValues[line[0] - 'A'];

		for (char c : line)
		{
			current_letter = numeralValues[c - 'A'];
			if (current_letter < current_min) line_total -= current_letter;
			else
			{
				line_total += current_letter;
				current_min = current_letter;
			}
		}

		//check the ones, tens and hundred place one digit at a time
		int bestWay = 0;
		for (int i = 0; i < 3; i++)
		{
			bestWay += mostEfficient[line_total % 10];
			line_total /= 10; //remove the last decimal digit
		}

		//line_total should only contain the thousands place at this point so just add this to 
		//the total total as there's no shortcuts for reducing the number of M's in a Roman Numeral
		bestWay += line_total;
		answer += (line.length() - bestWay); //add the differences in lengths
	}

	myFile.close(); //make sure to close the file
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 743
	//ran in 0.0004566 seconds
}

//NOTES
/*
Per the rules, there are only 7 different kinds of Roman numerals: I, V, X, L, C, D and M, which all happen to be capital letters. Instead of wasting
the time to convert these roman numerals to numeric values over and over again I'm just going to create an array with a size of 26 to represent the
alphabet and store the value of the roman numerals at the correct location. For example, C is the 3rd letter of the alphabet so the
third value of this array will be 100, the Roman Numeral value of C. All of the unused letters will be left with a value of 0.

Since we know that all of the Roman numbers in the text file are valid (per the question text), my plan is to just convert them all into decimal
representations and then "rebuild" them based off of this decimal value. I say "rebuild" in quotes because I'm not actually going to remake any strings, I'm 
just going to keep track of how many Roman Numerals the number "would" have if built in the most efficient manner.

After a decimal number is figured out from the roman numerals, we can calculate the "most efficient" way to re-construct it by going one decimal 
column at a time, mainly the ones, tens and hundreds place. The numerals I and V handle the ones place, X and L handle the tens place and
C and D handle the hundreds place. The patterns for the most efficient way to make any digit is the same regardless of which column we're looking at
as shown below
a certain digit based on which decimal column it's in. For example, in the one's column here are the most efficient ways to make each number:

[0, I, II, III, IV, V, VI, VII, VIII, IX]
[0, X, XX, XXX, XL, L, LX, LXX, LXXX, XC]
[0, C, CC, CCC, CD, D, DC, DCC, DCCC, CM]

For the ones, tens and hundred place you can see that the most efficient way to make each digit can be described by this array:
[0, 1, 2, 3, 2, 1, 2, 3, 4, 2]

By using modular division we can look at our decimal number one digit at a time and figure out the most efficient way to rebuild the Roman numeral
by looking at the above array. Anything after the hundreds place is handled by just adding M's and there isn't a
more efficient way to do it.

Overall this ends up running in under a millisecond. I think the algorithm is pretty darn good, reading the actual numbers from the text file is what
takes the longest here so I don't really see myself as being to improve upon this runtime all that much.
*/