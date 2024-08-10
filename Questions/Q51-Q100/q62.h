#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes []powers_of_ten
#include <map>
#include <vector>

//Cubic Permutations
long long convertToTenDigit(long long n)
{
	//Takes the number n and converts it into a ten digit representation of its own digits
	long long representation = 0;
	while (n > 0)
	{
		representation += powers_of_ten[n % 10];
		n /= 10;
	}

	return representation;
}

std::pair<std::string, double> q62()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 9223372036854775807; //set answer as the largest unsigned long for quick comparison later

	std::map<long long, std::vector<long long> > found_cubes;
	long long i = 346, cube, rep; //cube holds the current cubic number and rep holds the ten digit representation of it

	while (true)
	{
		cube = i * i * i;
		rep = convertToTenDigit(cube);

		found_cubes[rep].push_back(cube);
		if (found_cubes[rep].size() == 5)
		{
			for (int j = 0; j < 5; j++)
				if (found_cubes[rep][j] < answer) answer = found_cubes[rep][j];
			break;
		}
		i++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 127035954683
	//ran in 0.0021824 seconds
}

//NOTES
//This question seems a little daunting at first because the permutations given in the sample are fairly large so it can be assumed that the actual answer will be much
//larger, however, since we really only need to iterate through the base numbers of the cubes we shouldn't really need to deal with that many large numbers. For example
//10,000 cubed is 1,000,000,000,000 so by iterating from 0-10000 we actually get all the way into the trillions. I think the best way to handle checking for permutations
//will be to convert each cubed number into a ten digit base ten number where each digit represents the numbers of digits in the cube (i.e. the number 103,823 would be
//represnted as 0100002111 with 9 being the most significant digit and 0 being the least). The 10 digit representation will become the key of a map and any number with
//the same digits will be put into that vector of the map. The answer will be the smallest number in the first map vector to reach a length of 5.

//Typically using maps isn't very fast but since I chose to save number representations as 10 digit values using a different type of container would take up way too much
//space. The code only takes about 2 milliseconds to run and took all of 10 minutes to formulate and code up though so I'm happy with this result. The second page of PE
//problems has been much harder than the first page so far so it was nice to have a bit of an easier problem.