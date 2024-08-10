#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>
#include <iostream>
#include <algorithm>

void freqPrint(std::vector<std::pair<std::string, float> > freqs)
{
	std::cout << '[';
	for (int i = 0; i < freqs.size() - 1; i++) std::cout << '(' << freqs[i].first << ", " << freqs[i].second << "), ";
	std::cout << '(' << freqs.back().first << ", " << freqs.back().second << ")] " << std::endl;
	return;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	double room_width = 11.3542, room_length = 20.7083, room_height = 8.0, mode_frequency = 0, c2 = 1086.9 / 4.0;
	std::vector<std::pair<std::string, float> > mode_frequencies;

	//I could do everything in the same loop but I'm splitting it all out
	//to keep everything a little more organized

	//length+width resonances
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			mode_frequency = c2 * sqrt((i / room_length) * (i / room_length) + (j / room_width) * (j / room_width));
			if (mode_frequency <= 300) mode_frequencies.push_back({ "F_" + std::to_string(i) + '_' + std::to_string(j) + "_0", mode_frequency });
		}
	}

	//sort the modes by frequency. I copied this from online so I should study it a bit
	std::sort(mode_frequencies.begin(), mode_frequencies.end(), [](auto& left, auto& right) {return left.second < right.second; });
	std::cout << "Tangiential modes between room length and width:\n";
	freqPrint(mode_frequencies);
	mode_frequencies.clear();

	//length+height resonances
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			mode_frequency = c2 * sqrt((i / room_length) * (i / room_length) + (j / room_height) * (j / room_height));
			if (mode_frequency <= 300) mode_frequencies.push_back({ "F_" + std::to_string(i) + "_0_" + std::to_string(j), mode_frequency });
		}
	}

	//sort the modes by frequency. I copied this from online so I should study it a bit
	std::sort(mode_frequencies.begin(), mode_frequencies.end(), [](auto& left, auto& right) {return left.second < right.second; });
	std::cout << "\nTangiential modes between room length and height:\n";
	freqPrint(mode_frequencies);
	mode_frequencies.clear();

	//width+height resonances
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			mode_frequency = c2 * sqrt((i / room_width) * (i / room_width) + (j / room_height) * (j / room_height));
			if (mode_frequency <= 300) mode_frequencies.push_back({ "F_0_" + std::to_string(i) + "_" + std::to_string(j), mode_frequency });
		}
	}

	//sort the modes by frequency. I copied this from online so I should study it a bit
	std::sort(mode_frequencies.begin(), mode_frequencies.end(), [](auto& left, auto& right) {return left.second < right.second; });
	std::cout << "\nTangiential modes between room width and height:\n";
	freqPrint(mode_frequencies);

	//vprint(mintageCount(1451500, { 110, 1500 }));

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}