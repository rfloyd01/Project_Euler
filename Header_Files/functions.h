#pragma once

#include <Header_Files/print.h>
#include <Header_Files/int_64x.h>
#include <vector>
#include <string>
#include <iostream>

//arrays with common values
extern const int powers_of_two[11]; // = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
extern long long powers_of_ten[11]; // = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000 };

struct fraction
{
	int numerator;
	int denominator;
};

//Prime Number Functions
std::vector<bool> p_sieve(int maximum);
void PrimesWithSieve(bool* prime_array, int maximum, std::vector<int>& primes);
std::vector<int> primes(int maximum);
std::vector<int> p_factors(int number);
//std::vector<int> PrimeFactors(int number);
std::vector<int> getFactors(int number);
bool primeNumberTest(long long number);
std::vector<std::vector<int> > AllPrimeFactors(int n);

template <typename T>
std::vector<T> PrimeFactors(T number)
{
	//returns a list of the prime factors of number, with repititions
	std::vector<T> p_facts;
	T i = 2;
	while (number > 1)
	{
		if (number % i == 0)
		{
			p_facts.push_back(i);
			number /= i;
		}
		else i++;
	}

	return p_facts;
}

int NumberOfFactors(int n);
int char_to_int(char a);
char int_to_char(int a);
int_64x BigChoose(int m, int n);
int_64x BigPow(int_64x n, int p);
long long factorial(int n);
int gcd(int a, int b);
long long gcd(long long a, long long b);
long long extendedEuclideanGCD(long long a, long long b, long long *x, long long *y);
bool coprime(int a, int b);
int numberOfDigits(long long n);
void FaraySequence(int maximum, std::vector<fraction>& pairs, fraction low = { 0, 1 }, fraction high = { 1, 1 }, bool new_pair = false);
void orderedFaraySequence(int maximum, std::vector<fraction>& pairs, fraction low = { 0, 1 }, fraction high = { 1, 1 }, bool new_pair = false, int insertIndex = -1);
void Farey_Iterative(int n);
std::vector<int> baseConversion(int n, int b);
long long ChineseRemainderTheorem(long long n, long long mod, std::vector<std::pair<long long, long long> > *equations = nullptr);

//Binomial Functions
long long choose(int m, int n);
long long recursiveChoose(int m, int n);
long long binomialMod(long long n, long long m, long long mod, int isPrime = 2);
long long binomialModPrime(long long n, long long m, long long p, long long* factorials = nullptr, long long* inverse_factorials = nullptr);
long long binomialModePrimeAdvanced(long long n, long long m, long long p);
long long lucasTheorem(long long n, long long m, long long p);
long long generalizedLucasTheorem(long long n, long long m, long long p, long long q);
long long primeFactorialInstances(long long k, long long p);

//Partition Functions
std::vector<long long> polynomial_multiply(std::vector<long long>& p1, std::vector<long long>& p2, int cut_off = 0);
std::vector<int> polynomial_mod_multiply(std::vector<int>& p1, std::vector<int>& p2, int mod, int cut_off = 0);
long long polcoeff(std::vector<std::vector<long long> >& polynomials, int order);
int polcoeff(std::vector<std::vector<int> >& polynomials, int order, int mod);
void getPartitions(int n, std::vector<std::vector<std::vector<int> > > &all_partitions, int maxNumber = 0, int maxLength = 0, 
	int currentLength = 0, int currentValue = 0, std::vector<int>* currentPartition = nullptr, bool recursive = false);
long long permutationsOfPartitions(int n, int l, int maximum = 0, int minimum = 0, bool quick_choose = true);

//Combinatorics Functions
std::vector<int> nonRotatedPermutations(std::vector<int> numbers, int max_length);

//Modular Arithmetic Functions
template <typename T>
T ModMult(T a, T b, T mod)
{
	//This function returns a * b % mod which is useful when multiplying 2 large numbers would normally overflow a long long int
	//as a caveat it only works if the value of mod is 63 bits or less

	if (a < 0)
	{
		std::cout << a << ", " << b << std::endl;
	}
	if (b >= ((unsigned long long)1 << 63))
	{
		std::cerr << "ModMult():: Second input too large, overflow may lead to incorrect answer." << std::endl;
	}

	T result = 0;
	while (a != 0)
	{
		if (a & 1) result = (result + b) % mod;
		a >>= 1;
		b = (b << 1) % mod;
	}
	return result;
}

template <typename T>
T ModPow(T base, T exp, T modulus, bool overflow)
{
	//returns base ^ exp % modulus
	//if overflow == 1 then the ModMult() function is used instead of normal multiplication to prevent overflow of 64-bit integers
	//this causes the function to take longer to run, but will prevent potential unwanted errors

	base %= modulus;
	T result = 1;
	while (exp > 0)
	{
		//if (exp & 1) result = (result * base) % modulus;
		//base = (base * base) % modulus;
		//exp >>= 1;
		if (exp & 1)
		{
			if (overflow) result = ModMult(result, base, modulus);
			else result = (result * base) % modulus;
		}
		if (overflow) base = ModMult(base, base, modulus);
		else base = (base * base) % modulus;
		exp >>= 1;
	}
	return result;
}

template <typename T>
T ModularMultiplicativeInverse(T b, T mod, int isPrime = 2)
{
	//I've run into issues in the past where I need to use division with modular arithmetic, however, this causes issues. 
	//Instead of dividing, you need to multiply by the modular multiplicative inverse, i.e. instead of doing a / b = x, you 
	//need to do a * (1/b) = x. This modular multiplicative inverse can be found using Euler's theorem, basically,
	//b ^ -1 == b ^ (m-2) MOD m.

	//If we're unsure whether or not the modulus is a prime number (isPrime = 2), we do a quick
	//primality test
	bool prime = isPrime;
	if (isPrime == 2) prime = primeNumberTest((long long)mod);
	if (prime) return ModPow(b, mod - 2, mod, true);

	//if mod isn't a prime then we need to calculate the inverse differently
	T m0 = mod, y = 0, x = 1;

	if (mod == 1) return 0;

	while (b > 1)
	{
		//q is quotient
		T q = b / mod, t = mod;

		//m is remainder now, process same as Euclid's algo
		mod = b % mod, b = t;
		t = y;

		//Update y and x
		y = x - q * y;
		x = t;
	}

	// Make x positive
	if (x < 0) x += m0;

	return x;
}

template <typename T>
T BinomialMod(T n, T k, T m);

template <typename T>
T BinomialModLargePrime(T n, T k, T m, long long* factorials)
{
	//when m is larger than n we can use this function as opposed to the other BinomialMod function.
	//factorials is a precalculated array of factorials that have been modulus divided by m. If we're
	//passed a nullptr then this will be calculated now.
	if (factorials == nullptr)
	{
		//create array for all factorials % m up to n
		factorials = new long long[n + 1]();
		factorials[0] = 1;
		for (int i = 1; i <= n; i++) factorials[i] = factorials[i - 1] * i % m;
	}

	T mult_inverse = ModPow((T)((factorials[n - k] * factorials[k]) % m), (T)(m - 2), (T)m, 1);
	//std::cout << "choose(" << n << ", " << k << ") MOD " << m << " = " << (factorials[n] * mult_inverse) % m << std::endl;
	return (factorials[n] * mult_inverse) % m;
}

void modularMultiplicativeInverseRange(int n, long long mod, long long* inverses);
void modularMultiplicativeInverseRangeOverflowSafe(int n, long long mod, long long* inverses);

long long MyPow(long long x, unsigned long long p);

template <typename T>
void MatrixVectorMult(T* M, T* V, const int size)
{
	//This function takes a vector V and multiplies it by a matrix M
	//Since arrays are used the size of the vector needs to be given.
	//The size of the vector needs to be provided as well. Care needs to 
	//be given to making sure that M is the correct size to multiply with
	//V as there is no check for it in the function

	std::vector<T> temp;

	for (int i = 0; i < size; i++)
	{
		T num = 0;
		for (int j = 0; j < size; j++)
		{
			num += *(M + (size * i) + j) * (*(V + j));
		}
		temp.push_back(num);
	}

	for (int i = 0; i < size; i++) *(V + i) = temp[i];
}

//Other
std::vector<long long> mintageCount(long long total_mintage, std::vector<long long> highest_value);

class BigNum
{
public:
	BigNum();
	BigNum(int zeros);
	BigNum(std::string num);

	//Addition functions
	void Add(BigNum num);
	void Add(std::vector<int> num);
	void Add(int num);
	void Add(long long num);

	//Multiplication functions
	void Multiply(BigNum num);
	void Multiply(std::vector<int> num);
	void Multiply(int num);
	void Multiply(long long num);

	std::vector<int> GetVec();
	int length();

	//Conversion functions
	int GetInt();
	int GetInt(std::vector<int> nums);
	long long GetLongInt();
	long long GetLongInt(std::vector<int> nums);

	//Print/Return functions
	void Print();
	std::string GetNumberString(); //Since these numbers can be very large there needs to be an option to print out a string instead of a physical number
	std::string GetNumberStringFirstXDigits(int x);
	std::string GetNumberStringLastXDigits(int x);
	int FirstXDigits(int x);
	int LastXDigits(int x);

private:
	std::vector<int> number;
	std::string number_string;
	int digits;

	//Functions to convert normal numbers into vectors
	std::vector<int> ConvertToVector(int num);
	std::vector<int> ConvertToVector(long long num);

	//Clean up functions
	void TruncateZeros(); //If the BigNum has any leading zeros this will get rid of them for better readability
	void UpdateNumberString();

	void LeftShift(std::vector<int>& nums, int shift);
};