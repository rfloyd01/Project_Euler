#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //MyPow

//Fractions involving the number of different ways a number can be expressed as a sum of powers of 2
std::pair<std::string, double> q175()
{
	auto run_time = std::chrono::steady_clock::now();

	int currentColumn = 0;
	int goalFraction[2] = { 987654321 / 9, 123456789 / 9 }; //represents the ratio of f(n - 1) to f(n), this is the inverse of what the problem asks for
	std::vector<bool> directionTaken;

	//we have two options for terminating the below loop. Either goalFraction[0] will become 1, or,
	//goalFraction[1] - goalFraction[0] will equal 1.
	while (true)
	{
		if (goalFraction[0] == 1) break;
		else if (goalFraction[1] - goalFraction[0] == 1)
		{
			//we're all the way to the right side of the grid so we need to update the currentColumn value
			currentColumn = MyPow(2, goalFraction[0]) - 2;
			break;
		}

		//take whichever value is smaller and subtract it from the other. If the left side is smaller
		//then add a 0 to the direction vector, otherwise add a 1 to the direction vector.
		if (goalFraction[0] > goalFraction[1])
		{
			directionTaken.push_back(0); //0 represents left
			goalFraction[0] -= goalFraction[1];
		}
		else
		{
			directionTaken.push_back(1); //1 represents right
			goalFraction[1] -= goalFraction[0];
		}
	}

	//Now that we're at the outside of the grid, iterate backwards over the direction vector and move in 
	//the opposite direction as indicated by each value to get back to the goal fraction
	int power = goalFraction[1] - 1;
	for (int i = directionTaken.size() - 1; i >= 0; i--)
	{
		currentColumn *= 2;
		if (!directionTaken[i]) currentColumn += 1;
		power++;
	}

	std::string answer = "2^" + std::to_string(power) + " + " + std::to_string(currentColumn);
	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1,13717420,8
	//ran in 0.0000049 seconds
}

//NOTES
//Notes have been transferred to project-floyd