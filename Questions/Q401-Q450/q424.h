#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
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
	//if the clue doesn't exist the value is -2
	int vertical, horizontal;
	//int verticalOptions[2][10], horizontalOptions[2][10];
	int verticalAnswerLength, horizontalAnswerLength;
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
	int options[10];
	char clue;
	ClueTile* horizontalClue, verticalClue;

	AnswerTile()
	{
		this->pieceType = "Answer";
		this->clue = 'O';
		for (int i = 0; i < 10; i++) options[i] = i;
	}

	void printPossibilities()
	{
		//prints all posibilities for each digit of both the horizontal and vertical clues
		std::cout << "Answer Tile Possibilities: ";
		for (int i = 0; i < 10; i++) if (this->options[i] != -1) std::cout << this->options[i] << " ";
		std::cout << std::endl;
	}
};
struct LetterPossibility
{
	int size = 10;
	int possibilities[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int answer = -1;

	void solve(int i)
	{
		if (i > 9 || i < 0) return;
		for (int i = 0; i < 10; i++) possibilities[i] = -1;
		possibilities[i] = i;
		size = 1;
		answer = i;
	}

	void remove(std::vector<int> numbers)
	{
		//removes all of the numbers in the given vector, however, only if
		//they haven't been removed already
		for (int i = 0; i < numbers.size(); i++)
		{
			if (possibilities[numbers[i]] == numbers[i])
			{
				possibilities[numbers[i]] = -1;
				size--;
			}
		}
		if (size == 1)
		{
			//set the answer if we've reduced the size variable to 1
			for (int i = 0; i < 10; i++)
				if (possibilities[i] != -1)
				{
					answer = i;
					break;
				}
		}
	}

	void print()
	{
		std::string answerString = "[";
		for (int i = 0; i < 10; i++)
		{
			if (possibilities[i] != -1)
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

//Functions for Board Manipulation
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
		//for each clue tile, set the length of any corresponding horizontal and vertical answers
		if (currentClue->horizontal != -2)
		{
			int clueLength = 1;
			while (true)
			{
				if (clueLocations[i].second + clueLength == dimension) break;
				if ((*(*(board + clueLocations[i].first) + clueLocations[i].second + clueLength))->pieceType != "Answer") break;
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
				clueLength++;
			}
			currentClue->verticalAnswerLength = clueLength - 1;
		}

		if (currentClue->hor1 != 'Z')
		{
			leadCharacters.insert(currentClue->hor1);
			letterPossibilities[currentClue->hor1 - 'A'].remove({ 0, 4, 5, 6, 7, 8, 9 }); //this letter must be a 1, 2 or 3
			//carry out a check here to see if there are only 2 answer tiles for this clue, if so, then the first digit as a 1
			if (currentClue->horizontalAnswerLength == 2)
			{
				letterPossibilities[currentClue->hor1 - 'A'].solve(1);
				//we can also remove 8 and 9 as options from the second letter (assuming it isn't the same as the first letter)
				if (currentClue->hor2 != currentClue->hor1) letterPossibilities[currentClue->hor2 - 'A'].remove({ 8, 9 });
				oneFound = true;
			}
			else if (currentClue->horizontalAnswerLength == 3)
			{
				letterPossibilities[currentClue->hor1 - 'A'].remove({ 3 }); //too short to start with a 3
				if (letterPossibilities[currentClue->hor1 - 'A'].answer == 2)
				{
					//if we've already found which letter 1 is then eliminating 3 here means the answer must be 2
					//if the answer block only has a length of three and the first digit is 2 then the second digit can only be 0, 1, 2, 3 or 4
					//because the maximum 3 digit number starting with 2 is 9+8+7 = 24;
					letterPossibilities[currentClue->hor2 - 'A'].remove({ 5, 6, 7, 8, 9 });
				}
			}
			else if (currentClue->horizontalAnswerLength == 6)
			{
				//with an answer of this length (which should be possible on a 7x7 board) we can eliminate 1 as a possibility as the smallest number we can 
				//make is 1+2+3+4+5+6 = 21
				letterPossibilities[currentClue->hor1 - 'A'].remove({ 1 });
			}
		}
		else if (currentClue->hor2 != 'Z') letterPossibilities[currentClue->hor2 - 'A'].remove({ 0, 1, 2 }); //single digit clues can't be 0, 1 or 2

		if (currentClue->vert1 != 'Z')
		{
			leadCharacters.insert(currentClue->vert1);
			letterPossibilities[currentClue->vert1 - 'A'].remove({ 0, 4, 5, 6, 7, 8, 9 }); //this letter must be a 1, 2 or 3
			//carry out a check here to see if there are only 2 answer tiles for this clue, if so, then the first digit as a 1
			if (currentClue->verticalAnswerLength == 2)
			{
				letterPossibilities[currentClue->vert1 - 'A'].solve(1);
				//we can also remove 8 and 9 as options from the second letter (assuming it isn't the same as the first letter)
				if (currentClue->vert2 != currentClue->vert1) letterPossibilities[currentClue->vert2 - 'A'].remove({ 8, 9 });
				oneFound = true;
			}
			else if (currentClue->verticalAnswerLength == 3)
			{
				letterPossibilities[currentClue->vert1 - 'A'].remove({ 3 }); //too short to start with a 3
				if (letterPossibilities[currentClue->vert1 - 'A'].answer == 2)
				{
					//if we've already found which letter 1 is then eliminating 3 here means the answer must be 2
					//if the answer block only has a length of three and the first digit is 2 then the second digit can only be 0, 1, 2, 3 or 4
					//because the maximum 3 digit number starting with 2 is 9+8+7 = 24;
					letterPossibilities[currentClue->vert2 - 'A'].remove({ 5, 6, 7, 8, 9 });
				}
			}
			else if (currentClue->verticalAnswerLength == 6)
			{
				//with an answer of this length (which should be possible on a 7x7 board) we can eliminate 1 as a possibility as the smallest number we can 
				//make is 1+2+3+4+5+6 = 21
				letterPossibilities[currentClue->vert1 - 'A'].remove({ 1 });
			}
		}
		else if (currentClue->vert2 != 'Z') letterPossibilities[currentClue->vert2 - 'A'].remove({ 0, 1, 2 }); //single digit clues can't be 0, 1 or 2
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
					if (oneFound) letterPossibilities[i].remove({ 0, 1, 4, 5, 6, 7, 8, 9 });
					else letterPossibilities[i].remove({ 0, 4, 5, 6, 7, 8, 9 });
				}
			}
			else letterPossibilities[i].remove({ 1, 2, 3 });
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
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 1 });
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
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 2 });
			if (letterPossibilities[i].size == 1 && letterPossibilities[i].answer == 3)
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
			if (letterPossibilities[i].size > 1) letterPossibilities[i].remove({ 0 });

	//Temporary printing function
	for (int i = 0; i < 10; i++)
	{
		std::cout << (char)(i + 'A') << ": ";
		letterPossibilities[i].print();
	}
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

std::pair<std::string, double> q424()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//Per the problem text there are 200 games, we need to read in each board 1 at a time, convert the string into a physical game board
	//and then solve it before moving onto the next board
	std::ifstream myFile;
	std::string boardString;
	myFile.open("Resources/q424.txt");

	//TODO: Change this loop from 1 to 200 after working out puzzle solving logic
	for (int i = 0; i < 1; i++)
	{
		std::getline(myFile, boardString);
		int dimension = boardString[0] - '0';
		BoardTile*** board = new BoardTile**[dimension]; //we won't know the dimension until we read it from file so we initialize the board using pointers
		std::vector<std::pair<int, int> > clueLocations; //an array holding the location of the clues in the puzzle for faster lookup
		std::vector<LetterPossibility> letterPossibilities; //represents the available numbers that each letter can represent, the puzzle is solved when each vector has length of 1
		for (int i = 0; i < 10; i++)
		{
			LetterPossibility lp;
			letterPossibilities.push_back(lp);
		}

		int currentIndex = 2; //always start scanning right after the first comma
		for (int i = 0; i < dimension; i++)
		{
			BoardTile** tiles = new BoardTile*[dimension]; //must use the new keyword here as we don't know the dimension at compile time
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

		//call the initial algorithm to try and solve some of our clues
		initialClueSolver(board, dimension, clueLocations, letterPossibilities);

		//look at the hierarchy of single letters. In the example puzzle we know that B < H because the H clue at location (1, 1) has B as a component.
		//Then looking down at the D clue at location (2, 1) we can see that H is a component of it, therefore, H < D. So from looking at just these
		//two clues we can see that B < H < D. We can also see that I < A for the same reason when looking at clue (3, 0) although currently that doesn't
		//tie into the BHD chain. In the example we were able to solve numbers 0-3 just from looking at the two digit clues, which means the other letters
		//must be 4-9. Since H must be larger than B, H CAN'T be 4 and likewise D CAN'T be 4 OR 5. On the flipside, this also means that B CAN'T be 8 or 9
		//while H also CAN'T be a 9. These relationships go for I and A as well
		numberHeirarchy(board, dimension, clueLocations, letterPossibilities);

		//create a list of all ways to get all numbers in Kakuro (with a maximum 6x6 board) with all numbers. For example, 15 can be made with two digits
		//by adding 9+6 or 8+7, three digits by adding 9+5+1, 9+4+2, 8+6+1, 8+5+2, 8+4+3, 7+6+2, 7+5+3 or 6+5+4, four digits by adding 9+3+2+1, 8+4+2+1,
		//7+5+2+1, 7+4+3+1, 6+5+3+1 or 6+4+3+2 and five digits by adding 5+4+3+2+1. We now need to look at all the individual answer boxes and reduce the
		//list of what's possible to go there based on 1) Any clues solved with the initialClueSolver() and 2) Any letters currently in the puzzle.

		//Once we've weedled the answer numbers down by looking at limitations from the clues, it's time to weedle the numbers further down by seeing
		//how some of these clue boxes play off of eachother. For example, the furthest right clue in the sample is 10. The answer box below this was
		//weedled down to option of only 6, 7, 8 and 9 because they intersect with the 30 clue to the left. The box below this one had it's options
		//trimmed down to 3, 4, 5, 7, 8 and 9 because they intersect with a clue of 12. Looking at how the options in these two boxes can combine to
		//max a 10 we can see that only a 6 in the top box and 4 in the bottom box, or 7 in the top box and 3 in the bottom box are possible. So we eliminate
		//the 8 and 9 from the top box and the 5-9 in the bottom box. Basically, this algorithm will be a brute force appraoch to make sure that all of the
		//numbers being listed as possibilites can actually be used in some sense to create the answer.

		//After getting all of the acceptable numbers into the puzzle, we again look at the clues, seeing if we can glean more info now. This time, however,
		//we're going to look at the maximum and minimum values we can have for each (unsolved) clue based on the numbers in each answer box. We didn't do 
		//this the first time around because each answer box contained all the digits 1-9 so it wouldn't have been very helpful.
		
		//printBoard(board, dimension);
		
		
		delete[] board; //delete the board after each iteration to save on memory
	}

	//Currently it takes 0.0012555 seconds just to create all of the boards. I think this is so slow because I'm creating lots of things in heap memory

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