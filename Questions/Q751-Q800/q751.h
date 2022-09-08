#pragma once

#include <Header_Files/pch.h>
#include <cmath>

//Concatenation Coincidence
std::pair<std::string, double> q751()
{
	auto run_time = std::chrono::steady_clock::now();

	std::string answer = "2.";
	bool cont = true;
	int decimalDigits = 24;
	
	//add decimalDigits amount of 0's to start off
	for (int i = 0; i < decimalDigits; i++) answer += '0';

	while (cont)
	{
		int subStringEnd = 2;
		long double currentB = std::stod(answer);
		std::string a_string = "2.";
		while (true)
		{
			currentB = floor(currentB) * (1 + currentB - floor(currentB));
			std::string newDigits = std::to_string((int)currentB);
			a_string += newDigits;
			subStringEnd += newDigits.length();

			//See if the a_string has more than 24 decimal digits, if so, remove the digits we don't need
			if (a_string.length() > decimalDigits + 2)
			{
				a_string = a_string.substr(0, decimalDigits + 2);
				subStringEnd = decimalDigits + 2;
			}
			
			if (answer.substr(0, subStringEnd) != a_string)
			{
				//we've found a discrepancy, set the first subStringEnd digits of ans
				//to equal a_string and try again
				answer = a_string;
				for (int i = 0; i < (decimalDigits + 2 - answer.length()); i++) answer += '0';
				break;
			}

			if (a_string.length() == decimalDigits + 2)
			{
				cont = false;
				break;
			}
		}
	}
	
	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 2.223561019313554106173177
	//ran in 0.0000238 seconds
}

//NOTES
/*
It took me a few minutes to understand just what the question was asking, but now that I see what they want this problem actually seems very easy. We can just
start with a starting string of 2.000000000000... and see how far we get until the calculated value no longer matches the start value. In this case we only 
last one iteration:

b1 = 2.0000, a1 = 2
b2 = 2 * 1.000, a2 = 2
2.0 doesn't match 2.2

What we do from this point is substitute the calculated value in to our start value. So now the start value becomes 2.20000... and we try again.

b1 = 2.2000, a1 = 2
b2 = 2 * 1.200 = 2.400, a2 = 2
b3 = 2 * 1.400 = 2.800, a3 = 2
2.20 doesn't match 2.22

So now we update our answer string to 2.22000... and try again. We can just keep following this same procedure until we're correct out to 24 decimal digits.
*/