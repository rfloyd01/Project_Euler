#pragma once

#include <Header_Files/pch.h>

//Number Splitting
std::pair<std::string, double> q710()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0; //not sure how big answer will be so use long int

	int i = 2, mod = 1000000; //only keep the last 6 digits of any value
	int without_twos[3] = { 1, 1, 1 }, with_twos[2] = { 0, 1 };

	while (true)
	{
		int temp = without_twos[1];
		with_twos[0] = (with_twos[0] + with_twos[1]) % mod;
		with_twos[1] = (with_twos[0] + temp) % mod;

		//we now have the next two values of our sequence
		if (!with_twos[0])
		{
			answer = 2 * i + 1;
			break;
		}
		if (!with_twos[1])
		{
			answer = 2 * i;
			break;
		}

		//setup for the next round
		without_twos[1] = without_twos[2];
		without_twos[2] = (without_twos[2] + without_twos[0]) % mod;
		without_twos[0] = (without_twos[0] + temp) % mod;
		i++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1275000
	//ran in 0.0037486 seconds
}

//NOTES
/*
Questions dealing with palindromes can be tricky sometimes, as can questions about partitions. This questions happens to feature both of these topics. Instead 
of trying to create partitions of numbers that are palindromes, I think what I'll do instead is just create 
*/

//FIrst Algorithm
//const int limit = 50;
//long long permutations_without_two[limit + 1] = { 1, 1, 1, 2 }, permutations_with_two[limit + 1] = { 0, 0, 1, 2 }, palindromes_with_two[limit + 1] = { 0 };

////int difference = 0;
////for (int i = 4; i <= limit; i++)
////{
////	permutations_without_two[i] = 2 * permutations_without_two[i - 1] - difference;
////	permutations_with_two[i] = 2 * permutations_with_two[i - 1] + difference++; //increment difference here
////}
//for (int i = 4; i <= limit; i++)
//{
//	//add values from permutations_without_two first, making sure to skip the number 2
//	permutations_without_two[i] += permutations_without_two[i - 1];
//	for (int j = 3; j <= i; j++) permutations_without_two[i] += permutations_without_two[i - j];

//	//add 2 to permutations without 2
//	permutations_with_two[i] += permutations_without_two[i - 2];

//	//add 2 to permutations with 2
//	for (int j = i - 1; j > 1; j--) permutations_with_two[i] += permutations_with_two[j];
//}

////Do even and odd numbers separately from even numbers without 2's
//for (int i = 1; i <= limit; i++)
//	for (int j = 2; j <= i / 2; j++)
//		palindromes_with_two[i] += permutations_with_two[j];

////Mirroring permutations without any 2's and sticking a 2 into the middle of it
//for (int i = 2; i <= limit; i += 2) palindromes_with_two[i] += permutations_without_two[(i-2)/2];

////for (int i = 0; i <= limit; i++) std::cout << i << ": " << palindromes_with_two[i] << std::endl;
//for (int i = 0; i <= limit - 2; i++)
//{
//	if (i % 2) std::cout << i << ": " << palindromes_with_two[i - 1] << " - " << permutations_without_two[i / 2 - 1] << " = " << palindromes_with_two[i] << std::endl;
//	else std::cout << i << ": " << permutations_with_two[i / 2] << std::endl;
//}