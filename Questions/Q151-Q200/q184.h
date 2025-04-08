#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <cmath>

//Triangles Containing the Origin
struct TriangleLine
{
	fraction slope;
	long long totalPointsBelowXAxis;
	long long totalPointsOnLine;
};

std::pair<std::string, double> q184()
{
	auto run_time = std::chrono::steady_clock::now();
	long long radius = 105;
	long long answer = 0;

	//First generate all distinct line slopes that will be used in order.
	std::vector<fraction> slopes;
	orderedFaraySequence(radius - 1, slopes);
	slopes.push_back({ 1, 1 }); //add a slope of 1 which isn't included with Faray sequence implementation

	//Memoize the height of the circle at all location for x
	std::vector<double> circleHeights = { (double)radius };
	long long radiusSquared = radius * radius, totalLaticePointsOnLines = 0, pointsInQuadrant = 0;
	for (int x = 1; x < radius; x++)
	{
		circleHeights.push_back(sqrt(radiusSquared - x * x));
		pointsInQuadrant += (long long)(circleHeights.back() - 0.0000001);
	}

	//int r2 = radius * radius;
	//for (int i = radius - 1; i > 0; i--)
	//{
	//	int jCeil = ceil(sqrt(r2 - i * i));
	//	if (i < jCeil) jCeil = i;
	//	for (int j = 1; j < jCeil; j++)
	//	{
	//		if (gcd(i, j) == 1)
	//		{
	//			slopes.push_back({ i, j });
	//		}
	//	}
	//}

	////Add a slope of 1 and then sort the slopes from largest to smallest
	//slopes.push_back({ 1, 1 });
	//std::sort(slopes.begin(), slopes.end(), std::greater<>());
	//int x = 5;

	//Use these slopes to calculate the number of lattice points that fall
	//above and below the line in the bottom right quadrant for all values of 
	//X. Also, calculate the total number of points above the line in the 
	//bottom right quadrant.
	std::vector<TriangleLine> lines;
	for (int i = 0; i < slopes.size(); i++)
	{
		long long totalPointsAbove = 0;
		for (int x = 1; x < radius; x++)
		{
			//Multiplying the numerator by x and then dividing by the denominator
			//will make sure that location won't have floating point precision errors
			//in the case that location is an integer.
			double location = slopes[i].numerator * x / (double)slopes[i].denominator;
			if (location > circleHeights[x]) location = circleHeights[x];

			//First count the points above the line y = m*x
			totalPointsAbove += ceil(location) - 1;
		}

		//Calculate the number of lattice points that fall on each line in the upper left quadrant.
		//Any points that fall on the outer circle need to be excluded so reduce the radius by a 
		//small number when performing the calculation.
		long long totalPointsOnLine = (radius - 0.000000001) / sqrt(slopes[i].numerator * slopes[i].numerator + slopes[i].denominator * slopes[i].denominator);
		if (totalPointsOnLine != 0)
		{
			//Add a new line (as long as it has at least 1 lattice point inside the circle), as well as it's inverse to the line array.
			lines.push_back({ slopes[i], totalPointsAbove, totalPointsOnLine });
			totalLaticePointsOnLines += totalPointsOnLine;
		}
	}

	//Add all of the lines with a slope greater than 1
	for (int i = lines.size() - 2; i >= 0; i--)
	{
		totalLaticePointsOnLines += lines[i].totalPointsOnLine;
		lines.push_back({ { slopes[i].denominator, slopes[i].numerator }, pointsInQuadrant - lines[i].totalPointsOnLine - lines[i].totalPointsBelowXAxis, lines[i].totalPointsOnLine });
	}

	//Add the horizontal, vertical and m=1 lines as they are excluded from the Faray Sequence above.
	//Reverse the order of the lines so we move from larger slopes to smaller ones.
	lines.insert(lines.begin(), { { (int)radius, 1 }, pointsInQuadrant, (long long)(radius - 1) }); //vertical line
	lines.push_back({ { 0, 1 },  0, radius - 1 }); //horizontal line
	std::reverse(slopes.begin(), slopes.end());

	//Iterate over all combinations of lines, counting triangles of type 1 and  
	//type 2 that we can create along the way
	for (int line1 = 0; line1 < lines.size() - 1; line1++)
	{
		//Since the lines are ordered by their slope, the amount of lattice points between 
		//two lines will simply equal the total number of lattice points on all lines that 
		//are in between them. Everytime we iterate line2 to the next line, add the points 
		//contained on it to the running total of points to calculate the points contained between 
		//line1 and the next line2.
		int latticePointsBetweenLines = lines[line1 + 1].totalPointsOnLine, type1Triangles = 0;
		for (int line2 = line1 + 2; line2 < lines.size(); line2++)
		{
			type1Triangles += lines[line2].totalPointsOnLine * latticePointsBetweenLines;
			latticePointsBetweenLines += lines[line2].totalPointsOnLine;
		}

		//Add all of the Type 1 triangles calculated above to the final answer
		answer += lines[line1].totalPointsOnLine * type1Triangles;

		//Add up all of the Type 2 triangles to the answer
		answer += totalLaticePointsOnLines * lines[line1].totalPointsOnLine * lines[line1].totalPointsBelowXAxis;
	}
	
	return { std::to_string(4 * answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
         
	//the answer is 1725323624056
	//ran in 0.010367 seconds
}

//NOTES
//*notes are now kept on Project-Floyd.com

//Currently when radius is 2 or 3 I get the correct answer, however, when the radius is 5 I'm undercounting by 52 triangles. May need to take a look
//at some drawings to see what the edge cases are. Most likely it has something to do with the fact that the grid when the radius i s2 or 3 is a perfect
//square, however, this may not be the case when the radius is 5.

//Rand into an issue with counting the number of lattice points between spots on different lines with the same X value. As an example, when the radius of 
//the circle is 5, line 1 is vertical and line 2 has a slope of 1/3, and we're looking at X=2. The bottom of the circle has a height of 4.582575 while the 
//m=1/3 line has a height of 0.667 at this location. The difference between the two points is only 3.916, however, there are actually 4 lattice points 
//between the two points and not 3. A little reflection on this shows that the distance between points the ressults in N lattice points can be as little 
//as N-1 + 0.00000...1 or as great as N+1 - 0.000..1. I'll need a more effective way to accurately count the lattice points between two locations. Since 
//I already split the lines into an upper and lower one, maybe what I should do is round the decimal of the upper line up, and round the decimal of the lower
//line down to integers. The number of lattice points in this scenarios would then be floor(lower) - ceil(upper) + 1. The problem is that this equation 
//won't work when the given line falls directly on a lattice point. I could always add a little bit to the upper line and subtract a little bit from the lower 
//line to make sure this isn't an issue, however, this will lead to issues where the lines in question are already outside of the circle. 

//One thing that I suppose would work is to keep adding 1 to the upper number until it's larger than the lower number. In the above example we'd get:
//0.6 + 1 = 1.6
//1.6 + 1 = 2.6
//2.6 + 1 = 3.6

//0.6 --> 4.5  (4 - 0) = 4
//1 --> 3