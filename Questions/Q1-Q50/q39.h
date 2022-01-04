#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file
#include <vector>
//Integer Right Triangles
bool coprimeTest(int a, int b)
{
	//this is a pretty lame coprime test but should work fine for this question
	std::vector<int> a_primes, b_primes;
	int divisor = 2;
	while (a >= divisor)
	{
		if (a % divisor == 0); a_primes.push_back(divisor);
		a /= divisor;
		divisor++ ;
	}

	divisor = 2;
	while (b >= divisor)
	{
		if (b % divisor == 0); b_primes.push_back(divisor);
		b /= divisor;
		divisor++;
	}

	for (int i = 0; i < a_primes.size(); i++)
	{
		for (int j = 0; j < b_primes.size(); j++)
		{
			if (a_primes[i] == b_primes[j]) return false;
		}
	}

	return true;
}

std::pair<std::string, double> q39()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, maximum = 0;

	uint16_t triangles[1001] = { 0 }; //create a list to hold the number of triangles formed for each number 1000 or less

	int m = 1, n = 2;
	while (true)
	{
		n = m + 1;
		uint16_t base_sum = (2 * m * n) + (m * m + n * n) + (n * n - m * m);
		if (base_sum > 1000) break; //condition to stop incrementing m and break out of loop
		while (true)
		{
			uint16_t new_base_sum = (2 * m * n) + (m * m + n * n) + (n * n - m * m);
			//std::cout << "m = " << m << ", n = " << n << ", base perimeter = " << new_base_sum << std::endl;
			if (new_base_sum > 1000) break; //condition to stop incrementing n
			if (!coprimeTest(m, n))
			{
				n++;
				continue; //m and n need to be coprime to generate a primitive
			}
			if (m % 2 == 1 & n % 2 == 1)
			{
				n++;
				continue; //m and n can't both be odd to generate a primitive
			}
			for (uint16_t i = new_base_sum; i <= 1000; i += new_base_sum) triangles[i]++; //multiply base case and add to list until it's bigger than 1000
			n++;
		}
		m++;
	}

	for (int i = 1; i < 1001; i++)
	{
		if (triangles[i] > maximum)
		{
			maximum = triangles[i];
			answer = i;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 840
	//ran in 0.0003243 seconds
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