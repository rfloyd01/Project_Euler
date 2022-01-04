#include <iostream>
#include <time.h>
#include "functions.h" //includes Vector and primes
#include "print.h"
//yoyoyo
//Prime Frog

int minimum = 1, maximum = 500;
int length = 15;

void q329()
{
	clock_t t = clock();
	std::cout.precision(10);
	std::vector<std::vector<float> > seqs;
	double answer = 0;

	for (int start = minimum; start <= maximum; start++)
	{
		std::vector<std::vector<std::vector<float> > > sequences;
		std::vector<std::vector<float> > curr; sequences.push_back(curr);
		std::vector<float> yoo; yoo.push_back(1); yoo.push_back(start);
		//the first number of each possible sequence will be the probablility of getting that sequence
		//the probability isn't the same for each sequence since at the ends of the number line there's only one
		//choice of where the frog can go instead of two like on every other spot
		sequences[0].push_back(yoo);
		for (int i = 1; i < length; i++)
		{
			std::vector<std::vector<float> > next; sequences.push_back(next);
			for (int j = 0; j < sequences[0].size(); j++)
			{
				std::vector<float> current = sequences[0][j];
				if (current.back() == minimum)
				{
					current.push_back(minimum + 1);
					sequences[1].push_back(current);
				}
				else if (current.back() == maximum)
				{
					current.push_back(maximum - 1);
					sequences[1].push_back(current);
				}
				else
				{
					current.push_back(current.back() + 1);
					current[0] *= .5; //50-50 chance of going forward or backward
					sequences[1].push_back(current);
					current.erase(current.end() - 1);
					current.push_back(current.back() - 1);
					sequences[1].push_back(current);
				}
			}
			sequences.erase(sequences.begin());
		}
		for (int k = 0; k < sequences[0].size(); k++) seqs.push_back(sequences[0][k]);
	}

	//only interested in the sequence of PPPPNNPPPNPPNPN
	int hits[15] = { 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0 };

	std::vector<int> primes = p_sieve(maximum);

	for (int i = 0; i < seqs.size(); i++)
	{
		//vfprint(seqs[i]);
		double prob = seqs[i][0];
		for (int j = 1; j < seqs[i].size(); j++) //can't start at 0 index as this is a probability number
		{
			if (primes[seqs[i][j]] == hits[j - 1]) prob *= (2.0 / 3.0);
			else prob *= (1.0 / 3.0);
		}
		answer += prob;
	}

	std::cout << answer / 500 << std::endl;
	std::cout << "Ran in " << double(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;
	return;
}

//Answer is xxx
//Runs in xxx seconds