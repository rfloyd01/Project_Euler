#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/bigint.h>

//Square Root Digital Expansion
std::pair<std::string, double> q80()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	
	for (int i = 2; i < 100; i++)
	{
		//initialize variables
		int squirt = sqrt(i);
		if (squirt * squirt == i) continue; //skip perfect squares
		bigint left_num = 2 * squirt;
		bigint right_num = i - squirt * squirt;
		answer += squirt;
		for (int j = 0; j < 99; j++)
		{
			left_num *= 10;
			right_num *= 100;
			int next_digit = 0;
			while (true)
			{
				if ((left_num + next_digit) * next_digit > right_num)
				{
					next_digit--;
					break;
				}
				next_digit++;
			}
			//add the next digit to current answer and then set up left_num and right_num variables for next step
			answer += next_digit;
			right_num -= (left_num + next_digit) * next_digit;
			left_num += 2 * next_digit;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 40886
	//ran in 0.103072 seconds
}

//NOTES
//The problem states that the square root for any non-square integer will be an infite set of digits with no pattern. In this case, Using the law of averages one
//would expect there to be the same number of each type of digit as the square root is taken out to infinity decimal places. The sum of all 10 digits is 45 which means
//that an average value for each number is 45 / 10 = 4.5. 4.5 times 100 digits of decimal expansion = 450, times 90 non-square numbers under 100 = 40,500 so I'd expect
//the answer to be somewhere within 10% of that number. Not that knowing that really help but it may come in handy for debugging potentiall. To solve this problem there's
//a method of calculating square roots which is very similar to long division. Just do that for each non-square number under 100 and keep track of the digits in an array.
//Add up all the numbers in the array at the end and we should be done.

//The method for calculating square roots by hand isn't as compact as I thought it would be and ends up needing 1-2 digits for every digit that is desired to be calculated
//(i.e. to calculate a square root out to 100 digits it requires a 100-200 digit natural number to be used during the calculation). Not that this is a major setback but the
//bigint class needs to be used now which will inherantly slow down the run time. 