#include "modular_math.h"
#include "functions.h"
#include "Header_Files/uint128_t.h"

#include <cmath>

long long BetterModMult(long long a, long long b, long long m)
{
	//This method returns the value of (a * b) % m. Normally this arithmetic wouldn't
	//be hard to carry out with standard operators, however, in instances where a * b 
	//will overflow a 64-bit integer before modulus division can be applied, a better
	//aproach needs to be taken. This method will employ one of a few methods in order
	//to efficiently calculated (a * b) % m without overflow.

	//Given a small enough modulus m, there's no fear of overflow and we can carry out 
	//a standard operation.
	if (m <= 3037000500) return (a % m * b % m) % m;

	//Before doing anything check for negative values in a and b. Most approaches for
	//the mod mult method require bitwise operations which makes it easier to treat 
	//all numbers as positive (MSB can be a 1). The final result can simply be multiplied
	//by negative 1 mod m at the end if necessary.
	int negative_multiplier = 1;
	if (a == 0 || b == 0) return 0;

	if (a < 0)
	{
		negative_multiplier *= -1;
		a *= -1;
	}

	if (b < 0)
	{
		negative_multiplier *= -1;
		b *= -1;
	}

	//convert to unsigned long long to utilize all 64 bits if necessary
	unsigned long long a_ = a % m, b_ = b % m, max = (1ULL << 63ULL) - 1ULL;

	//Before performing any of the more involved algorithms, see if overflow will actually happen.
	//If overflow will happen
	if (max / b_ >= a_) return (a_ * b_) % m * negative_multiplier;

	int x = 5;

	return 0;
}

long long MontgomeryMultiplication(unsigned long long a, unsigned long long b, unsigned long long n)
{
	//This algorithm gives a way to solve the problem X = AB (mod N) when multiplying a and b together
	//would normally cause integer overflow. Normally calculating the modulus requires division which is 
	//inherently slow on computers. This algorithm instead uses an auxilliary modulus, R, where gcd(R, N) = 1.
	//This auxilliary modulus can (but doesn't have to be) be set to a power of 2, so that any division which
	//does need to take place can simply be done bit bitwise shifting operations.

	//This algorithm requires putting numbers into their "Montgomery" form, that is, putting them into their residue 
	//class with respect to R (aR mod N). For example, suppose that N = 17 and R = 100. The Montgomery forms
	//for the numbers 3, 5, 7 and 15 would be 11 (300 mod 17), 7 (500 mod 17), 3 (700 mod 17) and 4 (1500 mod 17)
	//respectively.

	//Addition and subtraction in Montgomery form are the same as in standard arithmetic, that is:
	//aR + bR = (a + b)R and aR - bR = (a - b)R. Multiplication is a little more complicated though and is expressed as:
	//(aR mod N)(bR mod N) mod N = (abR)R mod N. Ultimately what we want is abR mod N, so there's an extra factor of R
	//on the right side of the equation needs to be removed. As an example the product of the Montgomery forms of 7 and 15
	//(where N = 17 and R = 100) would be 3 * 4 = 12. We can't simply divide out by R here because 12 isn't divisible by
	//100, so extra effort needs to be taken to remove the extra R factor. In order to remove the extra R factor it can
	//be multiplied by some integer, R', such that RR' == 1 (mod N). Essentially, we need to multiply R by the modular
	//inverse of R mod N. Multiplying by R' and the multiplication equation becomes (aR mod N)(bR mod N)R' = (aR)(bR)R^-1 =
	//(ab)R (mod N). We know that R' exists because N and R are coprime with each other and can be calculated with 
	//the extended Euclidean algorithm.

	//Here's an example of using Montgomery Mutliplication to find the answer to (7 * 15) mod 17.
	//  1. Choose any value of R where gcd (R, N) = 1, select 100 for this example.
	//  2. Convert 7 and 15 to their Montgomery forms and multiply --> 700 mod 17 = 3 and 1500 mod 17 = 4 --> 3 * 4 = 12
	//  3. Use the extended Euclidean Algorithm to calculate N' --> extendedGCD(100, 17, R', N')
	//		3a. Normally the extended Euclidean Algorithm solves the equation RR' + NN' = 1, however we need it to be in 
	//          the form RR' - NN' = 1. This can be achieved simply by multiplying N' by -1 and then modulus dividing by R
	//      3b. In this case using the extended Euclidean Algorithm gives N' = -47, which then gets transformed to 47
	//          with step 3a.
	//  4. Call the REDC algorithm with the following inputs: R, N, N' and T (the result of the multiplication from step 2).
	//  5. Calling REDC(100, 17, 47, 12) returns a value of 11.
	//  6. Convert 11 from Montgomery form back to standard form --> 100X = 11 (mod 17) --> x = 3 (mod 17)
	//  7. Just to test out that this is correct 7 * 15 = 105 and 105 % 17 = 3 

	//Using this Method with an Even Modulus:
	//--------------------------------------
	//The above example shows that R doesn't need to be a power of 2 to work properly, however, the division and modulus
	//division steps of the REDC algorithm become much simpler when it is. If R is of the form 2^k, then a / r becomes
	//a >> k and a % r becomes a & (r - 1). Since R and N must be coprime with each other, this would imply that using
	//Montgomery multiplication isn't worth it when N is an even number, as choosing a power of 2 for R won't be possible.
	//This isn't entirely the case though. In the case that N is an even number we can break it down into two numbers,
	//a and b, such that a*2^k * b = N. This can easily be done by right shifting N until it becomes an odd number.

	//With a and b in these forms we can solve the separate equations X = ab (mod b) and X = ab (mod 2^k) and then 
	//combine these two answers via the Chinese Remainder Theorem. The first equation will can be solved via the 
	//method explained above and the second equation can be trivially solved since modular division by a power of 2
	//can be done with a subtraction and bitwise AND.

	//Limitations:
	//------------
	//The biggest limitation to this method currently is that N mst be less than 2^62. This is because anything larger than
	//this will cause R to be 2^63 which will be seen as a negative number. It's possible to handle larger numbers, however,
	//they need to be separated into separate computer words. A separate method is used to carry this out.

	////Step 0: TODO: make sure that a and b have been mod divided by N
	a %= n;
	b %= n;
	bool evenModulus = false;
	int p = 1;

	if (n % 2 == 0)
	{
		//To use this method the modulus 'n' needs to be an odd number. If 'n' is even,
		//we shift it right until it becomes odd. The amount that it gets shifted by, k, can 
		//be represented as 2^k. We can then break the original congruence of x == ab % n
		//into the following two congruences: x1 == ab % (n >> k), x2 == ab % (2^k). The first
		//of these congruences now satisfies the requirement that n by odd so we carry out the
		//algorithm as normal. The second of these congruences is now trivial since we're modulus
		//dividing by a power of 2. We get x2 = ab & (2^k - 1). After each of the congruences is
		//solved we combine them with the Chinese Remainder Theorem, that is X == ab % n
		//--> X == x1 (mod n >> k), X == x2 (mod 2^k) --> X = x1 - (2>>k) * u * (x1 - x2)
		//where u is obtained via the extended Euclidean formula where u*(2>>k) + v(2^k) = 1.

		evenModulus = true;
		while ((n & 1LL) == 0)
		{
			n >>= 1;
			p++;
		}
	}

	long long answer;
	if (n < (1LL << 32))
	{
		//If n < 2^32 there's no fear of overflowing 64-bit integers during
		//intermediate calculations so we can calculate the answer using built-in
		//types only
		answer = (a * b) % n;// MontgomeryMultiplication32Bit(a, b, n);
	}
	else
	{
		//If n >= 2^32 there's a possibility of overflow occuring during some
		//intermediate calculations. This means that calculations must be 
		//carried out with a custom defined 128-bit integer class which is a 
		//little slower than using built-in types only.
		answer = MontgomeryMultiplication64Bit(a, b, n);
	}

	if (evenModulus)
	{
		//If n was originally even then we carry out the necessary Chinese Remainder operations
		//to get our final answer;
	}

	return answer;
}

long long MontgomeryMultiplication32Bit(unsigned long long a, unsigned long long b, unsigned int n)
{
	//When n is smaller than 2^32 then we can safely multiply a and b together withoug worrying
	//about overflow. This means that operations can take place with standard long integers instead of 
	//the slower uint128_t class.
	if (n % 2 == 0)
	{
		long long p = 1;
		while ((n & 1LL) == 0)
		{
			a >>= 1;
			p <<= 1;
		}

		//carry out CRT operations
	}

	//Step 0: TODO: make sure that a and b have been mod divided by N
	a %= n;
	b %= n;

	//Step 1: Select R to be the smallest power of 2 that's larger than N.
	int k = ceil(log2(n));
	unsigned long long r = 1LL << k;

	//Step 2: Calculate the N' value to use in reduction steps from N and R
	long long n_prime, r_prime;
	extendedEuclideanGCD(r, n, &r_prime, &n_prime);
	n_prime = (n_prime * -1 + r) & (r - 1);

	//Step 2: Convert A and B into Montgomery Form
	long long rModN = r % n; //is this just n_prime? check as it would be more efficient
	long long rSquared = (rModN * rModN) % n;
	a = EfficientREDC(r, n, n_prime, (a * rSquared) % n, k);
	b = EfficientREDC(r, n, n_prime, (b * rSquared) % n, k);

	//Step 3: Multiply a and b in their Montgomery Forms and reduce.
	long long answer = EfficientREDC(r, n, n_prime, a * b, k);

	//Step: Convert product from Montgomery Form to Standard Form
	return EfficientREDC(r, n, n_prime, answer, k);
}

long long MontgomeryMultiplication64Bit(unsigned long long a, unsigned long long b, unsigned long long n)
{
	//Step 0: TODO: make sure that a and b have been mod divided by N
	a %= n;
	b %= n;

	//Step 1: Select R to be the smallest power of 2 that's larger than N.
	int k = ceil(log2(n));
	unsigned long long r = 1LL << k;

	//Step 2: Calculate the N' value to use in reduction steps from N and R
	long long n_prime, r_prime;
	extendedEuclideanGCD(r, n, &r_prime, &n_prime);
	n_prime = (n_prime * -1 + r) & (r - 1);

	//Step 2: Convert A and B into Montgomery Form
	long long rModN = r % n; //is this just n_prime? check as it would be more efficient
	long long rSquared = (rModN * rModN) % n;
	a = EfficientREDC(r, n, n_prime, (a * rSquared) % n, k);
	b = EfficientREDC(r, n, n_prime, (b * rSquared) % n, k);

	//Step 3: Multiply a and b in their Montgomery Forms and reduce.
	long long answer = EfficientREDC(r, n, n_prime, a * b, k);

	//Step: Convert product from Montgomery Form to Standard Form
	return EfficientREDC(r, n, n_prime, answer, k);
}

long long REDC(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t)
{
	long long m = ((t % r) * n_prime) % r; //if r = 2^k then this line becomes ((t & (r - 1)) * n_prime) & (r - 1)
	long long t_prime = (t + m * n) / r; //if r = 2^k then this line becomes (t + m * n) >> k
	if (t_prime >= n) return t_prime - n;
	else return t_prime;
}

long long EfficientREDC(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t, int k)
{
	//In order to use this algorithm without overflow we need to store intermediate calculations in a 128-bit 
	//integer.

	//First calculate m = ((t % r) * n') %r
	uint128_t m = t & (r - 1);
	m *= n_prime;
	m &= (r - 1);
	
	//Next, calculate t' = (t + m * n) / r
	uint128_t t_prime = m;
	t_prime *= n;
	t_prime += t;
	t_prime >>= k;

	//If t' is larger than n simply subtract n from it
	if (t_prime >= n) t_prime -= n;
	
	//t' should be at most a 64-bit integer so return the first word only of the 128-bit integer
	return t_prime.getWords()[0];
}

long long EfficientREDC32Bit(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t, int k)
{
	//This method should be called when n < 2^32. In this scenario, n is less than 2^32, which means that at most r = 2^32
	//and n_prime will also be less than 2^32. It's possible that t in this scenario will be larger than 2^32, however, it gets
	//modulus divided by r so will get reduced to a 32 bit number
	unsigned long long m = ((t & (r - 1)) * n_prime) & (r - 1); //if r = 2^k then this line becomes ((t & (r - 1)) * n_prime) & (r - 1)
	unsigned long long t_prime = (t + m * n) >> k; //if r = 2^k then this line becomes (t + m * n) >> k
	if (t_prime >= n) return t_prime - n;
	else return t_prime;
}