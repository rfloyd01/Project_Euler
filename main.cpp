#include <Header_Files/pch.h>
#include <Questions/Q901-Q950/q936.h>
#include <Header_Files/test.h>

int main()
{
	//All project euler questions are handled in separate header files. Each question is it's own function which returns a pair where first is the answer and second is the time to run
	//The question number is stored in the question_number variable automatically for ease of reading

	auto answer = q936();
	//auto answer = test();

	std::cout << "The answer is " << answer.first << std::endl;
	std::cout << "Program ran in " << answer.second << " seconds" << std::endl;

	//codeParse();

	return 0;
}