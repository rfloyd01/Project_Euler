#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>

//Best Approximations
struct longFraction
{
	//simple struct to help store convergents with large
	//numerators and denominators
	unsigned long long numerator;
	unsigned long long denominator;
};

void getBetterApproximation(unsigned long long n, longFraction* convergents, unsigned long long& answer, unsigned long long limit)
{
	//Ultimately this method looks at two fractions and figures out which one
	//is closer to sqrt(n). Under normal circumstances this wouldn't be that
	//difficult, however, both fractions are extremely close (i.e. within 
	//50 decimal places) of the root so normal comparison techinques aren't
	//an option.
	
	//The convergents array passed in contains the last two convergents 
	//of sqrt(n) that don't exceed the given limit. We attempt to calculate
	//a semi-convergent of sqrt(n) with a denominator as close to the limit 
	//as possible using these two known convergents.
	longFraction known = convergents[1], potential;
	long long k = (limit - convergents[0].denominator) / known.denominator;

	if (k == 0)
	{
		//The mediant has a denominator exceeding the limit so simply add 
		//the largest convergent's denominator to the answer.
		answer += known.denominator;
		return;
	}

	potential.numerator = known.numerator * k + convergents[0].numerator;
	potential.denominator = known.denominator * k + convergents[0].denominator;

	//Instead of directly comparing the convergent and semi-convergent to sqrt(n),
	//we add them together, square the result and compare this to 4n. This allows us 
	//to avoid calculating the square root out to 50+ decimal places which is a very 
	//slow operation. Since both fractions will be so close, they are converted into
	//decimal form and stored in vectors to make digit by digit comparison easier.
	int decimalPlaces = 60;
	std::vector<int> decimalKnown = convertFractionToDecimalVector(known.numerator, known.denominator, decimalPlaces);
	std::vector<int> decimalPotential = convertFractionToDecimalVector(potential.numerator, potential.denominator, decimalPlaces);
	
	//Determine which of the two fractions is larger.
	longFraction* largerFraction = &potential, *smallerFraction = &known;
	if (greaterVector(decimalKnown, decimalPotential))
	{
		largerFraction = &known;
		smallerFraction = &potential;
	}

	std::vector<int> decimalSum = addDecimalVectors(decimalKnown, decimalPotential);
	std::vector<int> fourN = convertFractionToDecimalVector(4 * n, 1, decimalPlaces);
	std::vector<int> squaredSum = manualMultiplication(decimalSum, decimalSum);

	//If 4*n is larger than the sum of the squares it means that the larger of the 
	//two fractions is closer to root(n), otherwise the smaller of the two fractions 
	//is clsoer to root(n)
	if (greaterVector(fourN, squaredSum)) answer += largerFraction->denominator;
	else answer += smallerFraction->denominator;

	return;
}

void getSquareRootConvergents(longFraction* convergents, int n, unsigned long long root_n, unsigned long long limit)
{
	//This method calculates the convergents of the continued fraction reprsenting sqrt(n).
	//For example, for sqrt(13) these convergents are 3/1, 4/1, 7/2, 11/3, 18/5, 119/33...
	//The largest two convergents with denominators less than or equal to the given limit are
	//stored in the passed in convergents array.
	unsigned long long a = root_n, m = 0, d = 1;
	unsigned long long temp = 0;

	//initialize convergent array and variables needed for iterative calculations
	m = d * a - m;
	d = (n - m * m) / d;
	a = (root_n + m) / d;

	convergents[0] = { root_n, 1 };
	convergents[1] = { a * convergents[0].numerator + 1, a };

	while (true)
	{
		m = d * a - m;
		d = (n - m * m) / d;
		a = (root_n + m) / d;

		//Check to see if next denominator will exceed the limit
		if ((limit - convergents[0].denominator) / convergents[1].denominator < a) return;

		temp = convergents[1].numerator;
		convergents[1].numerator = convergents[1].numerator * a + convergents[0].numerator;
		convergents[0].numerator = temp;

		temp = convergents[1].denominator;
		convergents[1].denominator = convergents[1].denominator * a + convergents[0].denominator;
		convergents[0].denominator = temp;
	}
}

std::pair<std::string, double> q192()
{
	auto run_time = std::chrono::steady_clock::now();
	unsigned long long answer = 0, limit = 1000000000000;
	longFraction convergents[2];

	//for (unsigned long long i = 1; i <= 100000; i++)
	//{
	//	//Carry out a simple check to make sure we skip square numbers
	//	unsigned long long i_root = sqrt(i);
	//	if (i_root * i_root != i)
	//	{
	//		getSquareRootConvergents(&convergents[0], i, i_root, limit);
	//		getBetterApproximation(i, &convergents[0], answer, limit);
	//	}
	//}

	double basePixelDistance = 265.0 / (11.0 / 3.0 - sqrt(13));
	for (int i = 0; i <= 10; i++)
	{
		double fraction = (i * 18.0 + 11.0) / (i * 5.0 + 3.0);
		std::cout << "Distance between sqrt(13) and " << i * 18 + 11 << "/" << i * 5 + 3 << " is " << (fraction - sqrt(13)) * basePixelDistance << " pixels." << std::endl;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 57060635927998347
	//ran in 0.690674 seconds
}

//NOTES
//For more detailed notes check Project-Floyd.com, as for an overview of the solution:
//
//1. For each n from 1 to 100,000 calculate the convergents of the continued fraction representation of sqrt(n). Keep the largest two where denominator
//   is less than 10^12, call these fractions a/c and b/d respectively (c < d < 10^12). 
//2. Calculate the largest semi-convergent via semi = (a + bk) / (c + dk) where the denominator is also less than 10^12. If there are no semi-convergents
//   with a denominator less than 10^12, add d to the answer and skip step 3.
//3. Which ever fraction is closer to sqrt(n), b/d or (a + bk) / (c + dk), add that denominator to the final answer. Both fractions will be extremely
//   close to the root so their decimal forms are stored in vectors for digit by digit comparison out to 60 decimal places
//