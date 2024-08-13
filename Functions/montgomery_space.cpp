#include "montgomery_space.h"

#include <Header_Files/uint128_t.h>
#include "Functions/functions.h"
#include <cmath>

MontgomerySpace::MontgomerySpace(uint64_t n)
{
	if (n > 0x100000000) throw; //The methods in this class only work if n is <= 2^32
	this->n = n;

	//Calculate r to be the largest power of 2 greater than n
	this->k = ceil(log2(n));
	this->r = 1ULL << k;

	//Calculate n' using the Newton Method
	this->n_prime = 1;
	for (int i = 2; i <= k; i++)
	{
		n_prime *= 2 - n_prime * n;

		//reduce n_prime % 2^i
		n_prime &= ((1ULL << i) - 1);
	}

	//The above loop solves n' for the equation r*r' + n*n' = 1, however,
	//we need to solve n' for the equation r*r' - n*n' = 1. Simply multiply
	//the calculated n' by -1 and modulus divide by r.
	n_prime *= -1;
	n_prime &= (r - 1);

	//Calculate r^2 % n which can be used to quickly transform numbers into the
	//Montgomery Space. This can in turn be used to calculated r^3 % n which is 
	//helpful for computing modular inverses
	this->r2 = slowModMult(this->r, this->r, n);
	this->r3 = REDC(this->r2 * this->r2);
}

void MontgomerySpace::calculateInverses(uint64_t range)
{
	//Calculate the inverses outside of Montgomery space and then convert them to their
	//Montgomery Form

	//TODO: It may be beneficial to copy the code for modularMultiplicativeInverseRange() here
	//so that functions.h doesn't need to be included in the header file. This way a circular
	//reference can be avoided if we ever wanted to include montgomery_space.h inside of functions.

	//TODO: It's possible that the Montgomery forms of numbers fall outside of the original range
	//given when "range" is smaller than n - 1. For example, if "range" is 10 and n is 101, then it's
	//likely that some of the inverses calculated will be larger than the original range of 10. Either
	//need to increase the length of the inverses array to accomodate these cases, or, create a second
	//array which can be used for quickly pointing non-Montgomery numbers to their Montgomery equivalent
	//so that the non-montgomery array can be used.

	long long* inverses = new long long[range + 1];
	modularMultiplicativeInverseRange(range, this->n, inverses);
	for (int i = 0; i <= range; i++) this->inverses.push_back({ 0 });

	for (int i = 1; i <= range; i++)
	{
		MontgomeryNumber num = Transform(i);
		this->inverses[num.value] = Transform(inverses[i]);
	}
}

MontgomeryNumber MontgomerySpace::Transform(uint64_t a)
{
	//This method transforms an integer from normal space into Montgomery Space.
	return { REDC(a % this->n * this->r2) };
}

uint64_t MontgomerySpace::Revert(MontgomeryNumber a)
{
	//This method takes a number in Montgomery Space and reverts it back into
	//normal space.
	return REDC(a.value);
}

MontgomeryNumber MontgomerySpace::Add(MontgomeryNumber _a, MontgomeryNumber _b)
{
	//This method adds two numbers that are in Montgomery form.
	uint64_t add = _a.value + _b.value;
	return { add - this->n * (add >= this->n) }; //if we overflow n simply subtract n from the addition
}

MontgomeryNumber MontgomerySpace::Subtract(MontgomeryNumber _a, MontgomeryNumber _b)
{
	//This method returns the subtraction of two numbers in Montgomery form
	return { _a.value - _b.value + this->n * (_b.value > _a.value) }; //if the subtraction is negative simply add n to it
}

MontgomeryNumber MontgomerySpace::Multiply(MontgomeryNumber _a, MontgomeryNumber _b)
{
	//This method multiplies two numbers which have already been transformed into 
	//the Montgomery Space. The result is a number that also exists in the 
	//Montgomery Space.
	return { REDC(_a.value * _b.value) };
}

MontgomeryNumber MontgomerySpace::Divide(MontgomeryNumber _a, MontgomeryNumber _b)
{
	//The division a / b in Montgomery space is the same thing as multiuplying
	//by the modular multiplicative inverse. If any inverses have been precalculated
	//use them, otherwise manually compute the inverse
	if (this->inverses.size() >= _b.value) return { Multiply(_a, this->inverses[_b.value])};
	else return { Multiply(_a, MontgomeryInverse(_b)) };
}

uint64_t MontgomerySpace::REDC(uint64_t t)
{
	//First calculate m = ((t % r) * n') % r
	/*uint64_t m = t & (this->r - 1);
	m *= this->n_prime;
	m &= (this->r - 1);*/
	uint64_t m = t * this->n_prime & (this->r - 1);

	//Next, calculate t' = (t + m * n) / r
	uint64_t t_prime = m;
	t_prime *= this->n;
	t_prime += t;
	t_prime >>= this->k;

	//If t' is larger than n simply subtract n from it
	if (t_prime >= this->n) t_prime -= this->n;

	//t' should be at most a 64-bit integer so return the first word only of the 128-bit integer
	return t_prime;
}

uint64_t MontgomerySpace::slowModMult(uint64_t a, uint64_t b, uint64_t m)
{
	//Because of the left shift operation in the while loop, b can inadvertently
	//be set to 0 if it's larger than 63-bits. In this case, it must be cast to
	//a 128-bit integer for the first iteration of the loop
	uint64_t result = 0;
	
	while (a != 0)
	{
		if (a & 1) result = (result + b) % m;
		a >>= 1;
		b = (b << 1) % m;
	}
	return result;
}

void MontgomerySpace::ExtendedEuclideanAlgorithm(uint64_t a, uint64_t b, uint64_t* x, uint64_t* y)
{
	//Computes ax + by = GCD(a, b)
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return;
	}

	uint64_t x1, y1; // To store results of recursive call
	ExtendedEuclideanAlgorithm(b % a, a, &x1, &y1);

	// Update x and y using results of
	// recursive call
	*x = y1 - (b / a) * x1;
	*y = x1;
}

MontgomeryNumber MontgomerySpace::MontgomeryInverse(MontgomeryNumber _a)
{
	//This method takes in the Montgomery form number '_a' and returns the inverse of the non-Montgomery number 'a'.
	//The inverse of 'a' in Montgomery Space is equivalent to (R / a) % n. To calculate this
	//inverse, we take the Montgomery form number _a (which is equivalent to aR % n)
	//and calculate its inverse (_a^-1) with respect to n via the Extended Euclidean algorithm.
	//This inverse, 1/_a % r is equivalent to 1 / (aR) % n. We then take _a^-1, multiply it be R^3 % n
	//and feed this into the REDC() method (R^3 % n is calculated and saved when the Montgomery
	//Space is first instantiated). All that the REDC() method effectively does is 
	//multiply the input by 1/R, meaning the output is R^3 * 1/aR * 1/R = R/a. Since all of
	//this happens inside of Montgomery Space the % n is implicit and we have our final result
	//of (R/a) % n.

	//It took me quite awhile to understand how inversion works in Montgomery space so just
	//to summarize and give an example.
	//
	//1. Start with a normal number, a
	//2. Convert it to its Montgomery form, _a, by _a = aR % n;
	//3. Pass _a into this method
	//    3a. First _a^-1 = (1/_a) % n = (1/aR) % n is calculated
	//    3b. Then a^-1 = R/a is calculated via REDC(_a^-1 * R^3 % n)
	//4. The result is still in Montgomery form so we can use it directly with other Montgomery Form numbers

	//Example: Let's say that N = 17, R = 32 and we start with the non-Montgomery form number
	//a = 2. Our goal with this method is to find the Montgomery form of 1/a. To do this we
	//convert a into Montgomery form -> _a = Transform(a) -> _a = 2 * 32 % 17 = 13. We then pass the 
	//number 13 into this method which results in MontgomeryInverse(13) = 16. This value 
	//of 16 is equivalent to the 2 / 32 % 17.
	
	//*note - There's another way to find the inverse of a in Montgomery Space which may seem
	//more intuitive. Simply calculate the inverse of a outside of Montgomery Space with the
	//extended Euclidean algorithm (i.e. a * a^-1 == 1 mod n). Then take this inverse and convert
	//it to Montgomery Space via a^-1 * R mod n. Although this approach works, it needs to occur
	//outside of Montgomery space, while this method occurs inside of Montgomery space and so 
	//is much faster.

	uint64_t _aInverse, nInverse;
	ExtendedEuclideanAlgorithm(this->n, _a.value, &nInverse, &_aInverse);
	return { REDC(_aInverse * this->r3) };
}