#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <regex>

//This header file is used to practice with using regular expressions
//to match text

std::pair<std::string, double> regexPractice()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	std::string regExpression = "\\B!!\\B";
	std::string expression = "!  a bc!!";
	

	//regex_search will see if the regular expression exists anywhere in the given string.
	//For example, if the regEx is "m" and the string is "hey mama" then regex_search will
	//return true because m exists in the given string
	
	//regex_match will see if the given string is a perfect match for the regular expression.
	//For example, if the regEx is "m" and the string is "hey mama" then regex_match will
	//return false because m isn't a perfect match

	if (std::regex_search(expression, std::regex(regExpression))) std::cout << "Found a match baby" << std::endl;
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}

//the answer is 2 ^ 13717420 - 1 + 256