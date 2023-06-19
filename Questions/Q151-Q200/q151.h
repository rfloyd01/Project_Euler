#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Paper sheets of standard sizes: an expected-value problem
void recursiveCreateEnvelopesInOrder(int max_length, int paper_left, long long current_envelope, int current_level, long long total_sheets, double* expected_values, long long* adder,
	bool solve = false)
{
	if (!solve)
	{
		//This portion of the function is simply for figuring out the next envelope in the sequence
		if (current_level < 0)
		{
			//After creating the new envlope, figure out it's expected value by building on values already calculated.
			//This is done by calling the "solve" part of the recursive function
			recursiveCreateEnvelopesInOrder(max_length, paper_left, current_envelope, current_level, total_sheets, expected_values, adder, true);
		}
		else
		{
			int stop = paper_left / powers_of_two[current_level + 1];
			long long addz = 0;
			for (int i = 0; i <= stop; i++) //add new sheets to the envelope one at a time
			{
				recursiveCreateEnvelopesInOrder(max_length, paper_left - i * powers_of_two[current_level], current_envelope + addz, current_level - 1, total_sheets + i, expected_values, adder);
				addz += powers_of_ten[current_level];
			}
		}
	}
	else
	{
		//This portion of the function is for calculating the expected values of already found envelopes
		long long shifter = current_envelope;

		for (int i = 0; i < max_length; i++)
		{
			//Find the expected values from all envelopes that can be obtained from the current one by removing
			//a single piece of paper. Divide these partial values by the probability of selecting them and then
			//add all these partial values together to get the expected value for the current envelope.
			int digit = shifter % 10;
			if (digit) //use modulus division to see what sheets of paper are in the envelope
			{
				double previously_calculated_value = expected_values[current_envelope + adder[i]];

				//Multiply the previously calculated value by the probability of picking the current sheet from the envelope.
				previously_calculated_value *= digit;
				previously_calculated_value /= total_sheets;

				expected_values[current_envelope] += previously_calculated_value;
			}
			shifter /= 10; //shift over to the next paper size
		}

		//Check to see if there's only one sheet in the envelope currently. If there is it means that the calculated expected value
		//will go up by 1 for this envelope configuration
		if (total_sheets == 1) expected_values[current_envelope] += 1;
	}
}

std::pair<std::string, double> q151()
{
	auto run_time = std::chrono::steady_clock::now();

	const int smallest_sheet = 5;
	long long adders[smallest_sheet], subtractor = 1, adder = 0;

	//Memoize the amounts to subtract from the envelope configurations when removing a single sheet of paper.
	//For example, if we have a single sheet of A3, A4, and A5 paper the configuration will be 111. Removing the sheet
	//of A3 will cause the amount of A4 and A5 paper to increase by one, so overallthe configuration number changes by
	//11 - 100 = -89. This allows for quick math later on.
	for (int i = 0; i < smallest_sheet; i++)
	{
		adders[i] = adder - subtractor;
		adder = adder * 10 + 1;
		subtractor *= 10;
	}

	double expected_values[1112] = { 0 };
	recursiveCreateEnvelopesInOrder(smallest_sheet, powers_of_two[smallest_sheet - 1], 0, smallest_sheet - 1, 0, expected_values, adders);

	return { std::to_string(expected_values[1111] - 1), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 0.464399
	//ran in 0.0000247 seconds
}

//NOTES
/*
*/