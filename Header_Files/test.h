#pragma once

#include <Header_Files/pch.h>

void recursiveTripleQuad(std::vector<std::pair<int, int> >& tripleQuadCandidates, std::vector<std::pair<int, int> >& tripleQuadActual, int currentCombo, int numbersInCombo, int currentLocation, int numberOfNumbers, bool triplePossible)
{
	//base case
	if ((numberOfNumbers == 3 && triplePossible) || numberOfNumbers == 4)
	{
		//we've found a triple or a quad, add it to the tripleQuadActual vector
		tripleQuadActual.push_back({ numbersInCombo, currentCombo });
	}

	//since we need at least a triple, no need to check further in the array than 3 options from the end
	for (int i = currentLocation; i < tripleQuadCandidates.size(); i++)
	{
		int newCombo = currentCombo | tripleQuadCandidates[i].second;
		int bits = 0;
		//count the bits in the combo (unfortunately I only know how to do this 1 bit at a time)
		for (int j = 0; j < 10; j++)
			if (newCombo & powers_of_two[j]) bits++;

		if (bits > 4) continue;

		//if we haven't continued yet then there's a chance for a match here
		recursiveTripleQuad(tripleQuadCandidates, tripleQuadActual, newCombo, numbersInCombo | powers_of_two[tripleQuadCandidates[i].first], i + 1, numberOfNumbers + 1, (triplePossible && (bits <= 3)));
	}
}

std::pair<std::string, double> test()
{
	//ignore these lines, I use them in other header files
	auto run_time = std::chrono::steady_clock::now();
	long long answer = -1;
	/*
	A: [6, 7, 8], 
	B: [7, 8], 
	C: [2], 
	D: [6, 7, 8], 
	E: [3, 4, 6, 8], 
	F: [0, 3, 4, 5, 6, 7, 8, 9]
	G: [6, 7, 8, 9], 
	H: [1], 
	I: [0, 3, 4, 5, 6, 7], 
	J: [0, 3, 4, 6, 8]
	*/

	std::vector<int> options = { 0b0111000000, 0b0110000000, 0b0000000100, 0b0111000000, 0b0101011000, 0b1111111001, 0b1111000000, 0b0000000010, 0b0011111001, 0b0101011001 };
	std::vector<std::pair<int, int> > potential_triples_quads, actual_triples_quads;

	for (int i = 0; i < 10; i++)
	{
		//currently I don't know a good way at a glance to tell how many bits are 1 vs. 0 in a binary number
		//so will have to count manually here
		int bitCount = 0;
		for (int j = 0; j < 10; j++)
			if (options[i] & powers_of_two[j]) bitCount++;

		if ((bitCount == 2 || bitCount == 3) || bitCount == 4)
		{
			int matches = 1;
			for (int j = i + 1; j < 10; j++)
			{
				if (options[i] == options[j]) matches++;
			}

			if (matches == bitCount)
			{
				//we have a double or triple on our hands. Do one final loop through all the letters and if the
				//current letter isn't part of the double or triple then remove those digits
				for (int j = 0; j < 10; j++)
				{
					if (options[i] != options[j])
					{
						std::cout << "yooo" << std::endl;
					}
				}
			}
			else
			{
				//we didn't find any naked doubles, triples or quads so add this letter to the triples_quads vector
				potential_triples_quads.push_back({ i, options[i] });
			}
		}
	}

	recursiveTripleQuad(potential_triples_quads, actual_triples_quads, 0, 0, 0, 0, true);

	for (int i = 0; i < actual_triples_quads.size(); i++)
		std::cout << actual_triples_quads[i].first << ", " << actual_triples_quads[i].second << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)
}
