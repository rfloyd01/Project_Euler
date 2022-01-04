#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>

//Digit factorials
std::pair<std::string, double> q34()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 0, fact = 0;

	std::vector<int> factorials = { 1 };
	for (int i = 1; i < 10; i++) factorials.push_back(factorials.back() * i);
	
	while (fact <= limit)
	{
		limit += factorials.back();
		fact = fact * 10 + 1;
	}

	for (int i = 10; i <= limit; i++)
	{
		int num = i, factorial_sum = 0;
		while (num > 0)
		{
			factorial_sum += factorials[num % 10];
			num /= 10;
		}
		if (factorial_sum == i)
		{
			//std::cout << i << std::endl;
			answer += i;
		}
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 40730
	//ran in 0.0599156 seconds
}

//NOTES
//Similar to problem 30, we find an upper limit by seeing where adding 9! can no longer reach the number it's a part of.
//In this case 9! * 7 = 2903040 which is 7 digits long. So if you had an 8 digit number of all 9's, the sum of its
//digit factorials would only be a 7 digit number. The problem will only get worse as numbers get bigger so there's no
//need to check any numbers higher than 2,903,040. Create a vector that stores all of the factorials from 1-9 so nothing
//has to be calculated more than once. After running the code, the maximum instance is in the 40,000's, but I can't see
//a way to reasonably lower the upper limit without knowing this fact before hand so I've left the upper limit in the
//2,000,000's in the code.
