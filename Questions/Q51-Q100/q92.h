#pragma once

#include <Header_Files/pch.h>

//Square Digit Chains
const int limit = 600;
int squares[10] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };

int GetDigitSquareSum(int n)
{
	int ans = 0, temp = n;
	while (n > 0)
	{
		ans += *(squares + n % 10);
		n /= 10;
	}
	return ans;
}

int CalculateChain(int number, int* chains)
{
	if (*(chains + number)) return *(chains + number);

	*(chains + number) = CalculateChain(GetDigitSquareSum(number), chains);
	return *(chains + number);
}

std::pair<std::string, double> q92()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int chains[limit] = { 0 }, num;

	//initialize values for 1 and 89
	chains[1] = 1;
	chains[89] = 89;

	for (int i = 1; i < limit; i++) if (CalculateChain(i, chains) == 89) answer++;

	//no need for recursive algorithm anymore once array fills up
	//just look at the second number of each chain to see if it ends in 89
	for (int i = limit; i < 10000000; i++)
		if (chains[GetDigitSquareSum(i)] == 89) answer++;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 8581146
	//ran in 0.067477 seconds
}

//NOTES
//I've done some of these chain problems before so the best solution is typically to create an array to store values for numbers 
//already seen before and avoid calculating the same chain multiple times. In the case of this problem we shouldn't need a very
//large array. We're looking at starting numbers below 10,000,000 which means that largest starting number is 9,999,999. If you square
//and sum all of those 9's it only adds up to 567, so every other starting number below 10,000,000 will have the next number in its
//chain be less than that. A well designed recursive algorithm should be able to keep track of every number currently in a chain
//without the need to store any values in an array which should be a pretty good time and memory saver. Eventually you hit a point
//where it's impossible for the sum of the squares of the digits to be greater than or equal to the current number (this happens at
//243 because 9^2 + 9^2 = 162 and 9^2 + 9^2 + 9^2 = 243). This means that we can use the recursive function up until number 243, and 
//from that point on it doesn't matter which number we start at as the next number of its chain will already have been discovered. So
//we can use the recursive chain from numbers 1 - 243, then from 244 - 567 we don't need the recursive function, just look at the next
//number in the starting number's chain and then save it into our chain array, finally from 568 and onwards we don't even need to save
//values anymore, just look at the next number in the chain and be done with it. To save on lines of code I think it's alright to just
//use the recursive algorithm until the limit, it probably won't effect the time that much.