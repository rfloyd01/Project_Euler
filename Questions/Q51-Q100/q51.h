#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes primes(), <vector>
#include <Header_Files/print.h>
#include <map>
#include <cmath> //includes pow()

//Prime Digit Replacements
int base_two[10] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 }; //used for quickly creating binary numbers
int used_digits[10] = { 0 }; //helps keep track of where in a number the repeated digits are
int repeated_digits[10] = { 0 }; //keeps track of which digits have been repeated in a number
int max_repetitions = 6; //how many digits in the number are allowed to be repeats. Since I'm looking at numbers under 1000000 make this 6

std::vector<int> getBinaryMasks(int n)
{
	//given a set of repeating digits, gives a binary representation for how to choose at least 2 of the repeating digits
	//for example, given the set [2, 2, 2] you can pick the following indices to get repeating 2's: (0, 1), (0, 2) or (1, 2)
	//in a binary representation this would be 110, 101 and 011 respectively. 
	std::vector<int> ways;
	for (int i = 3; i < pow(2, n); i++) if (i & (i - 1)) ways.push_back(i); //don't add anything to list that's a power of 2, this means only 1 repeat digit is selected
	return ways;
}
void maskIterate(int number_of_repeats, int n, int wanted_digit, std::vector<int>& repeats, std::vector<std::vector<int> > &digit_masks)
{
	//this function takes a prime n with repeating digits and returns all of the ways there are to utilize 2 or more of the repeated digits
	//i.e. in a number like 53233 this function would spit out (5x2xx, 532xx, 5x2x3, 5x23x). Stores these possible options in the vector repeats

	int n_copy, new_num, binary, js_encountered, location, current_digit;
	for (int k = 0; k < digit_masks[number_of_repeats].size(); k++)
	{
		n_copy = n;
		new_num = 0;
		binary = 0;
		js_encountered = 0; //counts the number of times used_digit has been encountered within the number. Called j's because this was originally in a different loop and the variable was j
		location = 0;
		while (n_copy != 0)
		{
			current_digit = n_copy % 10;
			if (current_digit == 0) current_digit = 10; //this is an attempt to fix leading zero's issue, not sure if it will work
			if ((current_digit % 10) != wanted_digit) //need to use %10 for the case of 0 which is now a 10 lol
			{
				new_num *= 10;
				new_num += current_digit;
			}
			else
			{
				//compare current location in number to the current bit mask
				if (base_two[js_encountered] & digit_masks[number_of_repeats][k])
				{
					binary += base_two[location];
				}
				else
				{
					new_num *= 10;
					new_num += current_digit;
				}
				js_encountered++;
			}
			n_copy /= 10;
			location++;
		}
		repeats.push_back(binary);
		repeats.push_back(new_num);
	}
}
std::vector<int> getRepeatingDigits(int n, std::vector<std::vector<int> >& digit_masks)
{
	//this function returns a vector in case there is more than one set of repeating digits in n
	for (int i = 0; i < 10; i++) used_digits[i] = 0; //make sure to reset used digits array
	for (int i = 0; i < 10; i++) repeated_digits[i] = 0; //make sure to reset used digits array

	std::vector<int> repeats;
	int n_copy = n, current_digit, new_num;
	bool has_repeats = false;
	int i = 0;

	//this first loop finds all of the digits in n which are repeated
	while (n_copy > 0)
	{
		current_digit = n_copy % 10;
		if (used_digits[current_digit])
		{
			has_repeats = true; //know to not break out of function at end of loop
		}
		repeated_digits[current_digit]++;
		used_digits[current_digit] += base_two[i]; //create a binary number in each index of used digits
		n_copy /= 10;
		i++;
	}

	if (!has_repeats) return repeats; //returns an empty vector if no numbers are repeated

	for (int j = 0; j < 10; j++)
	{
		//check each digit 0-9 to see how many times it repeats, use maskIterate function to iterate through all the possible repeat masks
		if (repeated_digits[j] > 1 && repeated_digits[j] <= max_repetitions) maskIterate(repeated_digits[j], n, j, repeats, digit_masks);
	}
	return repeats;
}

std::pair<std::string, double> q51()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;

	//create a vector that lists all the ways to choose either 2, 3, 4, 5 or 6 digits
	std::vector<std::vector<int> > digit_masks;
	for (int i = 0; i <= max_repetitions; i++)
	{
		std::vector<int> yeet = getBinaryMasks(i);
		digit_masks.push_back(yeet);
	}

	std::vector<int> prims = primes(1000000); //not sure how high I'll need to search so start out at 1,000,000 ceiling
	std::vector<std::map<int, std::vector<int> > > prime_container;
	for (int i = 0; i < 64; i++)
	{
		//since we're only searching for primes under 1000000, the maximum value of the bit mask cover would be a 6 digit binary number.
		//111111 = 63 so that's our max value
		std::map<int, std::vector<int> > dummy;
		prime_container.push_back(dummy);
	}

	for (int i = 0; i < prims.size(); i++)
	{
		std::vector<int> repeats = getRepeatingDigits(prims[i], digit_masks);
		for (int j = 0; j < repeats.size(); j += 2) //go up by two because repeats array has at least one non-repeating set for each repeating set
		{
			prime_container[repeats[j]][repeats[j + 1]].push_back(prims[i]);
		}
	}

	bool cont = true;
	for (int i = 0; i < prime_container.size(); i++)
	{
		for (auto iter = prime_container[i].begin(); iter != prime_container[i].end(); iter++)
		{
			if (iter->second.size() == 8)
			{
				answer = iter->second[0]; //we're looking for the smallest number in the set so just return the first item in vector
				cont = false;
				break;
			}
		}
		if (!cont) break; //stop searching as soon as an answer is found
	}
	//runs in 0.0575925 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 121313
	//ran in 0.0575925 seconds
}

//NOTES
//Not sure how high up I'll need to search so starting with an upper limit of a million to start off. Sieve all primes under 1,000,000 and then go through that list
//and elimiate any primes that don't have at least 2 repeating digits. If the prime does have repeating digits, save the prime in a vector where each index is a binary
//number that keeps track of which digit locations are repeated (i.e. the number 12451 would get a value of 10001 so would thus be saved at index 17 of the vector). Each element
//in the vector will be a pair where the first element is a base 10 number which keeps track of what the non-repeated digits are (in the example above 12451 would be written as
//0000110120 == 110120) and the second element of the pair will be another vector which holds all prime numbers that fit into that repeated and non-repeated digits category. If
//a number has more than one set of repeating digits it will be saved in more than one section of the overall container. Once all primes have been sorted through, obtaining the
//answer should just be a matter of iterating over the primes one last time, seeing which container has a length of 8 and then returning the first element of that vector. Didn't
//initially find the answer, my theory for this is because not every number has only one representation. For example the prime number 56333 can be represented as either a single
//triple (56xxx) or three different doubles (563xx, 56x3x, and 56xx3). Because some numbers have multiple representations my method isn't properly putting a single number into
//every category it belongs to. This is a shame because my code currently runs in .04 seconds which is a good amount of time, but the extra code necessary to put a number into 
//all potential categories will mostly likely slow it down by a good amount. If there's only a pair of matching numbers then nothing will change, however, as soon as a third
//matching digit gets introduced the placements jumps from 1 to 4. A 4th matching digit makes the placements jump to 10. After accounting for multiple masks of the same number
//the answer was found and it didn't actually effect the run time all that much. I guess the number of primes under 1000000 with more than 2 of the same digit are at least somewhat
//rare.