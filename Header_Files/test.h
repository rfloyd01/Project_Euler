#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>

//test
std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1; //answer could be initialized to no-pivot as there's only one case of that

	std::vector<std::vector<std::vector<int> > > partitions;
	getPartitions(12, partitions, 6, 5);

	for (int i = 0; i < partitions.size(); i++)
		for (int j = 0; j < partitions[i].size(); j++) vprint(partitions[i][j]);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)
}
