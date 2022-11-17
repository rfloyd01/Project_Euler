#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <fstream>

long long getPalindromeT(long long n)
{
	long long pal = 0;
	while (n > 0)
	{
		pal *= 10;
		pal += n % 10;
		n /= 10;
	}
	return pal;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	long long current_number = 0;

	for (int i = 1; i < 10000; i++)
	{
		bool lychrel = true;
		current_number = i;

		for (int j = 0; j < 50; j++)
		{
			long long pal = getPalindromeT(current_number);
			if (current_number == pal) //test current number for palindromeness
			{
				if (j > 0) //starting on a palindrome doesn't count, as can be seen in example text
				{
					lychrel = false;
					break; //no need to iterate all the way up to 50
				}
			}

			current_number += pal;
		}

		if (lychrel) answer++;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}


//current best ratio for N=250 is 133.00456453