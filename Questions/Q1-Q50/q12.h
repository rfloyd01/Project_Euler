#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h> //includes NumberOfFactors()

//Highly divisible triangular number
int efficientNumberOfFactors(int n, std::vector<int>& primez)
{
	//returns the number of factors for n, effectively only searches 
	//up to the square root of n for a more efficient runtime
	int factors = 1;
	for (int i = 0; i < primez.size(); i++)
	{
		if (primez[i] * primez[i] > n) return factors * 2;

		int exponent = 1;
		while (n % primez[i] == 0)
		{
			exponent++;
			n /= primez[i];
		}
		factors *= exponent;
		if (n == 1) return factors;
	}
}
std::pair<std::string, double> q12()
{
	auto run_time = std::chrono::steady_clock::now();

	int n = 500, answer = 1;
	int evenFactors = 1, oddFactors = 1, currentEvenNumber = 1, currentOddNumber = 1; //even number is really set at 2, but gets divided by 2 to calculate triangle number
	std::vector<int> prims = primes(65500); //kind of cheap here, but I know the answer won't overflow a 32-bit integer so we shouldn't need primes higher than this

	while (true)
	{
		if (evenFactors * oddFactors > n)
		{
			answer = currentEvenNumber * currentOddNumber;
			break;
		}

		//calculate the number of factors for the next odd number
		currentOddNumber += 2;
		oddFactors = efficientNumberOfFactors(currentOddNumber, prims);
 
		if (evenFactors * oddFactors > n)
		{
			answer = currentEvenNumber * currentOddNumber;
			break;
		}

		//calculate the number of factors for the next even number
		currentEvenNumber++; //triangle numbers get divided by 2 so keep track of n/2 for even numbers instead of n
		evenFactors = efficientNumberOfFactors(currentEvenNumber, prims);
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 76576500
	//ran in 0.0013069 seconds
}