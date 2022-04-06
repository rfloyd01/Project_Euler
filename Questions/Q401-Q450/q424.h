#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes powers_of_two[] and powers_of_ten[]
#include <vector>
#include <fstream>

//Kakuro

//Classes, Structs and Enums for Problem
enum Letters { A, B, C, D, E, F, G, H, I, J };
enum Directions { Horizontal, Vertical };
enum Tiles { Board, Clue, Answer };

class KakuroBoard;

struct Possibilities
{
	int digits = 0b1111111111;
	bool changed = true; //used for skipping steps when nothing has changed.
	//initialize to true so that functions run from the get-go

	void remove(int num)
	{
		//Allows for the removal of multiple bits at a time. The input number is a binary
		//representation of the bits to remove from 'digits'.
		//If something has already been removed we don't want to add it back, for
		//this reason we do a binary AND before doing a binary XOR
		digits ^= (digits & num);

		//this function only gets called if there are digits to be removed,
		//therefore any time this function get's called the changed boolean
		//should be set to true
		changed = true;
	}
	int getLowest()
	{
		//returns the decimal value of lowest bit in 'digits'
		return log(digits & -digits) / log(2);
	}
	int getHighest()
	{
		//returns the decimal value of highest bit in 'digits'
		return log(digits) / log(2);
	}
	bool solved()
	{
		//this function returns true if there's only 1 binary digit left in 'digits',
		//otherwise it returns false
		if (!(digits & (digits - 1))) return true;
		else return false;
	}
	void print()
	{
		//prints all of the digits that are still in the 'digits' binary number
		for (int i = 0; i < 10; i++)
			if (this->digits & powers_of_two[i]) std::cout << i << " ";
	}
};
class LetterPossibilities
{
private:
	Possibilities letterPossibilities[10];
	int numbersSolved = 0;
	long long answer = 0;

	void solve(int letter, int value)
	{
		//iterate through all letters in the letterPossibility array
		//and remove the solved bit from each. If this in turn solves
		//more letters, then solve function is recursively called.

		//first see if the letter's already been solved
		for (int i = 0; i < 10; i++)
			if (i != letter)
				this->remove(i + 'A', value);

		this->numbersSolved |= value;
		this->answer += powers_of_ten[9 - letter] * (log(value) / log(2));
	}

public:
	void remove(char l, int value)
	{
		int letter = l - 'A'; //first, convert the character to an integer
		//removes the possibilities from the given letter, however, only if any of those possibilities
		//still exist as options
		if (!(value & letterPossibilities[letter].digits)) return;
		letterPossibilities[letter].remove(value);

		//finally, check to see if removing bits has caused the number to become solved
		if (letterPossibilities[letter].solved()) this->solve(letter, letterPossibilities[letter].digits);
	}
	void printLetterPossibilities()
	{
		//prints out the current possibilities for each letter
		//which is helpful during debugging.
		std::cout << "Current Letter Possibilities:\n";
		for (int i = 0; i < 10; i++)
		{
			std::cout << (char)(i + 'A') << ": ";
			letterPossibilities[i].print();
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	int getNumbersSolved() { return this->numbersSolved; }
	Possibilities* seePossibility(char letter) { return &this->letterPossibilities[letter - 'A']; }
	long long getAnswer() { return this->answer; }
};
class BoardTile
{
protected:
	std::pair<int, int> location = { 0, 0 };
	int type = 0;

public:
	void setLocation(std::pair<int, int> loc)
	{
		this->location = loc;
	}
	std::pair<int, int> getLocation()
	{
		return this->location;
	}
	void setType(int t)
	{
		this->type = t;
	}
	int getType()
	{
		return this->type;
	}
};
class ClueTile : public BoardTile
{
private:
	char clueLetters[2][2] = { {' ', ' '}, {' ', ' '} };
	Possibilities* cluePossibilities[2][2] = { {nullptr, nullptr}, {nullptr, nullptr} };
	int answerLengths[2] = { 0, 0 };
	int solvedClueValues[2] = { 0, 0 };

public:
	void setClueLetter(int orientation, int digit, LetterPossibilities* possibilities, char letter)
	{
		this->clueLetters[orientation][digit] = letter;
		this->cluePossibilities[orientation][digit] = possibilities->seePossibility(letter);
	}
	Possibilities* getCluePossibility(int orientation, int digit) { return this->cluePossibilities[orientation][digit]; }
	char getClueLetter(int orientation, int digit) { return this->clueLetters[orientation][digit]; }
	void incrementAnswerLength(int orientation) { this->answerLengths[orientation]++; }
	int getClueAnswerLength(int orientation) { return this->answerLengths[orientation]; }
	int getSolvedClueValue(int orientation) { return this->solvedClueValues[orientation]; }
	void attemptClueSolve()
	{
		//looks at all the cluePossibility pointers for a given orientation. If all non-nullptr
		//possibilities have been solved the the solvedClueValues array will be update
		for (int orientation = Horizontal; orientation <= Vertical; orientation++)
		{
			int solved = 0, toSolve = 0, answer = 0;
			for (int i = 0; i < 2; i++)
			{
				if (this->cluePossibilities[orientation][i] == nullptr) continue;
				toSolve++;
				if (this->cluePossibilities[orientation][i]->solved())
				{
					solved++;
					answer += (pow(10, !i) * this->cluePossibilities[orientation][i]->getHighest());
				}
			}
			if (answer && (toSolve == solved)) this->solvedClueValues[orientation] = answer;
		}
	}
	bool cluePossibilitiesChanged(int orientation)
	{
		//if clue doesn't exist, return false
		if (this->cluePossibilities[orientation][1] == nullptr) return false;

		//we want to see if EITHER clue letter has changed, not just one, so if the
		//first digit of clue has changed, return true
		if (this->cluePossibilities[orientation][1]->changed) return true;

		//if first letter hasn't changed, and second letter doesn't exist, return false
		if (this->cluePossibilities[orientation][0] == nullptr) return false;

		//if second letter does exist and has changed, return true
		if (this->cluePossibilities[orientation][0]->changed) return true;

		//if first and second letter haven't changed, return false
		return false;
	}
};
class AnswerTile : public BoardTile
{
private:
	Possibilities answerPossibilities;
	ClueTile* governingClues[2];
	KakuroBoard* governingBoard;
	char internalClue = ' ';
	int solvedAnswer = 0;

	void answerTrimViaSolvedAnswer(bool print = false);
	
public:
	void remove(int num);
	void setInternalClue(char letter) { this->internalClue = letter; }
	void setSolvedAnswer(int ans) { this->solvedAnswer = ans; }
	int getSolvedAnswer() { return this->solvedAnswer; }
	void setClueTile(int orientation, ClueTile* clue) { this->governingClues[orientation] = clue; }
	char getInternalClue() { return this->internalClue; }
	void setBoard(KakuroBoard* board) { this->governingBoard = board; }
	ClueTile* getGoverningClue(int orientation) { return this->governingClues[orientation]; }
	Possibilities* getPossibilities() { return &this->answerPossibilities; }
};
class KakuroBoard
{
private:
	int dimension = 0, totalClues = 0, totalAnswers = 0;
	BoardTile*** board;
	ClueTile** clueTiles;
	AnswerTile** answerTiles;
	LetterPossibilities letterPossibilities;

public:
	BoardTile* getTile(int row, int col)
	{
		//returns a pointer to the board tile at the specified location
		return *(*(this->board + row) + col);
	}
	void setBoard(BoardTile*** b) { this->board = b; }
	std::pair<int, ClueTile**> getClues()
	{
		//returns a list containing all of the clue tiles as well as
		//the number of clues in the list
		return { this->totalClues, this->clueTiles };
	}
	std::pair<int, AnswerTile**> getAnswers()
	{
		//returns a list containing all of the answer tiles as well as
		//the number of answers in the list
		return { this->totalAnswers, this->answerTiles };
	}
	void setDimension(int d)
	{
		this->dimension = d;
	}
	void setTotalClueTile(int tiles) { this->totalClues = tiles; }
	void setTotalAnswerTile(int tiles) { this->totalAnswers = tiles; }
	BoardTile*** getBoard() { return this->board; }
	void setClueTile(ClueTile** clueArray) { this->clueTiles = clueArray; }
	void setAnswerTile(AnswerTile** answerArray) { this->answerTiles = answerArray; }
	int getClueAmount() { return this->totalClues; }
	int getAnswerAmount() { return this->totalAnswers; }
	int getDimension() { return this->dimension; }
	LetterPossibilities* getLetterPossibilities() { return &this->letterPossibilities; }
	long long getAnswer() { return this->letterPossibilities.getAnswer(); }

	void printBoard()
	{
		//prints out the kakuro board so it's easier to visualize
		//the maximum length of an individual tile is 7 (a clue tile with two digit
		//horzontal and vertical clues) so every tile will be normalized to this distance
		BoardTile* currentTile;
		ClueTile* currentClue;
		AnswerTile* currentAnswer;

		std::cout << " ";
		for (int i = 0; i < this->dimension; i++) std::cout << "---- ";
		std::cout << std::endl;

		for (int i = 0; i < dimension; i++)
		{
			for (int k = 0; k < 2; k++)
			{
				std::cout << "|";
				for (int j = 0; j < dimension; j++)
				{
					currentTile = this->getTile(i, j);
					if (currentTile->getType() == Board) std::cout << "xxxx|";
					else if (currentTile->getType() == Answer)
					{
						currentAnswer = (AnswerTile*)currentTile;
						std::cout << " " << currentAnswer->getInternalClue() << currentAnswer->getInternalClue() << " |";
					}
					else
					{
						currentClue = (ClueTile*)currentTile;
						if (k == 0) std::cout << "x\\" << currentClue->getClueLetter(k, 0) << currentClue->getClueLetter(k, 1) << '|';
						else std::cout << currentClue->getClueLetter(k, 0) << currentClue->getClueLetter(k, 1) << "\\x|";
					}
				}
				std::cout << std::endl;
			}
			std::cout << " ";
			for (int i = 0; i < this->dimension; i++) std::cout << "---- ";
			std::cout << std::endl;
		}
	}
	void printAllCluePossibilities()
	{
		std::cout << "All clue possibilities for Kakuro Board:\n";
		for (int i = 0; i < this->totalClues; i++)
		{
			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				ClueTile* currentClue = *(this->clueTiles + i);
				if (currentClue->getClueLetter(orientation, 1) == ' ') continue; //no clue here

				if (orientation) std::cout << "Vertical   ";
				else std::cout << "Horizontal ";
				std::cout << "Possibilities for clue at (" << currentClue->getLocation().first << ", " << currentClue->getLocation().second <<
					") are: ";

				bool oneDigit = false;
				if (currentClue->getClueLetter(orientation, 0) == ' ') oneDigit = true;//this is a 1 digit clue

				for (int j = 0; j < 10; j++) //represents the 10's digit
				{
					if (!oneDigit && !(powers_of_two[j] & currentClue->getCluePossibility(orientation, 0)->digits)) continue;

					for (int k = 0; k < 10; k++) //represents the 1's digit
						if (powers_of_two[k] & currentClue->getCluePossibility(orientation, 1)->digits) std::cout << 10 * j + k << " ";

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
	}
	void printAllAnswerPossibilities()
	{
		std::cout << "All answer tile possibilities for Kakuro Board:\n";
		for (int i = 0; i < this->totalAnswers; i++)
		{
			AnswerTile* currentAnswer = *(this->answerTiles + i);
			std::cout << "Possibilities for tile at (" << currentAnswer->getLocation().first << ", " << currentAnswer->getLocation().second <<
				") are: ";

			for (int j = 0; j < 10; j++)
				if (powers_of_two[j] & currentAnswer->getPossibilities()->digits) std::cout << j << " ";

			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	void printAllLetterPossibilities() { this->letterPossibilities.printLetterPossibilities(); }
};

//Function to create all ways of creating numbers in Kakuro (limited to maximum of 6 digits)
void kakuroNumberCombinations(std::vector<std::vector<int> >& combos)
{
	//Nothing fancy about this algorithm, it just keeps adding numbers wherever possible. The structure for the final
	//vector is as follows. The first vector represents how many digits we have and the second vector represents the number
	//we're trying to make. Each integer in the second vector is a binary representation of all the digits that can be
	//used to make the current number at the current digits.

	//For example, combos[3][8] is a binary number representing all of the digits that can be used to make the number
	//8 while using 3 distinct digits. We can make 8 with 3 digits with [5, 2, 1] or [4, 3, 1]. Since the digits 1 - 5
	//or all used here then combos[3][8] would be 0b0000111110 = 62. To get this final vector we'll need to keep vectors
	//of all the actual combinations as we're building them (i.e. [5, 2, 1] and [4, 3, 1] would need to be saved), however,
	//these can be discarded at the end of this function

	std::vector<std::vector<std::vector<std::vector<int> > > > constructionCombos;

	for (int i = 0; i < 7; i++)
	{
		//6 digits is the maximum clue length for this problem (real kakuro goes up to 9)
		std::vector<std::vector<std::vector<int> > > constructionNumberOfDigits;
		std::vector<int> numberOfDigits;
		constructionCombos.push_back(constructionNumberOfDigits);
		combos.push_back(numberOfDigits);
	}

	int rowMaximum = 0, currentNum = 9;
	for (int i = 1; i < 7; i++)
	{
		//we start at 1 because we can't make any 0 digit numbers. Technically we can't use any 1 digit numbers on a kakuro board either, however, we build off of them
		//to get our actual combinations.
		rowMaximum += currentNum--;
		for (int j = 0; j <= rowMaximum; j++)
		{
			std::vector<std::vector<int> > actualNumber;
			constructionCombos[i].push_back(actualNumber);
			combos[i].push_back(0);
		}
	}

	//set all of the necessary 1 digit building blocks
	for (int i = 1; i <= 9; i++)
	{
		constructionCombos[1][i].push_back({ i });
		combos[1][i] = powers_of_two[i];
	}

	//now iterate over every combo from the 1 digit row to the 5 digit row, adding single digits that are greater than current last digit
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 0; j < constructionCombos[i].size(); j++)
		{
			if (constructionCombos[i][j].size() == 0) continue; //no need to check any combos that aren't possible
			for (int k = 0; k < constructionCombos[i][j].size(); k++)
			{
				int kBinary = 0;
				for (int l = 0; l < constructionCombos[i][j][k].size(); l++) kBinary |= powers_of_two[constructionCombos[i][j][k][l]];
				for (int l = constructionCombos[i][j][k].back() + 1; l <= 9; l++)
				{
					//create a copy of the existing vector each time we add a new digit to it
					std::vector<int> copy = constructionCombos[i][j][k];
					copy.push_back(l);
					constructionCombos[i + 1][j + l].push_back(copy);
					combos[i + 1][j + l] |= (kBinary | powers_of_two[l]);
				}
			}
		}
	}
}

//Functions for Kakuro Board Creation
void createBoard(std::string boardString, KakuroBoard& board)
{
	int currentIndex = 2, dimension = (boardString[0] - '0'); //always start scanning right after the first comma
	BoardTile*** newBoard = new BoardTile**[dimension]; //create board with 'dimension' number of rows
	int clueTiles = 0, answerTiles = 0;

	for (int i = 0; i < dimension; i++)
	{
		BoardTile** tiles = new BoardTile * [dimension]; //each row of the board must have 'dimension' number of columns
		*(newBoard + i) = tiles;

		bool stop = false;
		for (int j = 0; j < dimension; j++)
		{
			//any given tile can be a plain tile (represented by an X), answer tile (represented by a 0 or letter A-J) are a clue tile (represented by a '(' ).
			//If it's one of the first two options tile creation is easy as we only need a single character. The clue tiles though we will need to scan character
			//by character until hitting the closing ')'.
			BoardTile* newTile;

			//check to see if this is the very last tile
			if ((i == (dimension - 1)) && (j == (dimension - 1))) stop = true;

			if (boardString[currentIndex] == 'X')
			{
				//create a new blank tile and move on
				newTile = new BoardTile();
				newTile->setType(Board);

			}
			else if (boardString[currentIndex] == '(')
			{
				//we need to scan until hitting a closing ')' to figure out which clues are present
				clueTiles++;
				ClueTile* ct = new ClueTile();
				int clueType = 0;
				while (boardString[++currentIndex] != ')')
				{
					if (boardString[currentIndex] == 'v') clueType = 1;
					else if (boardString[currentIndex] == ',') continue;

					if (boardString[currentIndex + 2] == ',' || boardString[currentIndex + 2] == ')')
						ct->setClueLetter(clueType, 1, board.getLetterPossibilities(), boardString[++currentIndex]); //this is only a one digit clue
					else
					{
						//this is a two digit clue, add both characters
						ct->setClueLetter(clueType, 0, board.getLetterPossibilities(), boardString[++currentIndex]);
						ct->setClueLetter(clueType, 1, board.getLetterPossibilities(), boardString[++currentIndex]);
					}
				}

				//after parsing the clue, set the newTile pointer
				newTile = (BoardTile*)ct;
				newTile->setType(Clue);
			}
			else
			{
				//create a new answer tile
				answerTiles++;
				AnswerTile* at = new AnswerTile();
				if (boardString[currentIndex] == 'O') at->setInternalClue(' ');
				else at->setInternalClue(boardString[currentIndex]);
				newTile = (BoardTile*)at;
				newTile->setType(Answer);
			}

			currentIndex += 2;
			newTile->setLocation({ i, j }); //add the clue location to tile
			*(tiles + j) = newTile; //add the new tile to the board

			if (stop) break; //break out of loop once we hit the last tile
		}
	}
	board.setDimension(dimension);
	board.setBoard(newBoard);
	board.setTotalClueTile(clueTiles);
	board.setTotalAnswerTile(answerTiles);

	//Now that the board string has been read, we need to create the ClueTile and AnswerTile arrays for the board, as well as
	//set the amount of answer tiles for each clue, and which clues govern which answer tiles
	ClueTile** clueArray = new ClueTile * [board.getClueAmount()];
	AnswerTile** answerArray = new AnswerTile * [board.getAnswerAmount()];
	int clueNumber = 0, answerNumber = 0;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			BoardTile* currentTile = board.getTile(i, j);
			if (currentTile->getType() == Clue)
			{
				//add the clue to the clue array
				ClueTile* currentClue = (ClueTile*)currentTile;
				*(clueArray + (clueNumber++)) = currentClue;

				//iterate until the end of the answer tiles are reached in both the horizontal and vertical directions
				for (int orientation = Horizontal; orientation <= Vertical; orientation++)
				{
					if (currentClue->getClueLetter(orientation, 1) != ' ')
					{
						//we set k in the below loop by using the fact that orientation = 0 for Horizontal and 1 for Vertical.
						//By converting to a boolean this means that when looking at horizontal clues, k starts at a value of j + 1
						//while for vertical clues k will start at a value of i + i. We use the same logic to increment to the next tile.
						int kStart = !(bool)orientation * j + (bool)orientation * i;
						for (int k = kStart + 1; k < dimension; k++)
						{
							//again, a somewhat complex way for selecting the tile, but basically if we're looking at a horizontal clue then only j will increase
							//and if we're looking at a vertical clue then only i will increase.
							AnswerTile* currentAnswer = (AnswerTile*)board.getTile((bool)orientation * (k - kStart) + i, !(bool)orientation * (k - kStart) + j);
							if (currentAnswer->getType() != Answer) break;

							//if we find an answer tile we set it's appropriate clueTile reference and increment the length of the clueAnswer by 1
							currentAnswer->setClueTile(orientation, currentClue);
							currentClue->incrementAnswerLength(orientation);
						}
					}
				}
			}
			else if (currentTile->getType() == Answer)
			{
				//the brunt of work in this part of board creation is handled when we find clues, all we need to do when finding an
				//answer tile is to add it to the answer array, and set the pointer to the kakuro board
				AnswerTile* currentAnswer = (AnswerTile*)currentTile;
				*(answerArray + (answerNumber++)) = currentAnswer;
				currentAnswer->setBoard(&board);
			}
		}
	}

	//finally set the clue and answer arrays in the Kakuro Board object. We're now done setting up the board
	board.setClueTile(clueArray);
	board.setAnswerTile(answerArray);
}
void createLetterHeirarchy(KakuroBoard& board, std::pair<int, int>* letterHeirarchy, int currentChain, int currentLocation, bool heirarchyCreated = false, bool higher = false)
{
	if (!heirarchyCreated)
	{
		//First we need to scan through all of the answer tiles until we find ones with a single letter in them. If either of the answer tile's governing clues are single
		//digit then the letter in the answer tile is added to the heirarchy.
		std::pair<int, AnswerTile**> answers = board.getAnswers();
		for (int i = 0; i < answers.first; i++)
		{
			AnswerTile* currentAnswer = *(answers.second + i);
			if (currentAnswer->getInternalClue() == ' ') continue; //this tile doesn't have an internal clue
			board.getLetterPossibilities()->remove(currentAnswer->getInternalClue(), 0b0000000001); //as a bonus, 0 can be removed as a possibility from any letter in an answer tile

			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				ClueTile* currentClue = currentAnswer->getGoverningClue(orientation);
				if (currentClue->getClueLetter(orientation, 0) == ' ')
				{
					//We found a single digit clue with a lettered answer tile, at both letters to the heirarchy
					letterHeirarchy[currentClue->getClueLetter(orientation, 1) - 'A'].first |= powers_of_two[currentAnswer->getInternalClue() - 'A'];
					letterHeirarchy[currentAnswer->getInternalClue() - 'A'].second |= powers_of_two[currentClue->getClueLetter(orientation, 1) - 'A'];
				}
			}
		}

		//after reading all of the clues we can expand on the heirarchy but combinging letters that are inderectly linked. For example, we may find in a puzzle that
		//A > B in one clue, and that B > D in another clue. This loop here will relate all three of these letters together so that A > B > D. Likewise, if we had
		//A > B in one clue and D > B in another clue, there wouldn't be any concrete relationship between A and D so no connection is made here.
		for (int i = 0; i < 10; i++)
		{
			//call the recursive function on both the lowest numbers in the heirarchy and the highest
			if (letterHeirarchy[i].first == 0 && letterHeirarchy[i].second > 0) createLetterHeirarchy(board, letterHeirarchy, 0, i, true, true);
			else if (letterHeirarchy[i].first > 0 && letterHeirarchy[i].second == 0) createLetterHeirarchy(board, letterHeirarchy, 0, i, true, false);
		}
	}
	else
	{
		//this part of the function is only called after the heirarchy list is set up
		if (higher)
		{
			letterHeirarchy[currentLocation].first |= currentChain;
			for (int i = 0; i < 10; i++)
				if (letterHeirarchy[currentLocation].second & powers_of_two[i]) createLetterHeirarchy(board, letterHeirarchy, currentChain | powers_of_two[currentLocation], i, true, higher);
		}
		else
		{
			letterHeirarchy[currentLocation].second |= currentChain;
			for (int i = 0; i < 10; i++)
				if (letterHeirarchy[currentLocation].first & powers_of_two[i]) createLetterHeirarchy(board, letterHeirarchy, currentChain | powers_of_two[currentLocation], i, true, higher);
		}
	}
}

//Forward Decleration Functions
void AnswerTile::remove(int num)
{
	//takes a binary number as input and removes it's bits as
	//possibilities. If no bits actually match then nothing happens
	if (!(num & this->answerPossibilities.digits)) return;

	//if there are digits to remove, remove them and mark the answer
	//tile possibilities as having been changed
	this->answerPossibilities.remove(num);

	//if the answer tile has a letter in it, the letterPossibility should reflect
	//what's allowable in the answer tile
	if (this->internalClue != ' ')
		this->governingBoard->getLetterPossibilities()->remove(this->internalClue, ~this->answerPossibilities.digits & 0b1111111111);

	//check to see if we've solved the answer tile. If so set the answer
	if (this->answerPossibilities.solved())
	{
		this->solvedAnswer = this->answerPossibilities.getHighest();
		this->answerTrimViaSolvedAnswer(); //after solving an answer tile, this will trim other tiles
	}
}
void AnswerTile::answerTrimViaSolvedAnswer(bool print)
{
	if (print)
	{
		this->governingBoard->printBoard();
		std::cout << "Solved answer at location (" << this->getLocation().first << ", " << this->getLocation().second << ")" << std::endl;
		this->governingBoard->printAllAnswerPossibilities();

	}
	//When an answer tile is solved, this function removes the solved number as a possibility from all other answer tiles
	//in the same row and column of the kakuro board
	int clueLocation = this->governingClues[Horizontal]->getLocation().second, removeDigit = this->answerPossibilities.digits;
	AnswerTile* removeAnswer;

	//loop through every answer tile tied to the current tile's horizontal clue and remove the appropriate digit
	for (int j = clueLocation + 1; j <= this->governingClues[Horizontal]->getClueAnswerLength(Horizontal) + clueLocation; j++)
	{
		if (j == this->getLocation().second) continue; //don't remove the possibility from actual answer tile
		removeAnswer = (AnswerTile*)this->governingBoard->getTile(this->governingClues[Horizontal]->getLocation().first, j);
		removeAnswer->remove(removeDigit);
	}

	clueLocation = this->governingClues[Vertical]->getLocation().first; //update the starting position
	//loop through every answer tile tied to the current tile's vertical clue and remove the appropriate digit
	for (int j = clueLocation + 1; j <= this->governingClues[Vertical]->getClueAnswerLength(Vertical) + clueLocation; j++)
	{
		if (j == this->getLocation().first) continue; //don't remove the possibility from actual answer tile
		removeAnswer = (AnswerTile*)this->governingBoard->getTile(j, this->governingClues[Vertical]->getLocation().second);
		removeAnswer->remove(removeDigit);
	}

	if (print) this->governingBoard->printAllAnswerPossibilities();
}

//Functions for Kakuro Board Manipulation
void initialClueSolver(KakuroBoard& board)
{
	//this function is used for the initial look at our clues to see if we can convert any letters right off the bat. It should only be called one
	//time, as soon as the board has been created.
	int leadCharacters = 0, leadCharactersFound = 0; //used to see if we have three different letters in leading positions of double digit clues
	int fourDigitAnswers[10] = { 0 }; //used to potentially find which letter is 2
	std::pair<int, ClueTile**> clues = board.getClues();
	LetterPossibilities* letterPossibilities = board.getLetterPossibilities();

	//scan all of the clues looking for two digits clues as they can help us solve for number 0-3
	for (int i = 0; i < clues.first; i++)
	{
		ClueTile* currentClue = *(clues.second + i);
		for (int orientation = Horizontal; orientation <= Vertical; orientation++)
		{
			if (currentClue->getClueLetter(orientation, 0) != ' ')
			{
				//we've found a two digit clue, check to see if it's a new letter
				if (!(leadCharacters & powers_of_two[currentClue->getClueLetter(orientation, 0) - 'A']))
				{
					leadCharacters |= powers_of_two[currentClue->getClueLetter(orientation, 0) - 'A'];
					leadCharactersFound++;
				}

				//this letter must be a 1, 2 or 3 so remove 0 and 4-9 as possibilities
				letterPossibilities->remove(currentClue->getClueLetter(orientation, 0), 0b1111110001);

				//carry out a check here to see if there are only 2 answer tiles for this clue, if so, then the first digit as a 1
				if (currentClue->getClueAnswerLength(orientation) == 2)
				{
					letterPossibilities->remove(currentClue->getClueLetter(orientation, 0), 0b0000001100); //remove 3 and 2 as options
					letterPossibilities->remove(currentClue->getClueLetter(orientation, 1), 0b1100000000); //remove 9 and 8 as options from 1's place (17 is largest)
				}
				else if (currentClue->getClueAnswerLength(orientation) == 3)
				{
					letterPossibilities->remove(currentClue->getClueLetter(orientation, 0), 0b0000001000); //clue is too short to start with a 3

					//it's possible that the above line will solve the current letter as 2, if this happens then we can remove 5-9 as options for the
					//second digit as the largest three digit number starting with 2 is 24 (7 + 8 + 9)
					if (letterPossibilities->seePossibility(currentClue->getClueLetter(orientation, 0))->digits == powers_of_two[2])
						letterPossibilities->remove(currentClue->getClueLetter(orientation, 1), 0b1111100000);
				}
				else if (currentClue->getClueAnswerLength(orientation) == 4)
					fourDigitAnswers[currentClue->getClueLetter(orientation, 0) - 'A'] |= powers_of_two[currentClue->getClueLetter(orientation, 1) - 'A']; //helpful for finding 2 later on
				else if (currentClue->getClueAnswerLength(orientation) == 6)
					letterPossibilities->remove(currentClue->getClueLetter(orientation, 0), 0b0000000010); //clue is too long to start with a 1
			}
			else if (currentClue->getClueLetter(orientation, 1) != ' ') letterPossibilities->remove(currentClue->getClueLetter(orientation, 1), 0b0000000111); //single digit clues can't be 0, 1 or 2
		}
	}

	//If we found three distinct letters at the start of two digit clues, then we can eliminate 1, 2 and 3 as options from the other letters
	if (leadCharactersFound == 3)
		for (char letter = 'A'; letter <= 'J'; letter++)
			if (!(leadCharacters & powers_of_two[letter - 'A'])) letterPossibilities->remove(letter, 0b0000001110);


	int numbersFound = letterPossibilities->getNumbersSolved(); //this number gives a binary representation of which numbers we've solved so far

	//At this point, if we've figured out which letter is 1, but not which one is 2, we can initiate a check to see if we have multiple two digit clues
	//with an answer length of four that start with the same letter (which isn't 1). If this is the case, then the letter must be 2.
	if ((numbersFound & powers_of_two[1]) && !(numbersFound & powers_of_two[2]))
	{
		for (int i = 0; i < 10; i++)
		{

			if (fourDigitAnswers[i] == 0) continue; //no two digit clues start with this letter with answer length 4
			if (letterPossibilities->seePossibility(i + 'A')->digits == 0b0000000010) continue; //this letter has been found to be 1

			//We're looking at the letter that represent either 2 or 3. If there are multiple bits then the letter equals 2.
			//Check to see if fourDigitAnswers[i] is a power of 2 to check if there are multiple bits
			if (fourDigitAnswers[i] & (fourDigitAnswers[i] - 1))
			{
				letterPossibilities->remove((char)(i + 'A'), 0b1111111011); //remove everything other than 2 as an option
				numbersFound = letterPossibilities->getNumbersSolved(); //make sure that we update this variable
			}
		}
	}

	//See if we've discovered 3 along the way, if so then it's possible to solved for 0 if the clue starting with 3
	//has an answer length of 4
	if (numbersFound & powers_of_two[3])
	{
		for (int i = 0; i < 10; i++)
		{
			if (fourDigitAnswers[i] == 0) continue; //no two digit clues start with this letter with answer length 4
			if (letterPossibilities->seePossibility(i + 'A')->digits == 0b0000001000)
			{
				//There's at least 1 clue starting with 3 that has an answer length of 4. Re-scan all the clues until we find it
				//and then set the second letter of the clue equal to 0
				for (int j = 0; j < clues.first; j++)
				{
					ClueTile* currentClue = *(clues.second + j);
					for (int orientation = Horizontal; orientation <= Vertical; orientation++)
						if (currentClue->getClueLetter(orientation, 0) == (i + 'A'))
							if (currentClue->getClueAnswerLength(orientation) == 4)
								letterPossibilities->remove(currentClue->getClueLetter(orientation, 1), 0b1111111110); //remove everything but 0
				}
			}
		}
	}

	//At this point we've solved as best we can the digits 0-3. Iterate through all the clues one last time to see if we've actually
	//solved any of them
	for (int i = 0; i < clues.first; i++)
	{
		ClueTile* currentClue = *(clues.second + i);
		currentClue->attemptClueSolve();
	}
}
void possibilityTrimViaHeirarchy(KakuroBoard& board, std::pair<int, int>* letterHeirarchy, int currentLocation = 0, bool recursion = false)
{
	//This was a tricky little function to implement. The problem was that even if we can see at a glance all of the
	//letters that are larger or smaller than the current one, we can't tell the exact order. This means that we can't
	//just iterate through the list and instead have to visit each letter recursively. The function is carried out in three steps.
	if (!recursion)
	{
		//this block calls the recursive part of the function on any letter that's at the base of a heirarchy.
		//i.e. if letter heirarchy was B < H < D, recursive function is only called on B.
		for (int i = 0; i < 10; i++)
			if (letterHeirarchy[i].first == 0 && letterHeirarchy[i].second > 0)
				possibilityTrimViaHeirarchy(board, letterHeirarchy, i, true);
	}
	else
	{
		//Step 1: Take the lowest value of the current letter and remove it (as well as anything lower than it) as an option from all letters in the higher than list
		int low = 0b1111111111 >> (9 - board.getLetterPossibilities()->seePossibility(currentLocation + 'A')->getLowest());
		for (int i = 0; i < 10; i++)
			if (letterHeirarchy[currentLocation].second & powers_of_two[i]) board.getLetterPossibilities()->remove(i + 'A', low);

		//Step 2: Recursively Call this function on everything in the higher than list
		for (int i = 0; i < 10; i++)
			if (letterHeirarchy[currentLocation].second & powers_of_two[i]) possibilityTrimViaHeirarchy(board, letterHeirarchy, i, true);

		//Step 3: Take the highest value of the current letter and remove it (as well as anything higher than it) as an option from all letters in the lower than list
		int high = (0b1111111111 << board.getLetterPossibilities()->seePossibility(currentLocation + 'A')->getHighest()) & 0b1111111111;
		for (int i = 0; i < 10; i++)
			if (letterHeirarchy[currentLocation].first & powers_of_two[i]) board.getLetterPossibilities()->remove(i + 'A', high);
	}
}
void answerTrimViaClue(KakuroBoard& board, std::vector<std::vector<int> >& combos)
{
	//In this function we visit every AnswerTile on the board and try to reduce the total number of digits that can go inside of them.
	//This is done by A) looking at any letters actually placed in these AnswerTile at the start of the puzzle and B) looking at the
	//horizontal and vertical clues that govern the box. For example, if we figured out that one of the clues was 12, and there were
	//two answer boxes making up that clue's answer then our options to fill in the three boxes would be (9, 3), (8, 4) and (7, 5).
	//At a glance we can see that the numbers 1, 2, 3 and 6 couldn't be utilized here.
	std::pair<int, AnswerTile**> answers = board.getAnswers();

	for (int i = 0; i < answers.first; i++)
	{
		AnswerTile* currentAnswer = *(answers.second + i);
		if (currentAnswer->getSolvedAnswer() > 0) continue; //no need to do anything with a tile we've already solved

		//First look to see if the answer tile has a letter associated with it. We use the bitwise ~ to reverse all the bits because we want to 
		//remove anything that ISN'T an option. The & 0b1111111111 is to ensure our number doesn't wrap around to a negative value
		if (currentAnswer->getInternalClue() != ' ')
			currentAnswer->remove(~(board.getLetterPossibilities()->seePossibility(currentAnswer->getInternalClue())->digits) & 0b1111111111);

		//Look at all options in the clue possibilities list and for both horizontal and vertical clues and compare with the combos vector
		//for the given clue length
		for (int orientation = Horizontal; orientation <= Vertical; orientation++)
		{
			//if there haven't been any changes to the governing clue then there's no reason to try and remove possibilities
			if (!currentAnswer->getGoverningClue(orientation)->cluePossibilitiesChanged(orientation)) continue;

			if (currentAnswer->getGoverningClue(orientation)->getClueLetter(orientation, 1) != ' ')
			{
				int allowableDigits = 0; //a binary number representing all digits that can be used

				//iterate through all possible clues based on the possibility values of the clue letters
				bool oneDigit = false;
				if (currentAnswer->getGoverningClue(orientation)->getClueLetter(orientation, 0) == ' ') oneDigit = true; //this is a 1 digit clue

				for (int j = 0; j < 10; j++) //represents the 10's digit
				{
					if (!oneDigit && !(powers_of_two[j] & currentAnswer->getGoverningClue(orientation)->getCluePossibility(orientation, 0)->digits)) continue;

					for (int k = 0; k < 10; k++) //represents the 1's digit
						if (powers_of_two[k] & currentAnswer->getGoverningClue(orientation)->getCluePossibility(orientation, 1)->digits)
							allowableDigits |= combos[currentAnswer->getGoverningClue(orientation)->getClueAnswerLength(orientation)][10 * j + k];

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}

				//Remove anything not included in 'allowedDigits'
				currentAnswer->remove(~allowableDigits & 0b1111111111);
			}
		}
	}

	
}
void clueTrimViaAnswer(KakuroBoard& board, std::pair<int, int>& newPossibilities, bool& found, bool recursion = false, ClueTile* currentClue = nullptr,
	int goal = 0, int currentNumber = 0, int usedNumbers = 0, bool horizontal = false, int level = 0)
{
	//This function looks at all of the number possibilities for the clue and sees if there's anyway to hit each of these
	//possibilities with the possible numbers in the answer boxes. If any of the clue possibilities can't be acheived then
	//the clue letters are updated to reflect this.
	if (!recursion)
	{
		//this first part of the function is just for setting things up for the recursive part of the algorithm
		std::pair<int, ClueTile**> clues = board.getClues();
		for (int i = 0; i < clues.first; i++)
		{
			ClueTile* currentClue = *(clues.second + i);
			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				if (currentClue->getSolvedClueValue(orientation) != 0) continue; //if this clue has been solved already then skip it
				if (currentClue->getClueLetter(orientation, 1) == ' ') continue; //the clue doesn't exist so skip it

				bool oneDigit = false;
				if (currentClue->getClueLetter(orientation, 0) == ' ') oneDigit = true; //we're looking at a one digit clue

				//iterate through all of the possibilities for the current clue
				std::pair<int, int> newCluePossibilities = { 0, 0 }; //a pair of binary integers representing allowable possibilities for each clue letter
				for (int j = 0; j < 10; j++) //represents the 10's digit
				{
					if (!oneDigit && !(powers_of_two[j] & currentClue->getCluePossibility(orientation, 0)->digits)) continue;

					for (int k = 0; k < 10; k++) //represents the 1's digit
					{
						if (powers_of_two[k] & currentClue->getCluePossibility(orientation, 1)->digits)
						{
							bool found = false; //used to break recursion as soon as a possibility is found
							clueTrimViaAnswer(board, newCluePossibilities, found, true, currentClue, 10 * j + k, 0, 0, !orientation);
						}
					}

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}

				//remove any possibilities not included in newCluePossibilities for the appropriate clue letter
				board.getLetterPossibilities()->remove(currentClue->getClueLetter(orientation, 1), ~newCluePossibilities.second & 0b1111111111);
				if (!oneDigit) board.getLetterPossibilities()->remove(currentClue->getClueLetter(orientation, 0), ~newCluePossibilities.first & 0b1111111111);

				//After updates have been made, see if the clue has been solved.
				currentClue->attemptClueSolve();
			}
		}
	}
	else
	{
		//This is the recursive part of the function. Every possibility proven to still work is appended to the newPossibilities vector.
		//A new vector is created as opposed to erasing elements from the old one as it's more efficient to just create a new one.
		if (level == currentClue->getClueAnswerLength(!horizontal))
		{
			//base of recursion. If we made it here check to see if we've hit the goal.
			//If so then set the 
			if (currentNumber == goal)
			{
				found = true;
				newPossibilities.first |= powers_of_ten[goal / 10];
				newPossibilities.second |= powers_of_two[goal % 10];
			}
			return;
		}

		AnswerTile* currentAnswer = (AnswerTile*)board.getTile(currentClue->getLocation().first + !horizontal * (level + 1), currentClue->getLocation().second + horizontal * (level + 1));
		for (int i = currentAnswer->getPossibilities()->getLowest(); i <= currentAnswer->getPossibilities()->getHighest(); i++)
		{
			if (powers_of_two[i] & usedNumbers) continue; //don't pick a numbers that's been picked already
			if (i + currentNumber > goal) break; //numbers will only get higher so break out of loop

			clueTrimViaAnswer(board, newPossibilities, found, true, currentClue, goal, currentNumber + i, usedNumbers | powers_of_two[i], horizontal, level + 1);

			if (found) return;
		}
	}
}
void answerTrimViaAnswer(KakuroBoard& board, std::pair<int, int> location, int* answerTilePossibilities, int* selectedNumbers, int clueSize = 0, int currentNumber = 0, int currentSum = 0,
	int goal = 0, bool horizontal = false, bool recursion = false, int level = 0)
{
	if (!recursion)
	{
		//this part of the function is just to get everything set up for the recursive part below
		std::pair<int, ClueTile**> clues = board.getClues();
		for (int i = 0; i < clues.first; i++)
		{
			ClueTile* currentClue = *(clues.second + i);
			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				if (currentClue->getClueAnswerLength(orientation) == 0) continue;

				//loop through all of the answer tiles for the current clue, if all tiles have already been
				//answered, or if they haven't changed since last iteration (and clue is also the same)
				//then there's no reason to check again.
				int clueLength = currentClue->getClueAnswerLength(orientation), numberSolved = 0, numberChanged = 0;
				int* currentTileOptions = new int[clueLength](), * tileOptions = new int[clueLength](), * usedDigits = new int[clueLength]();
				bool oneDigit = false;
				for (int j = 0; j < clueLength; j++)
				{
					AnswerTile* currentAnswer = (AnswerTile*)board.getTile(currentClue->getLocation().first + (orientation * (j + 1)), currentClue->getLocation().second + (!orientation * (j + 1)));
					currentTileOptions[j] = currentAnswer->getPossibilities()->digits;
					if (currentAnswer->getSolvedAnswer() != 0) numberSolved++;
					numberChanged += currentAnswer->getPossibilities()->changed;
				}

				if ((numberSolved == clueLength) || (numberChanged == 0 && currentClue->cluePossibilitiesChanged(orientation))) goto nextClue;

				//Something has changed since last iteration so we go ahead and carry out the check
				if (currentClue->getClueLetter(orientation, 0) == ' ') oneDigit = true; //this is a 1 digit clue

				//check all possibilites for the current clue
				for (int j = 0; j < 10; j++) //represents the 10's digit
				{
					//if this is a two digit number j isn't included in the binary number, continue
					if (!oneDigit && !(powers_of_two[j] & currentClue->getCluePossibility(orientation, 0)->digits)) continue;

					for (int k = 0; k < 10; k++) //represents the 1's digit
					{
						if (powers_of_two[k] & currentClue->getCluePossibility(orientation, 1)->digits)
						{
							answerTrimViaAnswer(board, { currentClue->getLocation().first + orientation, currentClue->getLocation().second + !orientation },
								tileOptions, usedDigits, clueLength, 0, 0, 10 * j + k, !orientation, true);

							//If every answer tile is confirmed to have the same possibilities before checking every potential clue
							//we break out early (as checking more clues won't be able to 'add' more possibilities)
							bool earlyBreakOut = true;
							for (int l = 0; l < clueLength; l++)
							{
								//check the tileOption array to see if it's identical to the current options for the clue's answer
								//tiles. If so, then nothing can be eliminated and we return from this functino early
								if (currentTileOptions[l] != tileOptions[l])
								{
									earlyBreakOut = false;
									break;
								}
							}
							if (earlyBreakOut) goto nextClue;
						}
					}

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}

				//update the answer tile possibilities to reflect what's stored in the tileOptions array
				for (int j = 0; j < currentClue->getClueAnswerLength(orientation); j++)
				{
					AnswerTile* currentAnswer = (AnswerTile*)board.getTile(currentClue->getLocation().first + (orientation * (j + 1)), currentClue->getLocation().second + (!orientation * (j + 1)));
					bool solvedYet = currentAnswer->getPossibilities()->solved(); //check to see if the current answer tile has been solved yet

					if (!solvedYet) currentAnswer->remove(~tileOptions[j] & 0b1111111111); //remove anything that isn't a possibility
				}

				//use a label to potentially exit from above nested loops
			nextClue:
				delete[] usedDigits;
				delete[] tileOptions;
				delete[] currentTileOptions;
			}
		}
		//After running the function, go through and set the changed variable to false for every letter.
	    //The other functions in the game loop will reset these variables to true if they get changed again.
		for (int i = 0; i < 10; i++)
			board.getLetterPossibilities()->seePossibility(i + 'A')->changed = false;

		AnswerTile** answerTiles = board.getAnswers().second;
		for (int i = 0; i < board.getAnswers().first; i++)
		{
			AnswerTile* currentAnswer = *(answerTiles + i);
			currentAnswer->getPossibilities()->changed = false;
		}
	}
	else
	{
		//the recursive part of the function
		AnswerTile* currentAnswer = (AnswerTile*)board.getTile(location.first, location.second);
		if (level == (clueSize - 1))
		{
			//Base of recursion. We're currently looking at the final answer tile for the clue, instead of iterating through
			//all of it's options, just see if the number we need exists as a possibility
			int numberNeeded = goal - currentSum;

			if (numberNeeded > 0 && numberNeeded <= 9) //if the number isn't possible don't even check
			{
				//if the needed number is a possibility in the answer tile and hasn't been used yet we're good
				if ((powers_of_two[numberNeeded] & currentAnswer->getPossibilities()->digits) && (!(powers_of_two[numberNeeded] & currentNumber)))
				{
					selectedNumbers[level] = powers_of_two[numberNeeded];
					for (int i = 0; i < clueSize; i++) answerTilePossibilities[i] |= selectedNumbers[i];
					selectedNumbers[level] = 0;
				}
			}

			return;
		}

		//if the current answer tile has already been solved then we must select that number, no need to loop
		if (currentAnswer->getSolvedAnswer() != 0)
		{
			int solvedNumber = currentAnswer->getSolvedAnswer();
			if (!(powers_of_two[solvedNumber] & currentNumber)) //make sure we haven't already selected the number in a previous box
			{
				if (!(currentSum + solvedNumber >= goal)) //make sure adding this number won't exceed the goal
				{
					selectedNumbers[level] = powers_of_two[solvedNumber]; //add number to selected vector
					answerTrimViaAnswer(board, { location.first + !horizontal, location.second + horizontal },
						answerTilePossibilities, selectedNumbers, clueSize, currentNumber | powers_of_two[solvedNumber], currentSum + solvedNumber, goal, horizontal, true, level + 1);
					selectedNumbers[level] = 0; //remove number from selected vector
				}
			}
		}
		else
		{
			//otherwise, loop through all possiblities that can go in the current number tile
			for (int i = currentAnswer->getPossibilities()->getLowest(); i <= currentAnswer->getPossibilities()->getHighest(); i++)
			{
				//select any possible number that won't make us go over the goal
				if (powers_of_two[i] & currentAnswer->getPossibilities()->digits) //make sure that the number can go in the box
				{
					if (!(powers_of_two[i] & currentNumber)) //make sure we haven't already selected the number in a previous box
					{
						if (!(currentSum + i >= goal)) //make sure adding this number won't exceed the goal
						{
							selectedNumbers[level] = powers_of_two[i]; //add number to selected vector
							answerTrimViaAnswer(board, { location.first + !horizontal, location.second + horizontal },
								answerTilePossibilities, selectedNumbers, clueSize, currentNumber | powers_of_two[i], currentSum + i, goal, horizontal, true, level + 1);
							selectedNumbers[level] = 0; //remove number from selected vector
						}
					}
				}
			}
		}
	}
}
void possibilityTrimViaPossibility(KakuroBoard& board, bool recursion = false, std::vector<std::pair<int, int> >* ptq = nullptr, std::vector<std::pair<int, int> >* atq = nullptr, int currentCombo = 0,
	int numbersInCombo = 0, int currentLocation = 0, int numberOfNumbers = 0, bool triplePossible = false)
{
	//this function attempts to trim down possibilities for the letters simply by looking at the relationships for what's possible in the letters.
	//This function implements what I like to call "Sudoku solving techniques", i.e. looking for doubles, triples, and other Sudoku techniques just
	//in the letter possilities themselves. Due to the nature of triples and quads only needing to be a subset, it's easier to take a recursive approach
	//when looking for them so this function is split into a recursive, and non-recursive section.

	//Step 1. Look for naked doubles, triples and quadruples
	if (!recursion)
	{
		std::vector<std::pair<int, int> > potential_triples_quads, actual_triples_quads;
		for (int i = 0; i < 10; i++)
		{
			//count the number of possibilities for each letter. Anything with 2, 3 or 4 can
			//potentially be a double, triple or quad
			Possibilities* currentPossibility = board.getLetterPossibilities()->seePossibility(i + 'A');
			int bitCount = 0;
			for (int j = 0; j < 10; j++)
				if (currentPossibility->digits & powers_of_two[j]) bitCount++;

			if ((bitCount == 2 || bitCount == 3) || bitCount == 4)
			{
				int matches = 1;
				for (int j = i + 1; j < 10; j++) //check the current double, triple or quad against all other letters for a potential match
					if (currentPossibility->digits == board.getLetterPossibilities()->seePossibility(j + 'A')->digits) matches++;


				if (matches == bitCount)
				{
					//if the number of matches equals the bit count then we have a double, triple or quad on our hands. Iterate through
					//all the letters possibilities and remove the matched numbers from anything not part of the double, triple or quad.
					for (int j = 0; j < 10; j++)
						if (currentPossibility->digits != board.getLetterPossibilities()->seePossibility(j + 'A')->digits)
							board.getLetterPossibilities()->remove(j + 'A', currentPossibility->digits);
				}
				else
				{
					//we didn't find any doubles, triples or quads so add this letter to the triples_quads vector
					potential_triples_quads.push_back({ i, currentPossibility->digits });
				}
			}
		}

		//recursively search through the letters to see if we have any triples or quads.
		possibilityTrimViaPossibility(board, true, &potential_triples_quads, &actual_triples_quads, 0, 0, 0, 0, true);

		//remove letter possibilities based on whether or not any triples or quads were found
		for (int i = 0; i < actual_triples_quads.size(); i++)
		{
			//the first number in the pair represents the Letters that are members of the triple/quad while the second number
			//of the pair represents the numbers that actually make up the quad.
			for (int j = 0; j < 10; j++)
				if (!(actual_triples_quads[i].first & powers_of_two[j]))
					board.getLetterPossibilities()->remove(j + 'A', actual_triples_quads[i].second);
		}

		//Step 2. Check for hidden singles
		//One final check we do here, is to see if there are any numbers which only exist as a possibility within a single number. For example,
		//if the letter possibilities looked like this: A: [3, 4, 5, 6, 7, 8, 9], B: [2, 3, 4, 5, 6, 7, 8, 9], C: [2, 3, 4, 5, 6, 7, 8, 9]
		//D: [3, 4, 5, 6], E: [2, 3, 4, 5, 6, 7, 8, 9], F: [6, 7], G: [7, 8, 9], H: [7, 8], I: [1], J: [0, 2, 3, 4, 5, 6, 7, 8, 9]... We can
		//see that the number 0 only exists within the J array, therefore J MUST be 0.
		for (int i = 0; i < 10; i++)
		{
			//TODO: Add a way to skip i values that have already been solved
			int letterCount = 0, location = 0;
			bool single = true;
			for (int j = 0; j < 10; j++)
			{
				if (board.getLetterPossibilities()->seePossibility(j + 'A')->digits & powers_of_two[i])
				{
					letterCount++;
					location = j;
				}
				if (letterCount > 1)
				{
					single = false;
					break;
				}
			}
			if (single) board.getLetterPossibilities()->remove(location + 'A', ~powers_of_two[i] & 0b1111111111);
		}
	}
	else
	{
		//this is the recursive part of the function
		if ((numberOfNumbers == 3 && triplePossible) || numberOfNumbers == 4) //base case
		{
			//we've found a triple or a quad, add it to the tripleQuadActual vector
			atq->push_back({ numbersInCombo, currentCombo });
		}

		//since we need at least a triple, no need to check further in the array than 3 options from the end
		for (int i = currentLocation; i < ptq->size(); i++)
		{
			int newCombo = currentCombo | ptq->at(i).second;
			int bits = 0;
			//count the bits in the combo (unfortunately I only know how to do this 1 bit at a time)
			for (int j = 0; j < 10; j++)
				if (newCombo & powers_of_two[j]) bits++;

			if (bits > 4) continue;

			//if we haven't continued yet then there's a chance for a match here
			possibilityTrimViaPossibility(board, true, ptq, atq, newCombo, numbersInCombo | powers_of_two[ptq->at(i).first], i + 1, numberOfNumbers + 1, (triplePossible && (bits <= 3)));
		}
	}
}

std::pair<std::string, double> q424()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	bool dummy = false; //a dummy variable needed to initialize some recursive functions

	//Before looping through all the puzzles, create a vector that stores all the different ways to solve (up to 6 digit) Kakuro clues.
	//This will make it much quicker to see at a glance what possibilities can be removed from answer tiles
	std::vector<std::vector<int> > combinations;
	kakuroNumberCombinations(combinations);

	std::ifstream myFile;
	std::string boardString;
	myFile.open("Resources/q424.txt");
	int greatest = 0, printGame = -1;

	for (int i = 0; i < 200; i++)
	{
		//Read the next board from text file
		std::getline(myFile, boardString);

		//Create the Kakuro Board
		KakuroBoard kakuroBoard;

		//Fill in the Kakuro board using a function to parse the board string
		createBoard(boardString, kakuroBoard);

		//Before getting to our solve loop, there are a few setup activities
		//we can do to help reduce the total number of options for each letter
		std::pair<int, int > letterHeirarchy[10] = { {0, 0} };
		createLetterHeirarchy(kakuroBoard, letterHeirarchy, 0, 0);
		initialClueSolver(kakuroBoard);

		if (i == printGame) kakuroBoard.printBoard();

		//Now that setup is complete, we enter our solve loop and don't exit it
		//until all 10 letters have been discovered
		int loopCount = 0;
		while (kakuroBoard.getLetterPossibilities()->getNumbersSolved() != 0b1111111111)
		{
			//The first thing we do is examine our letter heirarchy and see if we can eliminate any letter possibilities.
			//A recursive function is called on all letters at the base of a heirarchy chain
			possibilityTrimViaHeirarchy(kakuroBoard, letterHeirarchy);

			//Second we use "Sudoku Style" elimination techniques to try and reduce the letter possibilites even further
			possibilityTrimViaPossibility(kakuroBoard);

			//Next we attempt to trim the list of possible numbers for each answer tile on the board by looking at their clues
			answerTrimViaClue(kakuroBoard, combinations);
			
			//With the answer tiles trimmed down a little bit now, we take a look back at the clues and see if we can reduce the
			//possibilities for each clue based on the new answer tiles
			clueTrimViaAnswer(kakuroBoard, letterHeirarchy[0], dummy); //letterHeirarchy[0] and dummy are used as placeholders

			//Call this function again just in case any of the clues changed from call to clueTrimViaAnswer()
			answerTrimViaClue(kakuroBoard, combinations);

			//Now we use a brute force recursive algorithm to see if each of the possible numbers for every answer tile can actually be
			//used in a solution. Any numbers that aren't found to be part of a solution are removed from the possibility list.
			answerTrimViaAnswer(kakuroBoard, { 0, 0 }, nullptr, nullptr);
		}
		answer += kakuroBoard.getAnswer(); //after solving the puzzle add the answer to the final total
	}

	myFile.close(); //close down the file after reading all boards

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1059760019628
	//ran in 0.225812 seconds --> 0.187944 seconds --> 0.176166 seconds --> 0.0920652 seconds --> 0.0743379 seconds --> 0.0645144 seconds
	//
	//changes
	//original solve --> changed a < to a <= in one of the functions --> automatic cascading of answer tile solving --> stop answerTrivViaAnswer recursion at one level before base -->
	// break out of answerTimViaAnswer before checking each clue possibility if all answer tiles are found to remain the same --> moved possibility trim via possibility to second function
	// of loop and added a second call to answerTrimViaClue after call to clueTrimViaAnswer

	//I think the only way to meaningfully cutdown on the runtime at this point will be to completely overhaul how the answerTrimViaAnswer works. Currently it's just a complete
	//brute force check. I added logic so that this function only gets called if there are changes that were made, however, it didn't really shave off that much time. SO when it
	//does get called, it's still taking a long time. Maybe implement another check so that it just doesn't run if there are too many options? and hope more options get weeded
	//out elsewhere? Or maybe implement sudoku style elimination on answer boxes before making calls to this function?
}