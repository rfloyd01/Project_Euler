#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <Header_Files/SLL.h>

//Paper sheets of standard sizes: an expected-value problem
struct PaperEnvelope
{
	//A representation of a possible Envelope configuration 
	long long configuration = -1; //tells us the make up of sheets inside the envelope
	long long expected_value = 0; //expected number of times to get 1 sheet from this envelope % mod

	void print(int total_sheets)
	{
		long long shifter = (long long)0x7F << (8 * (total_sheets - 1));
		for (int i = total_sheets - 1; i > 0 ; i--)
		{
			std::cout << ((configuration & shifter) >> (8 * i)) << " ";
			shifter >>= 8;
		}
		std::cout << (configuration & 0x7F) << ": " << expected_value << std::endl;
	}
};

std::ostream& operator<<(std::ostream& os, const PaperEnvelope& env)
{
	return os << "[c: " << env.configuration << ", ev: " << env.expected_value << "]";
}

int EnvelopeBinarySearch(long long configuration, std::vector<PaperEnvelope>& envelopes)
{
	//employs a binary search to quickly find the index of the envelope with the given configuration
	int i = 0, j = envelopes.size() - 1;

	while (i <= j)
	{
		int search_index = (i + j) / 2;
		long long found_configuration = envelopes[search_index].configuration;
		if (found_configuration == configuration) return search_index;
		else if (found_configuration > configuration) j = search_index - 1;
		else i = search_index + 1;
	}

	return -1; //couldn't find the envelope
}

void recursiveCreateEnvelopesInOrder(int max_length, int paper_left, long long current_envelope, int current_level, long long total_sheets, std::vector<PaperEnvelope>& envelope_combinations, long long* adder,
	long long* inverses, long long mod, bool solve = false)
{
	if (!solve)
	{
		//This portion of the function is simply for figuring out the next envelope in the sequence
		if (current_level < 0)
		{
			PaperEnvelope envelope = { current_envelope, 0 };
			envelope_combinations.push_back(envelope);

			//after creating the new envlope, figure out it's expected value by building on values already calculated.
			//This is done by calling the "solve" part of the recursive function
			recursiveCreateEnvelopesInOrder(max_length, paper_left, current_envelope, current_level, total_sheets, envelope_combinations, adder, inverses, mod, true);
		}
		else
		{
			int stop = paper_left / powers_of_two[current_level + 1];
			long long addz = 0;
			for (int i = 0; i <= stop; i++) //add new sheets to the envelope one at a time
			{
				recursiveCreateEnvelopesInOrder(max_length, paper_left - i * powers_of_two[current_level], current_envelope + addz, current_level - 1, total_sheets + i, envelope_combinations, adder, inverses, mod);
				addz += ((long long)1 << (8 * current_level));
			}
		}
	}
	else
	{
		//This portion of the function is for calculating the expected values of already found envelopes
		long long shifter = (long long)0x7F << (8 * (max_length - 1));

		for (int i = max_length - 1; i >= 0; i--)
		{
			//Add the partial value found from the recursion to the current expected value (since the envelopes are calculated in ascending order, removing sheets from 
			//the current envelope will always result in creating an envelope that already has had it's expected value calculated. We cycle through all the 
			//different sheet types in the current envelope and remove a single sheet, calculate the odds of removing this sheet form the envelope and multiply that 
			//by the already calculated expected value of the envelope with this single sheet removed.
			if (current_envelope & shifter) //use binary AND to see if sheets of current size exist in envelope
			{
				//use a binary search to find the index of the already calculated envelope in the envelop_combinations array
				long long previously_calculated_value = envelope_combinations[EnvelopeBinarySearch(current_envelope + adder[i], envelope_combinations)].expected_value;

				//Multiply the previously calculated value by the probability of picking the current sheet from the envelope.
				//We need to give our expected values mod 1,000,000,007 so modular arithmetic is used here.
				
				previously_calculated_value = ModMult(previously_calculated_value, ((current_envelope & shifter) >> (8 * i)), mod);
				previously_calculated_value = ModMult(previously_calculated_value, inverses[total_sheets], mod);
				
				envelope_combinations.back().expected_value = (envelope_combinations.back().expected_value + previously_calculated_value) % mod;
			}
			shifter >>= 8; //shift over to the next paper size
		}

		//Check to see if there's only one sheet in the envelope currently. If there is it means that the calculated expected value
		//will go up by 1 for this envelope configuration
		if (total_sheets == 1) envelope_combinations.back().expected_value += 1;
	}
}

void createEnvelopesInOrder(const int smallest_sheet, long long *adders, std::vector<PaperEnvelope>& envelopeCombinations, long long* inverses, long long mod)
{
	recursiveCreateEnvelopesInOrder(smallest_sheet, powers_of_two[smallest_sheet - 1], 0, smallest_sheet - 1, 0, envelopeCombinations, adders, inverses, mod);
	PaperEnvelope envelope = { (long long)1 << (8 * (smallest_sheet - 1)), 0 };
	envelopeCombinations.push_back(envelope);
	recursiveCreateEnvelopesInOrder(smallest_sheet, 0, envelopeCombinations.back().configuration, smallest_sheet - 1, 1, envelopeCombinations, adders, inverses, mod, true); //last envelope is calculated separately
}

std::pair<std::string, double> Paper_Sheets()
{
	auto run_time = std::chrono::steady_clock::now();

	const int smallest_sheet = 5;
	long long adders[smallest_sheet], inverses[65], subtractor = 0x1, adder = 0, mod = 1000000007;
	for (int i = 0; i < smallest_sheet; i++)
	{
		adders[i] = adder - subtractor;
		adder = (adder << 8) + 1;
		subtractor <<= 8;
	}
	std::vector<PaperEnvelope> possible_envelopes;

	modularMultiplicativeInverseRange(64, mod, inverses);
	createEnvelopesInOrder(smallest_sheet, adders, possible_envelopes, inverses, mod);
	long long answer = possible_envelopes.back().expected_value;

	//Uncomment below line to print out all the possibilities and expected values like HackerRank wants
	//for (int i = 0; i < possible_envelopes.size(); i++) possible_envelopes[i].print(smallest_sheet);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}

//NOTES
/*
* Before adding modular arithmatic I get the following answers and times for A3 - A8 sheets of paper
* A3 = 2.5 (0.0000549s)
* A4 = 2.555556 (0.0000833s)
* A5 = 2.464399 (0.000123s)
* A6 = 2.380889 (0.0001864s)
* A7 = 2.328843 (0.0019154s)
* A8 = 2.297650 (0.0550843s)
*/