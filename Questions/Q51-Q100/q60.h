#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes primes(), primeNumberTest(), numberOfDigits(), []powers_of_ten
#include <cmath> //includes pow()

//Prime Pair Sets
bool canConcatanate(int a, int b, int a_length, int b_length)
{
	//this function checks whether the number ab (made by concatanating a and b) and the number
	//ba (the number made by concatanating b and a) are prime. The function only returns true
	//if both ab and ba are prime

	long long ab = powers_of_ten[b_length] * a + b;

	if (primeNumberTest(ab))
	{
		long long ba = powers_of_ten[a_length] * b + a;
		if (primeNumberTest(ba)) return true;
		else return false;
	}
	else return false;
}

bool numberTest(std::vector<std::vector<int> >& prime_concat, int* current_numbers, int test_number, int start_location)
{
	//sees if test number is compatible with current_numbers[1] - current_numbers[start_location]. It's known that test_number works with current_numbers[0]
	for (int i = start_location; i > 0; i--)
	{
		bool end_of_loop = true;
		for (int j = 0; j < prime_concat[*(current_numbers + i)].size(); j++)
		{
			if (prime_concat[*(current_numbers + i)][j] == test_number)
			{
				end_of_loop = false;
				break;
			}
			else if (prime_concat[*(current_numbers + i)][j] > test_number) return false;
		}
		if (end_of_loop) return false; //sometimes there are no numbers larger in a vector than the test number, if a complete cycle is made then return false
	}
	return true;
}

std::pair<std::string, double> q60()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 8400;

	std::vector<int> prims = primes(limit), prime_digit_lengths;
	std::vector<std::vector<int> > prime_concat;
	for (int i = 0; i < limit; i++)
	{
		std::vector<int> yeet;
		prime_concat.push_back(yeet); //default initialize prime_concat with empty vectors
	}

	for (int i = 0; i < prims.size(); i++) prime_digit_lengths.push_back(numberOfDigits(prims[i]));

	for (int i = 0; i < prims.size(); i++)
	{
		for (int j = i + 1; j < prims.size(); j++)
		{
			if (canConcatanate(prims[i], prims[j], prime_digit_lengths[i], prime_digit_lengths[j]))
			{
				prime_concat[prims[i]].push_back(prims[j]);
			}
		}
	}
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 << std::endl;

	bool cont = true;
	int winning_numbers[5] = { 0, 0, 0, 0, 0 };
	for (int first_num = 0; first_num < prims.size(); first_num++)
	{
		winning_numbers[0] = prims[first_num];
		for (int second_num = 0; second_num < prime_concat[prims[first_num]].size(); second_num++)
		{
			//the second number will always work with the first number so no need for a test
			winning_numbers[1] = prime_concat[prims[first_num]][second_num];
			for (int third_num = second_num + 1; third_num < prime_concat[prims[first_num]].size(); third_num++)
			{
				//this is the first level where a test will have to be carried out. number three will work with number one because it's chosen from
				//number one's list, however, it isn't known if it will work with number two
				if (numberTest(prime_concat, winning_numbers, prime_concat[prims[first_num]][third_num], 1))
				{
					//if number 3 passes then go to loop for number 4
					winning_numbers[2] = prime_concat[prims[first_num]][third_num];
					for (int fourth_num = third_num + 1; fourth_num < prime_concat[prims[first_num]].size(); fourth_num++)
					{
						if (numberTest(prime_concat, winning_numbers, prime_concat[prims[first_num]][fourth_num], 2))
						{
							//if number 4 passes then go to loop for number 5
							winning_numbers[3] = prime_concat[prims[first_num]][fourth_num];
							for (int fifth_num = fourth_num + 1; fifth_num < prime_concat[prims[first_num]].size(); fifth_num++)
							{
								if (numberTest(prime_concat, winning_numbers, prime_concat[prims[first_num]][fifth_num], 3))
								{
									//if number 5 passes then we have a winner
									winning_numbers[4] = prime_concat[prims[first_num]][fifth_num];
									//std::cout << "[" << winning_numbers[0] << ", " << winning_numbers[1] << ", " << winning_numbers[2] << ", " << winning_numbers[3] << ", " << winning_numbers[4] << "]" << std::endl;
									cont = false;
									break;
								}
							}
							//at this stage none of the attempts at number 5 worked, change number 4 and try again
							if (!cont) break;
						}
					}
					//at this stage none of the attempts at number 4 worked, change number 3 and try again
					if (!cont) break;
				}
			}
			//at this stage none of the attempts at number 3 worked, change number 2 and try again
			if (!cont) break;
		}
		//at this stage none of the attempts at number 2 worked, change number 1 and try again
		if (!cont) break;
	}

	answer = winning_numbers[0] + winning_numbers[1] + winning_numbers[2] + winning_numbers[3] + winning_numbers[4];
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 26033
	//ran in 0.26605 seconds
}

//NOTES
//A brute force attempt at this problem isn't really feasible because we don't know what the highest needed prime will be. Taking a shot in the dark, let's say the maximum prime
//needed is right around 20,000. There are ~2,600 primes under 20,000 so a brute force method would need to test all choose(2600, 5) = 9.86 x 10^14 possibilities to pick 5 primes
//under the limit which would probably take years to run. What I think will work better is to test every prime against every other prime to see which ones can be concatanated
//succesfully. Each prime number will get its own vector populated with all of the primes that it can concatenate with. The first few elements of this nested vector would be
//[[], [], [], [7, 11, 17, 31 ...], [], [], [], [19 ...] ..]. 3 and 7 are the first elements that can succesfully concatenate with anything as can be seen. Once this nested
//vector is complete for all primes under 20,000 a nested loop that's five levels deep can be used to determine the five primes that will work. The first two loops will select
//numbers 1 and 2 of the series. Numbers 2, 3, 4 and 5 are all primes that will be picked from number 1's vector so it is known that all of these numbers will work. Once number 
//three is selected it needs to be tested so see if it works with number 2 by doing a quick search in number 2's vector. If it is then go onto number 4, if not try a different
//number three. Number 4 will need to be tested against numbers 2 and 3 while number 5 will need to be tested against 2, 3 and 4. This might not be the most elegant solution but 
//I spent some time trying to come up with a recursive approach and it didn't seem to work out so well. 

//After running the code I found two interesting things. First was that I actually found two answers, and the first answer found didn't have the smallest sum: [7, 1237, 2341, 12409, 18433]
//are five primes with a sum of 34,427. The second answer: [13, 5197, 5701, 6733, 8389] with a sum of 26,033 is the correct one but was found second because the first element is larger
//than the first element of the other answer. It seems like I didn't need to check any primes higher than 8389 to get the right answer so the initial ceiling of 20,000 wasn't aggregiously
//off the mark. The second interesting thing was the shear amount of time it took to organize the nested vector of primes that concatenate with eachother. I didn't think it would take
//that long but of the 3.601 seconds it took for the initial program to run, 3.55 of those seconds were dedicated to creating the nested prime concat vector and only 0.051 seconds 
//were needed to search through the nested vector to find the answers. If there was someway to speed up the concat test that would be the best approach to reducing the run time for this
//code. After completing the code once and knowing what the actual prime limit is, reducing the limit from 20,000 to 8,400 reduces the run time from 3.601 seconds to 0.4813 seconds which
//is a much better time although still not great and it feels a little cheap doing it when I already know the answer.

//I think the logic I used for the actual solving of this problem is pretty good, so to speed up my answer I really just need to look at the prime generation and sorting
//part of the problem. To do this I need to either find a quicker way to concatenate two integers, find a quicker way to run a prime test or I can generate primes high enough
//so that I don't need a prime test. A quick Google search for "Fastest way to Concatenate Integers" didn't really yield anything interesting, the best solution I saw was to convert
//the integers to strings, concatenate the strings, and then turn the final string back into an integer. I have to imagine that way would be much slower than my current way but
//let's test it. The program run time went from 3.55 seconds to 3.69 seconds so indeed my way was quicker, if only by a little. Another quick thought I had was to just create another
//vector which holds the lengths of each prime that I want to test, this way I'm not testing the same numbers over and over. Doing this technique brought the run time down to a whopping
//3.46 seconds, so basically no speedup. In this case I either need a quicker primality test or I just need to seive more primes so I don't need the primality test at all. Considering
//I'm already using the Miller-Rabin test which is one of the fastest tests I know of I'll probably have to go with the later. Knowing that the maximum prime needed is about 8,400
//I would need to seive all primes up to 84,008,400 to quickly test if concatenated numbers are prime or not. Doing this method ended up being .2 seconds slower then my previous best
//with a limit of 8400 though.

//After reading the forum for this question it seems like all the people with the fastest times used a precompiled list of primes up to 100,000,000 which honestly feels a little cheap
//to me so I'm actually pretty happy with how my initial 3.46 seconds turned out. For now I've changed the limit to 8,400 and with the other slight speedups I've found the runtime is
//now .444 seconds which I'm satisfied with for now.