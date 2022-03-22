#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <vector>
#include<set>
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
};
class LetterPossibilitays
{
private:
	Possibilities letterPossibilities[10];
	int solved = 0, lettersFound = 0;

	void solve(int letter, int value)
	{
		//iterate through all letters in the letterPossibility array
		//and remove the solved bit from each. If this in turn solves
		//more letters, then solve function is recursively called.
		for (int i = 0; i < 10; i++)
			if (i != letter)
				this->remove(i, value);

		this->solved |= powers_of_two[letter];
		this->lettersFound++;
	}

public:
	void remove(int letter, int value)
	{
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
			for (int j = 0; j < 10; j++)
				if (this->letterPossibilities[i].digits & powers_of_two[j]) std::cout << j << " ";

			std::cout << std::endl;
		}
		std::cout << lettersFound << " Letters have been solved." << std::endl;
		std::cout << std::endl;
	}
	int getLettersFound()
	{
		return this->lettersFound;
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
	std::vector<std::vector<int> > cluePossibilities;
	LetterPossibilitays* currentLetterPossibilities;
	int answerLengths[2] = { 0, 0 };
	int solvedClueValues[2] = { 0, 0 };

public:
	void setClueLetter(int orientation, int digit, int letter)
	{
		this->clueLetters[orientation][digit] = letter;
	}
	char getClueLetter(int orientation, int digit) { return this->clueLetters[orientation][digit]; }
	void incrementAnswerLength(int orientation) { this->answerLengths[orientation]++; }
	int getClueAnswerLength(int orientation) { return this->answerLengths[orientation]; }
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
	void setClueTile(int orientation, ClueTiles* clue)
	{
		this->governingClues[orientation] = clue;
	}
	char getInternalClue() { return this->internalClue; }
	ClueTiles* getGoverningClue(int orientation) { return this->governingClues[orientation]; }
};
class KakuroBoard
{
private:
	int dimension = 0, totalClues = 0, totalAnswers = 0;
	BoardTiles*** board;
	ClueTiles** clueTiles;
	AnswerTiles** answerTiles;

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
};

//Functions for Kakuro Board manipulation
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
						ct->setClueLetter(clueType, 1, boardString[++currentIndex]); //this is only a one digit clue
					else
					{
						//this is a two digit clue, add both characters
						ct->setClueLetter(clueType, 0, boardString[++currentIndex]);
						ct->setClueLetter(clueType, 1, boardString[++currentIndex]);
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

std::pair<std::string, double> q424Revised()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//Before looping through all the puzzles, create a vector that stores all the different ways to solve (up to 6 digit) Kakuro clues.
	//This will make it much quicker to see at a glance what possibilities can be removed from answer tiles
	std::vector<std::vector<std::pair<int, std::vector<std::vector<int> > > > > combinations;
	kakuroNumberCombinations(combinations);

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
		LetterPossibilitays currentLetters;

		//fill in the Kakuro board using a function to parse the board string
		createBoard(boardString, kakuroBoard);

		
	}

	/*std::pair<int, ClueTiles**> clues = kakuroBoard.getClues();
		std::pair<int, AnswerTiles**> answers = kakuroBoard.getAnswers();

		for (int i = 0; i < answers.first; i++)
		{
			AnswerTiles* currentAnswer = *(answers.second + i);
			std::cout << "Answer " << i + 1 << "'s horizontal clue is located at [" << currentAnswer->getGoverningClue(Horizontal)->getLocation().first << ", " << currentAnswer->getGoverningClue(Horizontal)->getLocation().second
				<<"] and vertical clue is located at [" << currentAnswer->getGoverningClue(Vertical)->getLocation().first << ", " << currentAnswer->getGoverningClue(Vertical)->getLocation().second << ']' << std::endl;
		}*/

		//test print function
		//kakuroBoard.printBoard();

	myFile.close(); //close down the file after reading all boards

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}