#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Number Mind
struct ClueState
{
	long long initialGuess;
	int initialCorrectDigits;
	int currentCorrectDigits;
	int digits[16];

	void separateDigits()
	{
		long long initialGuessCopy = this->initialGuess;
		for (int i = 0; i < 16; i++)
		{
			digits[i] = initialGuessCopy % 10;
			initialGuessCopy /= 10;
		}
	}
};

class Puzzle
{
public:
	Puzzle(std::vector<ClueState>& clueStates, std::vector<unsigned int>& possibilities, std::vector<unsigned long long> numberOfExclusions)
	{
		this->clueStates = clueStates;
		this->possibilities = possibilities;
		this->numberOfExclusions = numberOfExclusions;
	}

	bool checkCandidates(int clueNumber, int index)
	{
		//picks the given number from the given clue as a correct number candidate. For a candidate to be 
		//successful, then it can't cause any of the other clues to have too many correct digits.
		int digit = this->clueStates[clueNumber].digits[index];
		for (int i = clueNumber + 1; i < this->clueStates.size(); i++)
		{
			//if (i == clueNumber) continue; //no need to check clue against itself

			if (this->clueStates[i].digits[index] == digit)
			{
				//there's a match so increment the currentCorrectDigits for the given clue
				this->clueStates[i].currentCorrectDigits++;

				//If there are too many correct digits for the current clue then revert all previous
				//clues back to their previous state and return false from this method
				if (this->clueStates[i].currentCorrectDigits > this->clueStates[i].initialCorrectDigits)
				{
					for (int j = i; j >= 0; j--)
					{
						if (j == clueNumber) continue; //once again skip the current clue
						if (this->clueStates[j].digits[index] == digit) this->clueStates[j].currentCorrectDigits--;
					}
					return false;
				}
			}
		}

		//If we get to this part of the method it means the candidate is viable
		return true;
	}

	void removeCandidate(int clueNumber, int index)
	{
		//removes the given number from the given clue as a candidate for success. This means anywhere this number falls into 
		//the same slot of a different clue can have it's currentCorrectDigits value decremented by one. This method is only 
		//to be called after the above checkCandidates() method returns true for the same inputs
		int digit = this->clueStates[clueNumber].digits[index];
		for (int i = 0; i < this->clueStates.size(); i++)
		{
			if (i == clueNumber) continue; //no need to check clue against itself

			if (this->clueStates[i].digits[index] == digit)
			{
				//there's a match so increment the currentCorrectDigits for the given clue
				this->clueStates[i].currentCorrectDigits--;
			}
		}
	}

	bool eliminatePossibilities(int clueNumber, int badDigits)
	{
		//The badDigits variable is a binary number representing digits of the current clue which have been deemed to be bad.
		//This means that all of these digits can be removed from their respective slots of the possibilities array. If any of 
		//the values of the possibility array fall to 0, it means that the current path we're exploring is incorrect so this 
		//method returns false.

		//create a copy of the posibilities array as it currently exists in the case it needs to be reset due to failure.
		std::vector<int> possibilitiesCopy;
		std::vector<unsigned long long> exclusionsCopy;
		for (int i = 0; i < this->possibilities.size(); i++)
		{
			possibilitiesCopy.push_back(possibilities[i]);
			exclusionsCopy.push_back(numberOfExclusions[i]);
		}

		for (int i = 0; i < this->possibilities.size(); i++)
		{
			if (badDigits & powers_of_two[i])
			{
				//remove clueNumber.digit[i] from the array at possibilities[i]
				int digit = clueStates[clueNumber].digits[i];
				possibilities[i] &= ~powers_of_two[digit];
				numberOfExclusions[i] += (1ULL << (4 * digit));
				if (possibilities[i] == 0)
				{
					//it's no longer possible to put any digits in this location, meaning our current guess
					//is wrong. Reset the possibilities array and return false
					for (int j = i; j >= 0; j--)
					{
						this->possibilities[j] = possibilitiesCopy[j];
						this->numberOfExclusions[j] = exclusionsCopy[j];
					}
					return false;
				}
			}
		}

		//if we make it here then there are no issues so return true
		return true;

		//TODO: If any of the values of the possibilities array become a power of 2 it means that there's only one viable
		//number that can go in this spot. Consider adding some auto-solving methods here to speed things up.
	}

	void restorePossibilities(int clueNumber, int badDigits)
	{
		//restores the given digits from the given clue number in the possibilities array
		for (int i = 0; i < this->possibilities.size(); i++)
		{
			if (badDigits & powers_of_two[i])
			{
				int digit = clueStates[clueNumber].digits[i];
				numberOfExclusions[i] -= (1ULL << (4 * digit));
				if ((numberOfExclusions[i] & (15ULL << (4 * digit))) == 0) possibilities[i] |= powers_of_two[digit];
			}
		}
	}

	std::vector<ClueState> clueStates;
	std::vector<unsigned int> possibilities;
	std::vector<unsigned long long> numberOfExclusions;
	int currentGuess[16];
	int totalCorrectGuesses = 0;
	int bestTotalGuesses = 0;
};

bool recursiveNumberBuilder(Puzzle& puzzle, int totalDigits, int ore, int currentClue = 0, int selectedDigits = 0)
{
	if (currentClue == puzzle.clueStates.size())
	{
		//We're at the base of the recursion. Check the current guess of the puzzle and see if 
		//there are any -1 digits. If not then the puzzle is solved. If there are, check the 
		//possibilities array of the puzzle to see if there are any digits that can be put into 
		//the missing slot. If we can add digits to all missing slots then the puzzle is also 
		//solved.
		std::string answer = "";
		for (int i = totalDigits - 1; i >= 0; i--)
		{
			if (puzzle.currentGuess[i] != -1) answer += std::to_string(puzzle.currentGuess[i]);
			else
			{
				if (puzzle.possibilities[i] != 0) answer += std::to_string((int)log2(puzzle.possibilities[i]));
				else return false;
			}
		}

		std::cout << answer << std::endl;
		return true;
	}

	//There are at most 3 digits to pick for any given clue, so instead of coming up with some recursive way to 
	//select all digits, use three nested loops (and only utilize the levels needed).
	int digitsToSelect = puzzle.clueStates[currentClue].initialCorrectDigits - puzzle.clueStates[currentClue].currentCorrectDigits;

	if (digitsToSelect == 0)
	{
		//This clue currently has it's solved digits requirement satisfied from previous clues. This means any digit
		//not currently in the selectedDigits should be considered bad and removed from the puzzle possibilities before
		//recursing down to the next level.
		bool victory = false;
		if (puzzle.eliminatePossibilities(currentClue, ore ^ selectedDigits))
		{
			//recurse down to the next level
			victory = recursiveNumberBuilder(puzzle, totalDigits, ore, currentClue + 1, selectedDigits);
			puzzle.restorePossibilities(currentClue, ore ^ selectedDigits);
		}
		return victory;
	}
	else
	{
		for (int i = 0; i < totalDigits; i++)
		{
			if ((selectedDigits & powers_of_two[i])) continue;
			if (!(puzzle.possibilities[i] & powers_of_two[puzzle.clueStates[currentClue].digits[i]])) continue; //this digit is already in use or can't be used here
			if (!puzzle.checkCandidates(currentClue, i)) continue; //this clue causes discrepencies in the puzzle so skip it
			selectedDigits |= powers_of_two[i]; //add i to the current used digits list
			puzzle.currentGuess[i] = puzzle.clueStates[currentClue].digits[i];
			puzzle.totalCorrectGuesses++;
			puzzle.clueStates[currentClue].currentCorrectDigits++;

			if (digitsToSelect > 1)
			{
				for (int j = i + 1; j < totalDigits; j++)
				{
					if ((selectedDigits & powers_of_two[j])) continue;
					if (!(puzzle.possibilities[j] & powers_of_two[puzzle.clueStates[currentClue].digits[j]])) continue; //this digit is already in use or can't be used here
					if (!puzzle.checkCandidates(currentClue, j)) continue; //this clue causes discrepencies in the puzzle so skip it
					selectedDigits |= powers_of_two[j]; //add i to the current used digits list
					puzzle.currentGuess[j] = puzzle.clueStates[currentClue].digits[j];
					puzzle.totalCorrectGuesses++;
					puzzle.clueStates[currentClue].currentCorrectDigits++;

					if (digitsToSelect > 2)
					{
						for (int k = j + 1; k < totalDigits; k++)
						{
							if ((selectedDigits & powers_of_two[k])) continue;
							if (!(puzzle.possibilities[k] & powers_of_two[puzzle.clueStates[currentClue].digits[k]])) continue; //this digit is already in use or can't be used here
							if (!puzzle.checkCandidates(currentClue, k)) continue; //this clue causes discrepencies in the puzzle so skip it
							selectedDigits |= powers_of_two[k]; //add i to the current used digits list
							puzzle.currentGuess[k] = puzzle.clueStates[currentClue].digits[k];
							puzzle.totalCorrectGuesses++;
							puzzle.clueStates[currentClue].currentCorrectDigits++;

							//Eliminate possibilities base
							if (puzzle.eliminatePossibilities(currentClue, ore ^ selectedDigits))
							{
								//recurse down to the next level
								if (recursiveNumberBuilder(puzzle, totalDigits, ore, currentClue + 1, selectedDigits)) return true;
								puzzle.restorePossibilities(currentClue, ore ^ selectedDigits);
							}

							//remove the current candidate
							puzzle.removeCandidate(currentClue, k);
							selectedDigits ^= powers_of_two[k];
							puzzle.currentGuess[k] = -1;
							puzzle.totalCorrectGuesses--;
							puzzle.clueStates[currentClue].currentCorrectDigits--;
						}
					}
					else
					{
						//Eliminate possibilities base
						if (puzzle.eliminatePossibilities(currentClue, ore ^ selectedDigits))
						{
							//recurse down to the next level
							if (recursiveNumberBuilder(puzzle, totalDigits, ore, currentClue + 1, selectedDigits)) return true;
							puzzle.restorePossibilities(currentClue, ore ^ selectedDigits);
						}
					}

					//remove the current candidate
					puzzle.removeCandidate(currentClue, j);
					selectedDigits ^= powers_of_two[j];
					puzzle.currentGuess[j] = -1;
					puzzle.totalCorrectGuesses--;
					puzzle.clueStates[currentClue].currentCorrectDigits--;
				}
			}
			else
			{
				//Eliminate possibilities base
				if (puzzle.eliminatePossibilities(currentClue, ore ^ selectedDigits))
				{
					//recurse down to the next level
					if (recursiveNumberBuilder(puzzle, totalDigits, ore, currentClue + 1, selectedDigits)) return true;
					puzzle.restorePossibilities(currentClue, ore ^ selectedDigits);
				}
			}

			//remove the current candidate
			puzzle.removeCandidate(currentClue, i);
			selectedDigits ^= powers_of_two[i];
			puzzle.currentGuess[i] = -1;
			puzzle.totalCorrectGuesses--;
			puzzle.clueStates[currentClue].currentCorrectDigits--;
		}
	}
	
	return false;
}

bool recursiveNumberBuilderBad(Puzzle& puzzle, int totalDigits, int ore, int currentClue = 0, int selectedDigits = 0)
{
	if (currentClue == puzzle.clueStates.size())
	{
		//We're at the base of the recursion. Check the current guess of the puzzle and see if 
		//there are any -1 digits. If not then the puzzle is solved. If there are, check the 
		//possibilities array of the puzzle to see if there are any digits that can be put into 
		//the missing slot. If we can add digits to all missing slots then the puzzle is also 
		//solved.
		std::string answer = "";
		for (int i = totalDigits - 1; i >= 0; i--)
		{
			if (puzzle.currentGuess[i] != -1) answer += std::to_string(puzzle.currentGuess[i]);
			else
			{
				if (puzzle.possibilities[i] != 0) answer += std::to_string((int)log2(puzzle.possibilities[i]));
				else return false;
			}
		}

		std::cout << answer << std::endl;
		return true;
	}

	//There are at most 3 digits to pick for any given clue, so instead of coming up with some recursive way to 
	//select all digits, use three nested loops (and only utilize the levels needed).
	int digitsToSelect = puzzle.clueStates[currentClue].initialCorrectDigits - puzzle.clueStates[currentClue].currentCorrectDigits;

	if (digitsToSelect == 0)
	{
		//This clue currently has it's solved digits requirement satisfied from previous clues. This means any digit
		//not currently in the selectedDigits should be considered bad and removed from the puzzle possibilities before
		//recursing down to the next level.
		bool victory = false;
		if (puzzle.eliminatePossibilities(currentClue, ore ^ selectedDigits))
		{
			//recurse down to the next level
			victory = recursiveNumberBuilderBad(puzzle, totalDigits, ore, currentClue + 1, selectedDigits);
			puzzle.restorePossibilities(currentClue, ore ^ selectedDigits);
		}
		return victory;
	}
	else
	{
		for (int i = 0; i < totalDigits; i++)
		{
			if ((selectedDigits & powers_of_two[i])) continue;
			if (!(puzzle.possibilities[i] & powers_of_two[puzzle.clueStates[currentClue].digits[i]])) continue; //this digit is already in use or can't be used here
			if (!puzzle.checkCandidates(currentClue, i)) continue; //this clue causes discrepencies in the puzzle so skip it
			selectedDigits |= powers_of_two[i]; //add i to the current used digits list
			puzzle.currentGuess[i] = puzzle.clueStates[currentClue].digits[i];
			puzzle.totalCorrectGuesses++;
			puzzle.clueStates[currentClue].currentCorrectDigits++;

			//recurse down to the next level while staying on the same clue
			if (recursiveNumberBuilderBad(puzzle, totalDigits, ore, currentClue, selectedDigits)) return true;

			//remove the current candidate
			puzzle.removeCandidate(currentClue, i);
			selectedDigits ^= powers_of_two[i];
			puzzle.currentGuess[i] = -1;
			puzzle.totalCorrectGuesses--;
			puzzle.clueStates[currentClue].currentCorrectDigits--;
		}
	}

	return false;
}

std::pair<std::string, double> q185()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//Copy clues over from the question and order them based on how
	//many correct digits there are. Remove the clue with no right digits
	std::vector<ClueState> clueStates;
	clueStates.push_back({ 2321386104303845, 0 });
	clueStates.push_back({ 5855462940810587, 3 });
	clueStates.push_back({ 9742855507068353, 3 });
	clueStates.push_back({ 7890971548908067, 3 });
	clueStates.push_back({ 4296849643607543, 3 });
	clueStates.push_back({ 8690095851526254, 3 });
	clueStates.push_back({ 1748270476758276, 3 });
	clueStates.push_back({ 3041631117224635, 3 });
	clueStates.push_back({ 1841236454324589, 3 });
	clueStates.push_back({ 5616185650518293, 2 });
	clueStates.push_back({ 4513559094146117, 2 });
	clueStates.push_back({ 2615250744386899, 2 });
	clueStates.push_back({ 6442889055042768, 2 });
	clueStates.push_back({ 2326509471271448, 2 });
	clueStates.push_back({ 5251583379644322, 2 });
	clueStates.push_back({ 2659862637316867, 2 });
	clueStates.push_back({ 3847439647293047, 1 });
	clueStates.push_back({ 3174248439465858, 1 });
	clueStates.push_back({ 8157356344118483, 1 });
	clueStates.push_back({ 6375711915077050, 1 });
	clueStates.push_back({ 6913859173121360, 1 });
	clueStates.push_back({ 4895722652190306, 1 });

	for (int i = 0; i < clueStates.size(); i++) clueStates[i].separateDigits();

	std::vector<unsigned int> possibilities(16, 0b1111111111);
	std::vector<unsigned long long> exclusions(16, 0);

	Puzzle puzzle(clueStates, possibilities, exclusions);

	for (int i = 0; i < 16; i++) puzzle.currentGuess[i] = -1;
	recursiveNumberBuilder(puzzle, 16, MyPow(2, 16) - 1);
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4640261571849533
	//ran in 0.777879 seconds
}

//NOTES
//*notes are now kept on Project-Floyd.com

//With the possibilties array I currently have an array of length 16 where each index contains a binary number telling us which numbers have 
//been eliminated from contention at the specified index. After some debugging I've realized that a single number can be eliminated from contention
//by multiple different clues, so even then we change the selected digit from one clue which might add a number back to the possibilities array, these 
//numbers may still be excluded based on other clues which haven't changed yet. Instead of a binary number (representing a boolean flag for each digit), 
//a hexadecimal number should be used at each location which holds a count of the total number of clues causing an exclusion. This way, when we change 
//up which digit is selected and cause some possibilities to come back, 