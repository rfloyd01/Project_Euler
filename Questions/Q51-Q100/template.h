#pragma once

#include <Header_Files/pch.h> //There are a few header files needed to make the below code work, so included them in a precompiled header file

//Question name
std::pair<std::string, double> qxxx()
{
	//Use a string as the first part of the pair so that every question returns an answer of the same form (normally you would need ints, doubles, long ints, etc. to store different answers)
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//Body of question
	//Body
	//Body

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}