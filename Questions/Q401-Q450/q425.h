#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes primeNumberTest(), powers_of_ten[]
#include <vector>
#include <fstream>

//Prime Connection
bool modifiedPrimeTest(int n)
{
	if (n == 1) return true;
	else return primeNumberTest(n);
}
void buildTwosRelativePrimes(bool* twosRelatives, int current_number, int maximum, int current_level)
{
	if (current_number + powers_of_ten[current_level] > maximum) return;

	for (int i = 1; i < 10; i++)
	{
		if (modifiedPrimeTest(current_number + i * powers_of_ten[current_level]))
		{
			//std::cout << current_number + i * powers_of_ten[current_level] << std::endl;
			*(twosRelatives + current_number + i * powers_of_ten[current_level]) = true;
			buildTwosRelativePrimes(twosRelatives, current_number + i * powers_of_ten[current_level], maximum, current_level + 1);
		}
	}
}


std::pair<std::string, double> q425()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	int currentLimit = 100;
	bool* twosRelatives = new bool[currentLimit](), *prims = new bool[currentLimit]();
	for (int i = 2; i < currentLimit; i++) prims[i] = true;

	//Prime Sieve
	for (int i = 2; i <= sqrt(currentLimit); i++)
		if (prims[i])
			for (int j = i * i; j < currentLimit; j += i) prims[j] = false;

	for (int i = 2; i < currentLimit; i++)
		if (prims[i]) std::cout << i << std::endl;
	//buildTwosRelativePrimes(twosRelatives, 0, currentLimit, 0);

	//std::vector<int> prims = primes(currentLimit);
	//std::cout << prims.size() << std::endl;

	/*for (int i = 0; i < prims.size(); i++)
	{
		if (!twosRelatives[prims[i]])
		{
			answer += prims[i];
			std::cout << prims[i] << std::endl;
		}
	}*/

	delete[] twosRelatives;
	delete[] prims;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}

//Notes:
/*
* It seems that the larger N is, the higher percentage of 2's-relatives you find. The sum of the non-relatives
* below 1000 is only 431. This could be a minimum of 1 number (431) and a maximum of 29 numbers (the 30th triangle number is 435). So in the case of 
* 1000, a maximum of 2.9% of numbers or non-2's-relatives. In the case of 10,000 the sum of the relatives is 78,728 which would need a minimum of
* 8 numbers to achieve (sum[9993-10000]) and a maximum of 397 numbers. So the maximum case here is 3.97%. If we go up roughly by
* 1 percentage point for every power of 10, then we can expect roughly, that we're looking for a maximum of 7% of 10,000,000
* which would be about 700,000 numbers. 
* 
* One thing to take into account here though, is that we're not looking for just any numbers, they must be prime nubmers. 10,000,000
* only has about 600,000 primes below it. What may be best here is to first create a boolean prime sieve up to 10,000,000. Then
* create another boolean array where we add all numbers that are 2's relatives without checking for primality. This should be pretty
* easy to do with some loops. Finally, just compare the two boolean arrays and only add numbers that are in the prime array but not
* the relative array.
* 
* On second thought, if we take away the limitation of 2's-relatives only consisting of primes, then every single number would
* be a 2's-relative so creating an array of all 2's-relative numbers (not just prime ones) would be pretty useless. Maybe it would
* be better to group primes into different groups where every number in a group is the same amount of digits and only of by one digit.
* So the first group of numbers would be the single digit primes: (2, 3, 5, 7), then you would have two digit primes. The first
* group is numbers that are 1 digit away from 11: (13, 17, 19, 31, 41, 61, 71). Then you have things that are 1 digit off from
* 13: (11, 17, 19, 23, 43, 53, 73, 83). And so on. Once you have all of the primes mapped out in this manner it would be simple
* enough to loop through these lists and find all of the 2's relatives. The big issue with this though, is that there will be
* A LOT of overlap between groups and we'll end up recording certain primes over and over again. This isn't going to be efficient
* timewise or memorywise.
* 
* My initial thought was to try and just build upwards digit by digit, but we run into a problem that the chains can actually
* go downwards i.e. 2 -> 3 -> 13 -> 113 -> 103. This is because when go a single digit at a time, you can't add a 0 as it doesn't
* actually change the number. To really add a 0 we would need to add 10 to the front of 3. So tacking all possible numbers to 3 would
* look like this [13, 23, 33, 43, 53, 63, 73, 83, 93, 103] instead of [03, 13, 23, 33, 43, 53, 63, 73, 83, 93]. If we did things
* this way, however, we'd run into the problem of "how can we get numbers with multiple 0's next to eachother?". For example,
* 1009 is a prime number. To get around this multiple zero issue it may just be easiest to handle 0's in it's own loop. So the first
* loop would check all numbers where we tak on normal digits: [13, 23, 33, 43, 53, 63, 73, 83, 93] and then the second loop would
* handle adding multiple 0's: [103, 1003, 10003, 100003, 1000003]. There's still one more snag here. If we're only adding digits to
* the front of our number then something like 503 would never get tested. So it isn't enough to add just factors of 10 to the front,
* we'd also need to add all multiples of that power of 10. So for our lowely prime number of 3, we end up adding 54 different numbers
* onto the front of it to test for primality.
* 
* I just had a thought. We're only "allowed" to change 1 digit at a time. Adding all of these multiples of 10 to the front of numbers
* would be changing multiple digits at a time, so we can't actually guarantee that we get 2's-relatives by doing this. Also I just realized
* there are plenty of other instances where we can only hit a relative by moving downwards, 11 for example. There's a few ways to get to
* 11: 2 -> 3 -> 13 -> 11 being the easiest. Since 1 isn't a prime number, then my building up method would never even check numbers that 
* end in a 1, which should be roughly a quarter of all prime numbers as once you get higher then 10, primes can only end in 1, 3, 7 and 9.
* 
* Currently the only thing I can think of is to every time we find a new 2's relative, try changing every digit in the number to every 
* possible digit. So this would be 10 * number_of_digits to check for primality every time we find one of these numbers and based on my
* assessment above there are a MINIMUM (but likely many more) of 10,000,000 - 700,000 = 9,300,000 of these numbers. Saying that on 
* average there are 5 digits for each number whith would be 50 * 9,300,000 = 465,000,000 primality checks to make. An algorithm like
* this would be pretty dam slow, but not out of the range for brute forcing. At first when I saw this was a 25% problem I thought it would
* be pretty easy but now I'm assuming that it was just brute forced by everyone.
*/