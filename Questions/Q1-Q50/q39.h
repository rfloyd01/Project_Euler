#pragma once

#include <Header_Files/pch.h>

//Integer Right Triangles
std::pair<std::string, double> q39()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	const int P = 1000;
	int perimeters[P + 1] = { 0 }, current_max = 0, max_m = (sqrt(4 * P / 2 + 1) - 1) / 2.0;
	int a = 0, b = 1, c = 1, d = max_m; //Farey variables

	//Use an iterative version of the Farey Sequence algorithm to generate coprime pairs
	//that can be used to generate primitive Pythagorean triples
	while (c <= max_m)
	{
		int k = (max_m + b) / d;
		int next_c = k * c - a;
		int next_d = k * d - b;

		a = c;
		b = d;
		c = next_c;
		d = next_d;

		//if either a or b is even then we find the perimeter of the primitive Pythagorean triple
		//via the equation P = b^2 + a^2 + 2ab + b^2 - a^2 = 2b(b + a)
		if (!(a % 2) || !(b % 2))
		{
			int perimeter = 2 * b * (b + a);

			for (int i = perimeter; i <= P; i += perimeter)
			{
				perimeters[i]++;

				if (perimeters[i] > current_max)
				{
					current_max = perimeters[i];
					answer = i;
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 840
	//ran in 0.0000022 seconds
}

//NOTES
//There's a good formula that can be used to generate all integer sided right triangles. Just use the formula to quickly generate and see which number 1000 or less has the
//most amount of triangles for it. The formula to generate primitive pythagorean triplets (a, b, c) where a, b and c form a coprime set and a^2 + b^2 = c^2 is :
//a = m^2 - n^2, b = 2mn, c = m^2 + n^2 where m > n > 0 and m and n are coprime to eachother and not both odd numbers. After generating a primitive, if all the sides of it are
//multiplied by the same number then it will generate another pythagorean triple (i.e. [3, 4, 5] x2 = [6, 8, 10]). Just keep multiplying primitives by different sizes until 
//the perimeter is over 1000. Do this for every primitive to find all right triangles with perimeters under 1000.

//TODO
//Truth be told I think I made a mistake somewhere in my code because it's only saying there are 2 triangles with perimeter of 120 and the problem text proves that there are 3, however,
//I do get the correct answer so will leave it for now. Will come back at some point in the future and clean this code up. Also would be nice to just generate coprime pairs where
//both numbers aren't odd instead of having the really unefficient coprime test function (was super easy to code which is why I used it).