#pragma once

#include <Header_Files/pch.h>
#include <fstream>

//Large sum
std::pair<std::string, double> q13()
{
	//Same as the above code but uses my own BigNum class to do all the math on large numbers
	//Ends up running just as fast as the original function but is way easier to implement (~30 lines of code vs. ~80)

	auto run_time = std::chrono::steady_clock::now();

	long long ans = 0;

	std::ifstream inFile;
	inFile.open("C:/Users/Bobby/Documents/Coding/C++/Project_Euler/Resources/q13.txt");
	char num; //variable used for reading characters

	//first need to read all numbers from text file
	for (int i = 0; i < 100; i++)
	{
		long long current_number = 0;
		for (int j = 0; j < 12; j++) //only read the first 12 digits of each number
		{
			inFile >> num;
			current_number *= 10;
			current_number += (num - '0'); //convert char to int
		}
		for (int j = 0; j < 38; j++) inFile >> num; //read to end of line
		ans += current_number;
	}
	std::string answer = std::to_string(ans); //convert int to string
	answer.erase(answer.begin() + 10, answer.end()); //remove everything other than first 10 characters

	return { answer , std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 5537376230
	//ran in 0.0003432 seconds
	//HackerRank Score = 100.00/100.00
}

//NOTES
//HACKERRANK UPDATE (11/9/2021)
/*
I came back to this problem after creating the int_64x class and surprisingly my original BigInt class outperforms it. This is due to the fact that BigInt manipulates
numbers as strings whereas int_64x keeps numbers in binary format. This makes int_64x pretty poor at creating numbers from strings, especially long numbers. The actual
arithmetic is much faster, however, it takes so long to actualy convert the numbers to binary that int_64x can't quite compete with BigInt here. Regardless, neither of
these classes is even actually needed. You can get the first 10 digits of the sum by mereley adding together the first 12 digits of all of the individual numbers. You use
the first 12 digits to ensure that you've accomodated for all of the appropriate carry over during the addition.
*/

//Old Euler Code
/*
BigNum answer(10);

	std::ifstream inFile;
	inFile.open("C:/Users/Bobby/Documents/Coding/C++/Project_Euler/Resources/q13.txt");

	//first need to read all numbers from text file
	for (int i = 0; i < 100; i++)
	{
		std::string num_str = "";
		for (int j = 0; j < 50; j++)
		{
			char num;
			inFile >> num;

			num_str += num;
		}
		answer.Add(BigNum(num_str));
	}
*/