#include <Header_Files/pch.h>
#include <Functions/continued_fractions.h>
#include <Functions/functions.h>
#include <Header_Files/print.h>

int gcdTest(int a, int b)
{
    if (a < 0)  a = -a;
    if (b < 0)  b = -b;

    int c;

    while (b > 0) {
        if (a >= b) {
            a -= b;
            if (a >= b) {
                a -= b;
                if (a >= b) {
                    a -= b;
                    if (a >= b) {
                        a -= b;
                        if (a >= b) {
                            a -= b;
                            if (a >= b) {
                                a -= b;
                                if (a >= b) {
                                    a -= b;
                                    if (a >= b) {
                                        a -= b;
                                        if (a >= b)  a %= b;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        c = a, a = b, b = c;
    }
    return a;
}

std::pair<std::string, double> test2()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

    int n = 10, n2;
    int C[1200], F, i, i2, j;
    unsigned long long int N;

    //First, count the points inside of the circle
    n2 = n * n;
    F = 0;
    for (i = 1; i < n; i++)
    {
        F += 1 + (int)sqrt(n2 - 1 - i * i);
    }
    F *= 4;

    //Next, count the number of distinct lines through the origin containing C points on them.
    //For example, when radius = 10 there are two lines with 9 points on them
    //(slopes of 0 and inf.) and two lines with 7 points on them (slopes of 
    //1 and -1) so C[9] = 2 and C[7] = 2.
    for (i = 1; i < n; i++)  C[i] = 0;
    for (i = -n + 1; i < n; i++)
    {
        i2 = i * i, j = 0;
        while (i2 + j * j < n2)
        {
            if (((i > 0) || (j > 0)) && (gcdTest(i, j) == 1))
            {
                if ((int)sqrt((n2 - 1) / (i2 + j * j)) == 9)
                {
                    int x = 5;
                }
                C[(int)sqrt((n2 - 1) / (i2 + j * j))]++;
            }
            j++;
        }
    }
    printf("Number of points inside the circle=%d\n", F + 1);

    N = 0;
    for (i = 1; i < n; i++) {
        for (j = i; j < n; j++) {
            if (i != j)  N += (long long int) i * j * C[i] * C[j] * (F - 2 * i - 2 * j);
            else      N += (long long int) i * i * C[i] * (C[i] - 1) * (F - 4 * i) / 2;
        }
    }

    answer = N / 3;
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}