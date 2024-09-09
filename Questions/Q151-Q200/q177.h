#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

//Integer Angled Quadrilaterals
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

bool diagonalIntegerCheck(int A, int B, int C, int D, int E, int F)
{
	double x = atan((sin(F + C) * sin(A) / sin(B)) / (sin(E) / sin(D) - (cos(F + C) * sin(A) / sin(B))));
	int intX = x;

	double diff = x - intX;

	if (diff <= 0.000000001 && diff >= -0.000000001) return true;
	return false;
}

std::pair<std::string, double> q177()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

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

	//Pre-create a list of indices which tells us which angles of 
	//each triangle to combine to form a quadrilateral.
	//std::vector<std::vector<std::vector<int> > > scalene_scalene_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}}, {{0, 0},{1, 2},{2, 1}}, {{0, 2},{1, 0},{2, 1}}, {{0, 1},{1, 2},{2, 0}}, {{0, 2},{1, 1},{2, 0}},
	//	{{0, 0},{2, 1},{1, 2}}, {{0, 1},{2, 0},{1, 2}}, {{0, 0},{2, 2},{1, 1}}, {{0, 2},{2, 0},{1, 1}}, {{0, 1},{2, 2},{1, 0}}, {{0, 2},{2, 1},{1, 0}},
	//	{{1, 0},{2, 1},{0, 2}}, {{1, 1},{2, 0},{0, 2}}, {{1, 0},{2, 2},{0, 1}}, {{1, 2},{2, 0},{0, 1}}, {{1, 1},{2, 2},{0, 0}}, {{1, 2},{2, 1},{0, 0}}
	//};
	//std::vector<std::vector<std::vector<int> > > scalene_isoceles_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}}, {{0, 1},{1, 2},{2, 0}}, {{0, 2},{1, 1},{2, 0}},
	//	{{0, 0},{2, 1},{1, 2}}, {{0, 1},{2, 0},{1, 2}}, {{0, 1},{2, 2},{1, 0}}, {{0, 2},{2, 1},{1, 0}},
	//	{{1, 0},{2, 1},{0, 2}}, {{1, 1},{2, 0},{0, 2}}, {{1, 1},{2, 2},{0, 0}}, {{1, 2},{2, 1},{0, 0}}
	//};
	//std::vector<std::vector<std::vector<int> > > scalene_equilateral_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}},
	//	{{0, 0},{2, 1},{1, 2}}, {{0, 1},{2, 0},{1, 2}},
	//	{{1, 0},{2, 1},{0, 2}}, {{1, 1},{2, 0},{0, 2}}
	//};
	//std::vector<std::vector<std::vector<int> > > isoceles_isoceles_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}}, {{0, 1},{1, 2},{2, 0}}, {{0, 2},{1, 1},{2, 0}},
	//	{{1, 0},{2, 1},{0, 2}}, {{1, 1},{2, 0},{0, 2}}, {{1, 1},{2, 2},{0, 0}}, {{1, 2},{2, 1},{0, 0}}
	//};
	//std::vector<std::vector<std::vector<int> > > isoceles_equilateral_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}},
	//	{{1, 0},{2, 1},{0, 2}}, {{1, 1},{2, 0},{0, 2}}
	//};
	//std::vector<std::vector<std::vector<int> > > equilateral_equilateral_indices =
	//{
	//	{{0, 0},{1, 1},{2, 2}}, {{0, 1},{1, 0},{2, 2}}
	//};

	////Empty vectors are used for repeat combinations
	//std::vector<std::vector<std::vector<std::vector<int> > > > triangleIndices = 
	//{
	//	scalene_scalene_indices, scalene_isoceles_indices, scalene_equilateral_indices,
	//	{}, isoceles_isoceles_indices, isoceles_equilateral_indices,
	//	{}, {}, equilateral_equilateral_indices
	//};

	std::vector<std::vector<int> > indices = { {0, 1, 2}, {1, 2, 0}, {0, 2, 1} };

	//Choose all possible pairs of triangles and combine them into quadrilaterals
	//along all possible edges. This is done using the triangleIndices vector and
	//the type of each triangle (scalened, isoceles, equilateral) .
	for (int i = 0; i < integerAngledTriangles.size(); i++)
	{
		for (int j = 0; j < integerAngledTriangles.size(); j++)
		{

			if (integerAngledTriangles[i].second[0] == 90 && integerAngledTriangles[i].second[1] == 45 && integerAngledTriangles[i].second[2] == 45)
			{
				if (integerAngledTriangles[j].second[0] == 90 && integerAngledTriangles[j].second[1] == 45 && integerAngledTriangles[j].second[2] == 45)
				{
					int x = 112;
				}
			}

		//	//figure out which part of the triangleIndices array to use based on the types of 
		//	//triangles we're currently looking at.
		//	int triangleIndex = 3 * integerAngledTriangles[i].first + integerAngledTriangles[j].first;
		//	std::vector<int> *triangleOne = &integerAngledTriangles[i].second, *triangleTwo = &integerAngledTriangles[j].second;
		//	if (integerAngledTriangles[j].first < integerAngledTriangles[i].first)
		//	{
		//		triangleOne = &integerAngledTriangles[j].second;
		//		triangleTwo = &integerAngledTriangles[i].second;
		//		triangleIndex = 3 * integerAngledTriangles[j].first + integerAngledTriangles[i].first;
		//	}

		//	//TODO: Print out triangle combos while testing
		//	std::cout << "Triangle One = [" << triangleOne->at(0) << ", " << triangleOne->at(1) << ", " << triangleOne->at(2) << "], Triangle Two = [";
		//	std::cout << triangleTwo->at(0) << ", " << triangleTwo->at(1) << ", " << triangleTwo->at(2) << "]" << std::endl;

		//	//iterate over all distinct edge combinations for the two triangles
		//	for (int k = 0; k < triangleIndices[triangleIndex].size(); k++)
		//	{
		//		//First calculate the unused angles for each triangle. If either of these angles is one
		//		//then it's impossible for the diagonal to be broken into integers so we continue
				//int triangleOneAngleThree = triangleOne->at(triangleIndices[triangleIndex][k][2][0]);
				//int triangleTwoAngleThree = triangleTwo->at(triangleIndices[triangleIndex][k][2][1]);
				////if (triangleOneAngleThree == 1 || triangleTwoAngleThree == 1) continue;

				//int triangleOneAngleOne = triangleOne->at(triangleIndices[triangleIndex][k][0][0]);
				//int triangleOneAngleTwo = triangleOne->at(triangleIndices[triangleIndex][k][1][0]);
				//int triangleTwoAngleOne = triangleTwo->at(triangleIndices[triangleIndex][k][0][1]);
				//int triangleTwoAngleTwo = triangleTwo->at(triangleIndices[triangleIndex][k][1][1]);
		//		
		//		if ((triangleOneAngleOne + triangleTwoAngleOne < 180) && (triangleOneAngleTwo + triangleTwoAngleTwo < 180))
		//		{
		//			diagonalIntegerCheck(triangleOneAngleThree, triangleOneAngleOne, triangleOneAngleTwo, triangleTwoAngleOne, triangleTwoAngleThree, triangleTwoAngleTwo);
		//		}
		//	}
		//	std::cout << std::endl;

			//std::cout << "Triangle One = [" << integerAngledTriangles[i].second[0] << ", " << integerAngledTriangles[i].second[1] << ", " << integerAngledTriangles[i].second[2] << "], Triangle Two = [";
			//std::cout << integerAngledTriangles[j].second[0] << ", " << integerAngledTriangles[j].second[1] << ", " << integerAngledTriangles[j].second[2] << "]" << std::endl;

			for (int triangleOneEdge = 0; triangleOneEdge <= integerAngledTriangles[i].first; triangleOneEdge++)
			{
				bool edgeOneSymmetric = (integerAngledTriangles[i].second[indices[triangleOneEdge][0]] == integerAngledTriangles[i].second[indices[triangleOneEdge][1]]);
				int A = integerAngledTriangles[i].second[indices[triangleOneEdge][2]];
				int B = integerAngledTriangles[i].second[indices[triangleOneEdge][0]];
				int C = integerAngledTriangles[i].second[indices[triangleOneEdge][1]];

				if (A == 1) continue;

				for (int triangleTwoEdge = 0; triangleTwoEdge <= integerAngledTriangles[j].first; triangleTwoEdge++)
				{
					if (i == j)
					{
						//when combining identical triangles there will be less edges to combine with creating the 
						//same quadrilateral. For example, given two scalene triangles with angles [70, 60, 50], combining
						//edge 70-60 of triangle one with edge 60-50 of triangle two will result in the same quadrilateral
						//as combining edge 60-50 of triangle one with edge 70-60 of triangle two.
					}
					else
					{
						//If the current edge of triangle one or two are composed of the same angle then there's only one
						//way to attach the triangles along the given edges. Otherwise there are two ways.

						int D = integerAngledTriangles[j].second[indices[triangleTwoEdge][0]];
						int E = integerAngledTriangles[j].second[indices[triangleTwoEdge][2]];
						int F = integerAngledTriangles[j].second[indices[triangleTwoEdge][1]];

						if (E == 1) continue;

						if (B + D < 180 && C + F < 180)
						{
							if (diagonalIntegerCheck(A, B, C, D, E, F))
							{
								std::cout << "A = " << A << ", B = " << B << ", C = " << C << ", D = " << D << ", E = " << E << ", F = " << F << std::endl;
							}
						}

						if (!edgeOneSymmetric && (integerAngledTriangles[j].second[indices[triangleTwoEdge][0]] != integerAngledTriangles[j].second[indices[triangleTwoEdge][1]]))
						{
							if (B + F < 180 && C + D < 180)
							{
								if (diagonalIntegerCheck(A, B, C, F, E, D))
								{
									std::cout << "A = " << A << ", B = " << B << ", C = " << C << ", D = " << F << ", E = " << E << ", F = " << D << std::endl;
								}
							}
						}
					}
				}
			}
			//std::cout << std::endl;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
//Notes have been transferred to project-floyd

/*
When picking isoceles indices, use [0, 1] and [1, 2] as this will guarantee unique pairs regardless of which 
two ordered angles are equal. For example, given the triangle [90, 45, 45] the pairs are [90, 45] and [45, 45]
while for the triangle [70, 70, 40] the pairs are [70, 70] and [70, 40].

Scalene index pairs  = [0, 1], [1, 2], [0, 2]
Isoceles index pairs = [0, 1], [1, 2]   
Equilateral pair     = [0, 1]

scalene-scalene:
Tri 1   Tri 2
-----   -----
[0, 1]  [0, 1] --> [[0, 0], [1, 1]], [[0, 1], [1, 0]]
[0, 1]  [0, 2] --> [[0, 0], [1, 2]], [[0, 2], [1, 0]]
[0, 1]  [1, 2] --> [[0, 1], [1, 2]], [[0, 2], [1, 1]]
[0, 2]  [0, 1] --> [[0, 0], [2, 1]], [[0, 1], [2, 0]]
[0, 2]  [0, 2] --> [[0, 0], [2, 2]], [[0, 2], [2, 0]]
[0, 2]  [1, 2] --> [[0, 1], [2, 2]], [[0, 2], [2, 1]]
[1, 2]  [0, 1] --> [[1, 0], [2, 1]], [[1, 1], [2, 0]]
[1, 2]  [0, 2] --> [[1, 0], [2, 2]], [[1, 2], [2, 0]]
[1, 2]  [1, 2] --> [[1, 1], [2, 2]], [[1, 2], [2, 1]]

scalene-isoceles:
Tri 1   Tri 2
-----   -----
[0, 1]  [0, 1] --> [[0, 0], [1, 1]], [[0, 1], [1, 0]]
[0, 1]  [1, 2] --> [[0, 1], [1, 2]], [[0, 2], [1, 1]]
[0, 2]  [0, 1] --> [[0, 0], [2, 1]], [[0, 1], [2, 0]]
[0, 2]  [1, 2] --> [[0, 1], [2, 2]], [[0, 2], [2, 1]]
[1, 2]  [0, 1] --> [[1, 0], [2, 1]], [[1, 1], [2, 0]]
[1, 2]  [1, 2] --> [[1, 1], [2, 2]], [[1, 2], [2, 1]]

scalene-equilateral:
Tri 1   Tri 2
-----   -----
[0, 1]  [0, 1] --> [[0, 0], [1, 1]], [[0, 1], [1, 0]]
[0, 2]  [0, 1] --> [[0, 0], [2, 1]], [[0, 1], [2, 0]]
[1, 2]  [0, 1] --> [[1, 0], [2, 1]], [[1, 1], [2, 0]]
*/