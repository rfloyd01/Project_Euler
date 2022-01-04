#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>

//Consecutive Prime Sum
std::pair<std::string, double> q50()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int max_chain = 0;
	const int maximum = 1000000;

	std::vector<int> prims;
	bool *prime_test = new bool[maximum]; //create array on heap instead of stack
	PrimesWithSieve(prime_test, maximum, prims); //fill out prime vector and prime_test array

	std::vector<long long> consecutive_prime_sums = { 2 };
	for (int i = 1; i < prims.size(); i++)
	{
		if (prims[i] < (maximum / 250)) consecutive_prime_sums.push_back(prims[i] + consecutive_prime_sums.back());
		else break; //testing showed that the largest chains have a relatively low maximum prime number so cap the sums at 1/20th the maximum (note this cap needs to change as maximum gets smaller)
	}

	for (int i = consecutive_prime_sums.size() - 1; i >= 0; i--)
	{
		if (i < max_chain) break; //no more prime sums can beat the current answer so break out

		if (consecutive_prime_sums[i] < maximum && prime_test[consecutive_prime_sums[i]])
			if (i > answer)
			{
				max_chain = i;
				answer = consecutive_prime_sums[i];
			}

		for (int j = 0; j < consecutive_prime_sums.size(); j++)
		{
			//as soon as a prime number is created break out of this loop as the primes will only get smaller as loop progresses
			if ((consecutive_prime_sums[i] - consecutive_prime_sums[j]) < maximum && prime_test[consecutive_prime_sums[i] - consecutive_prime_sums[j]])
			{
				if (i - j > max_chain)
				{
					max_chain = i - j;
					answer = (consecutive_prime_sums[i] - consecutive_prime_sums[j]);
					//std::cout << prims[j] << ", " << prims[i] << ", " << max_chain << std::endl;
				}
				break;
			}
		}
	}

	delete[] prime_test; //delete prime test off of the heap
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 997651
	//ran in 0.0032327 seconds
}

//NOTES
//As with all the first page Euler problems a brute force approach would work pretty easily here, however, I want to come up with something a little more clever than that. My
//first thought is to create a vector where each element represents the sum of all primes 2 to n. For example, the first 5 primes are 2, 3, 5, 7, 11 so my vector would look like
//(2, 5, 10, 17, 28). Since we're looking for the largest prime that can be represented by consecutive prime addition, we'll start from the back of the vector and then keep subtracting
//elements from the front until we get a prime value. Once we've hit our first prime though that isn't necessarily the answer. We won't have our answer until our starting point in
//the vector is lower than our current maximum. Also, since a sum is at least two numbers, then we don't need any primes over 500,000 because 500,000 + 500,001 = 1,000,001. In the end
//this took about half a second to run which is more than I would've thought, but anything under a second seems pretty good to me. Will return to this problem and try to reduce run time
//after reading forum posts.

//Didn't get this from the forums, but after thinking about it a little more I realized that I went into this problem with the mindset of finding the largest prime under 1 million and
//not purely the largest chain under one million. Looking at the largest prime would allow primes up to 500,000 to be used, however, the largest possible chain you could have with numbers
//of that size is only 2 or 3. Really the largest prime we need should be around 1,500 or so because the sum of 1 - 1,500 is roughly 1,000,000 so that would represent the longest possible
//chain. It is still possible to make a chain with numbers after 1,500 however the maximum chain length is only going to get smaller and smaller so it's much more likely the largest prime in
//the chain will occur before 1,500. 1,500 may be too conservative of a guess as there's some space between prime numbers so the actually cap should be higher. Looking at the sample text the
//largest chain under 1,000 has 21 elements. Actually solving yields the first prime to be 5 and the last prime to be 89 so the largest prime needed is only 1/10th the size of the maximum.
//It seems like the larger the maximum is the smaller the biggest prime in the chain will be relative to that maximum. I'm going to keep my code the same but change the limit from 500,000
//to 50,000. Not surprisingly the code runs much faster and still gives the same answer. Adding a print function shows that the largest prime in the chain is only 3,931 so I've set
//the limit at 1/250th of the maximum and still get the correct answer.

//Changing the ceiling of primes that I look at made the run time go from .42 seconds all the way down to .003 seconds which is a 140x speedup. It always feels a little cheasy when I change
//my initial ceiling after finding the answer to a problem but at least this time I reasoned out why the celing could be changed instead of changing it blindly so it feels more justified.