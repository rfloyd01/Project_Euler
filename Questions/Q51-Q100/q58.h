#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes primeNumberTest()

//Spiral Primes
std::pair<std::string, double> q58()
{
	auto run_time = std::chrono::steady_clock::now();
	int primes = 0, non_primes = 1, side_length = 1, jump = 0;
	long long current_num = 1; //use a long long as it's unsure from onset how large the numbers will get

	while (true)
	{
		side_length += 2; //every iteration will cause the length of the overall square to increase by 2
		jump += 2; //the distance between diagonals of the square increases by 2 each layer
		for (int i = 0; i < 4; i++)
		{
			current_num += jump;
			if (primeNumberTest(current_num)) primes++;
			else non_primes++;
		}

		if ((float)primes / (float)(non_primes + primes) < 0.10) break;
	}

	return { std::to_string(side_length), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 26241
	//ran in 0.0272147 seconds
}

//NOTES
//I'm not entirely sure how large the numbers are going to get here, so start with long integers and then use bigint class if necessary. Also, since I'm not sure how large
//the numbers will be getting then it makes sense to use a prime test instead of some sort of sieve, so will use the Miller-Rabin test that I've already coded up. Since we
//only care about the diagonals of the square there's no need to check each number one by one. Starting at the center of the square, each corner of the first layer can be
//found by adding 2 (1 + 2 = 3, 3 + 2 = 5, 5 + 2 = 7 and 7 + 2 = 9). That layer of the square will be complete after four iterations. To find the 4 corners of the second layer,
//4 is added (9 + 4 = 13, 13 + 4 = 17...). It's pretty easy to see the pattern here, to find the corners of the current layer just add (2 * layer_nuamber) to the most recent number
//a total of four times and then move onto the next layer. Every time this is done do a primality test on the number and keep going to a new layer until the ratio first falls 
//below 10%.