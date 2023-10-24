#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>
#include <vector>
#include <cmath>
#include <random>

int test_answer = 0;

bool playGame(std::vector<int>& board)
{
	//The optimal strategy is to first remove any of your pieces with the other
	//players piece on top. If none of these moves are available, then the next
	//best move is to remove any of your pieces on top of the other players piece.
	//If none of these are availble just remove the first availble piece you can find.
	//If there are no pieces to be found then the current player loses the game.

	//start with the gold player first
	bool first_player = 0, current_player = 0, fair = true;

	std::vector<int> board_copy = board; //make a copy of the board that we can play with
	std::vector<std::vector<int> > move_order = { {21, 12}, {12, 21}, {11, 22}, {1, 2} };
	std::vector<std::vector<int> > after_move = { {0, 0}, {2, 1}, {1, 2}, {0, 0} };

	for (int i = 0; i < 2; i++)
	{
		bool game_over = false;
		while (!game_over)
		{
			game_over = true;
			for (int j = 0; j < 4; j++)
			{
				auto it = std::find(board_copy.begin(), board_copy.end(), move_order[j][current_player]);
				if (it != board_copy.end())
				{
					*(it) = after_move[j][current_player];
					current_player = !current_player; //switch players
					game_over = false; //the game continues since a move was made
					break; //break out of current loop
				}
			}

			if (game_over)
			{
				if (first_player != current_player) return false; //the starting player has won so this game isn't fair
			}

		}

		//Reset the game and have player 2 start now
		board_copy = board;
		first_player = 1, current_player = 1;
	}
	

	return true; //both players lost when starting so the game is fair
}

void recursiveBoardCreate(int currentPiece, int maxPiece, std::vector<int>& board, int current_stack = 0)
{
	if (currentPiece == maxPiece)
	{
		//the base of the recursion, have each player start a game and confirm 
		//that they will both lose when playing the ideal strategy. If this is the 
		//case then print out the board
		if (playGame(board)) vprint(board);
		return;
	}

	//Just brute force create every board possible. 1 = a gold piece and 2 = a silver piece.
	//There are only 4 possible stacks: 22, 21, 12 and 11 so place this in ascending order to 
	//ignore and permutations.
	//if (currentPiece % 2)
	//{
	//	//a gold piece goes on bottom of the current stack and we recurse downwards
	//	board[currentPiece / 2] += 1;
	//	recursiveBoardCreate(currentPiece + 1, maxPiece, board);

	//	//change the current piece to a silver one and recurse down again
	//	board[currentPiece / 2] += 1;
	//	recursiveBoardCreate(currentPiece + 1, maxPiece, board);

	//	//remove the current piece when recursion is complete
	//	board[currentPiece / 2] -= 2;
	//}
	//else
	//{
	//	//a gold piece goes on top of the current stack and we recurse downwards
	//	board[currentPiece / 2] += 10;
	//	recursiveBoardCreate(currentPiece + 1, maxPiece, board);

	//	//change the current piece to a silver one and recurse down again
	//	board[currentPiece / 2] += 10;
	//	recursiveBoardCreate(currentPiece + 1, maxPiece, board);

	//	//remove the current piece when recursion is complete
	//	board[currentPiece / 2] -= 20;
	//}
	for (int i = current_stack; i < 4; i++)
	{
		board[currentPiece / 2] = 10 * (1 + (i < 2)) + (1 + (i + 1) % 2);

		/*if (currentPiece / 2 > 0 && (board[currentPiece / 2] > board[currentPiece / 2 - 1]))
		{
			std::cout << "huh" << std::endl;
		}*/

		recursiveBoardCreate(currentPiece + 2, maxPiece, board, i);
	}
	board[currentPiece / 2] = 0; //reset current stack before returning

	return;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//Create all possible games with N stacks and determine whether
	//the game is "fair", if so then print out the board.
	int n = 10;
	std::vector<int> board(n);

	recursiveBoardCreate(0, 2 * n, board);


	return { std::to_string(test_answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}