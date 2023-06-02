#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/SLL.h>

//Paper sheets of standard sizes: an expected-value problem
int distinct_envelopes = 0;

struct PaperEnvelope
{
	//A representation of a possible Envelope configuration 
	long long configuration = -1; //tells us the make up of sheets inside the envelope
	//int expected_value = 0; //expected number of times to get 1 sheet from this envelope % mod
	double expected_value = -1; //DEBUG, for now just use a double to make sure answers look right
	//int mod = 1000000007; //amount by which to mod the expected value by to prevent overflow

	//Potentially overload std::cout << here to use the SSL print method
};

std::ostream& operator<<(std::ostream& os, const PaperEnvelope& env)
{
	return os << "[c: " << env.configuration << ", ev: " << env.expected_value << "]";
}

std::pair<bool, SinglyLinkedListNode<PaperEnvelope>*> FindEnvelope(int configuration, SinglyLinkedList<PaperEnvelope>* envelopeList)
{
	//This function looks to see if certain envelope configuration exists in the list
	//of envelopes passed to the function. If it does exist, then this function returns a true
	//value as well as a pointer to the envelope in the list. If it doesn't exist, this function
	//returns false, as well as a pointer to the configuration that would come directly before 
	//the new configuration in the list. This way, we can choose to add the new configuration
	//in numeric order if we want.
	SinglyLinkedListNode<PaperEnvelope>* currentEnvelope = envelopeList->head;
	if (currentEnvelope == nullptr || currentEnvelope->value.configuration > configuration) return { false, nullptr }; //any new nodes will become the head

	while (currentEnvelope->next != nullptr)
	{
		if (currentEnvelope->next->value.configuration == configuration) return { true, currentEnvelope->next };
		if (currentEnvelope->next->value.configuration > configuration) break;
		currentEnvelope = currentEnvelope->next; //move ahead in the list
	}

	//if we make it to the end of the list
	return { false, currentEnvelope };
}

SinglyLinkedListNode<PaperEnvelope>* AddEnvelope(int newConfiguration, SinglyLinkedListNode<PaperEnvelope>* location, SinglyLinkedList<PaperEnvelope>* envelopeList)
{
	//adds a reference to the new envelope to the spot in an existing list right in front of
	//the current location.
	if (location == nullptr)
	{
		envelopeList->addToFront({ newConfiguration });
		return envelopeList->head;
	}
	else
	{
		envelopeList->addNext(location, { newConfiguration });
		return location->next;
	}
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

//double recursiveSheetSelectWithProbabilityHack(int* current_envelope, int current_sheets, SinglyLinkedList<PaperEnvelope>* solved_envelopes, int total_sheets, const int smallest_sheet)
//{
//	auto is_solved_envelope = FindEnvelope(current_sheets, solved_envelopes);
//	if (!is_solved_envelope.first)
//	{
//		distinct_envelopes++;
//		//We haven't explored this route yet so we keep recursing downwards. The idea of the recursion is that at each 
//		//level you can only take a single sheet of paper, so we take one sheet and do that whole recursion. After returning
//		//we put that piece of paper back and go forwards in the envelope array, taking the next sheet and recursion downwards
//		//again. All of the recursions initiated at this level will contribute to the number of hits and misses that make up 
//		//the pair which will go in the discovered ways array.
//
//		//First, we need to create the new envelope in the appropriate spot of the linked list, once the recursion is done we 
//		//can than update its expected value. We store a pointer to this new envelope to quickly update it later.
//		auto new_envelope_location = AddEnvelope(current_sheets, is_solved_envelope.second, solved_envelopes);
//
//		for (int i = smallest_sheet - 1; i >= 0; i--)
//		{
//			if (current_envelope[i])
//			{
//				//remove a sheet of paper from the current envelope column, and then add a sheet of paper to all 
//				//subsequent columns. Then recurse down.
//				current_envelope[i]--;
//				current_sheets -= powers_of_ten[i];
//				total_sheets--;
//
//				for (int j = i - 1; j >= 0; j--)
//				{
//					current_envelope[j]++;
//					current_sheets += powers_of_ten[j];
//					total_sheets++;
//				}
//
//				double recurse = recursiveSheetSelectWithProbabilityHack(current_envelope, current_sheets, solved_envelopes, total_sheets, smallest_sheet);
//
//				//After returning from the recursion we need to reset the envelope array before the next iteration, and then 
//				//add the results from the recursion
//				for (int j = i - 1; j >= 0; j--)
//				{
//					current_envelope[j]--;
//					current_sheets -= powers_of_ten[j];
//					total_sheets--;
//				}
//
//				current_envelope[i]++;
//				current_sheets += powers_of_ten[i];
//				total_sheets++;
//
//				recurse *= current_envelope[i];
//				recurse /= total_sheets;
//
//				//When adding the number of paths and hits that were found, we multiply be the number of sheets of paper
//				//that are in the column. In theory any of these sheets could've been picked and not changed the outcome
//				//so we need to ensure that all possible paths are explored.
//				new_envelope_location->value.expected_value += recurse;
//			}
//		}
//
//		//Check to see if there's only one sheet in the envelope currently, it means that the calculated expected value
//		//will go up by 1 for this envelope configuration
//		if (total_sheets == 1) new_envelope_location->value.expected_value += 1;
//		return new_envelope_location->value.expected_value;
//	}
//	else return is_solved_envelope.second->value.expected_value;
//}

double recursiveSheetSelectWithProbabilityHack(long long* adder, long long current_sheets, std::vector<PaperEnvelope>& envelopes_combinations, int total_sheets, const int smallest_sheet)
{
	int current_envelope_index = EnvelopeBinarySearch(current_sheets, envelopes_combinations); //should always return a valid index
	if (envelopes_combinations[current_envelope_index].expected_value < 0)
	{
		//We haven't calculated the expected value for this envelope yet so we keep recursing downwards. The idea of the recursion is that at each 
		//level you can only take a single sheet of paper, so we take one sheet and do that whole recursion. After returning
		//we put that piece of paper back and go forwards in the envelope array, taking the next sheet and recursion downwards
		//again. All of the recursions initiated at this level will contribute to the number of hits and misses that make up 
		//the pair which will go in the discovered ways array.
		envelopes_combinations[current_envelope_index].expected_value = 0; //set to 0 before starting expected value calculation
		long long shifter = (long long)0x7F << (8 * (smallest_sheet - 1));

		for (int i = smallest_sheet - 1; i >= 0; i--)
		{
			if (current_sheets & shifter) //use binary AND to see if sheets of current size exist in envelope
			{
				//remove a sheet of paper from the current envelope column, and then add a sheet of paper to all 
				//subsequent columns. Then recurse down.
				current_sheets += adder[i]; //removes one sheet form current size and adds one of all lesser sizes
				total_sheets += (i - 1);

				double recurse = recursiveSheetSelectWithProbabilityHack(adder, current_sheets, envelopes_combinations, total_sheets, smallest_sheet);

				//After returning from the recursion we need to reset the envelope array before the next iteration, and then 
				//add the results from the recursion
				current_sheets -= adder[i];
				total_sheets -= (i - 1);

				//Modify the value returned from the recursion by the probability of picking the current sheet from the envelope
				recurse *= ((current_sheets & shifter) >> (8 * i));
				recurse /= total_sheets;

				//Add the partial value found from the recursion to the current expected value
				envelopes_combinations[current_envelope_index].expected_value += recurse;
			}
			shifter >>= 8; //shift over to the next paper size
		}

		//Check to see if there's only one sheet in the envelope currently. If there is it means that the calculated expected value
		//will go up by 1 for this envelope configuration
		if (total_sheets == 1) envelopes_combinations[current_envelope_index].expected_value += 1;
	}
	return envelopes_combinations[current_envelope_index].expected_value;
}

//void recursiveCreateEnvelopesInOrder(int max_length, int paper_left, long long current_envelope, int current_level, std::vector<PaperEnvelope>& envelopeCombinations)
//{
//	if (current_level < 0)
//	{
//		//std::cout << current_envelope << std::endl;
//		PaperEnvelope envelope = { current_envelope };
//		envelopeCombinations.push_back(envelope);
//		distinct_envelopes++;
//	}
//	else
//	{
//		int stop = paper_left / powers_of_two[current_level + 1];
//		long long adder = 0;
//		for (int i = 0; i <= stop; i++)
//		{
//			recursiveCreateEnvelopesInOrder(max_length, paper_left - i * powers_of_two[current_level], current_envelope + adder, current_level - 1, envelopeCombinations);
//			adder += ((long long)1 << (8 * current_level));
//		}
//	}
//}

void recursiveCreateEnvelopesInOrder(int max_length, int paper_left, long long current_envelope, int current_level, int total_sheets, std::vector<PaperEnvelope>& envelope_combinations, long long* adder, bool solve = false)
{
	if (!solve)
	{
		//This portion of the function is simply for figuring out the next envelope in the sequence
		if (current_level < 0)
		{
			PaperEnvelope envelope = { current_envelope, 0.0 };
			envelope_combinations.push_back(envelope);

			//after creating the new envlope, figure out it's expected value by building on values already calculated.
			//This is done by calling the "solve" part of the recursive function
			recursiveCreateEnvelopesInOrder(max_length, paper_left, current_envelope, current_level, total_sheets, envelope_combinations, adder, true);
		}
		else
		{
			int stop = paper_left / powers_of_two[current_level + 1];
			long long addz = 0;
			for (int i = 0; i <= stop; i++) //add new sheets to the envelope one at a time
			{
				recursiveCreateEnvelopesInOrder(max_length, paper_left - i * powers_of_two[current_level], current_envelope + addz, current_level - 1, total_sheets + i, envelope_combinations, adder);
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
				double previously_calculated_value = envelope_combinations[EnvelopeBinarySearch(current_envelope + adder[i], envelope_combinations)].expected_value;

				//Multiply the previously calculated value by the probability of picking the current sheet from the envelope
				previously_calculated_value *= ((current_envelope & shifter) >> (8 * i));
				previously_calculated_value /= total_sheets;
				
				envelope_combinations.back().expected_value += previously_calculated_value;
			}
			shifter >>= 8; //shift over to the next paper size
		}

		//Check to see if there's only one sheet in the envelope currently. If there is it means that the calculated expected value
		//will go up by 1 for this envelope configuration
		if (total_sheets == 1) envelope_combinations.back().expected_value += 1;
	}
}

void createEnvelopesInOrder(const int smallest_sheet, long long *adders, std::vector<PaperEnvelope>& envelopeCombinations)
{
	recursiveCreateEnvelopesInOrder(smallest_sheet, powers_of_two[smallest_sheet - 1], 0, smallest_sheet - 1, 0, envelopeCombinations, adders);
	PaperEnvelope envelope = { (long long)1 << (8 * (smallest_sheet - 1)), 0 };
	envelopeCombinations.push_back(envelope);
	recursiveCreateEnvelopesInOrder(smallest_sheet, 0, envelopeCombinations.back().configuration, smallest_sheet - 1, 1, envelopeCombinations, adders, true); //last envelope is calculated separately
}

std::pair<std::string, double> Paper_Sheets()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//const int smallest_sheet = 6;
	//int total_sheets = 1;

	//int current_envelope[smallest_sheet] = { 0 };
	//current_envelope[smallest_sheet - 1] = 1; //we always start with a single sheet of A1 paper

	//SinglyLinkedList<PaperEnvelope> checkedEnvelopes;
	//recursiveSheetSelectWithProbabilityHack(current_envelope, powers_of_ten[smallest_sheet - 1], &checkedEnvelopes, total_sheets, smallest_sheet);
	////checkedEnvelopes.printList();
	//auto yeet = checkedEnvelopes.head;
	//while (yeet != nullptr)
	//{
	//	std::cout << yeet->value.configuration << std::endl;
	//	yeet = yeet->next;
	//}

	std::cout << std::hex;
	const int smallest_sheet = 8;
	long long adders[smallest_sheet], subtractor = 0x1, adder = 0;
	for (int i = 0; i < smallest_sheet; i++)
	{
		adders[i] = adder - subtractor;
		adder = (adder << 8) + 1;
		subtractor <<= 8;
	}
	std::vector<PaperEnvelope> possible_envelopes;

	createEnvelopesInOrder(smallest_sheet, adders, possible_envelopes); //TODO: With this method the recursion below is no longer necessary, eventually move all logic here
	
	//std::cout << EnvelopeBinarySearch(possible_envelopes.back().configuration, possible_envelopes) << std::endl;
	//recursiveSheetSelectWithProbabilityHack(adders, possible_envelopes.back().configuration, possible_envelopes, 1, smallest_sheet);

	std::cout << possible_envelopes.back() << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 0.464399
	//ran in 0.0000343 seconds

	//0x100000000000000
	//0x100000000000000
}

//NOTES
/*
* 3 -> 6
* 4 -> 18
* 5 -> 87
* 6 -> 642
* 7 -> 5873
*/