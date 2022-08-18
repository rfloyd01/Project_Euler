#pragma once

#include <Header_Files/pch.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>

//Names scores
std::pair<std::string, double> q22()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//first step is to copy the names
	std::ifstream inFile;
	std::vector<std::string> names;
	inFile.open("Resources/q22.txt");

	//read all of the names from the text file into the names vector
	std::string s;
	getline(inFile, s);

	std::istringstream ss(s);
	while (ss)
	{
		std::string name;
		if (!getline(ss, name, ',')) break;
		names.push_back(name.substr(1, name.length() - 2));
	}

	//sort the array using the algorithms library
	std::sort(names.begin(), names.end());
	int subtractor = 'A' - 1;

	//calculate name scores
	for (int i = 0; i < names.size(); i++)
	{
		int current_score = 0;
		for (int j = 0; j < names[i].size(); j++) current_score += names[i][j] - subtractor;

		answer += current_score * (i + 1);
	}

	inFile.close();
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 871198282
	//ran in 0.0023189 seconds
}