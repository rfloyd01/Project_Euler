#pragma once

#include <Header_Files/pch.h>

//Ordered Fractions
std::pair<std::string, double> q71()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = (1000000 - 5) / 7 * 3 + 2;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
	//the answer is 428570
	//ran in 0.0000009 seconds
}

//NOTES
//This program can be solved by utilizing something called the Farey sequence. Essentially you
//start with the fraction 0/1 on the left and 1/1 on the right and keep adding the numerators
//and denominators to get fractions in the middle which are coprime pairs. 0/1 + 1/1 = 1/2 for
//example. From here you can keep adding the new element to the element directly to its left 
//until the denominator reaches a certain value. If you only wanted fractions with denominators
//of 5 or less you would do: 0/1 + 1/1 = 1/2, 0/1 + 1/2 = 1/3, 0/1 + 1/3 = 1/4 and 0/1 + 1/4 = 1/5.
//From this point you go recursively backwards until you can "add" adjacent fractions and have the 
//denminator be equal to or less than the limit. In this case you'd go backwards to 1/2 + 1/3
// = 2/5. It's amazing how basic this algorithm is and is still able to produce, in order, every
//coprime pair with numbers less than or equal to a limit n.

//For the sake of this problem we're trying to find out which fraction with a denoniator less than or
//equal to 1,000,000 is directly on the left of 3/7. With the Faray sequence, we don't even need
//to program anything. Per the problem text we can see that 2/5 is directly next to 3/7 from the start,
//so we just need to keep adding the number directly to the left of 3/7 until the denominator is just
//shy of 1,000,000. 3/7 + 2/5 = 5/12, 3/5 + 5/12 = 8/17, etc. The answer can be found by the following
//equation: floor((1,000,000 - 5) / 7) * 3 + 2. Since we're always adding 3/7 it's pretty basic to
//figure out what the numerator and denominator will be. Since this simple equation can be used to find
//the answer the problem runs instantly