#include <Header_Files/pch.h>
#include <iostream>

int palindromeTestSixDigits(int y) {
    return 0;
}

std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

    /*int answer = 999999;
    bool cont = true;
    while (cont)
    {
        if (palindromeTestSixDigits(answer))
        {
            int start = 100, stop = sqrt(answer);
            for (int i = start; i < stop; i++)
            {
                if (answer % i == 0)
                {
                    if (answer / i > 999) continue;
                    {
                        cont = false;
                        break;
                    }
                }
            }
        }
        answer--;
    }
    answer++;*/
    //run_time = 0.0008639 seconds

	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 772843811
	//ran in 0.331892 seconds
}