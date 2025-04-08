#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>

#include <sstream>
#include <iomanip> // For std::fixed and std::setprecision

//Wandering Robots
std::pair<std::string, double> q575()
{
	auto run_time = std::chrono::steady_clock::now();
	double answer = 0;

	int gridSize = 1000;

	//Initialize the number of corner pieces (A), edge pieces (B)
	//and interior pieces (C) on the grid, as well as how many of each
	//of these piece types fall on a square number in the grid
	int A_Squares = 0, B_Squares = 0, C_Squares = 0;
	int A_Count = 4, B_Count = 4 * (gridSize - 2);
	int C_Count = gridSize * gridSize - A_Count - B_Count;

	for (int i = 1; i <= gridSize; i++)
	{
		//Calculate where in the grid (corner, edge, interior) that i^2 falls
		int square = i * i - 1; //subtract 1 for 0 indexing purposes
		int column = square % gridSize, row = square / gridSize;

		bool verticalEdge = (row == 0 || row == (gridSize - 1));
		bool horizontalEdge = (column == 0 || column == (gridSize - 1));

		if (verticalEdge && horizontalEdge) A_Squares++; //Corners
		else if (verticalEdge || horizontalEdge) B_Squares++; //Edges
		else C_Squares++; //Interior
	}

	//Calculate odds from counting method #1. Hand calculations showed that
	//A = 3B/4 and C = 5B/4
	fraction methodOneOdds = { 3 * A_Squares + 4 * B_Squares + 5 * C_Squares, 3 * A_Count + 4 * B_Count + 5 * C_Count };
	int methodOneGCD = gcd(methodOneOdds.numerator, methodOneOdds.denominator);
	methodOneOdds.numerator /= methodOneGCD;
	methodOneOdds.denominator /= methodOneGCD;

	//Calculate odds from counting method #2. Hand calculations showed that
	//A = 2B/3 and C = 4B/3
	fraction methodTwoOdds = { 2 * A_Squares + 3 * B_Squares + 4 * C_Squares, 2 * A_Count + 3 * B_Count + 4 * C_Count };
	int methodTwoGCD = gcd(methodTwoOdds.numerator, methodTwoOdds.denominator);
	methodTwoOdds.numerator /= methodTwoGCD;
	methodTwoOdds.denominator /= methodTwoGCD;

	//Take an equal weighted average of the above two fractions to get the 
	//final answer
	int denominatorLCM = lcm(2 * methodOneOdds.denominator, 2 * methodTwoOdds.denominator);
	fraction finalAnswer = { denominatorLCM / (2 * methodOneOdds.denominator) * methodOneOdds.numerator, denominatorLCM };
	finalAnswer.numerator += denominatorLCM / (2 * methodTwoOdds.denominator) * methodTwoOdds.numerator;
	answer = (double)finalAnswer.numerator / (double) finalAnswer.denominator;

	//Convert the final answer to the appropriate amount of decimal places
	std::ostringstream out;
	out << std::fixed << std::setprecision(12) << answer;
	
	return { out.str(), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 0.000989640561
	//ran in 0.0021868 seconds
}

//NOTES
/*
On the surface this problem seems easy enough, calculating the odds of the robot being in each square is straight forward (regardless of which of the two 
methodologies is correct). After enough rounds of updating the odds they should eventually reach a steady state and the square numbered spaces can have their
odds added up. The 5x5 grid is small enough that both methodologies can be calculated and we can then verify which methodology is the correct one. Moving up to
the 1000x1000 grid may be a challenge though. There are 1,000,000 different squares in this grid so if it takes many iterations before we reach a steady state 
then we may end up needing to do billions of multiplications to get to the answer. My gut says that the final odds for the same types of squares (i.e. corners,
edges 1 away from a corner, middle, etc.) will be the same so we can effectively divide the grid into quadrants and look at only one of them and get the same
answer. This will reduce the total calculations needed by 1/4 * 1/4 = 1/16 so it won't actually be that helpful if our runtime ends up being super long.

Before getting started, another thought here. Since the odds of moving from one square to the adjacent ones will always be the same then it should be possible to 
craft a matrix that we can multiply against our current odds to get the odds of the next turn. Call this multiplication matrix M. Since this multiplication 
matrix remains the same each turn, then we can find the odds after X amount of turns by simply calculating AM^X. To show how this works, let's pretend that the first of 
the methodologies is correct and we have an equal chance of remaing in the current square and moving to adjacent ones (i.e. a corner piece has 1/3 odds of staying 
in it instead of 1/2). Furthermore, let's assume that in turn 0 the starting square is randomized. This means that in a 3x3 grid there should be an equal 1/9 
chance of starting in each individual square

---------------------

After some thought I realized this could be solved by hand with equations. Go to ProjectFloyd for more details.
*/