#pragma once

#include <Header_Files/print.h>
#include <Header_Files/int_64x.h>
#include <vector>
#include <string>

//arrays with common values
extern int powers_of_two[11]; // = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
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
std::vector<int> PrimeFactors(int number);
std::vector<int> getFactors(int number);
bool primeNumberTest(long long number);

int NumberOfFactors(int n);
int char_to_int(char a);
char int_to_char(int a);
long long choose(int m, int n);
int_64x BigChoose(int m, int n);
long long factorial(int n);
int gcd(int a, int b);
long long gcd(long long a, long long b);
bool coprime(int a, int b);
int numberOfDigits(long long n);
void FaraySequence(int maximum, std::vector<fraction>& pairs, fraction low = { 0, 1 }, fraction high = { 1, 1 }, bool new_pair = false);

//Partition Functions
std::vector<long long> polynomial_multiply(std::vector<long long>& p1, std::vector<long long>& p2, int cut_off = 0);
std::vector<int> polynomial_mod_multiply(std::vector<int>& p1, std::vector<int>& p2, int mod, int cut_off = 0);
long long polcoeff(std::vector<std::vector<long long> >& polynomials, int order);
int polcoeff(std::vector<std::vector<int> >& polynomials, int order, int mod);
void getPartitions(int n, std::vector<std::vector<std::vector<int> > > &all_partitions, int currentLength = 0,
	int currentValue = 0, int maxNumber = 0, int maxLength = 0, std::vector<int>* currentPartition = nullptr, bool recursive = false);

//Template Functions
template <typename T>
T ModMult(T a, T b, T mod)
{
	//This function returns a * b % mod which is useful when multiplying 2 large numbers would normally overflow a long long int
	//as a caveat it only works if the value of mod is 63 bits or less

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
T ModPow(T base, T exp, T modulus, bool overflow);

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