#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <cmath>
#include <algorithm>
#include <set>

//Stealthy Numbers
std::pair<std::string, double> q757()
{
	auto run_time = std::chrono::steady_clock::now();

	//long long answer = 0, factor = 1, limit = 100000000000000;

	//Non-brute force method, currently double-counts some values
	//while (true)
	//{
	//	long long min_a = factor * factor, max_a = (sqrt((1 + factor) * (1 + factor) + 4 * ((1 + factor) / (float)factor) * limit) - (1 + factor)) / (2 * ((1 + factor)/(float)factor));
	//	if (min_a > max_a) break;
	//	//std::cout << factor << ": min_a = " << min_a << ", max_a = " << max_a << std::endl;

	//	answer += ((max_a - min_a) / factor) + 1;
	//	factor++;
	//}

	//Brute force method, works in 5 seconds
	long long answer = 1, factor = 1, limit = 100000000000000;
	std::vector<long long> hits;

	while (true)
	{
		long long a = factor * factor;
		long long N = a * (factor + 1) * (factor + 1);
		if (N > limit) break;

		while (true)
		{
			hits.push_back(N);
			a += factor;
			N = (a + factor) * (a + a / factor);
			if (N > limit) break;
		}

		factor++;
	}

	//sort the vector and then only count unique entries
	std::sort(hits.begin(), hits.end());
	for (int i = 1; i < hits.size(); i++)
		if (hits[i] != hits[i - 1]) answer++;
	//ran in 5.31048 seconds
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 75737353
	//ran in 5.31048 seconds
}

//NOTES
/*
* can first use a factor when a = factor^2. can only use that factor up until the point that 
*/