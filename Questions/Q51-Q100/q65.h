#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/bigint.h>

//Convergents of e
std::pair<std::string, double> q65()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int num = 2;

	//first create the repeated fraction for e
	const int limit = 100;
	int e[limit];
	e[0] = 2;
	for (int i = 1; i < limit; i++) e[i] = 1;
	for (int i = 2; i < limit; i += 3)
	{
		e[i] = num;
		num += 2;
	}

	//next go through the repeating fraction, continuously flipping the numerator and denomiator then adding the next numer in the sequence
	bigint numerator(1), denominator(e[limit - 1]), temp; //keep track of the numerator and denominator with the bigint class as they will get pretty large

	for (int i = limit - 2; i >= 0; i--)
	{
		numerator += denominator * e[i];
		temp = denominator;
		denominator = numerator;
		numerator = temp;
	}

	//Uncomment below line to see what the final answer is in fraction form
	//std::cout << denominator << "/" << numerator << std::endl;

	//add up the digits of the final numerator
	while (denominator > 0)
	{
		answer += denominator % 10;
		denominator /= 10;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 272
	//ran in .0000744 seconds
}

//NOTES
//After the issues I just ran into with problem 64 my heart dropped a little when I looked at this problem. After looking at it though it doesn't seem that actually
//calculating the repeated fraction will be necessary as the formula is given. All that should be necessary for this problem is to correctly write out the sequence of
//repeated fractions to the 100th term and then use bigint types to keep track of the numerator and denominator after each iteration. This problem is pretty trivial if
//using a bigint class, essentially just brute forced and it runs instantly. There's probably a more efficient way to do it, the above code wouldn't scale well if you
//were looking for the millionth iteration for example instead of the hundreth, but I think it's good enough for now.