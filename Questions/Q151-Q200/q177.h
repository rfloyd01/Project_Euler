#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <map>

//Integer Angled Quadrilaterals
double pi = PI;

enum TriangleType
{
	EQUILATERAL = 0,
	ISOCELES = 1,
	SCALENE = 2,
	END = 3
};

std::pair<TriangleType, std::vector<int> > createTriangleType(std::vector<int>& triangle)
{
	//Assign the given triangle a number based on it's type
	int equalAngles = 0b000;
	if (triangle[0] == triangle[1]) equalAngles |= 0b100;
	if (triangle[0] == triangle[2]) equalAngles |= 0b10;
	if (triangle[1] == triangle[2]) equalAngles |= 0b1;

	switch (equalAngles)
	{
	case 0b111: return { EQUILATERAL, triangle };
	case 0b0: return { SCALENE, triangle };
	default: return { ISOCELES, triangle };
	}
}

double diagonalIntegerCheck(int A, int B, int C, int D, int E, int F, std::vector<double>& sines, std::vector<double>& cosines, std::vector<std::vector<double>> & sinesOverSines)
{
    double x = atan((sines[F + C] * sinesOverSines[A][B]) / (sinesOverSines[E][D] - (cosines[F + C] * sinesOverSines[A][B]))) * 180.0 / pi;
	if (x < 0) x += 180; //x should be between 0 and 180
	int intXLower = x;
	int intXUpper = intXLower + 1;
	int intX = 0;

	double lowerDiff = x - intXLower;
	double upperDiff = intXUpper - x;

	if (lowerDiff <= 0.000000001)
	{
		intX = intXLower;
	}
	else if (upperDiff <= 0.000000001)
	{
		intX = intXUpper;
	}

	if (intX != 0)
	{
		//We have integer angles. Before returning a value, look at the triangles
		//that create the newly formed diagonal and see if they are equal to the 
		//current triangles (this will happen when combining 90-45-45 triangles along 
		//the 45-45 edges for example). In this scenario we return a value of 1, since 
		//the other triangle pair won't be created. If the new triangles are different
		//then the current ones return a value of 0.5. We will add the other 0.5 when 
		//the other triangle pair is created.
		int aPrime = F + C, bPrime = intX;

		if ((aPrime == A && (bPrime == B || bPrime == C)) || (aPrime == E && (bPrime == D || bPrime == F)))
		{
			//std::cout << "OG way" << std::endl;
			//oneCount++;
			return 1.0;
		}

		return 0.5;
	}
	return 0.0;
}

std::pair<std::string, double> q177()
{
	auto run_time = std::chrono::steady_clock::now();
	double answer = 0;

	//Create all integer angled triangles using an ordered integer
	//partition function and taking partitions of length 3
	std::vector<std::vector<std::vector<int> > > parts;
	getPartitions(180, parts, 180, 3);
	std::vector<std::pair<TriangleType, std::vector<int> > > integerAngledTriangles;
	for (int i = 0; i < parts[3].size(); i++)
	{
		integerAngledTriangles.push_back(createTriangleType(parts[3][i]));
	}
	parts.clear(); //delete the original partition array

	//We won't be carrying out trigonometric functions on any angles larger
	//than 180 degrees so it makes sense to memoize the following: cos(x), sin(x)
	//and sin(x)/sin(y). Before memoizing each result cast the inputs from degrees
	//to radians.
	std::vector<double> sines, cosines;
	std::vector<std::vector<double> > sinesOverSines = { {} }; //skip divisions by 0
	
	for (int i = 0; i <= 180; i++)
	{
		sines.push_back(sin(i * pi / 180.0));
		cosines.push_back(cos(i * pi / 180.0));
	}

	for (int i = 1; i < 180; i++)
	{
		sinesOverSines.push_back({0});
		for (int j = 1; j < 180; j++)
		{
			sinesOverSines.back().push_back(sines[i] / sines[j]);
		}
	}

	std::vector<std::vector<int> > indices = { {0, 1, 2}, {1, 2, 0}, {0, 2, 1} };

	//Choose all possible pairs of triangles and combine them into quadrilaterals
	//along all possible edges. This is done using the triangleIndices vector and
	//the type of each triangle (scalened, isoceles, equilateral) .
	for (int i = 0; i < integerAngledTriangles.size(); i++)
	{
		for (int j = i; j < integerAngledTriangles.size(); j++)
		{
			for (int triangleOneEdge = 0; triangleOneEdge <= integerAngledTriangles[i].first; triangleOneEdge++)
			{
				bool edgeOneSymmetric = (integerAngledTriangles[i].second[indices[triangleOneEdge][0]] == integerAngledTriangles[i].second[indices[triangleOneEdge][1]]);
				int A = integerAngledTriangles[i].second[indices[triangleOneEdge][2]];
				int B = integerAngledTriangles[i].second[indices[triangleOneEdge][0]];
				int C = integerAngledTriangles[i].second[indices[triangleOneEdge][1]];

				if (A == 1) continue;

				for (int triangleTwoEdge = 0; triangleTwoEdge <= integerAngledTriangles[j].first; triangleTwoEdge++)
				{
					if ((i == j) && (triangleTwoEdge < triangleOneEdge))
					{
						//when combining identical triangles there will be less edges to combine without creating the 
						//same quadrilateral. For example, given two scalene triangles with angles [70, 60, 50], combining
						//edge 70-60 of triangle one with edge 60-50 of triangle two will result in the same quadrilateral
						//as combining edge 60-50 of triangle one with edge 70-60 of triangle two.
						continue;
					}

					int D = integerAngledTriangles[j].second[indices[triangleTwoEdge][0]];
					int E = integerAngledTriangles[j].second[indices[triangleTwoEdge][2]];
					int F = integerAngledTriangles[j].second[indices[triangleTwoEdge][1]];

					if (E == 1) continue;

					if (B + D < 180 && C + F < 180)
					{
						answer += diagonalIntegerCheck(A, B, C, D, E, F, sines, cosines, sinesOverSines);
					}

					//If the current edge of triangle one or two are composed of the same angle then there's only one
					//way to attach the triangles along the given edges. Otherwise there are two ways.
					if (!edgeOneSymmetric && (integerAngledTriangles[j].second[indices[triangleTwoEdge][0]] != integerAngledTriangles[j].second[indices[triangleTwoEdge][1]]))
					{
						if (B + F < 180 && C + D < 180)
						{
							answer += diagonalIntegerCheck(A, B, C, F, E, D, sines, cosines, sinesOverSines);
						}
					}
				}
			}
		}
	}

	return { std::to_string((int)answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 129325
	//ran in 0.725622 seconds
}

//NOTES
//Notes have been transferred to project-floyd

/*
My logic worked out perfectly, the only bug I encountered was undercounting due to not properly rounding angles upwards as well as downwards. For example,
angles such as 34.000000000012 were being properly rounded down to 34 and counted as an integer, however, angles such as 34.9999999992 weren't being properly
rounded up to 35 and being counted as an integer.
*/