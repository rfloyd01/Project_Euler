#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>

//Counting Rectangles
std::pair<std::string, double> q85()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int limit = 2000000, counter = 2;
	std::vector<long long> triangle;
	triangle.push_back(0); triangle.push_back(1);

	//generate the triangle numbers with a seive so they don't need to be recalculated
	while (triangle.back() < limit)
	{
		triangle.push_back(triangle.back() + counter);
		counter++;
	}

	//multiply every pair to see what the closest value is
	int closest = limit, current_closest;
	bool cont = true; //as soon as triangle[i] * triangle[i] > 2000000 stop loop with this variable
	int index; //used to keep track of the appropriate j index

	for (int i = 1; i < triangle.size(); i++)
	{
		if (!cont) break;
		for (int j = i; j < triangle.size(); j++)
		{
			if (limit - (triangle[i] * triangle[j]) < 0) //iterate through until the value is over 2,000,000, from this point the distance will only get greater
			{
				if (j == i)
				{
					current_closest = -1 * (limit - (triangle[i] * triangle[j]));
					cont = false; //triangle[i] * triangle[i] will always be greater than the limit from here so break
				}
				else
				{
					if ((-1 * (limit - (triangle[i] * triangle[j]))) < (limit - (triangle[i] * triangle[j - 1])))
					{
						current_closest = -1 * (limit - (triangle[i] * triangle[j]));
						index = j;
					}
					else
					{
						current_closest = limit - (triangle[i] * triangle[j - 1]);
						index = j - 1;
					}
				}

				if (current_closest < closest)
				{
					closest = current_closest;
					answer = i * index;
				}
				break;
			}
		}
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 2772
	//ran in 0.0000533 seconds
}

//NOTES
//After a little bit of pencil and paper work it's fairly easy to see that the total number of rectangles
//in an m x n grid is equal to triangle(m) x triangle(n). I arrived to this conclusion in the following way.
//Create a rectangle by placing the top left corner all the way in the top left corner of the grid. A number
//of rectangles can now be created by placing the bottom right corner in different locations. From the 
//problem example using the 3x2 grid it can be seen that there are 6 locations that the bottom corner of
//the rectangle could be placed. Now put the top left corner one spot over on the grid and it can be seen 
//that there are 2 places to put the bottom right corner in the top row and two in the bottom row (2 x 2)
//for a total of 4. If you do this for every spot on the grid you get the following equations: (3x2) + (2x2)
// + (1x2) + (3x1) + (2x1) + (1x1) = 2 * (1 + 2 + 3) + 1 * (1 + 2 + 3) = (2 * 6 ) + (1 * 6) = 6 * (1+2)
// = 6 * 3. 6 is the third triangle number and 3 is the second triangle number and we started with a 3x2
//grid. With this easy equation we just need to generate all the triangle numbers up to 2,000,000 and
//multiply them in pairs to see what value is closest to 2,000,000. The final answer will be product of the
//indices of the triangle numbers chosen. The code runs very quick and the final answer is obtained by
//utilizing a 36 x 77 sized grid.