#pragma once

#include <Header_Files/pch.h>
#include <cmath> //includes sqrt()

//Triangular, Pentagonal and Hexagonal

bool PentagonalTest(long long n)
{
	//tests whether or not the given number is a pentagonal number. Worked out on paper with quadratic equation
	double pent = (1 + sqrt(1 + 24 * n)) / 6.0;

	//check wether or not pent has a decimal
	long long new_pent = pent * 10 / 10;
	if ((double)new_pent == pent) return true;
	return false;
}

std::pair<std::string, double> q45()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	long long i = 1;
	long long hex;

	while (true)
	{
		hex = i * (2 * i - 1);
		if (PentagonalTest(hex))
		{
			if (hex > 40755)
			{
				answer = hex;
				break;
			}
		}
		i++;
	}
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1533776805
	//ran in 0.0001466 seconds
}

//NOTES
//From the problem text it appears that all hexagonal numbers are also triangular numbers so I'm not going to bother calculating triangular numbers. I think the quickest
//way to do this will be to just calculate each hexagonal number and then test whether or not it's also pentagonal. The first hit after 40755 will be the answer. Test for
//pentagon-ness by using the quadratic formula on generating function for pentagon numbers. If the root is an integer then the number is pentagonal.