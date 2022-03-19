#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <vector>
#include<set>
#include <fstream>

//Kakuro

//Classes and Structs for Problem
class BoardTile
{
public:
	std::string pieceType = "Board";
	virtual void printPossibilities() {};
	bool numberFound(int* possibilities)
	{
		//loops through the given possibility array to see if only one of the digits 0-9 has a value greater than -1
		bool found = false;
		for (int i = 0; i < 10; i++)
		{
			if (*(possibilities + i) != -1)
			{
				if (found) return false;
				found = true;
			}
		}
		return true;
	}
	int id;
};
class ClueTile : public BoardTile
{
public:
	//vertical and horizontal hold the numeric values for the clues, if a clue hasn't been solved yet the value will be -1,
	//if the clue doesn't exist the value is -2, if the clue has been partially solved (for two digit clues) then the value
	//will be -3
	int vertical, horizontal;
	int verticalAnswerLength, horizontalAnswerLength;
	std::vector<int> verticalPossibilities, horizontalPossibilities; //represents the possibilities for each clue
	char vert1, vert2, hor1, hor2; //represent the clue letters

	//Constructors
	ClueTile()
	{
		//default constructor for ClueTile
		this->vertical = -1;
		this->horizontal = -1;
		this->pieceType = "Clue";
		//Z will represent that this part of the clue isn't present
		this->vert1 = 'Z';
		this->vert2 = 'Z';
		this->hor1 = 'Z';
		this->hor2 = 'Z';
		this->horizontalAnswerLength = 0;
		this->verticalAnswerLength = 0;
		//negative numbers mean that this type of clue isn't present
		this->vertical = -2;
		this->horizontal = -2;
	}
	ClueTile(bool v, bool h) : ClueTile()
	{
		//negative numbers mean that this type of clue isn't present
		if (v) this->vertical = -1;
		if (h) this->horizontal = -1;
	}
};
class AnswerTile : public BoardTile
{
public:
	int possibilities, answer;
	char clue;
	ClueTile* horizontalClue, *verticalClue; //every answer tile is part of a horizontal and vertical clue

	AnswerTile()
	{
		this->pieceType = "Answer";
		this->clue = 'O';
		this->answer = -1;
		this->possibilities = 0b1111111110; //0 isn't a possibility for answer tiles
	}

	void remove(int rem)
	{
		possibilities ^= (possibilities & rem);
		if (!(possibilities & (possibilities - 1))) answer = log(possibilities) / log(2);
	}

	int getLowest()
	{
		//returns the lowest number still available as a possibility.
		return log(possibilities & -possibilities) / log(2);
	}

	int getHighest()
	{
		//returns the highest number still available as a possibility
		return log(possibilities) / log(2);
	}

	void printPossibilities()
	{
		//prints all posibilities for each digit of both the horizontal and vertical clues
		std::cout << "Answer Tile Possibilities: ";
		for (int i = 0; i < 10; i++) if (possibilities & powers_of_two[i]) std::cout << i << " ";
		std::cout << std::endl;
	}
};
struct LetterPossibility
{
	int size = 10;
	int possibilities = 0b1111111111;
	int answer = -1;

	void solve(int i)
	{
		possibilities = powers_of_two[i];
		size = 1;
		answer = i;
	}

	int getLowest()
	{
		//returns the lowest number still available as a possibility.
		return log(possibilities & -possibilities) / log(2);
	}

	int getHighest()
	{
		//returns the highest number still available as a possibility
		return log(possibilities) / log(2);
	}

	void remove(int rem)
	{
		possibilities ^= (possibilities & rem);
		if (!(possibilities & (possibilities - 1))) answer = log(possibilities) / log(2);
	}

	void print()
	{
		std::string answerString = "[";
		for (int i = 0; i < 10; i++)
		{
			if (possibilities & powers_of_two[i])
			{
				answerString += std::to_string(i);
				answerString += ", ";
			}
		}
		answerString.pop_back();
		answerString.pop_back();
		answerString += ']';
		std::cout << answerString << std::endl;
	}
};

//Function to create all ways of creating numbers in Kakuro (limited to maximum of 6 digits)
 void kakuroNumberCombinations(std::vector<std::vector<std::pair<int, std::vector<std::vector<int> > > > >& combos)
{
	//this function creates all of the possible Kakuro combos (where the max clue length is 6) and stores them in a comlex vector.
	//The strucutre of the vector looks like this:
	/*
	* { {0 digit numbers (blank)},
	*   {1 digit numbers --> {{}, {{1}}, {{2}}, ... {{9}}},
	*   {2 digit numbers --> {{}, {}, {}, {ways to make 3 0b0110000000 {1, 2}}, {ways to make 4{1, 3}}, { ways to make 5{1, 4}, {2, 3}}, ... {ways to make 17{8, 9}}},
	*   .....
	*   {6 digit numbers --> {{}, {}, {}, .... {ways to make 21 {1, 2, 3, 4, 5, 6}}, {ways to make 27 {2, 3, 4, 5, 6, 7}, {1, 3, 4, 5, 6, 8}, ...} ...}
	* }
	*/

	//in front of each vector will be a binary number representing all of digits that make up the different ways of making a number out of a certain number of digits.
	//for example there are two ways to make 5 when using only two digits: (1, 4) or (2, 3). Therefore the digits 1, 2, 3 and 4 could potentially be used to create 5.
	//This means that the binary number in front of the two digit '5' vector would be 0b0111100000 = 30. A value of 1022 would mean that every digit 1-9 could be used
	//to make the current number at the current length

	//Nothing fancy about this algorithm, it just keeps adding numbers wherever possible. The hardest part is creating and iterating through the data structure
	//std::vector<std::vector<std::pair<int, std::vector<std::vector<int> > > > > combos;
	for (int i = 0; i < 7; i++)
	{
		std::vector<std::pair<int, std::vector<std::vector<int> > > > numberOfDigits;
		combos.push_back(numberOfDigits);
	}

	int rowMaximum = 0, currentNum = 9;
	for (int i = 1; i < 7; i++)
	{
		//we start at 1 because we can't make any 0 digit numbers. Technically we can't use any 1 digit numbers on a sudoko board, however, we build off of them
		//to get our actual combinations.
		
		rowMaximum += currentNum--;
		for (int j = 0; j <= rowMaximum; j++)
		{
			std::pair<int, std::vector<std::vector<int > > > actualNumber;
			actualNumber.first = 0; //binary number representation will always start at 0
			combos[i].push_back(actualNumber);
		}
	}

	//set all of the necessary 1 digit building blocks
	for (int i = 1; i <= 9; i++)
	{
		combos[1][i].first |= powers_of_two[i];
		combos[1][i].second.push_back({ i });
	}

	//now iterate over every combo from the 1 digit row to the 5 digit row, adding single digits that are greater than current last digit
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 0; j < combos[i].size(); j++)
		{
			if (combos[i][j].second.size() == 0) continue; //no need to check any combos that aren't possible
			for (int k = 0; k < combos[i][j].second.size(); k++)
			{
				int kBinary = 0;
				for (int l = 0; l < combos[i][j].second[k].size(); l++) kBinary |= powers_of_two[combos[i][j].second[k][l]];
				for (int l = combos[i][j].second[k].back() + 1; l <= 9; l++)
				{
					//create a copy of the existing vector each time we add a new digit to it
					std::vector<int> copy = combos[i][j].second[k];
					copy.push_back(l);
					combos[i + 1][j + l].second.push_back(copy);
					combos[i + 1][j + l].first |= (kBinary | powers_of_two[l]);
				}
			}
		}
	}

	//Comment the below out once I've confirmed everything is working
	/*for (int i = 2; i <= 6; i++)
	{
		std::cout << i << "-digit combinations:\n";
		for (int j = 0; j < combos[i].size(); j++)
		{
			if (combos[i][j].second.size() == 0) continue;
			std::cout << j << " (" << combos[i][j].first << "):\n";
			for (int k = 0; k < combos[i][j].second.size(); k++) vprint(combos[i][j].second[k]);
			std::cout << std::endl;
		}
	}*/

	//return &combos;
}

//Functions for Board Manipulation
void createBoard(BoardTile*** board, std::string& boardString, std::vector<std::pair<int, int> > &clueLocations, int dimension)
{
	int currentIndex = 2; //always start scanning right after the first comma
	for (int i = 0; i < dimension; i++)
	{
		BoardTile** tiles = new BoardTile * [dimension]; //must use the new keyword here as we don't know the dimension at compile time
		*(board + i) = tiles;
		bool stop = false;
		for (int j = 0; j < dimension; j++)
		{
			//any given tile can be a plain tile (represented by an X), answer tile (represented by a 0 or letter A-J) are a clue tile (represented by a '(' ).
			//If it's one of the first two options tile creation is easy as we only need a single character. The clue tiles though we will need to scann character
			//by character until hitting the closing ')'.
			BoardTile* newTile;

			//check to see if this is the very last tile
			if ((i == (dimension - 1)) && (j == (dimension - 1))) stop = true;

			if (boardString[currentIndex] == 'X')
			{
				//create a new blank tile and move on
				newTile = new BoardTile();
				currentIndex += 2;
			}
			else if (boardString[currentIndex] == '(')
			{
				bool horizontal = false, vertical = false;

				//at the clue location to the clueLocation vector
				std::pair<int, int> clueLocation = { i, j };
				clueLocations.push_back(clueLocation);

				//we need to scan until hitting a closing ')' to figure out which clues are present
				int k = currentIndex;
				while (boardString[++k] != ')')
				{
					if (boardString[k] == 'h') horizontal = true;
					else if (boardString[k] == 'v') vertical = true;
				}

				//create a new clue tile based on the clues present
				ClueTile* ct = new ClueTile(vertical, horizontal);
				currentIndex++; //increment off of the '(' character

				if (boardString[currentIndex] == 'h')
				{
					horizontal = true;
					//if this is a two digit clue then two digits away won't be a comma
					if (boardString[currentIndex + 2] == ',' || boardString[currentIndex + 2] == ')')
					{
						//only add the first digit
						//ct->hor1 = '0'; //in this case 0 represents no digit here
						ct->hor2 = boardString[currentIndex + 1];
						currentIndex += 3; //increment to the next part of the clue
					}
					else
					{
						//add both digits
						ct->hor1 = boardString[currentIndex + 1]; //in this case 0 represents no digit here
						ct->hor2 = boardString[currentIndex + 2];
						currentIndex += 4; //increment to the next part of the clue
					}
				}

				//at this point the letter at the current index will either be a 'v', or a ','
				if (boardString[currentIndex] == 'v')
				{
					//same deal here, need to check if we have 1 or 2 digits to the clue
					if (boardString[currentIndex + 2] == ')')
					{
						//only add the first digit
						//ct->vert1 = '0'; //in this case 0 represents no digit here
						ct->vert2 = boardString[currentIndex + 1];
						currentIndex += 4; //increment to the next part of the clue
					}
					else
					{
						//add both digits
						ct->vert1 = boardString[currentIndex + 1]; //in this case 0 represents no digit here
						ct->vert2 = boardString[currentIndex + 2];
						currentIndex += 5; //increment to the next part of the clue
					}
				}
				else currentIndex++; //move off the comma and go to next tile

				//after parsing the clue, set the newTile pointer
				newTile = ct;
			}
			else
			{
				//create a new answer tile
				AnswerTile* at = new AnswerTile();
				at->clue = boardString[currentIndex];
				newTile = at;
				currentIndex += 2;
			}

			newTile->id = dimension * i + j; //keep track of each tiles location on the board
			*(tiles + j) = newTile; //add the new tile to the board

			if (stop) break; //break out of loop once we hit the last tile
		}
	}
}
void initialClueSolver(BoardTile*** board, int dimension, std::vector<std::pair<int, int> > &clueLocations, std::vector<LetterPossibility> &letterPossibilities)
{
	//this function is used for the initial look at our clues to see if we can convert any letters right off the bat. It should only be called one
	//time, as soon as the board has been created.

	//use a set to track which characters are at the front of double digit clues
	std::set<char> leadCharacters;
	bool oneFound = false, twoFound = false, threeFound = false;

	for (int i = 0; i < clueLocations.size(); i++)
	{
		ClueTile *currentClue = (ClueTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second)); //this is some of my craziest pointer dereferencing
		AnswerTile* currentAnswer;
		//for each clue tile, set the length of any corresponding horizontal and vertical answers. Also assign the horizontal and vertical clue tiles for each 
		//answer tile on the board
		if (currentClue->horizontal != -2)
		{
			int clueLength = 1;
			while (true)
			{
				if (clueLocations[i].second + clueLength == dimension) break;
				if ((*(*(board + clueLocations[i].first) + clueLocations[i].second + clueLength))->pieceType != "Answer") break;
				currentAnswer = (AnswerTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second + clueLength));
				currentAnswer->horizontalClue = currentClue;
				clueLength++;
			}
			currentClue->horizontalAnswerLength = clueLength - 1;
		}
		if (currentClue->vertical != -2)
		{
			int clueLength = 1;
			while (true)
			{
				if (clueLocations[i].first + clueLength == dimension) break;
				if ((*(*(board + clueLocations[i].first + clueLength) + clueLocations[i].second))->pieceType != "Answer") break;
				currentAnswer = (AnswerTile*)(*(*(board + clueLocations[i].first + clueLength) + clueLocations[i].second));
				currentAnswer->verticalClue = currentClue;
				clueLength++;
			}
			currentClue->verticalAnswerLength = clueLength - 1;
		}

		if (currentClue->hor1 != 'Z')
		{
			leadCharacters.insert(currentClue->hor1);
			//letterPossibilities[currentClue->hor1 - 'A'].remove({ 0, 4, 5, 6, 7, 8, 9 }); //this letter must be a 1, 2 or 3
			letterPossibilities[currentClue->hor1 - 'A'].remove(0b1111110001); //this letter must be a 1, 2 or 3
			//carry out a check here to see if there are only 2 answer tiles for this clue, if so, then the first digit as a 1
			if (currentClue->horizontalAnswerLength == 2)
			{
				letterPossibilities[currentClue->hor1 - 'A'].solve(1);
				//we can also remove 8 and 9 as options from the second letter (assuming it isn't the same as the first letter)
				//if (currentClue->hor2 != currentClue->hor1) letterPossibilities[currentClue->hor2 - 'A'].remove({ 8, 9 });
				if (currentClue->hor2 != currentClue->hor1) letterPossibilities[currentClue->hor2 - 'A'].remove(0b1100000000);
				oneFound = true;
			}
			else if (currentClue->horizontalAnswerLength == 3)
			{
				//letterPossibilities[currentClue->hor1 - 'A'].remove({ 3 }); //too short to start with a 3
				letterPossibilities[currentClue->hor1 - 'A'].remove(0b0000001000); //too short to start with a 3
				if (letterPossibilities[currentClue->hor1 - 'A'].answer == 2)
				{
					//if we've already found which letter 1 is then eliminating 3 here means the answer must be 2
					//if the answer block only has a length of three and the first digit is 2 then the second digit can only be 0, 1, 2, 3 or 4
					//because the maximum 3 digit number starting with 2 is 9+8+7 = 24;
					//letterPossibilities[currentClue->hor2 - 'A'].remove({ 5, 6, 7, 8, 9 });
					letterPossibilities[currentClue->hor2 - 'A'].remove(0b1111100000);
				}
			}
			else if (currentClue->horizontalAnswerLength == 6)
			{
				//with an answer of this length (which should be possible on a 7x7 board) we can eliminate 1 as a possibility as the smallest number we can 
				//make is 1+2+3+4+5+6 = 21
				//letterPossibilities[currentClue->hor1 - 'A'].remove({ 1 });
				letterPossibilities[currentClue->hor1 - 'A'].remove(0b0000000010);
			}
		}
		//else if (currentClue->hor2 != 'Z') letterPossibilities[currentClue->hor2 - 'A'].remove({ 0, 1, 2 }); //single digit clues can't be 0, 1 or 2
		else if (currentClue->hor2 != 'Z') letterPossibilities[currentClue->hor2 - 'A'].remove(0b0000000111); //single digit clues can't be 0, 1 or 2

		if (currentClue->vert1 != 'Z')
		{
			leadCharacters.insert(currentClue->vert1);
			//letterPossibilities[currentClue->vert1 - 'A'].remove({ 0, 4, 5, 6, 7, 8, 9 }); //this letter must be a 1, 2 or 3
			letterPossibilities[currentClue->vert1 - 'A'].remove(0b1111110001);
			//carry out a check here to see if there are only 2 answer tiles for this clue, if so, then the first digit as a 1
			if (currentClue->verticalAnswerLength == 2)
			{
				letterPossibilities[currentClue->vert1 - 'A'].solve(1);
				//we can also remove 8 and 9 as options from the second letter (assuming it isn't the same as the first letter)
				//if (currentClue->vert2 != currentClue->vert1) letterPossibilities[currentClue->vert2 - 'A'].remove({ 8, 9 });
				if (currentClue->vert2 != currentClue->vert1) letterPossibilities[currentClue->vert2 - 'A'].remove(0b1100000000);
				oneFound = true;
			}
			else if (currentClue->verticalAnswerLength == 3)
			{
				letterPossibilities[currentClue->vert1 - 'A'].remove(0b0000001000); //too short to start with a 3
				if (letterPossibilities[currentClue->vert1 - 'A'].answer == 2)
				{
					//if we've already found which letter 1 is then eliminating 3 here means the answer must be 2
					//if the answer block only has a length of three and the first digit is 2 then the second digit can only be 0, 1, 2, 3 or 4
					//because the maximum 3 digit number starting with 2 is 9+8+7 = 24;
					letterPossibilities[currentClue->vert2 - 'A'].remove(0b1111100000);
				}
			}
			else if (currentClue->verticalAnswerLength == 6)
			{
				//with an answer of this length (which should be possible on a 7x7 board) we can eliminate 1 as a possibility as the smallest number we can 
				//make is 1+2+3+4+5+6 = 21
				letterPossibilities[currentClue->vert1 - 'A'].remove(0b0000000010);
			}
		}
		else if (currentClue->vert2 != 'Z') letterPossibilities[currentClue->vert2 - 'A'].remove(0b0000000111); //single digit clues can't be 0, 1 or 2
	}

	//TODO: I think I can safely remove this if statement
	if (leadCharacters.size() == 3)
	{
		//we can eliminate 1, 2, and 3 as possibilities for each letter not in the set and eliminate 0 + 4-9 as possibilities for each letter
		//in the set.
		for (int i = 0; i < 10; i++)
		{
			if (leadCharacters.find((char)i + 'A') != leadCharacters.end())
			{
				if (letterPossibilities[i].size > 1)
				{
					//if (oneFound) letterPossibilities[i].remove({ 0, 1, 4, 5, 6, 7, 8, 9 });
					if (oneFound) letterPossibilities[i].remove(0b1111110011);
					//else letterPossibilities[i].remove({ 0, 4, 5, 6, 7, 8, 9 });
					else letterPossibilities[i].remove(0b1111110001);
				}
			}
			//else letterPossibilities[i].remove({ 1, 2, 3 });
			else letterPossibilities[i].remove(0b0000001110);
		}
	}

	//If the 1 has been found then there's a good chance we can figure out if we have either a 2 or 3 as the leading digit for other double digit clues
	//If one of the other DD clues only has an answer length of 3 then it must start with a 2 (at least 4 numbers are necessary to reach a sum
	//of 30). Another check we can do, if there are two clues that start with the same letter and have answer length of 4 but have different second letters
	//we can guarantee the first letter should be a 2. This is because there's only one way to reach 30 with 4 numbers. Check to see if we've discovered
	//the 2 digit yet, if not, procede with the below logic
	std::set<char> fourDigits[10]; //create an array of 10 character sets
	if (oneFound && !twoFound)
	{
		//in case it hasn't already, remove 1 as an option from anything that hasn't been solved yet
		for (int i = 0; i < 10; i++)
		{
			//if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 1 });
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove(0b0000000010);
			if (letterPossibilities[i].size == 1 && letterPossibilities[i].answer == 2)
			{
				twoFound = true;
				break;
			}
		}

		for (int i = 0; i < clueLocations.size(); i++)
		{
			ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second)); //this is some of my craziest pointer dereferencing
			if (currentClue->horizontal != -2)
			{
				if (currentClue->hor1 != 'Z')
				{
					if (letterPossibilities[currentClue->hor1 - 'A'].answer != 1)
					{
						if (currentClue->horizontalAnswerLength == 3)
						{
							//this clue must start with a 2 as we've already discovered the '1' and there aren't enough boxes to reach 30
							letterPossibilities[currentClue->hor1 - 'A'].solve(2);
							twoFound = true;
							break; //break out of current for loop
						}
						else if (currentClue->horizontalAnswerLength == 4)
						{
							fourDigits[currentClue->hor1 - 'A'].insert(currentClue->hor2);
							if (fourDigits[currentClue->hor1 - 'A'].size() > 1)
							{
								//this clue must start with a 2 as we've already discovered the '1' and there are multiple clues of length 4 that start with this
								//character but have a different second character
								letterPossibilities[currentClue->hor1 - 'A'].solve(2);
								twoFound = true;
								break; //break out of current for loop
							}
						}
					}
				}
			}
			if (currentClue->vertical != -2)
			{
				if (currentClue->vert1 != 'Z')
				{
					if (letterPossibilities[currentClue->vert1 - 'A'].answer != 1)
					{
						if (currentClue->verticalAnswerLength == 3)
						{
							//this clue must start with a 2 as we've already discovered the '1' and there aren't enough boxes to reach 30
							letterPossibilities[currentClue->vert1 - 'A'].solve(2);
							twoFound = true;
							break; //break out of current for loop
						}
						else if (currentClue->verticalAnswerLength == 4)
						{
							fourDigits[currentClue->vert1 - 'A'].insert(currentClue->vert2);
							if (fourDigits[currentClue->vert1 - 'A'].size() > 1)
							{
								//this clue must start with a 2 as we've already discovered the '1' and there are multiple clues of length 4 that start with this
								//character but have a different second character
								letterPossibilities[currentClue->vert1 - 'A'].solve(2);
								twoFound = true;
								break; //break out of current for loop
							}
						}
					}
				}
			}
		}
	}

	if (oneFound && twoFound)
	{
		//in case it hasn't already, remove 2 as an option from anything that hasn't been solved yet
		for (int i = 0; i < 10; i++)
		{
			//if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 2 });
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove(0b0000000100);
			if (letterPossibilities[i].answer == 3)
			{
				threeFound = true;
				break;
			}
		}
	}

	bool zeroFound = false;
	if (threeFound)
	{
		//in case it hasn't already, remove 3 as an option from anything that hasn't been solved yet
		for (int i = 0; i < 10; i++)
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 3 });

		//another thing we can check for if the 3 has been found, look at any double digit clues starting with 3.
		//If these clues have a length of 4 then the second character of the clue MUST be a 0. The only 4 digit answer
		//in the 30's is 30
		for (int i = 0; i < clueLocations.size(); i++)
		{
			ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second));
			if (currentClue->horizontal != -2)
			{
				if (currentClue->hor1 != 'Z')
				{
					if (letterPossibilities[currentClue->hor1 - 'A'].answer == 3)
					{
						if (currentClue->horizontalAnswerLength == 4)
						{
							letterPossibilities[currentClue->hor2 - 'A'].solve(0);
							zeroFound = true;
							break; //break out of current for loop
						}
					}
				}
			}
			if (currentClue->vertical != -2)
			{
				if (currentClue->vert1 != 'Z')
				{
					if (letterPossibilities[currentClue->vert1 - 'A'].answer == 3)
					{
						if (currentClue->verticalAnswerLength == 4)
						{
							letterPossibilities[currentClue->vert2 - 'A'].solve(0);
							zeroFound = true;
							break; //break out of current for loop
						}
					}
				}
			}
		}
	}

	//in case it hasn't already, remove 0 as an option from anything that hasn't been solved yet
	if (zeroFound)
		for (int i = 0; i < 10; i++)
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove(0b0000000001);

	//Temporary printing function
	/*std::cout << "Board after initial clue check:\n";
	for (int i = 0; i < 10; i++)
	{
		std::cout << (char)(i + 'A') << ": ";
		letterPossibilities[i].print();
	}*/
}
void recursiveElimination(int location, int& lowest, int& highest, std::vector<std::pair<std::vector<int>, std::vector<int> > >& letterHeirarchy, std::vector<LetterPossibility>& letterPossibilities)
{
	if (letterHeirarchy[location].first.size() == 0)
	{
		//this is our base letter, when we first call the function we set the lowest value
		lowest = letterPossibilities[location].getLowest();

		//first recursively call this function on anything in the LARGER vector
		for (int i = 0; i < letterHeirarchy[location].second.size(); i++) recursiveElimination(letterHeirarchy[location].second[i], lowest, highest, letterHeirarchy, letterPossibilities);

		//then remove everything from possibilites that's greater than or equal to the 'highest' variable
		letterPossibilities[location].remove((0b1111111111 << highest) & 0b1111111111); //if highest = 6 then binary number in remove() would be 0b111100000
	}
	else if (letterHeirarchy[location].second.size() == 0)
	{
		//this is our largest letter, set the highest value (but only if it's not higher than the current highest (we visit some nodes multiple times so we don't
		//want to raise the highest variable)
		highest = letterPossibilities[location].getHighest() >= highest ? highest : letterPossibilities[location].getHighest();

		//then remove everything from possibilities anything that's less than or equal to the 'lowest' variable
		letterPossibilities[location].remove(0b1111111111 >> (10 - (lowest + 1)));
	}
	else
	{
		//this letter falls somewhere in the middle of the heirarchy. We need the ability to set both the 'lowest' and the
		//'highest' variable from here

		//first, remove everything from possibilities that's less than or equal to the 'lowest' variable
		letterPossibilities[location].remove(0b1111111111 >> (10 - (lowest + 1)));

		//now change the lowest variable so that it matches the lowest value of this letter
		lowest = letterPossibilities[location].getLowest();

		//now recursively call everything higher than this letter
		for (int i = 0; i < letterHeirarchy[location].second.size(); i++) recursiveElimination(letterHeirarchy[location].second[i], lowest, highest, letterHeirarchy, letterPossibilities);

		//remove everything from possibilites that's greater than or equal to the 'highest' variable
		letterPossibilities[location].remove((0b1111111111 << highest) & 0b1111111111); //if highest = 6 then binary number in remove() would be 0b111100000

		//finally, set the 'highest' variable equal to the highest possibility of this letter
		highest = letterPossibilities[location].getHighest();
	}
}
void numberHeirarchy(BoardTile*** board, int dimension, std::vector<std::pair<int, int> >& clueLocations, std::vector<LetterPossibility>& letterPossibilities, std::vector<std::pair<std::vector<int>, std::vector<int> > > letterHeirarchy, bool &heirarchyCreated)
{
	//This function attempts to create a heirarcy between letters, it does this by looking at single digit clues that have a letter in one of their answer boxes.
	//In the example puzzle we know that B < H because the H clue at location (1, 1) has B as a component. In other words, B + something else = H.
	//Then looking down at the D clue at location (2, 1) we can see that H is a component of it, therefore, H < D. So from looking at just these
	//two clues we can see that B < H < D. We can also see that I < A for the same reason when looking at clue (3, 0) . In the example we were able to
	//solve numbers 0-3 just from looking at the two digit clues, which means the other letters must be 4-9. Since H must be larger than B, H CAN'T be 4 and
	//likewise D CAN'T be 4 OR 5. On the flipside, this also means that B CAN'T be 8 or 9 while H also CAN'T be a 9. These relationships go for I and A as well.

	//To keep track of these relationships an array of pairs of vectors is used. The array will have a length of 10 where each element represents a single letter.
	//Each element will contain a pair of vectors. The first vector will hold all letters that are known to have a value LESS than the current letter while the second
	//vector will hold all letters that are known to have a value MORE than the current letter. The first time we call this function the heirarccy hasn't been created yet
	//so we implement the following code block
	if (!heirarchyCreated)
	{
		for (int i = 0; i < clueLocations.size(); i++)
		{
			ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second)); //this is some of my craziest pointer dereferencing
			if (currentClue->hor1 == 'Z' && currentClue->hor2 != 'Z')
			{
				//we've found a single digit clue, scan all if it's associated answer blocks to see if any letters are found
				for (int j = 1; j <= currentClue->horizontalAnswerLength; j++)
				{
					AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second + j));
					if (currentAnswer->clue != 'O')
					{
						letterHeirarchy[currentClue->hor2 - 'A'].first.push_back(currentAnswer->clue - 'A');  //add the found letter to the less than part of the current clue
						letterHeirarchy[currentAnswer->clue - 'A'].second.push_back(currentClue->hor2 - 'A'); //add the current clue to the more than part of the found letter

						//we also loop through all of the letters in the found letter's less than vector and add to the current clue's vector
						for (int k = 0; k < letterHeirarchy[currentAnswer->clue - 'A'].first.size(); k++)
						{
							letterHeirarchy[currentClue->hor2 - 'A'].first.push_back(letterHeirarchy[currentAnswer->clue - 'A'].first[k]);
							letterHeirarchy[letterHeirarchy[currentAnswer->clue - 'A'].first[k]].second.push_back(currentClue->hor2 - 'A');
						}
					}
				}
			}
			if (currentClue->vert1 == 'Z' && currentClue->vert2 != 'Z')
			{
				//we've found a single digit clue, scan all if it's associated answer blocks to see if any letters are found
				for (int j = 1; j <= currentClue->verticalAnswerLength; j++)
				{
					AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + clueLocations[i].first + j) + clueLocations[i].second));
					if (currentAnswer->clue != 'O')
					{
						letterHeirarchy[currentClue->vert2 - 'A'].first.push_back(currentAnswer->clue - 'A');  //add the found letter to the less than part of the current clue
						letterHeirarchy[currentAnswer->clue - 'A'].second.push_back(currentClue->vert2 - 'A'); //add the current clue to the more than part of the found letter

						//we also loop through all of the letters in the found letter's less than vector and add to the current clue's vector
						for (int k = 0; k < letterHeirarchy[currentAnswer->clue - 'A'].first.size(); k++)
						{
							letterHeirarchy[currentClue->vert2 - 'A'].first.push_back(letterHeirarchy[currentAnswer->clue - 'A'].first[k]);
							letterHeirarchy[letterHeirarchy[currentAnswer->clue - 'A'].first[k]].second.push_back(currentClue->vert2 - 'A');
						}
					}
				}
			}
		}
		heirarchyCreated = true;
	}

	//Now that are heirarchy table has been created we can try eliminate a few options for the letters in the table. A recursive function to do this is defined
	//above as recursiveElimination(). We scan the table to find base numbers (i.e. a number with nothing lower than it, but does have numbers higher then it)
	//and then call the recursive function at these locations
	for (int i = 0; i < 10; i++)
	{
		int lowest = -1, highest = 100; //pick an arbitrarily low and high number
		if (letterHeirarchy[i].first.size() == 0 && letterHeirarchy[i].second.size() > 0)
		{
			recursiveElimination(i, lowest, highest, letterHeirarchy, letterPossibilities);
		}
	}
	
	/*std::cout << "Board after letter heirarchy check:\n";
	for (int i = 0; i < 10; i++)
	{
		std::cout << (char)(i + 'A') << ": ";
		letterPossibilities[i].print();
	}*/
}
void printBoard(BoardTile*** board, int dimension)
{
	//prints out the kakuro board representation (using initial letters)
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if ((*(*(board + i) + j))->pieceType == "Board") std::cout << "X ";
			else if ((*(*(board + i) + j))->pieceType == "Answer")
			{
				AnswerTile* yo = (AnswerTile*)(*(*(board + i) + j));
				std::cout << yo->clue << " ";
			}
			else
			{
				ClueTile* yo = (ClueTile*)(*(*(board + i) + j));
				std::cout << '(';
				if (yo->horizontal != -2)
				{
					std::cout << 'h';
					if (yo->hor1 != '0') std::cout << yo->hor1;
					std::cout << yo->hor2;
					if (yo->vertical != -2) std::cout << ',';
					else std::cout << ") ";
				}
				if (yo->vertical != -2)
				{
					std::cout << 'v';
					if (yo->vert1 != '0') std::cout << yo->vert1;
					std::cout << yo->vert2;
					std::cout << ") ";
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void clueLetters(BoardTile*** board, int dimension, std::vector<std::pair<int, int> >& clueLocations, std::vector<LetterPossibility>& letterPossibilities)
{
	//attempts to solve clues with the current state of the letterPossibilities. iterate over all the clues and see which ones have
	//been solved or partially solved and then fill out the possibility vectors for any unsolved clues
	for (int i = 0; i < clueLocations.size(); i++)
	{
		ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocations[i].first) + clueLocations[i].second));

		//only look at existing clues that haven't been solved
		if (currentClue->horizontal == -3 || currentClue->horizontal == -1)
		{
			for (int j = 0; j < 10; j++)
			{
				if (currentClue->hor1 != 'Z' && !(powers_of_two[j] & letterPossibilities[currentClue->hor1 - 'A'].possibilities)) continue;

				for (int k = 0; k < 10; k++)
					if (powers_of_two[k] & letterPossibilities[currentClue->hor2 - 'A'].possibilities) currentClue->horizontalPossibilities.push_back(10 * j + k);

				if (currentClue->hor1 == 'Z') break;
			}

			if (currentClue->horizontalPossibilities.size() == 1) currentClue->horizontal = currentClue->horizontalPossibilities[0]; //this clue is solved

			/*std::cout << "Horizontal Partial Possibilites: ";
			vprint(currentClue->horizontalPossibilities);*/
		}

		//only look at existing clues that haven't been solved
		if (currentClue->vertical == -3 || currentClue->vertical == -1)
		{
			for (int j = 0; j < 10; j++)
			{

				if (currentClue->vert1 != 'Z' && !(powers_of_two[j] & letterPossibilities[currentClue->vert1 - 'A'].possibilities)) continue;

				for (int k = 0; k < 10; k++)
					if (powers_of_two[k] & letterPossibilities[currentClue->vert2 - 'A'].possibilities) currentClue->verticalPossibilities.push_back(10 * j + k);

				if (currentClue->vert1 == 'Z') break;
			}

			if (currentClue->verticalPossibilities.size() == 1) currentClue->vertical = currentClue->verticalPossibilities[0]; //this clue is solved

			/*std::cout << "Vertical Partial Possibilites: ";
			vprint(currentClue->verticalPossibilities);*/
		}
	}
}
void answerTrimViaClue(BoardTile*** board, int dimension, std::vector<LetterPossibility>& letterPossibilities, std::vector<std::vector<std::pair<int, std::vector<std::vector<int> > > > >& combos)
{
	//In this function we visit every AnswerTile on the board and try to reduce the total number of digits that can go inside of them. This is done by A) looking at the
	//horizontal and vertical clues that govern the box and B) looking at any letters actually placed in these AnswerTiles at the start of the puzzle.
	//For example, if we figured out that one of the clues was 12, and there were two answer boxes making up that clue's answer then our options to fill in the three boxes
	//would be (9, 3), (8, 4) and (7, 5). At a glance we can see that the numbers 1, 2, 3 and 6 couldn't be utilized here.
	AnswerTile* currentAnswer;
	BoardTile* boardTile;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			
			//I don't have the answer tiles saved into an array so we just scan the whole board.
			boardTile = (*(*(board + i) + j));
			if (boardTile->pieceType == "Answer")
			{
				currentAnswer = (AnswerTile*)boardTile;
				if (currentAnswer->answer > 0) continue; //no need to do anything with a tile we've already solved

				//first look to see if the answer tile has a letter associated with it. We use the bitwise ~ to reverse all the bits because we want to 
				//remove anything that ISN'T an option. The & 0b1111111111 is to ensure our number doesn't wrap around to a negative value
				if (currentAnswer->clue != 'O') currentAnswer->remove(~letterPossibilities[currentAnswer->clue - 'A'].possibilities & 0b1111111111);

				//Look at all options in the horizontal clue possibilities list and compare with the combos vector for the given clue length
				if (currentAnswer->horizontalClue->horizontalPossibilities.size() > 0)
				{
					//Create a combined binary number of what's allowed and then remove anything not allowed. For example,
					//if we knew that the clue had a length of 2 and had the potential of being 14, 15 or 16, we'd combine the allowable digit binary numbers
					//from these three possibilities. 14 with 2 digits can be (9, 5) or (8, 6), 15 can be (9, 6) or (8, 7) and 16 can be (9, 7). Combining these
					//possibilities means that we can have 5, 6, 7, 8 or 9 in the boxes.
					int allowableDigits = 0;
					for (int k = 0; k < currentAnswer->horizontalClue->horizontalPossibilities.size(); k++)
						allowableDigits |= combos[currentAnswer->horizontalClue->horizontalAnswerLength][currentAnswer->horizontalClue->horizontalPossibilities[k]].first;

					currentAnswer->remove(~allowableDigits & 0b1111111111); //remove digits that aren't allowed from the combined allowedDigits
				}

				//Same as the above block, but for the vertical clue
				if (currentAnswer->verticalClue->verticalPossibilities.size() > 0)
				{
					int allowableDigits = 0;
					for (int k = 0; k < currentAnswer->verticalClue->verticalPossibilities.size(); k++)
						allowableDigits |= combos[currentAnswer->verticalClue->verticalAnswerLength][currentAnswer->verticalClue->verticalPossibilities[k]].first;

					currentAnswer->remove(~allowableDigits & 0b1111111111); //remove digits that aren't allowed from the combined allowedDigits
				}

				//std::cout << "Answer Tile Location: (" << i << ", " << j << ")\n";
				//std::cout << "Possibilities";
				//currentAnswer->printPossibilities();
				//std::cout << std::endl;
			}
		}
	}
}
void recursiveAnswerElimination(BoardTile*** board, int&clueSize, std::pair<int, int> location, std::vector<int>& possibilities, std::vector<int>& selectedNumbers, int currentNumber, int currentSum, bool horizontal, int& goal, int level = 0)
{
	if (level == clueSize)
	{
		//base of recursion. If we made it here then all numbers in the selected numbers array will work.
		//Update the possibilities vector accordingly
		if (currentSum == goal)
			for (int i = 0; i < clueSize; i++) possibilities[i] |= selectedNumbers[i];

		return;
	}

	AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + location.first) + location.second));
	for (int i = currentAnswer->getLowest(); i <= currentAnswer->getHighest(); i++)
	{
		//select any possible number that won't make us go over the goal
		if (powers_of_two[i] & currentAnswer->possibilities) //make sure that the number can go in the box
		{
			if (!(powers_of_two[i] & currentNumber)) //make sure we haven't already selected the number in a previous box
			{
				if (!(currentSum + i > goal)) //make sure adding this number won't exceed the goal
				{
					selectedNumbers[level] = powers_of_two[i]; //add number to selected vector
					recursiveAnswerElimination(board, clueSize, {location.first + !horizontal, location.second + horizontal}, possibilities, selectedNumbers,
						currentNumber | powers_of_two[i], currentSum + i, horizontal, goal, level + 1);
					selectedNumbers[level] = 0; //remove number from selected vector
				}
			}
		}
	}
}
void answerTrimViaAnswer(BoardTile*** board, std::pair<int, int> clueLocation)
{
	ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocation.first) + clueLocation.second));
		
	if (currentClue->horizontal != -2)
	{
		std::vector<int> possibilities, currentSelection;
		for (int i = 0; i < currentClue->horizontalAnswerLength; i++)
		{
			possibilities.push_back(0);
			currentSelection.push_back(0);
		}

		//call the recursive function on every number in the clue's possibility vector.
		for (int i = 0; i <= currentClue->horizontalPossibilities.size(); i++)
		{
			recursiveAnswerElimination(board, currentClue->horizontalAnswerLength, { clueLocation.first, clueLocation.second + 1 }, possibilities,
				currentSelection, 0, 0, 1, currentClue->horizontalPossibilities[i]);
		}

		//set the possibility values for each of the answer blocks after the recursive function calls are complete
		for (int i = 1; i <= currentClue->horizontalAnswerLength; i++)
		{
			AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + clueLocation.first) + clueLocation.second + i));
			/*std::cout << "Pre Answer Tile Location: (" << clueLocation.first << ", " << clueLocation.second + i << ")\n";
			currentAnswer->printPossibilities();
			std::cout << std::endl;*/
			currentAnswer->possibilities = possibilities[i - 1];
			/*std::cout << "Post Answer Tile Location: (" << clueLocation.first << ", " << clueLocation.second + i << ")\n";
			currentAnswer->printPossibilities();
			std::cout << std::endl;*/
		}
	}

	if (currentClue->vertical != -2)
	{
		std::vector<int> possibilities, currentSelection;
		for (int i = 0; i < currentClue->verticalAnswerLength; i++)
		{
			possibilities.push_back(0);
			currentSelection.push_back(0);
		}

		//call the recursive function on every number in the clue's possibility vector.
		for (int i = 0; i <= currentClue->verticalPossibilities.size(); i++)
		{
			recursiveAnswerElimination(board, currentClue->verticalAnswerLength, { clueLocation.first + 1, clueLocation.second }, possibilities,
				currentSelection, 0, 0, 0, currentClue->verticalPossibilities[i]);
		}

		//set the possibility values for each of the answer blocks after the recursive function calls are complete
		for (int i = 1; i <= currentClue->verticalAnswerLength; i++)
		{
			AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + clueLocation.first + i) + clueLocation.second));
			/*std::cout << "Pre Answer Tile Location: (" << clueLocation.first + i << ", " << clueLocation.second << ")\n";
			currentAnswer->printPossibilities();
			std::cout << std::endl;*/
			currentAnswer->possibilities = possibilities[i - 1];
			/*std::cout << "Post Answer Tile Location: (" << clueLocation.first + i << ", " << clueLocation.second << ")\n";
			currentAnswer->printPossibilities();
			std::cout << std::endl;*/
		}
	}
}
bool recursiveClueElimination(BoardTile*** board, int& clueSize, std::pair<int, int> location, int currentNumber, int usedNumbers, int goal, bool horizontal, int level = 0)
{
	if (level == clueSize)
	{
		//base of recursion. If we made it here check to see if we've hit the goal.
		//If so then set the 
		if (currentNumber == goal)  return true;
		return false;
	}

	AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + location.first) + location.second));
	for (int i = currentAnswer->getLowest(); i <= currentAnswer->getHighest(); i++)
	{
		if (powers_of_two[i] & usedNumbers) continue; //don't pick a numbers that's been picked already
		if (i + currentNumber > goal) break; //numbers will only get higher so break out of loop

		if (recursiveClueElimination(board, clueSize, { location.first + !horizontal, location.second + horizontal }, currentNumber + i, usedNumbers | powers_of_two[i],
			goal, horizontal, level + 1)) return true;
	}
	return false; //if we haven't returned true by the end of the loop, then return false
}
void clueTrimViaAnswer(BoardTile*** board, std::vector<std::pair<int, int> >& clueLocations, std::vector<LetterPossibility>& letterPossibilities)
{
	//very similar to the answerTrimViaClue function, just the opposite. We look at every possible value for the current clue (if it hasn't been solved yet)
	//and then do a brute force recursion to see if it's possible to arrive at the given value. If not, then that value is removed from the
	//clue's possibility vector. If any clues have their possibility vectors reduced to a length of 1 then we've discovered new letter answers
	for (int clue = 0; clue < clueLocations.size(); clue++)
	{
		ClueTile* currentClue = (ClueTile*)(*(*(board + clueLocations[clue].first) + clueLocations[clue].second));
		bool removed = false;
		std::cout << clueLocations[clue].first << ", " << clueLocations[clue].second << std::endl;

		if (currentClue->horizontal == -1 || currentClue->horizontal == -3) //only look at unsolved or partially solved clues
		{
			//call the recursive function on every number in the clue's possibility vector.
			std::cout << currentClue->horizontalPossibilities.size() << std::endl;
			for (int i = 0; i <= currentClue->horizontalPossibilities.size(); i++)
			{
				if (!recursiveClueElimination(board, currentClue->horizontalAnswerLength, { clueLocations[clue].first, clueLocations[clue].second + 1 }, 0, 0,
					currentClue->horizontalPossibilities[i], true))
				{
					currentClue->horizontalPossibilities.erase(currentClue->horizontalPossibilities.begin() + i);
					removed = true;
				}
			}
			if (removed)
			{
				//if we ended up eliminating anything then we need to update the letterPossibilities array
				int clueLength = currentClue->horizontalPossibilities[0] < 10 ? 1 : 2;

				//check the one's place
				int onesPossibilities = 0, tensPossibilities = 0;
				for (int i = 0; i < currentClue->horizontalPossibilities.size(); i++)
				{
					onesPossibilities |= powers_of_two[currentClue->horizontalPossibilities[i] % 10];
					if (clueLength == 2) tensPossibilities |= powers_of_two[currentClue->horizontalPossibilities[i] / 10];
				}

				//remove anything from contention that isn't in either possibilities binary number
				letterPossibilities[currentClue->hor2 - 'A'].remove(~onesPossibilities & 0b1111111111);
				if (clueLength == 2) letterPossibilities[currentClue->hor1 - 'A'].remove(~tensPossibilities & 0b1111111111);

				removed = false; //reset this variable before doing vertical clue checks
			}
		}

		if (currentClue->vertical == -1 || currentClue->vertical == -3) //only look at unsolved or partially solved clues
		{
			//call the recursive function on every number in the clue's possibility vector.
			for (int i = 0; i <= currentClue->verticalPossibilities.size(); i++)
			{
				if (!recursiveClueElimination(board, currentClue->verticalAnswerLength, { clueLocations[clue].first + 1, clueLocations[clue].second }, 0, 0,
					currentClue->verticalPossibilities[i], false))
				{
					currentClue->verticalPossibilities.erase(currentClue->verticalPossibilities.begin() + i);
					removed = true;
				}
			}
			if (removed)
			{
				//if we ended up eliminating anything then we need to update the letterPossibilities array
				int clueLength = currentClue->verticalPossibilities[0] < 10 ? 1 : 2;

				//check the one's place
				int onesPossibilities = 0, tensPossibilities = 0;
				for (int i = 0; i < currentClue->verticalPossibilities.size(); i++)
				{
					onesPossibilities |= powers_of_two[currentClue->verticalPossibilities[i] % 10];
					if (clueLength == 2) tensPossibilities |= powers_of_two[currentClue->verticalPossibilities[i] / 10];
				}

				//remove anything from contention that isn't in either possibilities binary number
				letterPossibilities[currentClue->vert2 - 'A'].remove(~onesPossibilities & 0b1111111111);
				if (clueLength == 2) letterPossibilities[currentClue->vert1 - 'A'].remove(~tensPossibilities & 0b1111111111);
			}
		}
	}
}

std::pair<std::string, double> q424()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//Per the problem text there are 200 games, we need to read in each board 1 at a time, convert the string into a physical game board
	//and then solve it before moving onto the next board
	std::ifstream myFile;
	std::string boardString;
	myFile.open("Resources/q424.txt");

	//before looping through all the puzzles, create a complex vector storing all the different ways to solve (up to 6 digit) Kakuro clues
	std::vector<std::vector<std::pair<int, std::vector<std::vector<int> > > > > combinations;
	kakuroNumberCombinations(combinations);

	//TODO: Change this loop from 1 to 200 after working out puzzle solving logic
	for (int i = 0; i < 1; i++)
	{
		//Read the next board from text file
		std::getline(myFile, boardString);

		//Create variables for board creation
		int dimension = boardString[0] - '0';
		BoardTile*** board = new BoardTile**[dimension]; //we won't know the dimension until we read it from file so we initialize the board using pointers
		std::vector<std::pair<int, int> > clueLocations; //an array holding the location of the clues in the puzzle for faster lookup
		std::vector<LetterPossibility> letterPossibilities; //represents the available numbers that each letter can represent, the puzzle is solved when each vector has length of 1
		std::vector<std::pair<std::vector<int>, std::vector<int> > > letterHeirarchy;
		bool heirarchyCreated = false;
		for (int i = 0; i < 10; i++)
		{
			LetterPossibility lp;
			letterPossibilities.push_back(lp);

			std::pair<std::vector<int>, std::vector<int> > lh;
			letterHeirarchy.push_back(lh);
		}

		//Create the board from the input string
		createBoard(board, boardString, clueLocations, dimension);

		//Attempt to solve some values for characters based on two digit clues, also assign clue tiles to each answer tile
		initialClueSolver(board, dimension, clueLocations, letterPossibilities);

		//Set the initial number heirarchy and try to eliminate some more options
		numberHeirarchy(board, dimension, clueLocations, letterPossibilities, letterHeirarchy, heirarchyCreated);

		//This function attempts to put numbers to the individual letters of the puzzle
		clueLetters(board, dimension, clueLocations, letterPossibilities);

		//Now that the board is set up and we've done our initial analyses on it, it's time to go into a loop that executes until
		//the puzzle is solved. On each iteration of the loop we keep track of whether or not anything has changed. If we make it 
		//through an iteration without anything changing then a function is called to force pick an answer to see if it let's us
		//solve the puzzle (or reach an unsolvable state)
		for (int j = 0; j < 1; j++)
		{
			//Attempt to trim the list of possible numbers for each answer tile on the board by looking at the clues that dictate
			//the answer tiles.
			answerTrimViaClue(board, dimension, letterPossibilities, combinations);

			//Now we use a brute force recursive algorithm to see if each of the possible numbers for every answer tile can actually be
			//used in a solution. Any numbers that aren't found to be part of a solution are removed from the possibility list.
			for (int i = 0; i < clueLocations.size(); i++) answerTrimViaAnswer(board, { clueLocations[i].first, clueLocations[i].second });

			//In step two of this loop we were able to get information on the available numbers that go in the answer tiles by looking at the clues for
			//each answer tile. Well now we do the reverse. Since the numbers have (hopefully) been trimmed down in the answer boxes, we should in theory
			//be able to trim some of the possible clues that dictate these answer boxes.
			clueTrimViaAnswer(board, clueLocations, letterPossibilities);
		}
		
		std::cout << "Letter Possibilities after initial setup and checks:\n";
		for (int i = 0; i < 10; i++)
		{
			std::cout << (char)(i + 'A') << ": ";
			letterPossibilities[i].print();
		}

		std::cout << "Current Answer Tile Possibilities:";
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				if ((*(*(board + i) + j))->pieceType == "Answer")
				{
					AnswerTile* currentAnswer = (AnswerTile*)(*(*(board + i) + j));

					std::cout << "Tile (" << i << ", " << j << "): ";
					currentAnswer->printPossibilities();
				}
			}
			std::cout << std::endl;
		}

		
		
		//printBoard(board, dimension);
		
		
		delete[] board; //delete the board after each iteration to save on memory
	}

	myFile.close(); //close down the file after reading all boards
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds (this is current run time when goal is set to 10e14)
}

//NOTES
/*
initial thoughts:
At first I didn't think this looked to bad, just need to write a program that can solve 5x5 and 6x6 kakuro puzzles. A little more in-depth reading, however, showed that not only
do we need to fill in the puzzle, but we actually need to figure out the clues as well! That's interesting. I know that in Kakuro the maximum sum you can have for a row or column
is 45 (1-9). This means that the first digit of any clue that has double digits can't be 0 or 5-9. So that's a start for solving the clues. Likewise, none of the letters that make up
the single digit clues can be 0. We also know that 0 isn't allowed to go into any of the actual puzzle boxes. This means that the only place in which 0 could concievably go is the second
digit of one of the two digit clues. The question prompt does mention that only 9 of the 10 letters are guaranteed to show up so it's always possible that 0 won't even appear anywhere in
the puzzle at all.

Now that I think about it a little bit more, the largest grid size id 6x6 and we need at least 1 box in any row or column to hold clues. This means that the largest number string will be 5,
aka the maximum sum we can actually get is 35 (9+8+7+6+5) which means that the first digit of double digit clues can only be 1, 2 or 3. A little more thought shows that if we have a clue
which is two digits long and there are only two boxes available then the first digit MUST be a 1. The largest sum with two numbers is 9 + 8 = 17. Likewise, we must have at least 4 boxes for
it to be possible to have the first digit be a 3. There are only six ways to have the first digit be a three, 9+8+7+6 or any of 9+8+7+6+(5-1). This means that if there are only 4 boxes in the
row and and 1 has already been found to be less than 6, it's impossible for the first digit to be a 3.

Applying some of the above logic to the given example puzzle, we instantly can see that C, J and F have to be 1, 2 and 3 (but not in that order). This is because all of the 2 digit clues start
with these letters. Looking further, there are some clues that start with J and have a length of two, this means that J = 1. The rest of the two digit clues all have a length of 4. We know that
there's only 1 way to have the first digit be 3 when the length is 4, however, there are multiple clues of length 4 that start with a C. This means that C has to be 2 and then by process of
elimination F has to be 3. So we already have 3 numbers decoded just from looking at the two digit clues (although I doubt it will be this easy in each puzzle). My next step would probably be
to elimate all of the things that can't be 0. Right off the bat know that B, I and H can't because they're in the puzzle, and obviously C, J and F can't because we've already found those to be
1, 2 and 3. This only leaves A, D, E and G. It can't be A or D because those appear as single digit clues, which leaves us with only E or G as options. G doesn't appear anywhere in the puzzle
so it'll be hard to test that out, so we really just have E to look at. A little thought here shows us that E MUST be 0, because F is 3. If E was something else, like a 4, then we'd have a clue
of 34 with length of 4 which isn't possible. We know have digits 0-3 which is almost half way there!

Something else to point out at the onset here, there are 3 different types of tiles that appear on the board: Clue tiles, answer tiles and blank tiles. Ideally I'd like a single multi dimensional
array that can keep track of the whole board, however, it might get a little confusing using standard vectors and arrays to try and accomplish this. I've decided that I'll create three different
kinds of classes. The first class will just be called BoardTile and it will represent a generic gray tile with nothing in it. There will then be two different child classes that inherit from this
base one, AnswerTile and ClueTile. Each answer tile will have an integer value that starts at 0 and a 10 digit array where each number starts with a value of i. As numbers get eliminated from
contention for going in a certain answer tile, that corresponding digit will be set to a value of -1 in the array. When only one element of the array has a non-negative value this means we've 
discovered the correct answer for the tile and it will be set in the integer value. We can tell at a glance if we've solved a tile or not by seeing if the "answer" field is greater than 0. The ClueTile
will be similar, however, will feature four separate arrays. Each of these arrays will represent a digit of the horizontal clue and the vertical clue for the tile. Once the clue value has been
figured out then the integer values for horizontal and vertical will be filled in. Not all clue tiles have both horizontal and vertical clues. In this case, if one of the clues doesn't exist,
the integer value will be set to -2. The integer values will be set to -1 until the clue is solved. Both the ClueTile and AnswerTile classes can be upcast to the BoardTile class so everything
can be kept in the same array.

I can already see that this problem is going to be pretty darn complex. The way that I would LIKE to approach it is to first figure out the values for all the clues, then try to solve the puzzle.
I don't think it will be this cut and dry though as solving some of the clues will probably rely on putting actual numbers into the table. So here's kind of what I have in mind for a flow to start
off:

1. Start off by looking at all of the two digit clues
	- If the answer length is 2, then we know the first clue digit is a 1
	- If the answer length is 3, then we know the first clue digit can't be a 3
	- If the answer length is 4 or 5, the first digit can be a 1, 2 or a 3 so we'll need to look at the other two digit clues for answers
		- If there are two separate answers where the length is exactly 4 that have the same first letter but different second letters, the first letter can't be 3
	- If there are 3 different letters that start the two digit clues then we can isolate digits 1-3 to only three letters
		- Remove 1-3 as an option from all other letters, remove 0 and 4-9 as options for the three letters here 
	- If we're actually able to figure out any of digits 1-3
		- If 3 is discovered to be the first digit of a clue with length 4, then the second digit must be a 0
		- If 2 is discovered to be the first digit of a clue with length 3, then the second digit can't be 5-9 as 24 (9 + 8 + 7) is the largest three digit possibility
		- If 1 is discovered to be the first digit of a clue with length 2, then the second digit can't be 8-9 as 17 (9 + 8) is the largest two digit possibility

2. Eliminate 0 as a possibility from all 1 digit clues and any letters appearing in the actual puzzle
	- If we're able to figure out what 0 is at this point then add it wherever it appears in the puzzle

3. This is where things are going to get dicey, best case scenario here we know digits 0-3, worst case we don't know any of them. We need to actually start going through
the puzzle blocks and trying to eliminate possibilities.
	- Do a first pass through the clues to see if any have been fully figure out. If so we should be able to start eliminating possibilities from the answer blocks
		- For example, looking at the example, we figured out that one clue was 30 and had a length of 4. This means that the boxes must contain 9, 8, 7 and 6
			- We'd need to go through all of these answer blocks and eliminate 0-5 as options
		- As another example we knew that 10 was another clue with an answer length of two
			- 5 could be eliminated as an option from both answer blocks as 5 + 5 = 10 and doubles aren't allowed
	- 
	
*/