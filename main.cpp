#include <Header_Files/pch.h>
#include <Questions/Q251-Q300/q265.h>
#include <Header_Files/test.h>
#include <Questions/Q51-Q100/master51-100.h>

int main()
{
	//All project euler questions are handled in separate header files. Each question is it's own functino which returns a pair where first is the answer and second is the time to run
	//The question number is stored in the question_number variable automatically for ease of reading

	//auto answer = test();
	auto answer = q265();

	std::cout << "The answer is " << answer.first << std::endl;
	std::cout << "Program ran in " << answer.second << " seconds" << std::endl;

	//Run_51to100();

	return 0;
}