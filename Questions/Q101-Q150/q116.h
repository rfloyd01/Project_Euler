#pragma once

#include <Header_Files/pch.h>

//Red, Green or Blue Tiles
std::pair<std::string, double> q116()
{
	auto run_time = std::chrono::steady_clock::now();
	
	const int limit = 1123000010;
	long long mod = 1000000007;
	//long long red_ways[limit + 1] = { 1, 1, 2, 3 };
	//long long green_ways[limit + 1] = { 1, 1, 1, 2 };
	//long long blue_ways[limit + 1] = { 1, 1, 1, 1 };

	//for (int i = 4; i <= 50; i++)
	//{
	//	red_ways[i] = red_ways[i - 2] + red_ways[i - 1];//% mod;
	//	green_ways[i] = green_ways[i - 3] + green_ways[i - 1];// % mod;
	//	blue_ways[i] = blue_ways[i - 4] + blue_ways[i - 1];// % mod;
	//	//std::cout << red_ways[i] + green_ways[i] + blue_ways[i] - 3 << std::endl;
	//}

	//long long answer = red_ways[50] + green_ways[50] + blue_ways[50] - 3; //remove the ways where no colored tiles are used

	long long red_ways[2] = { 2, 3 };
	long long green_ways[3] = { 1, 1, 2 };
	long long blue_ways[4] = { 1, 1, 1, 1 };

	for (int i = 4; i <= limit; i++)
	{
		long long red_temp = red_ways[1], green_temp = green_ways[2], blue_temp = blue_ways[3];

		//red blocks
		red_ways[1] = (red_ways[1] + red_ways[0]) % mod;
		red_ways[0] = red_temp;

		//green blocks
		green_ways[2] = (green_ways[2] + green_ways[0]) % mod;
		green_ways[0] = green_ways[1];
		green_ways[1] = green_temp;

		//blue blocks
		blue_ways[3] = (blue_ways[3] + blue_ways[0]) % mod;
		blue_ways[0] = blue_ways[1];
		blue_ways[1] = blue_ways[2];
		blue_ways[2] = blue_temp;

		//std::cout << red_ways[i] + green_ways[i] + blue_ways[i] - 3 << std::endl;
	}
	long long answer = red_ways[1] + green_ways[2] + blue_ways[3] - 3; //remove the ways where no colored tiles are used

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 20492570929
	//ran in 0.0000009 seconds
}

//NOTES