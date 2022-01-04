#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes []powers_of_two, []powers_of_ten
#include <fstream>

//Passcode Derivation
std::pair<std::string, double> q79()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int passcodes[50];
	char num;
	std::ifstream inFile;
	inFile.open("Resources/q79.txt");

	//first save passcodes into an array
	for (int i = 0; i < 50; i++)
	{
		passcodes[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			inFile >> num;
			passcodes[i] *= 10;
			passcodes[i] += num - 48; //the character '0' has an ASCII value of 46
		}
	}

	//iterate through the digits 0-9 to see how many distinct numbers come before each one throughout the passcodes
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i== 5) continue; //we know from looking at the passcodes that there aren't any 4's or 5's so skip these numbers
		int used = 0, count = 0;

		for (int j = 0; j < 50; j++) //iterate through each passcode
		{
			if (passcodes[j] / 100 == i) continue; //nothing comes before digit in this passcode because it's the first digit
			if (passcodes[j] / 10 % 10 == i) //second digit is 'i', therefore the first digit in sequence comes before it in final password
			{
				if (!(used & powers_of_two[passcodes[j] / 100])) count++; //add to distinct count if number hasn't been encountered yet
				used |= powers_of_two[passcodes[j] / 100]; //keep track of seen numbers in the binary number 'used'
				continue;
			}
			if (passcodes[j] % 10 == i) //third digit is 'i', therefore the first two digits in sequence come before it in final password
			{
				if (!(used & powers_of_two[passcodes[j] / 100])) count++;
				if (!(used & powers_of_two[passcodes[j] / 10 % 10])) count++;
				used |= powers_of_two[passcodes[j] / 100]; //add first digit
				used |= powers_of_two[passcodes[j] / 10 % 10]; //add second digit
			}
		}

		answer += i * powers_of_ten[7 - count]; //there are 7 possible digits other than the number being tested so subtract final count from 7 and raise ten to that power
	}
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 73162890
	//ran in 0.0001959 seconds
}

//NOTES
//A quick glance at the codes in the text file and you can see two big things. First, none of the codes feature the same digit twice which means that the final
//password shouldn't have any repeat digits. Second, the digits 4 and 5 don't appear in any of these attempts so there's no need to include them in the final
//password. Using these two facts it can be assumed that the password is an 8 digit number with the digits 0, 1, 2, 3, 6, 7, 8 and 9 in some order. The fact that
//each digit only appears once will make this a lot easier, just go through each of the passcodes and see which digit comes first by figuring out which one digit
//has nothing that comes before it. Then go through the codes again and see which digit has nothing come before it other than the first digit. Keep repeating this
//process until the final password is done. It's possible that you won't be able to determine the order exactly in this manner, for example, if the first and
//second digit ever appear in the same passcode then there won't be a way to know which one is truly the first digit. Considering there are 8 digits in the final
//password there are choose(8, 3) = 56 ways to select three digits in order and 50 passcodes are supplied. It looks like there are a few repeat codes but there
//should still be enough information to solve the problem. My logic was good enough to solve the problem in under a millisecond although I'm curious to see if it
//would scale well for a longer password or how it would hold up to a password with repeat digits.