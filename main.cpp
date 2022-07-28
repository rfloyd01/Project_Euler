#include <Header_Files/pch.h>
#include <Questions/Q201-Q250/q240.h>
#include <Header_Files/test.h>
#include <Header_Files/test2.h>

int main()
{
	//All project euler questions are handled in separate header files. Each question is it's own functino which returns a pair where first is the answer and second is the time to run
	//The question number is stored in the question_number variable automatically for ease of reading

	long long dice_sides = 4, top_dice = 2, total_dice = 20;
	auto answer = test(dice_sides, top_dice, total_dice);
	auto answer2 = test2(dice_sides, top_dice, total_dice);
	//auto answer = q240();

	//std::cout << "The answer is " << answer.first << std::endl;
	//std::cout << "Program ran in " << answer.second << " seconds" << std::endl;
	for (int i = 0; i < answer.size(); i++)
		if (answer[i] != answer2[i]) std::cout << "Goal = " << i + top_dice << ", ways 1 = " << answer[i] << ", ways 2 = " << answer2[i] << std::endl;

	return 0;
}