#pragma once

#include <Header_Files/pch.h>

#include <Questions/q51-q100/q51.h>
#include <Questions/q51-q100/q52.h>
#include <Questions/q51-q100/q53.h>
#include <Questions/q51-q100/q54.h>
#include <Questions/q51-q100/q55.h>
#include <Questions/q51-q100/q56.h>
#include <Questions/q51-q100/q57.h>
#include <Questions/q51-q100/q58.h>
#include <Questions/q51-q100/q59.h>
#include <Questions/q51-q100/q60.h>
#include <Questions/q51-q100/q61.h>
#include <Questions/q51-q100/q62.h>
#include <Questions/q51-q100/q63.h>
#include <Questions/q51-q100/q64.h>
#include <Questions/q51-q100/q65.h>
#include <Questions/q51-q100/q66.h>
#include <Questions/q51-q100/q67.h>
#include <Questions/q51-q100/q68.h>
#include <Questions/q51-q100/q69.h>
#include <Questions/q51-q100/q70.h>
#include <Questions/Q51-Q100/q71.h>
#include <Questions/Q51-Q100/q72.h>
#include <Questions/q51-q100/q73.h>
#include <Questions/q51-q100/q74.h>
#include <Questions/q51-q100/q75.h>
#include <Questions/q51-q100/q76.h>
#include <Questions/q51-q100/q77.h>
#include <Questions/q51-q100/q78.h>
#include <Questions/q51-q100/q79.h>
#include <Questions/q51-q100/q80.h>
#include <Questions/q51-q100/q81.h>
#include <Questions/q51-q100/q82.h>
//#include <Questions/q51-q1000/q83.h>
#include <Questions/q51-q100/q84.h>
#include <Questions/q51-q100/q85.h>
#include <Questions/q51-q100/q86.h>
#include <Questions/q51-q100/q87.h>
//#include <Questions/q51-q100/q88.h>
//#include <Questions/q51-q100/q89.h>
#include <Questions/q51-q100/q90.h>
#include <Questions/q51-q100/q91.h>

//Questions 51 - 100 Master Program
void Run_51to100()
{
	//All question functions return the same type, a pair of <std::string, double>. Because of this a vector of function pointers that return that type has been created where pointers
	//to each function is stored. The functions are then looped through where the answer and time to run each function is printed out

	bool print_answers = 1;
	double total_run_time = 0;

	//Add completed problems to the function vector individually here
	std::vector<std::pair<std::string, double>(*)()> function_vector;
	function_vector.push_back(q51); function_vector.push_back(q52); function_vector.push_back(q53); function_vector.push_back(q54); function_vector.push_back(q55);
	function_vector.push_back(q56); function_vector.push_back(q57); function_vector.push_back(q58); function_vector.push_back(q59); function_vector.push_back(q60);
	function_vector.push_back(q61); function_vector.push_back(q62); function_vector.push_back(q63); function_vector.push_back(q64); function_vector.push_back(q65);
	function_vector.push_back(q66); function_vector.push_back(q67); function_vector.push_back(q68); function_vector.push_back(q69); function_vector.push_back(q70);
	function_vector.push_back(q71); function_vector.push_back(q72); function_vector.push_back(q73); function_vector.push_back(q74); function_vector.push_back(q75);
	function_vector.push_back(q76); function_vector.push_back(q77); function_vector.push_back(q78); function_vector.push_back(q79); function_vector.push_back(q80);
	function_vector.push_back(q81); function_vector.push_back(q82); /*function_vector.push_back(q83);*/ function_vector.push_back(q84); function_vector.push_back(q85);
	function_vector.push_back(q86); function_vector.push_back(q87); /*function_vector.push_back(q88); function_vector.push_back(q89);*/ function_vector.push_back(q90);
	function_vector.push_back(q91); //function_vector.push_back(q92); function_vector.push_back(q93); function_vector.push_back(q94); function_vector.push_back(q95);
	//function_vector.push_back(q96); function_vector.push_back(q97); function_vector.push_back(q98); function_vector.push_back(q99); function_vector.push_back(q100);

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

		if (print_answers) std::cout << "Question " << i + 51 << ": " << q.first << ", " << q.second << std::endl;
		total_run_time += q.second;
	}

	if (print_answers) std::cout << std::endl;
	std::cout << "Total run-time for questions 1-" << function_vector.size() << " was " << total_run_time << " seconds." << std::endl;
}

//NOTES