#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>

void RecursiveNumberBuild(long long* divisible, long long current_number = 0, long long place = 1)
{
    if (place == 1e12)
    {
        //base of the recursion. May not be low enough for now

        //at this level we simply look at the numbers which divide the 
        //current number. If anything new is found, or a number that's 
        //already been discovered has a lower value found, update the divisible list
        long long num1 = current_number, num2 = current_number + 9;

        for (int i = 1; i <= 500; i++)
        {
            if (num1 % i == 0)
            {
                if (!divisible[i]) divisible[i] = num1;
                else
                {
                    if (num1 < divisible[i]) divisible[i] = num1;
                }
            }
            if (num2 % i == 0)
            {
                if (!divisible[i]) divisible[i] = num2;
                else
                {
                    if (num1 < divisible[i]) divisible[i] = num2;
                }
            }
        }
    }
    else
    {
        //we simply tak on a 0 or 9 to the end of the current number
        RecursiveNumberBuild(divisible, current_number * 10, place * 10);
        RecursiveNumberBuild(divisible, (current_number + 9) * 10, place * 10);
    }
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

    long long* divisibility = new long long[501]();
    RecursiveNumberBuild(divisibility);

    //for (int i = 1; i <= 500; i++) std::cout << i << ": " << divisibility[i] << std::endl;

    delete[] divisibility;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}