#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>
#include <set>

//Cyclical Figurate Numbers

int two_powers[9] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };

void findAnswer(std::vector<std::vector<int> > &polygonal_numbers, std::vector<std::vector<std::pair<int, int> > > &starts_with,
	std::vector<std::vector<std::pair<int, int> > >& ends_with, int* chain, std::set<int>& answers, int final_chain_length, int useable_figurates,
	int current_end, int used_types, int level)
{
	//recursive function to comb through list of polygonal numbers to see if there's an existing chain of 6 based on the initial input starting number
	//chain is a pointer to the eventual answer, used types is an 8-bit char to keep track of which types of polygonal numbers have been used to avoid repeats,
	//it needs to start at 0, current_end is the last two digits of the current last number in the chain and is initialized from the starting number, cont is
	//a bool that causes the function to terminate when an answer is found (initialized to true) and level keeps track of the current level of the function
	//(initialized to 1).

	if (level == final_chain_length)
	{
		//end condition for the recursive function
		if ((*chain / 100) == (*(chain + (final_chain_length - 1)) % 100))
		{
			//add the sum of the current chain to the answers set. a set is used 
			//to make sure we don't count the same chain multiple times (i.e. [8128, 2882, 8281] and [2882, 8281, 8128])
			int ans = 0;
			for (int i = 0; i < final_chain_length; i++)
			{
				ans += *(chain + i);
				//std::cout << *(chain + i) << " ";
			}
			//std::cout << used_types << std::endl;
			answers.insert(ans);
		}
		
		return;
	}

	for (int num = 0; num < starts_with[current_end].size(); num++)
	{
		std::pair<int, int>* pair_pointer = &starts_with[current_end][num];
		if (!(useable_figurates & two_powers[pair_pointer->first])) continue; //can only use allowable figurate numbers
		if (used_types & two_powers[pair_pointer->first]) continue; //don't want to reuse number types
		else
		{
			//the number is allowed, however, some numbers belong to multiple figurate groups (all hexagonal numbers are also triangular 
			//for example). Make sure that the number doesn't already exist in the chain under a different figurate group.
			bool breaker = false;
			for (int i = 0; i < level; i++)
			{
				if (*(chain + i) == polygonal_numbers[pair_pointer->first][pair_pointer->second])
				{
					breaker = true;
					break; //if there's a match go to next iteration
				}
			}
			if (breaker) continue;
				
			*(chain + level) = polygonal_numbers[pair_pointer->first][pair_pointer->second];
			findAnswer(polygonal_numbers, starts_with, ends_with, chain, answers, final_chain_length, useable_figurates, *(chain + level) % 100, (used_types | two_powers[pair_pointer->first]), level + 1);
		}
	}
}

std::pair<std::string, double> q61()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	std::vector<int> dummy;
	std::vector<std::vector<int> > polygonal_numbers;
	polygonal_numbers.push_back(dummy); polygonal_numbers.push_back(dummy); polygonal_numbers.push_back(dummy); //to this so that triangle numbers are mapped to 3, square to 4, etc...

	//loop to generate polygonal numbers iteratively
	for (int i = 3; i < 9; i++)
	{
		std::vector<int> nums;
		int current_number = 1, adder = 1;
		while (current_number < 10000)
		{
			if (current_number > 999) nums.push_back(current_number); //we only care about 4 digit numbers for this problem
			adder += (i - 2);
			current_number += adder;
		}
		polygonal_numbers.push_back(nums);
	}

	std::vector<std::vector<std::pair<int, int> > > starts_with, ends_with;
	for (int i = 0; i < 100; i++)
	{
		//since we only care about the first and last two digits of any number overlapping, we only need to loop from 0 to 99
		std::vector<std::pair<int, int> > yo;
		starts_with.push_back(yo);
		ends_with.push_back(yo);
	}

	//next is to fill the start and end vectors
	for (int i = 3; i < 9; i++)
	{
		for (int j = 0; j < polygonal_numbers[i].size(); j++)
		{
			std::pair<int, int> heyhey = { i, j };
			starts_with[polygonal_numbers[i][j] / 100].push_back(heyhey);
			ends_with[polygonal_numbers[i][j] % 100].push_back(heyhey);
		}
	}

	//bool cont = true; //will keep looping as long as this variable is true
	const int chain_length = 5;
	int current_chain[chain_length] = { 0 }, useable_figurates = 0b011111000;// usable_figurates = 0b11111100;

	//const int chain_length = 6;
	//int current_chain[chain_length] = { 0 }, useable_figurates = 0b111111000;// usable_figurates = 0b11111100;

	std::set<int> answers;

	for (int i = 0; i < 100; i++) //start at 10 because anything less would have a lead 0 and not be a 4-digit number
	{
		for (int j = 0; j < ends_with[i].size(); j++)
		{
			if (!(useable_figurates & two_powers[ends_with[i][j].first])) continue; //can only use allowable figurate numbers

			current_chain[0] = polygonal_numbers[ends_with[i][j].first][ends_with[i][j].second];
			findAnswer(polygonal_numbers, starts_with, ends_with, current_chain, answers, chain_length, useable_figurates, current_chain[0] % 100, two_powers[ends_with[i][j].first], 1);
			//if (!cont) break;
		}
		//if (!cont) break;
	}

	//once the first chain is found, add up all of the links to get the answer
	//for (int i = 0; i < chain_length; i++) answer += current_chain[i];

	//sort all the sums in the answer set and then print out the sums
	for (int i : answers) answer = i;

	//uncomment the below line to see what the actual chain is
	//std::cout << "[" << current_chain[0] << ", " << current_chain[1] << ", " << current_chain[2] << ", " << current_chain[3] << ", " << current_chain[4] << ", " << current_chain[5] << "]" << std::endl;
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 28684
	//ran in 0.0001042 seconds
}

//NOTES
//I think the first order of business should be creating a vector each for the triangle, square, pentagonal ... numbers. The polygonal numbers can be generated with the
//equations given, but I think it would be even quicker to just add numbers in a series. For example, the triangle numbers can be found by adding each consecutive number in 
//a sequence starting from 1 (1 = 1 , 1+2 = 3, 1+2+3 = 6, 1+2+3+4 = 10 ...), the square numbers can be found by adding every second number in a sequence when starting from
//1 (1 = 1, 1 + 3 = 4, 1 + 3 + 5 = 9, 1 + 3 + 5 + 7 = 16 ...), the pentagonal numbers are found by adding every third number (1 = 1, 1 + 4 = 5, 1 + 4 + 7 = 12, 1 + 4 + 7 + 10
// = 22 ...) and this pattern continues for each new level of number. Unfortunately generating the numbers is by far the easiest part. If we knew the order of the cycle this
//problem would be pretty trivial (i.e. triangle, pentagonal, square, hexagonal...) but we don't know the order so multiple combinations may need to be tried. I think this 
//problem may be about efficiently organizing the data over everything else. Once the polygonal numbers are saved I think two more nested vectors should be created, one called
//starts_with and one called ends with and each of these vectors will be 100 elements long. Element 15 of the ends_with vector will be a vector that holds pairs representing
//what type of polygonal number it is that ends with 15 and the location within said polygonal vector i.e. [{3, 24}, {3, 85}, {5, 19}, {5, 44}, {6, 12}]. The begins_with vector
//will be the same , so the 35th element of that vector would look like: [{3, 39}, {5, 23}, {7, 17}]. When the numbers are divided like this you can see that there aren't actually
//too many possibilities for the numbers to chain. There are a lot, but not more than a computer should be able to handle easily. Once the numbers are divided like this it's only
//a matter of iterating through all of the ends with and starts with choices that match, making sure to not pick more than one type of polygonal number along the way and we should
//have our answer.

//My code ends up making mincemeat of the problem, but trying to keep track of six loops ended up being very cumbersome and time consuming. I should've just gone with a 
//recursive appraoch from the beginning and it would've been much easier. In the end what ended up causing the most time debugging was the fact that in my variable
//named "powers_of_two" I had put that 2^7 = 124 instead of 128 which meant that all Octagonal numbers were being ignored (I used a binary approach to see which types of numbers I
//had used already to avoid duplicate types. The octagonal nuumbers should've been represented by 2^7 = 10000000 so by having them equal to 124 = 01111100 they were getting excluded 
//from literally every other number type). It was no wonder I couldn't get the right answer for a few hours with a mistake like that. I'm too embarrased to upload this code to github
//until I write a proper recursive function as it would be too embarassing if anyone saw it.

//Writing the recursive function was really easy and the code runs in the same amount of time, I should've just done it from the beginning :)

//UPDATE
//I realized that there's no reason to start the initial search loop at 0, any number in the ends_with array that's at an index less than 10, 
//(i.e. the square number 1600) would require a number in the starts with array that begins with a 0. A leading 0 would indicate that the number 
//isn't 4-digits which we don't care about. Skipping the first 10 indices of the ends_with array offered a tiny speed up while still getting 
//the correct answer.

//HACKERRANK UPDATE
/*
The hackerRank version is a little different but the overall logic is the same. I realized that there were some edge cases that my original code missed, like making sure that the same 
number isn't used twice in the same chain (e.g. a number that is both triangular and hexagonal shouldn't appear twice in the same chain). I saved my original Euler code below.
*/

/*
Original Euler Code

int two_powers[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

void findAnswer(std::vector<std::vector<int> > &polygonal_numbers, std::vector<std::vector<std::pair<int, int> > > &starts_with, std::vector<std::vector<std::pair<int, int> > >& ends_with, int* chain, int current_end, char used_types, bool &cont, int level)
{
	//recursive function to comb through list of polygonal numbers to see if there's an existing chain of 6 based on the initial input starting number
	//chain is a pointer to the eventual answer, used types is an 8-bit char to keep track of which types of polygonal numbers have been used to avoid repeats,
	//it needs to start at 0, current_end is the last two digits of the current last number in the chain and is initialized from the starting number, cont is
	//a bool that causes the function to terminate when an answer is found (initialized to true) and level keeps track of the current level of the function
	//(initialized to 1).

	if (level == 6)
	{
		//end condition for the recursive function
		if ((*chain / 100) == (*(chain + 5) % 100)) cont = false;
		return;
	}

	for (int num = 0; num < starts_with[current_end].size(); num++)
	{
		std::pair<int, int>* pair_pointer = &starts_with[current_end][num];
		if (used_types & two_powers[pair_pointer->first - 1]) continue; //don't want to reuse number types
		else
		{
			*(chain + level) = polygonal_numbers[pair_pointer->first][pair_pointer->second];
			findAnswer(polygonal_numbers, starts_with, ends_with, chain, *(chain + level) % 100, (used_types | two_powers[pair_pointer->first - 1]), cont, level + 1);
			if (!cont) break;
		}
	}
}

std::pair<std::string, double> q61()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1;

	std::vector<int> dummy;
	std::vector<std::vector<int> > polygonal_numbers;
	polygonal_numbers.push_back(dummy); polygonal_numbers.push_back(dummy); polygonal_numbers.push_back(dummy); //to this so that triangle numbers are mapped to 3, square to 4, etc...

	//loop to generate polygonal numbers iteratively
	for (int i = 3; i < 9; i++)
	{
		std::vector<int> nums;
		int current_number = 1, adder = 1;
		while (current_number < 10000)
		{
			if (current_number > 999) nums.push_back(current_number); //we only care about 4 digit numbers for this problem
			adder += (i - 2);
			current_number += adder;
		}
		polygonal_numbers.push_back(nums);
	}

	std::vector<std::vector<std::pair<int, int> > > starts_with, ends_with;
	for (int i = 0; i < 100; i++)
	{
		//since we only care about the first and last two digits of any number overlapping, we only need to loop from 0 to 99
		std::vector<std::pair<int, int> > yo;
		starts_with.push_back(yo);
		ends_with.push_back(yo);
	}

	//next is to fill the start and end vectors
	for (int i = 3; i < 9; i++)
	{
		for (int j = 0; j < polygonal_numbers[i].size(); j++)
		{
			std::pair<int, int> heyhey = { i, j };
			starts_with[polygonal_numbers[i][j] / 100].push_back(heyhey);
			ends_with[polygonal_numbers[i][j] % 100].push_back(heyhey);
		}
	}

	bool cont = true; //will keep looping as long as this variable is true
	int current_chain[6] = { 0 };

	for (int i = 0; i < 100; i++) //start at 10 because anything less would have a lead 0 and not be a 4-digit number
	{
		for (int j = 0; j < ends_with[i].size(); j++)
		{
			current_chain[0] = polygonal_numbers[ends_with[i][j].first][ends_with[i][j].second];
			findAnswer(polygonal_numbers, starts_with, ends_with, current_chain, current_chain[0] % 100, two_powers[ends_with[i][j].first - 1], cont, 1);
			if (!cont) break;
		}
		if (!cont) break;
	}

	//once the first chain is found, add up all of the links to get the answer
	answer = current_chain[0] + current_chain[1] + current_chain[2] + current_chain[3] + current_chain[4] + current_chain[5];

	//uncomment the below line to see what the actual chain is
	//std::cout << "[" << current_chain[0] << ", " << current_chain[1] << ", " << current_chain[2] << ", " << current_chain[3] << ", " << current_chain[4] << ", " << current_chain[5] << "]" << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 28684
	//ran in 0.0001042 seconds
}
*/