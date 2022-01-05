#pragma once

#include <Header_Files/pch.h>
#include <cmath>

//Almost equilateral triangles
std::pair<std::string, double> q94()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = -6; //the triangles [1, 1, 2] and [1, 1, 0] get added which aren't actual triangles so subtract them here
	unsigned long long square = 5, square_add = 7, maximum_square = 4000000000000000000;
	long long num = 0, square_root = 0, a = 0;

	while (square < maximum_square)
	{
		if (square % 4 == 0)
		{
			num = square / 12 - 1;
			square_root = sqrt(num);
			if (square_root * square_root == num)
			{
				a = sqrt(square + 4);
				if ((a + 2) % 6 == 0) answer += ((a + 2) / 6 + (a + 2) / 6 + (a + 2) / 6 + 1);
				if ((a - 2) % 6 == 0) answer += ((a - 2) / 6 + (a - 2) / 6 + (a - 2) / 6 - 1);
			}
		}
		num = (square + 1) * 12 + 4;

		square += square_add;
		square_add += 2;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 518408346
	//ran in 2.38506 seconds
}

//NOTES
/*
First attempt will be a "brute force" approach. Any integral length isoceles triangle can be made where the different side length is +/- 1 from the
equal side lengths (other than the triangle [1, 1, 2] or [0, 0, 1] as these would be straight lines). The area for an isoceles triangle as a function of it's side
lengths a (equal sides) and b (not equal side) is Area = (sqrt(a^2 - b^2 / 4) * b) / 2. Per the problem text we don't want the perimeter to exceed
1,000,000,000 which means that the maximum value for a is 333,333,333 and b is 333,333,334. It's possible for be to be 1 less than a or 1 more than
a so we can create two equations based off of this and the equation for area of an isoceles triangle:

Area = (sqrt(a^2 - (a+1)^2/4) * (a+1)) / 2
Area = (sqrt(a^2 - (a-1)^2/4) * (a-1)) / 2

reducing these equations leads to:

Area = sqrt(3a^2 - 2a - 1) * (a+1) / 4
Area = sqrt(3a^2 + 2a - 1) * (a-1) / 4

Instead of cycling through different values of 'a' and hoping for a square, we can instead just cycle through numbers that are square and see what value
of 'a' would actually give us that square. Using the quadratic equation we can see that:

a =  (2 + sqrt(4 + 12 * (square + 1))) / 6 for the first case and
a = (-2 + sqrt(4 + 12 * (square + 1))) / 6 for the second case

Since the maximum value for a is 333,333,333 this limits the maximum value of square numbers to check at 4,000,000,000,000,000,000 on the dot. Although
the sqaures get further and further apart the higher we get, this still means that we'll have roughly 2,000,000,000 numbers to check which isn't
that ideal, but, this is a brute force approach after all.

We can reduce things a step further now, it's not enough to look at square numbers, we need to be looking at squares that when 1 is added to them, they're
multiplied by 12 and then have 4 added to them, they result in other square values. We're looking at squares in squares here! For example, the square number
64 would turn into ---> 4 + 12 * (64 + 1) = 784 ---> sqrt(784) = 28. So manipulating the square number 64 with our equation gave another square number. So
we really need to be looking at square numbers that are 0 == (square - 4) % 12. Basically if we can subtract 4 from the square, divide by 12 and get
zero it'll be eligible to then subtract by 1 and see if it's a square. Listing out the first 100 odd numbers, subtracting 4 from them and seeing which ones
are divisible by 12 shows a pattern:

[4 - 4] 0 % 12 = 0
[9 - 4] 5 % 12 = 5
[16 - 4] 12 % 12 = 0
[25 - 4] 21 % 12 = 9
[36 - 4] 32 % 12 = 8
[49 - 4] 45 % 12 = 9
[64 - 4] 60 % 12 = 0
[81 - 4] 77 % 12 = 5
[100 - 4] 96 % 12 = 0
[121 - 4] 117 % 12 = 9
[144 - 4] 140 % 12 = 8
[169 - 4] 165 % 12 = 9
[196 - 4] 192 % 12 = 0
[225 - 4] 221 % 12 = 5

So it looks like as long as the square - 4 is physically divisible by 4, then it will also be divisible by 12. I'm sure this can be proven but for now
I'm just going to take it as truth and see if it works out or not.
*/