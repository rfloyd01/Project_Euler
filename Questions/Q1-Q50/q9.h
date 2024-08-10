#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes gcd()

//Special Pythagorean triplet
std::pair <std::string, double> q9()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;
	bool cont = true;

	for (int n = 1; n < 1000; n++)
	{
		if (!cont) break;
		for (int m = n + 1; m < 1000; m++)
		{
			//test to see if we can make a primitive triple
			if (n % 2 && m % 2) continue; //can't both be odd
			if (gcd(n, m) > 1) continue; //need to be coprime

			int a = m * m - n * n;
			int b = 2 * m * n;
			int c = m * m + n * n;

			//if a + b + c is a factor of 1000 then we're done
			if (1000 % (a + b + c) == 0)
			{
				int factor = 1000 / (a + b + c);
				answer = a * b * c * factor * factor * factor;
				cont = false;
				break;
			}

			//if a + b + c is larger than 1000 then break
			if ((a + b + c) > 1000) break;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 31875000
	//ran in 0.0000007 seconds
}

//NOTES:

//HACKERRANK UPDATE:
//Initially my solution looked like this:
/*
for (int m = 21; m > 1; m--)
	{
		if (500 % m != 0) continue;
		int n = 500 / m - m;

		int a = m * m + n * n;
		int b = 2 * m * n;
		int c = m * m - n * n;

		if (a + b + c == 1000)
		{
			std::cout << a << " " << b << " " << c << std::endl;
			std::cout << m << " " << n << std::endl;
			answer = a * b * c;
			break;
		}
	}*/
//and had the following notes:
//per Wikipedia, Euclid's formula is a fundamental formula for generating Pythagorean triples given an arbitrary pair of integers m and n with m > n > 0.
//The formula states that a = m^2 - n^2, b = 2*m*n, c = m^2 + n^2. When n == 1, the maximum for m so that a + b + c <= 1000 is 21. Furthermore, m(m + n) must equal 500 per the equations and limit
//starting with an m of 21, search all combos where, m(m + n) = 500 and see which combo satisfies the question.
//
//After coming back to this problem to solve the HackerRank version I realized that the above code doesn't work for all input values as it's built around finding primitive Pythagorean triples
//and not all Pythagorean triples. I had to edit the code to work with non-primitive triples as well as primitive ones. I ended up getting passing the HackerRank test with a score of 100.00 with
//the following code:

/*
int number = 1000;
	long long answer = -1;

	if (number % 2 == 0) //only even numbers will work
	{
		//create a list with all the factors of number / 2
		std::vector<int> facts = factors(number / 2);

		for (int i = 0; i < facts.size(); i++)
		{
			//m must be less than the square root of the current factor so start search there
			int start = sqrt(facts[i]);

			for (int m = start; m > 1; m--) //m can't be 1
			{
				if (facts[i] % m) continue; //m needs to be a factor of facts[i]
				int n = facts[i] / m - m;

				if (n >= m) break; //m needs to be bigger than n, when n becomes bigger break loop

				if (n < 1) continue; //n needs to be larger than 0
				if (m % 2 && n % 2) continue; //m and n can't both be odd
				if (gcd(m, n) > 1) continue; //m and n must be coprime

				//we have a hit, see if it's greater than current max
				int mult = number / (2 * facts[i]);

				int a = (m * m - n * n) * mult;
				int b = 2 * m * n * mult;
				int c = (m * m + n * n) * mult;

				int num = a * b * c;
				if (num > answer) answer = num;
			}
		}
	}*/

//This code ends up solving the Project Euler version of the problem slower than my original Euler code however so I've decided to keep the original Euler code here.