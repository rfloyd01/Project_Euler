#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>

//Digit factorials
void recursiveFactorialBuild(int* factorials, int current_number, int current_sum, int& answer)
{
	//base cases
	if (current_number == current_sum) answer += current_number;
	else if ((current_number * 10 + 9) - current_number > current_sum + factorials[9]) return;

	//recursively add factorials to current number
	for (int i = 0; i < 10; i++) recursiveFactorialBuild(factorials, current_number * 10 + i, current_sum + *(factorials + i), answer);
}

std::pair<std::string, double> q34()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//create factorials
	int factorials[10] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 1; i < 10; i++) factorials[i] = factorials[i - 1] * i;

	//recursively look for answers
	for (int i = 1; i < 10; i++) recursiveFactorialBuild(factorials, i, factorials[i], answer);
	answer -= 3; //remove instances of 1! = 1 and 2! = 2

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 40730
	//ran in 0.0013857 seconds
}

//NOTES
//Similar to problem 30, we find an upper limit by seeing where adding 9! can no longer reach the number it's a part of.
//In this case 9! * 7 = 2903040 which is 7 digits long. So if you had an 8 digit number of all 9's, the sum of its
//digit factorials would only be a 7 digit number. The problem will only get worse as numbers get bigger so there's no
//need to check any numbers higher than 2,903,040. Create a vector that stores all of the factorials from 1-9 so nothing
//has to be calculated more than once. After running the code, the maximum instance is in the 40,000's, but I can't see
//a way to reasonably lower the upper limit without knowing this fact before hand so I've left the upper limit in the
//2,000,000's in the code.

//HACKERRANK UPDATE
/*
I modified my original approach to be recursive in nature which offered a nice speedup. The HackerRank version of the question is 
slightly different in that it wants the sum to be divisible by the starting number, not just equal to it, which will give more 
solutions. Because the question is different the improved Euler code is left above, with the HackerRank code shown below:

void recursiveFactorialBuild(int* factorials, int limit, int current_number, int current_sum, int& answer)
{
	//base cases
	if (current_number >= limit) return;
	else if (!(current_sum % current_number) && current_number > 9) answer += current_number;
	else if ((current_number * 10 + 9) - current_number > current_sum + factorials[9]) return;

	//recursively add factorials to current number
	for (int i = 0; i < 10; i++) recursiveFactorialBuild(factorials, limit, current_number * 10 + i, current_sum + *(factorials + i), answer);
}

std::pair<std::string, double> q34()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 2903040;

	//create factorials
	int factorials[10] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 1; i < 10; i++) factorials[i] = factorials[i - 1] * i;

	//recursively look for answers
	for (int i = 1; i < 10; i++) recursiveFactorialBuild(factorials, limit, i, factorials[i], answer);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 40730
	//ran in 0.0013857 seconds
}
*/
