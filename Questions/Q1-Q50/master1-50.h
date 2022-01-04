#pragma once

#include <Header_Files/pch.h>

#include <Questions/q1-q50/q1.h>
#include <Questions/q1-q50/q2.h>
#include <Questions/q1-q50/q3.h>
#include <Questions/q1-q50/q4.h>
#include <Questions/q1-q50/q5.h>
#include <Questions/q1-q50/q6.h>
#include <Questions/q1-q50/q7.h>
#include <Questions/q1-q50/q8.h>
#include <Questions/q1-q50/q9.h>
#include <Questions/q1-q50/q10.h>
#include <Questions/q1-q50/q11.h>
#include <Questions/q1-q50/q12.h>
#include <Questions/q1-q50/q13.h>
#include <Questions/q1-q50/q14.h>
#include <Questions/q1-q50/q15.h>
#include <Questions/q1-q50/q16.h>
#include <Questions/q1-q50/q17.h>
#include <Questions/q1-q50/q18.h>
#include <Questions/q1-q50/q19.h>
#include <Questions/q1-q50/q20.h>
#include <Questions/q1-q50/q21.h>
#include <Questions/q1-q50/q22.h>
#include <Questions/q1-q50/q23.h>
#include <Questions/q1-q50/q24.h>
#include <Questions/q1-q50/q25.h>
#include <Questions/q1-q50/q26.h>
#include <Questions/q1-q50/q27.h>
#include <Questions/q1-q50/q28.h>
#include <Questions/q1-q50/q29.h>
#include <Questions/q1-q50/q30.h>
#include <Questions/q1-q50/q31.h>
#include <Questions/q1-q50/q32.h>
#include <Questions/q1-q50/q33.h>
#include <Questions/q1-q50/q34.h>
#include <Questions/q1-q50/q35.h>
#include <Questions/q1-q50/q36.h>
#include <Questions/q1-q50/q37.h>
#include <Questions/q1-q50/q38.h>
#include <Questions/q1-q50/q39.h>
#include <Questions/q1-q50/q40.h>
#include <Questions/q1-q50/q41.h>
#include <Questions/q1-q50/q42.h>
#include <Questions/q1-q50/q43.h>
#include <Questions/q1-q50/q44.h>
#include <Questions/q1-q50/q45.h>
#include <Questions/q1-q50/q46.h>
#include <Questions/q1-q50/q47.h>
#include <Questions/q1-q50/q48.h>
#include <Questions/q1-q50/q49.h>
#include <Questions/q1-q50/q50.h>

//Questions 1 - 50 Master Program
void Run_1to50()
{
	//All question functions return the same type, a pair of <std::string, double>. Because of this a vector of function pointers that return that type has been created where pointers
	//to each function is stored. The functions are then looped through where the answer and time to run each function is printed out

	bool print_answers = 1;
	double total_run_time = 0;

	//Add completed problems to the function vector individually here
	std::vector<std::pair<std::string, double>(*)()> function_vector;
	function_vector.push_back(q1); function_vector.push_back(q2); function_vector.push_back(q3); function_vector.push_back(q4); function_vector.push_back(q5);
	function_vector.push_back(q6); function_vector.push_back(q7); function_vector.push_back(q8); function_vector.push_back(q9); function_vector.push_back(q10);
	function_vector.push_back(q11); function_vector.push_back(q12); function_vector.push_back(q13); function_vector.push_back(q14); function_vector.push_back(q15);
	function_vector.push_back(q16); function_vector.push_back(q17); function_vector.push_back(q18); function_vector.push_back(q19); function_vector.push_back(q20);
	function_vector.push_back(q21); function_vector.push_back(q22); function_vector.push_back(q23); function_vector.push_back(q24); function_vector.push_back(q25);
	function_vector.push_back(q26); function_vector.push_back(q27); function_vector.push_back(q28); function_vector.push_back(q29); function_vector.push_back(q30);
	function_vector.push_back(q31); function_vector.push_back(q32); function_vector.push_back(q33); function_vector.push_back(q34); function_vector.push_back(q35);
	function_vector.push_back(q36); function_vector.push_back(q37); function_vector.push_back(q38); function_vector.push_back(q39); function_vector.push_back(q40);
	function_vector.push_back(q41); function_vector.push_back(q42); function_vector.push_back(q43); function_vector.push_back(q44); function_vector.push_back(q45);
	function_vector.push_back(q46); function_vector.push_back(q47); function_vector.push_back(q48); function_vector.push_back(q49); function_vector.push_back(q50);

	if (print_answers)
	{
		std::cout << "Question#   Answer   Time to run (s)" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}

	for (int i = 0; i < function_vector.size(); i++)
	{
		std::pair<std::string, double>(*heyhey)();
		heyhey = function_vector[i];
		std::pair<std::string, double> q = heyhey();

		if (print_answers) std::cout << "Question " << i + 1 << ": " << q.first << ", " << q.second << std::endl;
		total_run_time += q.second;
	}

	if (print_answers) std::cout << std::endl;
	std::cout << "Total run-time for questions 1-" << function_vector.size() << " was " << total_run_time << " seconds." << std::endl;
}

//NOTES
//Currently this problem set runs in about half a second for an average run time of .01 seconds per question. The only questions currently running over this
//average which could probably be improved upon are: 10, 12, 22, 23, 25, 26, 34, 35, 37, 41 and 44. Of these the most agregious are 12, 41, 44 which clock in
//at .09, .09 and .06 seconds respectively. I don't think these problems really need to be worked on at this point but if I really want to lower the speed then
//these three questions would be the first place to start.