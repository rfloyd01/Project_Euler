#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/bigint.h>

//Convergents of e
std::pair<std::string, double> q65()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//first create the repeated fraction for e
	const int limit = 100;
	int e[limit] = { 2, 1, 2 };
	for (int i = 3; i < limit; i++) e[i] = 1;
	for (int i = 5; i < limit; i += 3) e[i] = 2 + e[i - 3];

	//next go through the repeating fraction, continuously flipping the numerator and denomiator then adding the next numer in the sequence
	bigint nums[2] = { 1, e[limit - 1] }; //keep track of the numerator and denominator with the bigint class as they will get pretty large
	bool numerator = 0;

	for (int i = limit - 2; i >= 0; i--)
	{
		nums[numerator] += nums[!numerator] * e[i];
		numerator = !numerator;
	}

	//Uncomment below line to see what the final answer is in fraction form
	//std::cout << denominator << "/" << numerator << std::endl;

	//add up the digits of the final numerator
	numerator = !numerator; //need to swap the numerator back with the denominator
	while (nums[numerator] > 0)
	{
		answer += nums[numerator] % 10;
		nums[numerator] /= 10;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 272
	//ran in 0.0000531 seconds
}

//NOTES
//After the issues I just ran into with problem 64 my heart dropped a little when I looked at this problem. After looking at it though it doesn't seem that actually
//calculating the repeated fraction will be necessary as the formula is given. All that should be necessary for this problem is to correctly write out the sequence of
//repeated fractions to the 100th term and then use bigint types to keep track of the numerator and denominator after each iteration. This problem is pretty trivial if
//using a bigint class, essentially just brute forced and it runs instantly. There's probably a more efficient way to do it, the above code wouldn't scale well if you
//were looking for the millionth iteration for example instead of the hundreth, but I think it's good enough for now.