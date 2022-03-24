#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <vector>
#include <set>
#include <fstream>

//Kakuro

//Classes, Structs and Enums for Problem
enum Letters { A, B, C, D, E, F, G, H, I, J };
enum Directions {Horizontal, Vertical};
enum Tiles {Board, Clue, Answer};

struct Possibilities
{
	int digits = 0b1111111111;

	void remove(int num)
	{
		//Allows for the removal of multiple bits at a time. The input number is a binary
		//representation of the bits to remove from 'digits'.
		//If something has already been removed we don't want to add it back, for
		//this reason we do a binary AND before doing a binary XOR
		digits ^= (digits & num);
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
class LetterPossibilitays
{
private:
	Possibilities letterPossibilities[10];
	int lettersSolved = 0, numbersSolved = 0, numberOfLettersSolved = 0;

	void solve(int letter, int value)
	{
		//iterate through all letters in the letterPossibility array
		//and remove the solved bit from each. If this in turn solves
		//more letters, then solve function is recursively called.
		for (int i = 0; i < 10; i++)
			if (i != letter)
				this->remove(i + 'A', value);

		this->lettersSolved |= powers_of_two[letter];
		this->numbersSolved |= value;
		this->numberOfLettersSolved++;
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
		std::cout << this->numberOfLettersSolved << " Letters have been solved." << std::endl;
		std::cout << std::endl;
	}
	int getNumberOfLettersFound()
	{
		return this->numberOfLettersSolved;
	}
	int getLettersSolved() { return this->lettersSolved; }
	int getNumbersSolved() { return this->numbersSolved; }
	Possibilities* seePossibility(char letter)
	{
		return &this->letterPossibilities[letter - 'A'];
	}
};
class BoardTiles
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
class ClueTiles : public BoardTiles
{
private:
	char clueLetters[2][2] = { {' ', ' '}, {' ', ' '} };
	//std::vector<int> cluePossibilities[2];
	//LetterPossibilitays* currentLetterPossibilities;
	Possibilities* cluePossibilities[2][2] = { {nullptr, nullptr}, {nullptr, nullptr} };
	int answerLengths[2] = { 0, 0 };
	int solvedClueValues[2] = { 0, 0 };

public:
	void setClueLetter(int orientation, int digit, LetterPossibilitays* possibilities, char letter)
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
	/*void addCluePossibility(int orientation, int possibility)
	{
		this->cluePossibilities[orientation].push_back(possibility);
	}*/
	//std::vector<int>* getCluePossibilities(int orientation) { return &this->cluePossibilities[orientation]; }
	//void setCluePossibilities(int orientation, std::vector<int> possibilities) { this->cluePossibilities[orientation] = possibilities; }
};
class AnswerTiles : public BoardTiles
{
private:
	Possibilities answerPossibilities;
	ClueTiles* governingClues[2];
	char internalClue = ' ';
	int solvedAnswer = 0;

public:
	void setInternalClue(char letter) { this->internalClue = letter; }
	void setSolvedAnswer(int ans) { this->solvedAnswer = ans; }
	int getSolvedAnswer() { return this->solvedAnswer; }
	void setClueTile(int orientation, ClueTiles* clue) { this->governingClues[orientation] = clue; }
	char getInternalClue() { return this->internalClue; }
	ClueTiles* getGoverningClue(int orientation) { return this->governingClues[orientation]; }
	Possibilities* getPossibilities() { return &this->answerPossibilities; }
};
class KakuroBoard
{
private:
	int dimension = 0, totalClues = 0, totalAnswers = 0;
	BoardTiles*** board;
	ClueTiles** clueTiles;
	AnswerTiles** answerTiles;
	LetterPossibilitays letterPossibilities;

public:
	BoardTiles* getTile(int row, int col)
	{
		//returns a pointer to the board tile at the specified location
		return *(*(this->board + row) + col);
	}
	void setBoard(BoardTiles*** b) { this->board = b; }
	std::pair<int, ClueTiles**> getClues()
	{
		//returns a list containing all of the clue tiles as well as
		//the number of clues in the list
		return { this->totalClues, this->clueTiles };
	}
	std::pair<int, AnswerTiles**> getAnswers()
	{
		//returns a list containing all of the answer tiles as well as
		//the number of answers in the list
		return { this->totalAnswers, this->answerTiles };
	}
	void setDimension(int d)
	{
		this->dimension = d;
	}
	void setTotalClueTiles(int tiles) { this->totalClues = tiles; }
	void setTotalAnswerTiles(int tiles) { this->totalAnswers = tiles; }
	BoardTiles*** getBoard() { return this->board; }
	void setClueTiles(ClueTiles** clueArray) { this->clueTiles = clueArray; }
	void setAnswerTiles(AnswerTiles** answerArray) { this->answerTiles = answerArray; }
	int getClueAmount() { return this->totalClues; }
	int getAnswerAmount() { return this->totalAnswers; }
	int getDimension() { return this->dimension; }
	LetterPossibilitays* getLetterPossibilities() { return &this->letterPossibilities; }

	void printBoard()
	{
		//prints out the kakuro board so it's easier to visualize
		//the maximum length of an individual tile is 7 (a clue tile with two digit
		//horzontal and vertical clues) so every tile will be normalized to this distance
		BoardTiles* currentTile;
		ClueTiles* currentClue;
		AnswerTiles* currentAnswer;

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
						currentAnswer = (AnswerTiles*)currentTile;
						std::cout << " " << currentAnswer->getInternalClue() << currentAnswer->getInternalClue() << " |";
					}
					else
					{
						currentClue = (ClueTiles*)currentTile;
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
				ClueTiles* currentClue = *(clueTiles + i);
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
			AnswerTiles* currentAnswer = *(answerTiles + i);
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
void kakuroNumberCombinationsRevised(std::vector<std::vector<int> >& combos)
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
void createBoard(std::string boardString, KakuroBoard &board)
{
	int currentIndex = 2, dimension = (boardString[0] - '0'); //always start scanning right after the first comma
	BoardTiles*** newBoard = new BoardTiles** [dimension]; //create board with 'dimension' number of rows
	int clueTiles = 0, answerTiles = 0;

	for (int i = 0; i < dimension; i++)
	{
		BoardTiles** tiles = new BoardTiles* [dimension]; //each row of the board must have 'dimension' number of columns
		*(newBoard + i) = tiles;

		bool stop = false;
		for (int j = 0; j < dimension; j++)
		{
			//any given tile can be a plain tile (represented by an X), answer tile (represented by a 0 or letter A-J) are a clue tile (represented by a '(' ).
			//If it's one of the first two options tile creation is easy as we only need a single character. The clue tiles though we will need to scan character
			//by character until hitting the closing ')'.
			BoardTiles* newTile;

			//check to see if this is the very last tile
			if ((i == (dimension - 1)) && (j == (dimension - 1))) stop = true;

			if (boardString[currentIndex] == 'X')
			{
				//create a new blank tile and move on
				newTile = new BoardTiles();
				newTile->setType(Board);
				
			}
			else if (boardString[currentIndex] == '(')
			{
				//we need to scan until hitting a closing ')' to figure out which clues are present
				clueTiles++;
				ClueTiles* ct = new ClueTiles();
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
				newTile = (BoardTiles*)ct;
				newTile->setType(Clue);
			}
			else
			{
				//create a new answer tile
				answerTiles++;
				AnswerTiles* at = new AnswerTiles();
				if(boardString[currentIndex] == 'O') at->setInternalClue(' ');
				else at->setInternalClue(boardString[currentIndex]);
				newTile = (BoardTiles*)at;
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
	board.setTotalClueTiles(clueTiles);
	board.setTotalAnswerTiles(answerTiles);

	//Now that the board string has been read, we need to create the clueTiles and answerTiles arrays for the board, as well as
	//set the amount of answer tiles for each clue, and which clues govern which answer tiles
	ClueTiles** clueArray = new ClueTiles * [board.getClueAmount()];
	AnswerTiles** answerArray = new AnswerTiles * [board.getAnswerAmount()];
	int clueNumber = 0, answerNumber = 0;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			BoardTiles* currentTile = board.getTile(i, j);
			if (currentTile->getType() == Clue)
			{
				//add the clue to the clue array
				ClueTiles* currentClue = (ClueTiles*)currentTile;
				*(clueArray + (clueNumber++)) = currentClue;

				//iterate until the end of the answer tiles are reached in both the horizontal and vertical directions
				for (int orientation = Horizontal; orientation <= Vertical; orientation++)
				{
					if (currentClue->getClueLetter(orientation, 1) != ' ')
					{
						//std::cout << "Clue at location " << i << ", " << j << " has a " << orientation << " clue." << std::endl;
						//we set k in the below loop by using the fact that orientation = 0 for Horizontal and 1 for Vertical.
						//By converting to a boolean this means that when looking at horizontal clues, k starts at a value of j + 1
						//while for vertical clues k will start at a value of i + i. We use the same logic to increment to the next tile.
						int kStart = !(bool)orientation * j + (bool)orientation * i;
						for (int k =  kStart + 1; k < dimension; k++)
						{
							//again, a somewhat complex way for selecting the tile, but basically if we're looking at a horizontal clue then only j will increase
							//and if we're looking at a vertical clue then only i will increase.
							AnswerTiles* currentAnswer = (AnswerTiles*)board.getTile((bool)orientation * (k - kStart) + i, !(bool)orientation * (k - kStart) + j);
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
				//answer tile is to add it to the answer array
				AnswerTiles* currentAnswer = (AnswerTiles*)currentTile;
				*(answerArray + (answerNumber++)) = currentAnswer;
			}
		}
	}

	//finally set the clue and answer arrays in the Kakuro Board object. We're now done setting up the board
	board.setClueTiles(clueArray);
	board.setAnswerTiles(answerArray);
}
void createLetterHeirarchy(KakuroBoard& board, std::pair<int, int>* letterHeirarchy, int currentChain, int currentLocation, bool heirarchyCreated = false, bool higher = false)
{
	if (!heirarchyCreated)
	{
		//First we need to scan through all of the answer tiles until we find ones with a single letter in them. If either of the answer tile's governing clues are single
		//digit then the letter in the answer tile is added to the heirarchy.
		std::pair<int, AnswerTiles**> answers = board.getAnswers();
		for (int i = 0; i < answers.first; i++)
		{
			AnswerTiles* currentAnswer = *(answers.second + i);
			if (currentAnswer->getInternalClue() == ' ') continue; //this tile doesn't have an internal clue
			board.getLetterPossibilities()->remove(currentAnswer->getInternalClue(), 0b0000000001); //as a bonus, 0 can be removed as a possibility from any letter in an answer tile

			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				ClueTiles* currentClue = currentAnswer->getGoverningClue(orientation);
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

//Functions for Kakuro Board Manipulation
void initialClueSolverRevised(KakuroBoard &board)
{
	//this function is used for the initial look at our clues to see if we can convert any letters right off the bat. It should only be called one
	//time, as soon as the board has been created.
	int leadCharacters = 0, leadCharactersFound = 0; //used to see if we have three different letters in leading positions of double digit clues
	int fourDigitAnswers[10] = { 0 }; //used to potentially find which letter is 2
	std::pair<int, ClueTiles**> clues = board.getClues();
	LetterPossibilitays* letterPossibilities = board.getLetterPossibilities();

	//scan all of the clues looking for two digits clues as they can help us solve for number 0-3
	for (int i = 0; i < clues.first; i++)
	{
		ClueTiles* currentClue = *(clues.second + i);
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
					ClueTiles* currentClue = *(clues.second + j);
					for (int orientation = Horizontal; orientation <= Vertical; orientation++)
						if (currentClue->getClueLetter(orientation, 0) == (i + 'A'))
							letterPossibilities->remove(currentClue->getClueLetter(orientation, 1), 0b1111111110); //remove everything but 0
				}
			}
		}
	}

	//At this point we've solved as best we can the digits 0-3. Iterate through all the clues one last time to see if we've actually
	//solved any of them
	for (int i = 0; i < clues.first; i++)
	{
		ClueTiles* currentClue = *(clues.second + i);
		currentClue->attemptClueSolve();
	}
}
void possibilityTrimViaHeirarchy(KakuroBoard& board, std::pair<int, int>* letterHeirarchy, int currentLocation, bool recursion = false)
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
void answerTrimViaClueRevised(KakuroBoard &board, std::vector<std::vector<int> > &combos)
{
	//In this function we visit every AnswerTile on the board and try to reduce the total number of digits that can go inside of them.
	//This is done by A) looking at any letters actually placed in these AnswerTiles at the start of the puzzle and B) looking at the
	//horizontal and vertical clues that govern the box. For example, if we figured out that one of the clues was 12, and there were
	//two answer boxes making up that clue's answer then our options to fill in the three boxes would be (9, 3), (8, 4) and (7, 5).
	//At a glance we can see that the numbers 1, 2, 3 and 6 couldn't be utilized here.
	std::pair<int, AnswerTiles**> answers = board.getAnswers();

	for (int i = 0; i < answers.first; i++)
	{
		AnswerTiles* currentAnswer = *(answers.second + i);
		if (currentAnswer->getSolvedAnswer() > 0) continue; //no need to do anything with a tile we've already solved

		//First look to see if the answer tile has a letter associated with it. We use the bitwise ~ to reverse all the bits because we want to 
		//remove anything that ISN'T an option. The & 0b1111111111 is to ensure our number doesn't wrap around to a negative value
		if (currentAnswer->getInternalClue() != ' ')
			currentAnswer->getPossibilities()->remove(~(board.getLetterPossibilities()->seePossibility(currentAnswer->getInternalClue())->digits) & 0b1111111111);

		//Look at all options in the clue possibilities list and for both horizontal and vertical clues and compare with the combos vector
		//for the given clue length
		for (int orientation = Horizontal; orientation < Vertical; orientation++)
		{
			//std::vector<int>* cluePossibilities = currentAnswer->getGoverningClue(orientation)->getCluePossibilities(orientation);
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
				currentAnswer->getPossibilities()->remove(~allowableDigits & 0b1111111111);
			}
		}

		//check to see if the current answer tile is solved, if so then update it's solved value
		if (currentAnswer->getPossibilities()->solved()) currentAnswer->setSolvedAnswer(currentAnswer->getPossibilities()->getHighest());

		//Check again to see if this answer tile has a letter inside of it, if anything was eliminated as a possiblity from the 
		//answer tile in the above FOR loop it should be reflected in that letterPossibility
		if (currentAnswer->getInternalClue() != ' ')
			board.getLetterPossibilities()->remove(currentAnswer->getInternalClue(), ~(currentAnswer->getPossibilities()->digits) & 0b1111111111);
	}
}
void clueTrimViaAnswerRevised(KakuroBoard &board, std::pair<int, int>& newPossibilities, bool &found, bool recursion = false, ClueTiles* currentClue = nullptr,
	int goal = 0, int currentNumber = 0, int usedNumbers = 0, bool horizontal = false, int level = 0)
{
	if (!recursion)
	{
		//this first part of the function is just for setting things up for the recursive part of the algorithm
		std::pair<int, ClueTiles**> clues = board.getClues();
		for (int i = 0; i < clues.first; i++)
		{
			ClueTiles* currentClue = *(clues.second + i);
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
							clueTrimViaAnswerRevised(board, newCluePossibilities, found, true, currentClue, 10 * j + k, 0, 0, !orientation);
						}
					}

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}

				//remove any possibilities not included in newCluePossibilities for the appropriate clue letter
				board.getLetterPossibilities()->remove(currentClue->getClueLetter(orientation, 1), ~newCluePossibilities.second & 0b1111111111);
				if (!oneDigit) board.getLetterPossibilities()->remove(currentClue->getClueLetter(orientation, 0), ~newCluePossibilities.first & 0b1111111111);

				//After updates have been made, see if the clue has been solved
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

		AnswerTiles* currentAnswer = (AnswerTiles*)board.getTile(currentClue->getLocation().first + !horizontal * (level + 1), currentClue->getLocation().second + horizontal * (level + 1));
		for (int i = currentAnswer->getPossibilities()->getLowest(); i <= currentAnswer->getPossibilities()->getHighest(); i++)
		{
			if (powers_of_two[i] & usedNumbers) continue; //don't pick a numbers that's been picked already
			if (i + currentNumber > goal) break; //numbers will only get higher so break out of loop

			clueTrimViaAnswerRevised(board, newPossibilities, found, true, currentClue, goal, currentNumber + i, usedNumbers | powers_of_two[i], horizontal, level + 1);

			if (found) return;
		}
	}
}
void answerTrimViaAnswerRevised(KakuroBoard &board, std::pair<int, int> location, int* answerTilePossibilities, int* selectedNumbers, int clueSize = 0, int currentNumber = 0, int currentSum = 0,
	int goal = 0, bool horizontal = false, bool recursion = false, int level = 0)
{
	if (!recursion)
	{
		//this part of the function is just to get everything set up for the recursive part below
		std::pair<int, ClueTiles**> clues = board.getClues();
		for (int i = 0; i < clues.first; i++)
		{
			ClueTiles* currentClue = *(clues.second + i);
			for (int orientation = Horizontal; orientation <= Vertical; orientation++)
			{
				if (currentClue->getClueAnswerLength(orientation) == 0) continue;
				int* tileOptions = new int[currentClue->getClueAnswerLength(orientation)]();
				int* usedDigits = new int[currentClue->getClueAnswerLength(orientation)]();

				//check all possibilites for the current clue
				bool oneDigit = false;
				if (currentClue->getClueLetter(orientation, 0) == ' ') oneDigit = true;//this is a 1 digit clue

				for (int j = 0; j < 10; j++) //represents the 10's digit
				{
					if (!oneDigit && !(powers_of_two[j] & currentClue->getCluePossibility(orientation, 0)->digits)) continue;

					for (int k = 0; k < 10; k++) //represents the 1's digit
						if (powers_of_two[k] & currentClue->getCluePossibility(orientation, 1)->digits)
							answerTrimViaAnswerRevised(board, { currentClue->getLocation().first + orientation, currentClue->getLocation().second + !orientation },
								tileOptions, usedDigits, currentClue->getClueAnswerLength(orientation), 0, 0, 10 * j + k, !orientation, true);

					//If we're looking at a one digit clue then break loop after one iteration
					if (oneDigit) break;
				}
				delete[] usedDigits;

				//update the answer tile possibilities to reflect what's stored in the tileOptions array
				for (int j = 0; j < currentClue->getClueAnswerLength(orientation); j++)
				{
					AnswerTiles* currentAnswer = (AnswerTiles*)board.getTile(currentClue->getLocation().first + (orientation * (j + 1)), currentClue->getLocation().second + (!orientation * (j + 1)));
					bool solvedYet = currentAnswer->getPossibilities()->solved(); //check to see if the current answer tile has been solved yet

					if (!solvedYet)
					{
						currentAnswer->getPossibilities()->digits = tileOptions[j];
						currentAnswer->setSolvedAnswer(currentAnswer->getPossibilities()->getHighest()); //set tile to solved status
						//Check to see if this answer tile has a letter inside of it, if so then we've solved for a letter
						if (currentAnswer->getInternalClue() != ' ')
							board.getLetterPossibilities()->remove(currentAnswer->getInternalClue(), ~(currentAnswer->getPossibilities()->digits) & 0b1111111111);
					}
				}
				delete[] tileOptions;
			}
		}
	}
	else
	{
		//the recursive part of the function
		if (level == clueSize)
		{
			//base of recursion. If we made it here then all numbers in the selected numbers array will work.
			//Update the possibilities vector accordingly
			
			if (currentSum == goal)
				for (int i = 0; i < clueSize; i++) answerTilePossibilities[i] |= selectedNumbers[i];

			return;
		}

		AnswerTiles* currentAnswer = (AnswerTiles*)board.getTile(location.first, location.second);
		for (int i = currentAnswer->getPossibilities()->getLowest(); i <= currentAnswer->getPossibilities()->getHighest(); i++)
		{
			//select any possible number that won't make us go over the goal
			if (powers_of_two[i] & currentAnswer->getPossibilities()->digits) //make sure that the number can go in the box
			{
				if (!(powers_of_two[i] & currentNumber)) //make sure we haven't already selected the number in a previous box
				{
					if (!(currentSum + i > goal)) //make sure adding this number won't exceed the goal
					{
						selectedNumbers[level] = powers_of_two[i]; //add number to selected vector
						answerTrimViaAnswerRevised(board, { location.first + !horizontal, location.second + horizontal },
							answerTilePossibilities, selectedNumbers, clueSize, currentNumber | powers_of_two[i], currentSum + i, goal, horizontal, true, level + 1);
						selectedNumbers[level] = 0; //remove number from selected vector
					}
				}
			}
		}
	}
}

std::pair<std::string, double> q424Revised()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	bool dummy = false; //a dummy variable needed to initialize some recursive functions

	//Before looping through all the puzzles, create a vector that stores all the different ways to solve (up to 6 digit) Kakuro clues.
	//This will make it much quicker to see at a glance what possibilities can be removed from answer tiles
	std::vector<std::vector<int> > combinations;
	kakuroNumberCombinationsRevised(combinations);

	std::ifstream myFile;
	std::string boardString;
	myFile.open("Resources/q424.txt");

	for (int i = 0; i < 1; i++)
	{
		//std::cout << "Starting game " << i << std::endl;
		//Read the next board from text file
		std::getline(myFile, boardString);

		//Create the necessary data structures
		KakuroBoard kakuroBoard;
		//LetterPossibilitays currentLetters;

		//Fill in the Kakuro board using a function to parse the board string
		createBoard(boardString, kakuroBoard);
		std::pair<int, ClueTiles**> clues;

		//Before getting to our solve loop, there are a few setup activities
		//we can do to help reduce the total number of options for each letter
		std::pair<int, int > letterHeirarchy[10] = { {0, 0} };
		createLetterHeirarchy(kakuroBoard, letterHeirarchy, 0, 0);
		initialClueSolverRevised(kakuroBoard);

		//Now that setup is complete, we enter our solve loop and don't exit it
		//until all 10 letters have been discovered
		//while (currentLetters.getLettersSolved() != 0b1111111111)
		for (int j = 0; j < 1; j++)
		{
			//The first thing we do is examine our letter heirarchy and see if we can eliminate any letter possibilities.
			//A recursive function is called on all letters at the base of a heirarchy chain
			possibilityTrimViaHeirarchy(kakuroBoard, letterHeirarchy, i);

			//Next we attempt to trim the list of possible numbers for each answer tile on the board by looking at their clues
			answerTrimViaClueRevised(kakuroBoard, combinations);

			//With the answer tiles trimmed down a little bit now, we take a look back at the clues and see if we can reduce the
			//possibilities for each clue based on the new answer tiles
			clueTrimViaAnswerRevised(kakuroBoard, letterHeirarchy[0], dummy); //letterHeirarchy[0] and dummy are used as placeholders

			//Now we use a brute force recursive algorithm to see if each of the possible numbers for every answer tile can actually be
			//used in a solution. Any numbers that aren't found to be part of a solution are removed from the possibility list.
			answerTrimViaAnswerRevised(kakuroBoard, { 0, 0 }, nullptr, nullptr);

			//Finally we use "Sudoku Style" elimination techniques to try and reduce the letter possibilites
		}
		//kakuroBoard.printAllCluePossibilities();
		//kakuroBoard.printAllAnswerPossibilities();
		kakuroBoard.printAllLetterPossibilities();

		//test print function
		//kakuroBoard.printBoard();
	}

	myFile.close(); //close down the file after reading all boards

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}