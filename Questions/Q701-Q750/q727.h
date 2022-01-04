#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes coprime

#include <iostream>
#include <cmath>
#include <vector>

//Triangle of Circular Arcs
void circleIntersection(int ra, int rb, int rc, double& x, double& y)
{
	//These equations were come up with by hand. There may be a way to reduce the equations, revisit if program runs too slowly.
	x = (ra * ra + ra * rb + ra * rc - rb * rc) / (double)(ra + rb);
	y = sqrt(ra * ra + 2 * ra * rc + rc * rc - x * x);
	std::cout << '[' << x << ", " << y << ']' << std::endl;
}

void triangleCenter(double ax, double ay, double bx, double by, double cx, double cy, double& Ex, double& Ey)
{
	Ex = (ax + bx + cx) / 3.0;
	Ey = (ay + by + cy) / 3.0;
}

double findDistance(int a, int b, int c)
{
	//find center of circle C

	//find where A+B touch, A+C touch and B+C touch
	//use the above locations to calculate E with triangleCenter()
}

std::pair<std::string, double> q727()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 100, count = 0; //count keeps track of the total number of triples to use for a weighted average
	double x, y;

	for (int i = 1; i <= limit -2; i++)
	{
		for (int j = i + 1; j < limit - 1; j++)
		{
			for (int k = j + 1; k < limit; k++)
			{
				if (coprime(i, j))
				{
					//since i and j are coprime the third number can be anything and the gcd of the three numbers will still be 1
					std::cout << i << ", " << j << ", " << k << std::endl;
					count++;
					circleIntersection(i, j, k, x, y);
				}
				else
				{
					//since i and j aren't coprime, then k will need to be coprime with either i or j in order for the gcd to be 1
					if (coprime(i, k))
					{
						std::cout << i << ", " << j << ", " << k << std::endl;
						count++;
						circleIntersection(i, j, k, x, y);
					}
					else if (coprime(j, k))
					{
						std::cout << i << ", " << j << ", " << k << std::endl;
						count++;
						circleIntersection(i, j, k, x, y);
					}
				}
			}
		}
	}

	std::cout << count << std::endl;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
//The radii of the three circles are all different from eachother with a maximum of 1 and a minimum of 100. This means at most there are choose(100, 3) = 161,700 combinations to choose.
//The actual limit should be less than this, however, because r_a, r_b and r_c need to have gcd == 1. Basically this problem shouldn't be too difficult for a brute force
//approach. Step one should be to generate all coprime triples with the numbers 1-100. After this, need to find the coordinates of the center of each circle in the Cartesian plane.
//Circle A will always be at (0, 0) and circle B will be at (r_a + r_b, 0). The center of circle C needs to be both a distance of r_a + r_c away from the center of circle A and
//r_b + r_c away from the center of circle B. There should be only two points that satisfy this criteria, it doesn't matter which one so just choose the one where the y value is
//positive. A function was devised on paper to get the values for cx and cy. From this point, the point E is the geometric center of the triangle formed from the three tangent points of
//A, B and C while the point D is the center of the circle that passes through all the tangent points. Wrote two separate functions to find those points. Since this is an expected value just
//loop through every iteration and multiply the distance between D and E by 1/total_number_triples.
