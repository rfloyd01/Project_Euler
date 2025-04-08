#include "continued_fractions.h"
#include "Functions/functions.h"

std::vector<int> generateContinuedFractionCoefficients(double num, int limit)
{
	int numInt;
	std::vector<int> coefficients;

	for (int i = 0; i < limit; i++)
	{
		numInt = (int)num;
		coefficients.push_back(numInt);

		num -= numInt;
		if (num == 0) break;

		num = 1.0 / num;
	}

	return coefficients;
}

std::vector<int> generateContinuedFractionCoefficients(int fraction[2], int limit)
{
	bool numerator = 0;
	std::vector<int> coefficients;

	for (int i = 0; i < limit; i++)
	{
		if (fraction[!numerator] == 0) break;

		coefficients.push_back(fraction[numerator] / fraction[!numerator]);
		fraction[numerator] -= coefficients.back() * fraction[!numerator];
		numerator = !numerator;
	}

	return coefficients;
}

int generateNextContinuedFractionCoefficient(double& num)
{
	int answer = (int)num;
	if (answer == num) return 0;

	num = 1.0 / (num - answer);
	return answer;
}

int generateNextContinuedFractionCoefficient(int* fraction[2])
{
	int answer = *fraction[0] / *fraction[1];
	int temp = *fraction[1];
	*fraction[0] -= answer * *fraction[1];
	*fraction[1] = *fraction[0];
	*fraction[0] = temp;

	return answer;
}

void generateNextFraction(double& num, int (&numerators)[2], int (&denominators)[2])
{
	int a = generateNextContinuedFractionCoefficient(num);

	int newNumerator = numerators[1] * a + numerators[0];
	int newDenominator = denominators[1] * a + denominators[0];

	numerators[0] = numerators[1];
	numerators[1] = newNumerator;

	denominators[0] = denominators[1];
	denominators[1] = newDenominator;
}

void generateNextFraction(double& num, uint128_t(&numerators)[2], uint128_t(&denominators)[2])
{
	int a = generateNextContinuedFractionCoefficient(num);

	uint128_t newNumerator = numerators[1] * a + numerators[0];
	uint128_t newDenominator = denominators[1] * a + denominators[0];

	numerators[0] = numerators[1];
	numerators[1] = newNumerator;

	denominators[0] = denominators[1];
	denominators[1] = newDenominator;
}