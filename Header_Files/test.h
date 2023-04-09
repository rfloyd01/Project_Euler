#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>

void NewFarey(int n)
{
	int a = 0, b = 1, c = 1, d = n;

	while (c <= n)
	{
		

		int k = (n + b) / d;
		int next_c = k * c - a;
		int next_d = k * d - b;

		//std::cout << a << "/" << b << " (" << c << ", " << d << ", " << k << ")" << std::endl;

		a = c;
		b = d;
		c = next_c;
		d = next_d;



	}
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	const int P = 5000000;
	int* perimeters = new int[P + 1](), current_max = 0, max_m = (sqrt(4 * P / 2 + 1) - 1) / 2.0;
	int a = 0, b = 1, c = 1, d = max_m;

	//iterative version of the Farey Sequence algorithm
	while (c <= max_m)
	{
		int k = (max_m + b) / d;
		int next_c = k * c - a;
		int next_d = k * d - b;

		a = c;
		b = d;
		c = next_c;
		d = next_d;

		//if either a or b is even then we create a primitive Pythagorean triple
		if (!(a % 2) || !(b % 2))
		{
			int perimeter = 2 * b * (b + a);

			for (int i = perimeter; i <= P; i += perimeter)
			{
				perimeters[i]++;

				if (perimeters[i] > current_max)
				{
					current_max = perimeters[i];
					answer = i;
				}
			}
		}
	}

	//for (int i = 0; i < coprime_pairs.size(); i++)
	//{
	//	if (coprime_pairs[i].numerator % 2 == 0 || coprime_pairs[i].denominator % 2 == 0)
	//	{
	//		/*count++;
	//		std::cout << coprime_pairs[i].numerator << "/" << coprime_pairs[i].denominator << std::endl;*/
	//		int perimeter = 2 * coprime_pairs[i].denominator * (coprime_pairs[i].denominator + coprime_pairs[i].numerator);

	//		for (int i = perimeter; i <= P; i += perimeter)
	//		{
	//			perimeters[i]++;

	//			if (perimeters[i] > current_max)
	//			{
	//				current_max = perimeters[i];
	//				answer = i;
	//			}
	//		}
	//	}
	//}
	//ran in 0.0000219 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}