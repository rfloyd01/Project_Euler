#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes fraction
#include <vector>

//Cuboid Routes
void FarayPythagoreanTripleGeneration(int c_limit, int* ways, fraction low = { 0, 1 }, fraction high = { 1, 1 }, bool new_pair = false)
{
	//every new Faray pair gets tested to see how many rooms can be generated
	if (new_pair)
	{
		if (low.numerator % 2 == 0 || low.denominator % 2 == 0) //this pair will result in a primitive Pythagorean Triple
		{
			int a = low.denominator * low.denominator - low.numerator * low.numerator;
			int b = 2 * low.denominator * low.numerator;
			int c = low.denominator * low.denominator + low.numerator * low.numerator;
			int triple[3] = { a, b, c };

			while (triple[2] < c_limit)
			{
				if (triple[0] > triple[1])
				{
					*(ways + triple[0]) += triple[1] / 2;
					if (triple[1] * 2 > triple[0]) *(ways + triple[1]) += triple[0] / 2 - (triple[0] - triple[1] - 1);
				}
				else
				{
					*(ways + triple[1]) += triple[0] / 2;
					if (triple[0] * 2 > triple[1]) *(ways + triple[0]) += triple[1] / 2 - (triple[1] - triple[0] - 1);
				}

				triple[0] += a;
				triple[1] += b;
				triple[2] += c;
			}
		}
	}

	//this portion advances to the next pair in the Faray sequence
	if ((low.denominator + high.denominator) * (low.denominator + high.denominator) < c_limit)
	{
		//in the pythagorean triplet C is found by adding m^2 and n^2. To maximize m, make n = 1. Therefore
		//maximum_c - 1 = m^2. Cutting the Faray sequence when the square of the denominator is greater than
		//the c limit should allow us to utilize every necessary triple
		FarayPythagoreanTripleGeneration(c_limit, ways, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true);
		FarayPythagoreanTripleGeneration(c_limit, ways, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false);
	}
}

std::pair<std::string, double> q86()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, count = 0;
	const int c_limit = 5000; //some arbitrarily high number
	int ways[c_limit] = { 0 };

	FarayPythagoreanTripleGeneration(c_limit, ways); //generates all Pythagorean triples where c < c_limit
	while (count < 1000000)
	{
		answer++;
		count += ways[answer];
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1818
	//ran in 0.0000512 seconds
}

//NOTES
//In order to get from one corner of the room to the other the spider will need to travel on a minimum of 
//two different surfaces. Since there are potentially 6 different surfaces to the room there are choose(6,2)
//ways to choose to surfaces for the spider to go on. However, three of those options can be eliminated because
//the surfaces don't share an edge, half of those options can be elimated as the spider only has access to 3
//of the 6 room surfaces to start off, and another half of those routes can be eliminated because they are
//mirror images of eachother. This leaves us with three different routes for the spider to take which are
//potentially the quickest (which is exactly what the problem text states). The length of these routes can
//be found with the following equations: route 1 = sqrt(length^2 + (width + height)^2), route 2 = 
//sqrt(width^2 + (length + height)^2) and route 3 = sqrt(height^2 + (length + width)^2). Since the problem
//states that we don't care about rotations of the room (i.e. a 1x2x3 room would be the same thing as a
//2x3x1 room or a 3x2x1 room) we can create rooms such that l >= w >= h so that route 1 will always be
//the shortest route. This makes things a little easier as now there's only 1 route to check instead of 
//three every time.

//With the route situation handled you can now see that the essence of the problem boils down finding different
//ways to sum square numbers that result in a square number. Looking again at the problem text we get that the
//route is the square route of 64 and 36, which is 100. In this case 36 comes from 6 and 64 comes from the
//addition of 3 and 5. If we look at the two squares though, there are a number of ways in which we can 
//decompose those numbers and still have the sum add up to 100. There's (6, [7, 1]), (6, [6, 2]), (6, [5, 3])
//and (6, [4, 4]). Or, if we decided to have 36 be the number that has two components there would be
//([5, 1], 8), ([4, 2], 8) and ([3, 3], 8). That means there are 7 distinct solutions where the spider's path
//is exactly 10 units long. Instead of having three loops to keep track of length, width and height variables
//it would be more efficient to just see which integers are possible to be expressed as the square root of 
//the sum of two squares, and then decompose those squares into every possibility. The only restriction here
//is that we need to make sure that when decomposing the squares that none of the components are larger than
//the limit of M. If for example M was limited to 7, there would only be 4 possible ways to have a path length
//of 10, and if M was limited to 6 then there would only be 3 ways. To find the answer we just need to find
//all squares that can be expressed as the sum of squares and keep iterating until there are at least
//1,000,000 distinct routes. Then we need to see what the highest value is of any wall face and that should
//be the correct M value.

//It struck me that instead of trying to figure out which sqaure numbers can sum into other squares that I
//could just use the Pythagorean triple generating function that has been used in other problems to 
//accomplish this. Pick an estimate for M, generate all triples such that c < sqrt(5) * M (that is the
//length of the path when length = width = height = M) and then break down a and b of the triple in as many
//ways as possiblea and add them to the final answer. Generating the triples shouldn't take very long so if
//the answer isn't very close to 1,000,000 just change the estimate of M and try again. B = 2mn so the
//limits for m and n in the generating function should be chosen such that sqrt(5) * M = 2mn. Furthermore
//a primitive triple is only formed when m and n are coprime and not both odd. If n == 1 then the largest
//value for m is roughly sqrt(5) * M / 2. Using that value of m as a limit, a Farey sequence can be utilized
//to generate the coprime pairs necessary to generate the Pythagorean triples.

//The end code runs pretty quickly, however, there were a few things that tripped me up before getting the
//final answer. I generated Pythogrean triples where side b was not longer than the limit M originally,
//however, I forgot that this side could then to be split into two smaller numbers (representing the width
//and height of the room) so it was possible to start with a b larger than M but after splitting have both
//components be smaller than M and therefore useable. Another issue was overcounting the number of useable
//decompositions of certain triples. When decomposing one of the numbers, you have to make sure that both
//of the new components are less then the other number. For example in the (6, 8, 10) triangle we can choose
//to decompose 6 into [1, 5], [2, 4] or [3, 3] which are all valid because 1-5 are less than 8. If we choose
//to decompose 8 the options are [1, 7], [2, 6], [3, 5] and [4, 4]. In this case [7, 1] isn't a valid choice
//because 7 is larger then 6 so we are no longer looking at the shortest path to the fly. After debugging 
//these two oversights the correct answer was obtained with a run time of 0.045 seconds.

//MORE NOTES
//I'm not 100 percent satisfied with this code. It works very well, however, I had to run the code a few times and
//change the value of M manually to figure out that the answer was 1818. If I put my main code into a loop that increments
//all the way up to the answer before breaking it takes about 20 seconds to run. The reason for this
//is that it has to start the Faray sequence over from scratch every time so there are a lot of coprime pairs
//the have to be calculated over and over.

//After reading the forum section for this problem I saw that someone
//had the exact same approach to this problem as I did (i.e. using pythagorean triples and splitting the
//two shorter sides into as many combos as possible), however, their solution runs quicker and doesn't need
//to know the answer before hand. They accomplish this just by putting some very high limit on the triples
//that are generated (as opposed to me where I limited the triples generated by selecting a certain M). From
//this point they create an array where the maximum value is C for the final triple generated and then each 
//triple is broken down by splitting up either A or B (just like I do). If A is broken up then the number of
//vaues that are possible are stored in array[b] and vice versa if B is broken up then the number of ways to
//break up the number are stored in array[a]. The last step is to just step through the array and add values
//until the sum first reaches over a million. All in all very similar to my method but allows a much higher
//limit to be selected so that the answer can be iterated to. By elimating the need to limit the faray search
//with the size of M, I was able to reduce the number of if statements by a lot which helped to speed things 
//up. Furthermore, by using C to limit the Faray search instead of M the number of elements in the Faray 
//sequence has reduced dramatically. With these changes the run time went from .045 seconds down to .00012
//seconds which is almost a 400x speedup!