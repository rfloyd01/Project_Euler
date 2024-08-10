#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes Primes()
#include <Header_Files/print.h>
#include <vector>
#include <map>

//Prime Permutations
void findSequences(std::vector<int>& p, int size, int maximum, std::vector<std::vector<std::string>>& answers, int location = 0, int distance = 0, int level = 0, bool recurse = false)
{
    //it's possible to find sequences that work where elements aren't right next to
    //each other. For example, given the primes [a, b, c, d, e, f, g] it's theoretically 
    //possible for c - b = b - a, as well as for g - e = e - b. For an exhaustive 
    //search all subsequence will need to be tested. The easiest approach to an
    //exhaustive search will be recursive.
    if (!recurse)
    {
        //the first two primes chosen set the distance for the arithmetic sequence so 
        //really any two primes can be chosen as the first two. We just need to make
        //sure that the first prime is less than the given maximum
        for (int i = 0; i <= p.size() - size; i++)
        {
            if (p[i] >= maximum) return;
            for (int j = i + 1; j <= p.size() - (size - 1); j++)
            {
                findSequences(p, size, maximum, answers, j, p[j] - p[i], 3, true);
            }
        }
    }
    else
    {
        //the recursive part of the sequence, just iterate forwards in an attempt to 
        //find a prime that's the appropriate distance. The variable "location" marks 
        //where the last prime in the sequence is.
        for (int i = location + 1; i < p.size(); i++)
        {
            if (p[i] - p[location] == distance)
            {
                if (level == size)
                {
                    //we've found a hit, print all the primes without spaces
                    int current_dist = distance * (size - 1);
                    std::string a = "";
                    for (int j = 0; j < size; j++)
                    {
                        a += std::to_string(p[i] - current_dist);
                        current_dist -= distance;
                    }
                    answers[a.length()].push_back(a);
                    return;
                }
                else
                {
                    //go to the next level of the recursion
                    findSequences(p, size, maximum, answers, i, distance, level + 1, true);
                }
            }
            else if (p[i] - p[location] > distance)
            {
                //the primes go in order so as soon as we find one that's too large,
                //the rest will be even larger so this sequence is a bust
                return;
            }
        }
    }
}

std::pair<std::string, double> q49()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	long long base_ten[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

	std::vector<int> prims = primes(10000);
	std::map<long long, std::vector<int> > permutations;
	int yeet;

	for (int i = 0; i < prims.size(); i++)
	{
		if (prims[i] < 1000) continue; //only care about 4 digit primes
		yeet = prims[i];
		long long feet = 0;

		for (int j = 0; j < 4; j++)
		{
			feet += base_ten[yeet % 10];
			yeet /= 10;
		}

		permutations[feet].push_back(prims[i]);
	}

	bool cont = true;

	for (auto iter = permutations.begin(); iter != permutations.end(); iter++)
	{
		if (iter->second.size() >= 3)
		{
			for (int i = 0; i <= iter->second.size() - 3; i++)
			{
				if ((iter->second[i + 1] - iter->second[i]) == (iter->second[i + 2] - iter->second[i + 1]))
				{
					if (iter->second[i] != 1487)
					{
						answer = (100000000 * (long long)iter->second[i]) + (10000 * (long long)iter->second[i + 1]) + (long long)iter->second[i + 2];
						cont = false;
						break;
					}
				}
			}

			if (!cont) break; //break out as soon as answer is found
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 296962999629
	//ran in 0.0002971 seconds
}

//NOTES
//Create a vector of pairs where each pairs consists of a 4 digit prime number and a base 5 representation of the digits included in the number. For example the number 9312
//would have a representation of 1000001110 and the number 7777 would have a representation of 0040000000 == 40000000. Keeping a base 5 representation will allow for quick 
//comparision of whether or not two numbers share the same digits. 

//MORE NOTES
//After thinking about it for a little bit I realized there was a more efficient way I could iterate through all the primes only once by using a map to store same permutation primes.
//Using this method didn't actually reduce the run time which surprises me a little bit, maybe this is just because the hit happens early on in the loop, or maybe because iteration through
//a map is just inherantly not as efficient as a vector?. With this new method though I discovered that there was actually a 4th prime permutation with the same digits (2699), but it isn't
//the same distance apart from the other 3 values so it's not counted so it wasn't all bad. The map method was only slower by .00003 seconds and looks better when written down so i've decided
//to keep this method and erase the other from code.