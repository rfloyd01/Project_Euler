#pragma once

#include <vector>
#include <iostream>
#include <string>

int total_animals = 9;

//this has nothing to do with Project Euler, just ended up saving it in here
void VecVecPrint(std::vector<std::vector<int> > board)
{
	for (int i = 0; i < board.size(); i++)
	{
		std::cout << "[ ";
		for (int j = 0; j < board[i].size() - 1; j++)
		{
			if (board[i][j] >= 10) std::cout << board[i][j] << ", ";
			else std::cout << " " << board[i][j] << ", ";
		}
		if (board[i].back() >= 10) std::cout << board[i].back() << "]" << std::endl;
		else std::cout << " " << board[i].back() << "]" << std::endl;
	}
}
void VecPrint(std::vector<int> yo)
{
	std::cout << '[';
	for (int i = 0; i < yo.size() - 1; i++) std::cout << yo[i] << ", ";
	std::cout << yo.back() << ']' << std::endl;
}
void MakeVecVec(std::vector<std::vector<int> >& vecvec, int rows, int cols)
{
	vecvec.clear();
	for (int i = 0; i < rows; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < cols; j++) row.push_back(0);
		vecvec.push_back(row);
	}
}

std::vector<int> MoveUp(std::vector<std::vector<int> >& board, int row, int col)
{
	std::vector<int> yo;

	if (board[row - 1][col]) yo = { row / 2 - 1, col / 2 };
	return yo;
}
std::vector<int> MoveDown(std::vector<std::vector<int> >& board, int row, int col)
{
	std::vector<int> yo;

	if (board[row + 1][col]) yo = { row / 2 + 1, col / 2 };
	return yo;
}
std::vector<int> MoveLeft(std::vector<std::vector<int> >& board, int row, int col)
{
	std::vector<int> yo;

	if (board[row][col - 1]) yo = { row / 2, col / 2 - 1 };
	return yo;
}
std::vector<int> MoveRight(std::vector<std::vector<int> >& board, int row, int col)
{
	std::vector<int> yo;

	if (board[row][col + 1]) yo = { row / 2, col / 2 + 1 };
	return yo;
}

void NextMoves(std::vector<std::vector<int> >& board, std::vector<std::vector<int> >& cur_board, int row, int col, int num)
{
	//std::cout << "Current space is: [" << row << ", " << col << "]" << std::endl;
	if (num == 0) cur_board[row][col] = 1;
	else cur_board[row][col] = num;

	std::vector<std::vector<int> > next_moves;
	std::vector<int> yo;

	if (row > 0) //if space right above hasn't been travelled to yet
	{
		if (cur_board[row - 1][col] == 0 | (num + 1) < cur_board[row - 1][col])
		{
			yo = MoveUp(board, row * 2, col * 2);
			if (yo.size()) next_moves.push_back(yo);
		}
	}

	if (row < (cur_board.size() - 1))
	{
		if (cur_board[row + 1][col] == 0 | (num + 1) < cur_board[row + 1][col]) //if space directly below hasn't been travelled to yet
		{
			yo = MoveDown(board, row * 2, col * 2);
			if (yo.size()) next_moves.push_back(yo);
		}
	}

	if (col > 0)
	{
		if (cur_board[row][col - 1] == 0 | (num + 1) < cur_board[row][col - 1]) //if space directly to the left hasn't been travelled to yet
		{
			yo = MoveLeft(board, row * 2, col * 2);
			if (yo.size()) next_moves.push_back(yo);
		}
	}

	if (col < (cur_board[row].size() - 1))
	{
		if (cur_board[row][col + 1] == 0 | (num + 1) < cur_board[row][col + 1]) //if space directly to the right hasn't been travelled to yet
		{
			yo = MoveRight(board, row * 2, col * 2);
			if (yo.size()) next_moves.push_back(yo);
		}
	}

	//VecVecPrint(next_moves); std::cout << std::endl;
	for (int i = 0; i < next_moves.size(); i++) NextMoves(board, cur_board, next_moves[i][0], next_moves[i][1], num + 1);
}
void Function(std::vector<int>& can_move_to, std::vector<std::vector<int> >& locations, std::vector<std::vector<int> >& answers, std::vector<std::vector<std::vector<std::vector<int> > > >& routes,
	std::vector<int>& current_path, int row, int col, int current_distance, int capacity, int& current_min)
{
	if (current_distance > current_min) return; //only care about finding the quickest route, as soon as current route becomes not the quickest there's no need to keep going with it

	if (capacity >= 4)
	{
		//the car is currently full so can only stop off at the house for an animal currently in the car
		for (int i = total_animals; i < 2 * total_animals; i++)
		{
			//houses are the second half of the locations array, may need to change 8 and 16 to accomodate variable distances
			if (can_move_to[i])
			{
				//if the house is available then go to it
				can_move_to[i] = 0;
				current_path.push_back(i);
				int new_row = locations[i][0], new_col = locations[i][1];
				int new_distance = routes[row][col][new_row][new_col];
				Function(can_move_to, locations, answers, routes, current_path, new_row, new_col, current_distance + new_distance, capacity - 1, current_min);
				can_move_to[i] = 1; //reset to 1 so that the house can be visited in further down levels of the recursive function
				current_path.pop_back();
			}
		}
	}
	else
	{
		//the car is not yet full so animals can be picked up, as well as houses visited if the animals are already in the car
		for (int i = 0; i < 2 * total_animals; i++)
		{
			//always start at the beginning of the list to allow for "out of order" permutations
			if (can_move_to[i])
			{
				//if the house or animal is available then go to it
				can_move_to[i] = 0;
				current_path.push_back(i);
				int new_row = locations[i][0], new_col = locations[i][1];
				int new_distance = routes[row][col][new_row][new_col];
				if (i < total_animals)
				{
					//if i < total_animals an animal was added, capcity++, also, adding an animal opens up a new house to be traveled to
					can_move_to[i + total_animals] = 1;
					Function(can_move_to, locations, answers, routes, current_path, new_row, new_col, current_distance + new_distance, capacity + 1, current_min);
					can_move_to[i + total_animals] = 0; //reset to 0 as the iteration is done so the animal is no longer in the car yet, therefore the house can't yet be travelled to
				}
				else Function(can_move_to, locations, answers, routes, current_path, new_row, new_col, current_distance + new_distance, capacity - 1, current_min); //if i >= 8 an animal was removed, capacity--
				can_move_to[i] = 1; //reset to 1 so that the house can be visited in further down levels of the recursive function
				current_path.pop_back();
			}
		}
	}

	if (current_path.size() == 2 * total_animals)
	{
		//a current_path of length 16 signifies that every animal has been dropped off at their house and the path is complete
		current_path.push_back(current_distance);
		answers.push_back(current_path);
		current_path.pop_back();

		current_min = current_distance; //to get this far in the algorithm this must be the currently most optimal path so need to update the current minimum
	}

	return;
}

void PetDetective()
{
	//STEP 0: Make sure the global variable "total_animals" reflects the number of animals that need to be rescued
	
	//STEP 1: Create the game board, every node is a 2, every road is a 1 and blank spaces are 0's
	std::vector<int> row1 = { 2, 1, 2, 0, 2, 1, 2, 1, 2 };
	std::vector<int> row2 = { 1, 0, 1, 0, 1, 0, 0, 0, 1 };
	std::vector<int> row3 = { 2, 1, 2, 1, 2, 1, 2, 1, 2 };
	std::vector<int> row4 = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	std::vector<int> row5 = { 2, 0, 2, 1, 2, 0, 2, 1, 2 };
	std::vector<int> row6 = { 1, 0, 1, 0, 1, 0, 1, 0, 1 };
	std::vector<int> row7 = { 2, 1, 2, 1, 2, 1, 2, 1, 2 };
	std::vector<std::vector<int> > board;
	board.push_back(row1); board.push_back(row2); board.push_back(row3); board.push_back(row4);
	board.push_back(row5); board.push_back(row6); board.push_back(row7);

	int rows = 4, columns = 5; //represent rows and colums the car can actually stop at, basically only every other row and column shown above

	//STEP 2: Input the starting postion of the car
	int start_row = 2;
	int start_column = 2;

	//STEP 3: Create a vector of Strings with all of the animals on the board, and put the locations of all animals and their respective houses into a single vector
	std::vector<std::string> animals;
	animals.push_back("Red Cat");
	animals.push_back("Purple Hedgehog");
	animals.push_back("Purple Otter");
	animals.push_back("Red Bird");
	animals.push_back("Green Turtle");
	animals.push_back("Blue Cat");
	animals.push_back("Yellow Dog");
	animals.push_back("Blue Dog");
	animals.push_back("Green Lizard");

	//keep the order of the locations below the same as the order of the animal strings listed above
	std::vector<int> red_cat = { 1, 0 }, red_cat_house = { 0, 4 };
	std::vector<int> purple_hedgehog = { 1, 2 }, purple_hedgehog_house = { 0, 0 };
	std::vector<int> purple_otter = { 2, 1}, purple_otter_house = { 2, 3 };
	std::vector<int> red_bird = { 3, 0 }, red_bird_house = { 2, 4 };
	std::vector<int> green_turtle = { 3, 4 }, green_turtle_house = { 3, 1 };
	std::vector<int> blue_cat = { 1, 1 }, blue_cat_house = { 2, 0 };
	std::vector<int> yellow_dog = { 3, 3 }, yellow_dog_house = { 0, 1 };
	std::vector<int> blue_dog = { 0, 2 }, blue_dog_house = { 3, 2 };
	std::vector<int> green_lizard = { 1, 4 }, green_lizard_house = { 0, 3 };

	//first add all of the animals to the list, then add the animal houses to the list, maintain the same order i.e. {animal1, animal2, animalN, house1, house2, houseN}
	std::vector<std::vector<int> > locations = { red_cat, purple_hedgehog, purple_otter, red_bird, green_turtle, blue_cat, yellow_dog, blue_dog, green_lizard,
												 red_cat_house, purple_hedgehog_house, purple_otter_house, red_bird_house, green_turtle_house, blue_cat_house, yellow_dog_house, blue_dog_house, green_lizard_house };

	//A map for each square on the board showing how far away every other spot on the board is created and stored in the quickest_routes array
	//A map with 24 nodes, therefore, will cause the quickest routes array to have 24 total, 24 node maps in it
	std::vector<std::vector<std::vector<std::vector<int> > > > quickest_routes;
	for (int i = 0; i < rows; i++)
	{
		std::vector<std::vector<std::vector<int> > > r;
		for (int j = 0; j < columns; j++)
		{
			std::vector<std::vector<int> > current_board; MakeVecVec(current_board, rows, columns);
			NextMoves(board, current_board, i, j, 0);
			current_board[i][j] = 0;
			r.push_back(current_board);
		}
		quickest_routes.push_back(r);
	}

	//fourth step is to iterate through all possibilities
	//you can only visit a square that has an animal on it, or a house for an animal that you have in your car which limits the total number of possibilities
	//We only want to visit the houses (not just pass over them) when the corresponding animal is in the car, so we initialize the can visit array with a one
	//for each animal and a 0 for each house
	std::vector<int> can_currently_visit;
	for (int i = 0; i < total_animals; i++) can_currently_visit.push_back(1);
	for (int i = 0; i < total_animals; i++) can_currently_visit.push_back(0);
	std::vector<std::vector<int> > answers;
	std::vector<int> current_path;
	int shortest_distance = 1000; //pick some arbitrarily large number so that routes smaller than it can be discovered

	//FINAL STEP: All possibilites are iterated through until the shortest route is found
	Function(can_currently_visit, locations, answers, quickest_routes, current_path, start_row, start_column, 0, 0, shortest_distance);

	//The below code prints out all of the steps to take in a readable fashion
	std::cout << "The following route takes " << answers.back().back() << " steps to complete." << std::endl << std::endl;
	for (int i = 0; i < answers.back().size() - 1; i++)
	{
		std::cout << "Step " << i + 1 << ": ";
		if (answers.back()[i] < total_animals) std::cout << "Pickup the " << animals[answers.back()[i]] << '.' << std::endl;
		else std::cout << "Dropoff the " << animals[answers.back()[i] - total_animals] << '.' << std::endl;
	}
}
