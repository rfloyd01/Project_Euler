#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes fraction, FaraySequence

//Right Triangles with Integer Coordinates
std::pair<std::string, double> q91()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 50;

	std::vector<fraction> coprime_pairs;
	FaraySequence(limit, coprime_pairs);
	coprime_pairs.push_back({ 1, 1 }); //manually add case for a slope of 1

	//calculate all triangles where 90 degree angle is touching at least one axis
	answer += 3 * limit * limit;

	int one, two, mult = 2; //integers to hold math in below loop

	//calculate all triangles with a slope less than or equal to 1
	for (int i = 0; i < coprime_pairs.size(); i++)
	{
		if (i == coprime_pairs.size() - 1) mult = 1; //change the multiplier for case of slope = 1
		fraction current_point = coprime_pairs[i];
		while (current_point.denominator <= limit)
		{
			//calculate triangles that can be formed down and to the right
			one = (limit - current_point.denominator) / coprime_pairs[i].numerator;
			two = current_point.numerator / coprime_pairs[i].denominator;
			if (one > 0 && two > 0) one < two ? answer += mult * one : answer += mult * two;

			//calculate triangles that can be formed up and to the left
			one = current_point.denominator / coprime_pairs[i].numerator;
			two = (limit - current_point.numerator) / coprime_pairs[i].denominator;
			if (one > 0 && two > 0) one < two ? answer += mult * one : answer += mult * two;

			//go to next point
			current_point.numerator += coprime_pairs[i].numerator;
			current_point.denominator += coprime_pairs[i].denominator;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 14234
	//ran in 0.0000638 seconds
}

//NOTES
//There are two different kinds of triangles that need to focused on: triangles where the 90 degree angle is
//at the origin, and triangles where the 90 degree angle isn't on the origin. Triangles where the 90 degree
//angle is on the origin are really easy to count because there must be a point on the origin, a point on the
//x-axis and a point on the y-axis. On an n x n grid there there are n^2 ways to choose two points where one
//each is on the x and y axis so that's how many different ways there are to make a triangle with a 90 degree
//angle at the origin. After a little looking it can be seen that the number of ways to make triangles where the
//90 degree angle is on the x or y axis is the same for when it's at the origin. So to get all the triangles
//where the 90 degree angle is touching at least one axis just take 3 * n^2. A little more complicated is to
//find the triangles where the 90 degree angle is off of the origin.

//When looking at a Cartesian coordinate system it's very helpful for this problem to realize that two lines
//with an inverse and opposite slope will be perpendicular to each other. For example, a line starting at the
//origin and ending at the point (4, 1) will have a slope of +1/4 so a line with a slope of -4 will be perpendicular
//to this line. Going from the point (4, 1) there are two different integer coordinates we can move to that
//are a slope of -4 away, the first point is down and to the right from the existing point with a rise of -4
//and a run of +1, aka (5, -3). The second possible point will be up and to the left from the original point
//with a rise of 4 and a run of -1, aka point (3, 5). The point (3, 5) is in the upper right quadrant of the
//Cartesean grid so it can be used to form a 90 degree triangle with points (0, 0), (4, 1) and (3, 5). If we
//look at all possible slopes that can be made with integers from 1 to 50, it will be possible to find all
//of the right angle triangles. To find all of the different slopes we just need to look at all of the coprime
//pairs of integers where no numbers are less then 50 (we look at coprime numbers because pairs that aren't 
//coprime can be reduced, such as a rise of 8 and a run of 2 is the same thing as a rise of 4 and a run of 1).
//Going back to the slope of 1/4 example, if n is 50, there are 12 distinct points (floor(50/4)) that will
//have a slope of 1/4 when a line is drawn from them to the origin: (4, 1), (8, 2), (12, 3) ... (48, 12). A line
//with a slope of -4 drawn from any of those 12 points will form a right angle triangle with integer 
//coordinates. 

//Also, to speed up the runtime of the problem a little bit we can just look at all triangles with a slope
//that's less than 1 and realize that there's a reflection of that triangle about the line y=x which will be
//contained in the first quadrant of the Cartesean grid. With this knowledge, we can easily generate all
//coprime pairs under 50 via the Faray sequence (which only generates fractions between 0 and 1). We can then
//double the answer from those fractions to represent the reflection of triangles over the line y=x. The only
//caveat to this is triangles where the initial line has a slope of 1 exactly. These triangles don't need to
//be doubled as their reflection over the line y=x is just the same as creating a triangle moving up and left
//instead of down and right.