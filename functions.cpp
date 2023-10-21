#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <cmath>

//arrays with common values
const int powers_of_two[11] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
long long powers_of_ten[11] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000 };

bool operator==(const fraction& a, const fraction& b)
{
	//TODO: May want to think about reducing the fractions first
	return ((a.numerator == b.numerator) && (a.denominator == b.denominator));
}

std::vector<bool> p_sieve(int maximum)
{
	std::vector<bool> sieve;
	for (int i = 0; i <= maximum; i++) sieve.push_back(1);
	sieve[0] = 0; sieve[1] = 0;
	for (int i = 2; i <= maximum; i++)
	{
		if (sieve[i])
		{
			for (int j = 2 * i; j <= maximum; j += i) sieve[j] = 0;
		}
	}
	return sieve;
}

void PrimesWithSieve(bool* prime_array, int maximum, std::vector<int> &primes)
{
	//a function that fills an existing array of size maximum with a prime sieve and takes all prime numbers below maximum and puts them into a vector
	//prime_array is a pointer to the first element of an array of integers, maximum is the size of the existing array and primes is a reference to an empty array of integers

	*prime_array = false; *(prime_array + 1) = false; //set element 0 and 1 to zero because neither of those numbers are prime
	bool cont = true; //used to break out of loop below when no more primes below the maximum can be found
	for (int i = 2; i < maximum; i++) *(prime_array + i) = true; //in case not done so already, set every number in the array to 1
	for (int i = 2; i < sqrt(maximum) + 1; i++) //no need to go past the square root of maximum as it won't help eliminate any more composite numbers
	{
		if (*(prime_array + i))
		{
			primes.push_back(i);
			for (int j = i * i; j < maximum; j += i)
			{
				if (j < 0) std::cout << "Integer overflow during prime sieve, need to change to long integers." << std::endl;
				*(prime_array + j) = false;
			}
		}
		if (!cont) break;
	}

	//One final loop to add any primes more than sqrt(maximum) to the prime vector
	for (int i = sqrt(maximum) + 1; i < maximum; i++)
		if (*(prime_array + i)) primes.push_back(i);
}

std::vector<int> primes(int maximum)
{
	//returns a list of primes from 2 to maximum
	std::vector<int> sieve, prims;
	for (int i = 0; i <= maximum; i++) sieve.push_back(1);
	sieve[0] = 0; sieve[1] = 0;
	for (int i = 2; i <= maximum; i++)
	{
		if (sieve[i])
		{
			for (int j = 2 * i; j <= maximum; j += i) sieve[j] = 0;
		}
	}

	for (int i = 2; i < sieve.size(); i++)
		if (sieve[i]) prims.push_back(i);

	return prims;
}

std::vector<int> p_factors(int number)
{
	std::vector<int> prims = primes(number);
	std::vector<int> facts; facts.push_back(0);

	int i = 0;
	while (number > 1)
	{
		if (number % prims[i] == 0)
		{
			facts[i]++;
			number /= prims[i];
		}
		else
		{
			i++;
			facts.push_back(0);
		}
	}
	return facts;
}

//std::vector<int> PrimeFactors(int number)
//{
//	//returns a list of the prime factors of number, with repititions
//	std::vector<int> p_facts;
//	int i = 2;
//	while (number > 1)
//	{
//		if (number % i == 0)
//		{
//			p_facts.push_back(i);
//			number /= i;
//		}
//		else i++;
//	}
//
//	return p_facts;
//}

std::vector<int> getFactors(int number)
{
	//returns an ordered vector containing all of the factors of "number"
	std::vector<int> front_nums = { 1 }, back_nums = { number };
	int square_root = sqrt(number);
	if (square_root > 1)
	{
		for (int i = 2; i < square_root; i++)
		{
			if (number % i == 0)
			{
				front_nums.push_back(i);
				back_nums.insert(back_nums.begin(), number / i);
			}
		}
		if (square_root * square_root == number) front_nums.push_back(square_root);
		else
		{
			if (number % square_root == 0)
			{
				front_nums.push_back(square_root);
				back_nums.insert(back_nums.begin(), number / square_root);
			}
		}
	}
	else if (number == 1) return front_nums;
	
	front_nums.insert(front_nums.end(), back_nums.begin(), back_nums.end());
	return front_nums;
}

bool primeNumberTest(long long number)
{
	//tests whether the input number is prime using the Miller-Rabin technique
	//the vector a holds the "witnesses" to be tested against.

	if (number == 2) return true;
	if (number % 2 == 0) return false; //quick check to make sure an even number hasn't been entered
	if (number < 2) return false; //for 0, 1 and negative inputs

	std::vector<int> a;

	if (number < 2047)
	{
		a.push_back(2);
	}
	else if (number < 1373653)
	{
		a.push_back(2);
		a.push_back(3);
	}
	else if (number < 9080191)
	{
		a.push_back(31);
		a.push_back(73);
	}
	else if (number < 25326001)
	{
		a.push_back(2);
		a.push_back(3);
		a.push_back(5);
	}
	else if (number < 3215031751)
	{
		a.push_back(2);
		a.push_back(3);
		a.push_back(5);
		a.push_back(7);
	}
	else if (number < 4759123141)
	{
		a.push_back(2);
		a.push_back(7);
		a.push_back(61);
	}
	else if (number < 1122004669633)
	{
		a.push_back(2);
		a.push_back(13);
		a.push_back(23);
		a.push_back(1662803);
	}

	//first write number in the form 2^r * d + 1 (by factoring out powers of 2 from n - 1)
	int r = 0;
	long long d = number - 1;
	while (d % 2 == 0)
	{
		d /= 2;
		r++;
	}

	//Witness loop, check all numbers in the 'a' vector
	for (int i = 0; i < a.size(); i++)
	{
		bool quit = true;
		long long x = ModPow((long long)a[i], d, number, 1);
		if (x == 1 || x == (number - 1)) continue;

		for (int j = 0; j < (r - 1); j++)
		{
			x = ModPow(x, (long long)2, number, 1);
			if (x == (number - 1))
			{
				quit = false;
				break;
			}
		}
		if (quit) return false;
	}
	return true;
}

int NumberOfFactors(int n)
{
	int i = 2, number_of_factors = 1;
	while (n > 1)
	{
		if (n % i == 0)
		{
			int num = 0;
			while (n % i == 0)
			{
				n /= i;
				num++;
			}
			number_of_factors *= (num + 1);
		}
		else i++;
	}

	return number_of_factors;
}

std::vector<std::vector<int> > AllPrimeFactors(int n)
{
	//returns a vector holding the prime factorization for all numbers up to n
	std::vector<std::vector<int> > prime_factors;

	for (int i = 0; i <= n; i++)
	{
		std::vector<int> p_facts;
		prime_factors.push_back(p_facts);
	}
	//prime_factors[1] = { 1 }; //1 isn't prime but we don't want to leave this vector blank

	for (int i = 2; i <= n; i++)
	{
		if (prime_factors[i].size() != 0) continue;
		prime_factors[i] = { i };

		int j = 2, limit = n / i;
		while (j <= limit)
		{
			if (prime_factors[j].size() > 0)
			{
				prime_factors[i * j] = prime_factors[j];
				prime_factors[i * j].push_back(i);
			}
			j++;
		}
	}

	return prime_factors;
}

int char_to_int(char a)
{
	//wasn't able to find an easy built in way to convert the numeric character to an integer so just wrote this function

	//TODO:this function is no longer needed, a char can be converted to an int by subtracting the '0' character. i.e. '1' - '0' = 1

	if (a == '0') return 0;
	else if (a == '1') return 1;
	else if (a == '2') return 2;
	else if (a == '3') return 3;
	else if (a == '4') return 4;
	else if (a == '5') return 5;
	else if (a == '6') return 6;
	else if (a == '7') return 7;
	else if (a == '8') return 8;
	else if (a == '9') return 9;
	else
	{
		std::cout << "Non-number passed to function, converting to a zero." << std::endl;
		return 0; //if a character that isn't a number is passed to function then just return a 0
	}
}

char int_to_char(int a)
{
	//wasn't able to find an easy built in way to convert the integer to a numeric character so just wrote this function
	//designed to take single digit integers only, if more digits are given then only the one's place digit will be returned

	if (a % 100 >= 10)
	{
		std::cout << "More than one digit passed to conversion function, only converting the first digit" << std::endl;
		a %= 10;
	}

	if (a == 0) return '0';
	else if (a == 1) return '1';
	else if (a == 2) return '2';
	else if (a == 3) return '3';
	else if (a == 4) return '4';
	else if (a == 5) return '5';
	else if (a == 6) return '6';
	else if (a == 7) return '7';
	else if (a == 8) return '8';
	else return '9';
}

long long choose(int m, int n)
{
	//returns m choose n
	//works by breaking the numerator and denominator of the choose function into prime factors. All of the prime factors of the denominator are used to cancel terms in the numerator
	//the final answer is reached by multiplying all the terms left over in the numerator

	long long answer = 1;
	int n2 = m - n;

	std::vector<int> numerator;
	std::vector<int> denominator;

	if (n2 > n)
	{
		for (int i = m; i > n2; i--)
		{
			std::vector<int> facts = PrimeFactors(i);
			for (int j = 0; j < facts.size(); j++) numerator.push_back(facts[j]);
		}
		for (int i = n; i > 1; i--)
		{
			std::vector<int> facts = PrimeFactors(i);
			for (int j = 0; j < facts.size(); j++) denominator.push_back(facts[j]);
		}
	}
	else
	{
		for (int i = m; i > n; i--)
		{
			std::vector<int> facts = PrimeFactors(i);
			for (int j = 0; j < facts.size(); j++) numerator.push_back(facts[j]);
		}
		for (int i = n2; i > 1; i--)
		{
			std::vector<int> facts = PrimeFactors(i);
			for (int j = 0; j < facts.size(); j++) denominator.push_back(facts[j]);
		}
	}

	for (int i = denominator.size() - 1; i >= 0; i--)
	{
		for (int j = 0; j < numerator.size(); j++)
		{
			if (numerator[j] == denominator[i])
			{
				numerator.erase(numerator.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < numerator.size(); i++) answer *= numerator[i];
	return answer;
}

long long recursiveChoose(int m, int n)
{
	//a recursive approach to the choose function
	if (!n) return 1;
	return (m * recursiveChoose(m - 1, n - 1)) / n;
}

int_64x BigChoose(int m, int n)
{
	//implements the int_64x class to allow the choose function to be used on numbers of any size
	n = (m - n) < n ? m - n : n;

	int_64x answer = 1;
	for (int i = m - n + 1; i <= m; i++) answer *= i;
	for (int i = n; i > 1; i--) answer /= i;

	return answer;
}

int_64x BigPow(int_64x n, int p)
{
	//returns n^p but utilizes the int_64x class
	if (p <= 0) return 1; //this function doesn't currently handle negative exponents
	if (n == 0) return 0;
	if (n == 1) return 1;

	int_64x res = 1, n_copy = n;
	
	while (p)
	{
		if (p & 1) res *= n_copy;
		p >>= 1;

		n_copy *= n_copy;
	}
	return res;
}

long long factorial(int n)
{
	long long answer = 1;
	for (int i = 2; i <= n; i++) answer *= i;
	return answer;
}

int numberOfDigits(long long n)
{
	//a down and dirty function that returns the number of digits in a signed long integer
	//didn't care about negative numbers while writing this so anything negative will return a value of 1

	//TODO: This can all just be computed with logs, re-write this at some point

	if (n < 10) return 1;
	else if (n < 100) return 2;
	else if (n < 1000) return 3;
	else if (n < 10000) return 4;
	else if (n < 100000) return 5;
	else if (n < 1000000) return 6;
	else if (n < 10000000) return 7;
	else if (n < 100000000) return 8;
	else if (n < 1000000000) return 9;
	else if (n < 10000000000) return 10;
	else if (n < 100000000000) return 11;
	else if (n < 1000000000000) return 12;
	else if (n < 10000000000000) return 13;
	else if (n < 100000000000000) return 14;
	else if (n < 1000000000000000) return 15;
	else if (n < 10000000000000000) return 16;
	else if (n < 100000000000000000) return 17;
	else if (n < 1000000000000000000) return 18;
	else if (n < 9223372036854775808) return 19;
	else
	{
		std::cout << "Number is too big for an unsigned long long type" << std::endl;
		return -1;
	}
}

void FaraySequence(int maximum, std::vector<fraction>& pairs, fraction low, fraction high, bool new_pair)
{
	//this function returns a vector of fraction types holding all of the coprime pairs with integers less than maximum
	if (new_pair) pairs.push_back({ low });

	//this portion advances to the next pair in the Faray sequence
	if (low.denominator + high.denominator <= maximum)
	{
		FaraySequence(maximum, pairs, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true);
		FaraySequence(maximum, pairs, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false);
	}
}

void orderedFaraySequence(int maximum, std::vector<fraction>& pairs, fraction low, fraction high, bool new_pair, int insertIndex)
{
	//Same as the above function but gives the fractions in appropriate order, slightly slower because of the insert function.
	if (new_pair) pairs.insert(pairs.begin() + insertIndex, { low });

	//this portion advances to the next pair in the Faray sequence
	if (low.denominator + high.denominator <= maximum)
	{
		orderedFaraySequence(maximum, pairs, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true, insertIndex + 1);
		orderedFaraySequence(maximum, pairs, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false, insertIndex);
	}
}

void Farey_Iterative(int n)
{
	//This method prints out the Farey sequence with maximum value n, in order.
	int a = 0, b = 1, c = 1, d = n;

	while (c <= n)
	{
		//uncomment below line to see the sequence
		//std::cout << a << "/" << b << std::endl;

		int k = (n + b) / d;
		int next_c = k * c - a;
		int next_d = k * d - b;

		a = c;
		b = d;
		c = next_c;
		d = next_d;
	}
}

std::vector<int> baseConversion(int n, int b)
{
	//Takes the number n which is in base 10 and converts it to be in base b.
	//A vector is used to store the new digits because not all bases higher than 10 have 
	//a convenient way to represent their digits (i.e. in hexadecimal numbers higher than 
	//9 are represented with the letters A-F).
	std::vector<int> n_b;
	while (n >= b)
	{
		n_b.push_back(n % b);
		n /= b;
	}
	n_b.push_back(n);
	
	return n_b;
}

long long ChineseRemainderTheorem(long long n, long long mod, std::vector<std::pair<long long, long long> >* equations)
{
	//The Chinese Remainder Theorem is used to solve n % mod = ? by means of breaking it into smaller problems. Basically,
	//the value of mod is broken down into factors that are coprime with eachother (for example, if mod was 100 then it 
	//could be broken down into factors of 4 (2 * 2) and 25 (5 * 5)) and we solve the smaller questions of n % factor_1 = a,
	//n % factor_2  = b ... The Chinese Remainder Theorem states that once we get all of these smaller answers, there will be 
	//a single value, X, where:
	//X = a (mod factor_1)
	//X = b (mod factor_2)
	//X = c (mod factor_3)
	//and so on. This value for x is the original answer to the question, i.e. n % mod = X. There are a number of reasons
	//as to why n % mod couldn't be calculated directly and this function should be used. A good example would be if 
	//"mod" isn't a prime number and we're using an equation that only works modulo prime numbers (Lucas' Theory as an example).

	//I've designed this function to work in two different ways, we can either feed it a value for n and mod, and let it 
	//calculate the conruence equations on it's own, or, we can just enter the congruence equations directly if they've
	//been calculated elsewhere.
	bool deleteEquations = false;
	if (equations == nullptr)
	{
		//if the "equations" variable is a nullptr it means that we need to come up with the congruence equations
		//here. To do this, we break down mod into it's prime factorization. Each prime factor will get its
		//own equation to ensure that all of the factors are coprime with eachother.
		
		equations = new std::vector<std::pair<long long, long long> >;
		deleteEquations = true; //this flag tells the function to delete the equations vector created in the line above before returning

		std::vector<long long> p_factors = PrimeFactors(mod);
		long long currentFactor = 1, currentPrime = p_factors[0];

		for (int i = 1; i < p_factors.size(); i++)
		{
			if (p_factors[i] != currentPrime)
			{
				equations->push_back({ n % currentFactor, currentFactor });
				currentFactor = 1;
				currentPrime = p_factors[i];
			}
			else currentFactor *= currentPrime;
		}
		equations->push_back({ n % currentFactor, currentFactor }); //make sure to add the final factor after loop ends
	}

	//Check to see if the congruence values are all equal, in this case the value of X would be the same as these
	//congruences (i.e. X == 3 (mod 4), X == 3 (mod 25) would indicate that X == 3 (mod 100)). This check is here
	//to speed things up when the modulo numbers are much larger than the congruency numbers.
	bool equalityCheck = true;
	for (int i = 1; i < equations->size(); i++)
	{
		if (equations->at(i).first != equations->at(i - 1).first)
		{
			equalityCheck = false;
			break;
		}
	}
	if (equalityCheck) return equations->at(0).first;

	//The equations in the "equations" vector have the form {n % factor_i, factor_i}. In essence, we need to solve
	//X = equations[i].first % equation[i].second for each value in the "equations" vector.
	long long m = 1; //reconstruct the original modulus
	for (int i = 0; i < equations->size(); i++) m *= equations->at(i).second;

	long long result = 0;
	/*for (int i = 0; i < equations->size(); i++)
	{
		long long x, y;
		extendedEuclideanGCD(m / equations->at(i).second, equations->at(i).second, &x, &y);
		result += (equations->at(i).first * x * m / equations->at(i).second);
	}*/
	for (int i = 0; i < equations->size(); i++)
	{
		long long withoutI = m / equations->at(i).second;
		result = (result + ModMult(ModMult(equations->at(i).first, ModularMultiplicativeInverse(withoutI, equations->at(i).second, 0), m), withoutI, m)) % m;
	}

	return ((result % m) + m) % m; //the extra mods are to handle negative results
}


//Partition Functions
std::vector<long long> polynomial_multiply(std::vector<long long>& p1, std::vector<long long>& p2, int cut_off)
{
	std::vector<long long> p3;
	int stop = p1.size() - 1 + p2.size() - 1;
	if (cut_off) stop = cut_off;
	for (int i = 0; i <= stop; i++) p3.push_back(0);

	for (int i = 0; i < p1.size(); i++)
	{
		if (i > stop)
		{
			//std::cout << "broke" << std::endl;
			break;
		}
		if (!p1[i]) continue;

		for (int j = 0; j < p2.size(); j++)
		{
			if (i + j > stop)
			{
				//std::cout << "woke" << std::endl;
				break;
			}
			if (!p2[j]) continue;
			p3[i + j] += p1[i] * p2[j];
		}
	}

	return p3;
}
std::vector<int> polynomial_mod_multiply(std::vector<int>& p1, std::vector<int>& p2, int mod, int cut_off)
{
	std::vector<int> p3;
	int stop = p1.size() - 1 + p2.size() - 1;
	if (cut_off) stop = cut_off;
	for (int i = 0; i <= stop; i++) p3.push_back(0);

	for (int i = 0; i < p1.size(); i++)
	{
		if (i > stop)
		{
			//std::cout << "broke" << std::endl;
			break;
		}
		if (!p1[i]) continue;

		for (int j = 0; j < p2.size(); j++)
		{
			if (i + j > stop)
			{
				//std::cout << "woke" << std::endl;
				break;
			}
			if (!p2[j]) continue;
			p3[i + j] += ModMult(p1[i], p2[j], mod);
		}
	}

	return p3;
}
long long polcoeff(std::vector<std::vector<long long> >& polynomials, int order)
{
	//this function returns the coefficient of order "order" of the generating function
	//expressed by "polynomials". Since we only care about finding the coefficient at "order"
	//code is added to stop any multiplication of higher orders to speed things up

	if (polynomials.size() == 0) return 1; //polynomial vector has no size so doing anything below will result in access violation

	std::vector<long long> coefficients;
	if (order < polynomials[0].size())
	{
		for (int i = 0; i <= order; i++) coefficients.push_back(polynomials[0][i]); //initialize coefficient vector to the necessary order
	}
	else coefficients = polynomials[0];

	for (int i = 1; i < polynomials.size(); i++) coefficients = polynomial_multiply(coefficients, polynomials[i], order);

	return coefficients[order];
}
int polcoeff(std::vector<std::vector<int> >& polynomials, int order, int mod)
{
	//this function returns the coefficient of order "order" of the generating function
	//expressed by "polynomials". Since we only care about finding the coefficient at "order"
	//code is added to stop any multiplication of higher orders to speed things up

	if (polynomials.size() == 0) return 1; //polynomial vector has no size so doing anything below will result in access violation

	std::vector<int> coefficients;
	if (order < polynomials[0].size())
	{
		for (int i = 0; i <= order; i++) coefficients.push_back(polynomials[0][i]); //initialize coefficient vector to the necessary order
	}
	else coefficients = polynomials[0];

	for (int i = 1; i < polynomials.size(); i++) coefficients = polynomial_mod_multiply(coefficients, polynomials[i], mod, order);

	return coefficients[order];
}
void getPartitions(int n, std::vector<std::vector<std::vector<int> > >& all_partitions, int maxNumber, int maxLength,
	int currentLength, int currentValue, std::vector<int>* currentPartition, bool recursive)
{
	//This recursive function returns a vector with all of the partitions of n where the maximum number allowed in
	//each partition is maxNumber and the maximum length allowed for each partition is denoted by maxLength.
	//If maxNumber and maxLength are left blank then this function will just return all of the possible
	//partitions.
	if (!recursive)
	{
		//The non-recursive part of the function is just used for setting everything up
		if (maxNumber <= 0 || maxNumber > n) maxNumber = n;
		if (maxLength <= 0 || maxLength > n) maxLength = n;

		//std::vector<std::vector<std::vector<int> > > all_partitions;
		std::vector<std::vector<int> > partition_length;

		for (int i = maxNumber; i > 0; i--)
		{
			std::vector<int> partition = { i };
			partition_length.push_back(partition);
		}

		//we use the 0th element of the all_partitions vector to build our other partitions. This element
		//gets deleted after the other partitions have been created.
		all_partitions.push_back(partition_length); //represents partitions of length 0
		partition_length.clear();

		//We start with an empty vector for all of the other lengths
		for (int i = 1; i <= maxLength; i++) all_partitions.push_back(partition_length);

		//Then we recursively build all partitions from the starting vectors in the 0th element
		for (int i = 0; i < all_partitions[0].size(); i++) getPartitions(n, all_partitions, maxNumber, maxLength, 1, all_partitions[0][i][0], &all_partitions[0][i], true);
		all_partitions[0].clear();
	}
	else
	{
		//the recursive part of the function, this section actually builds out all of our partitions. Start of with the base cases, of which 
		//there are two. Either we're one digit away from our max length, our we're not but have reached n
		if (currentValue == n)
		{
			//If the value of our current partition equals n then there's no need to continue, add the partition
			//and go back up a level
			all_partitions[currentLength].push_back(*currentPartition);
			return;
		}
		else if (currentLength == (maxLength - 1))
		{
			//there's only one number left that we can add, add it (if it's within our limits) and then return
			if (n - currentValue <= currentPartition->back())
			{
				currentPartition->push_back(n - currentValue);
				all_partitions[currentLength + 1].push_back(*currentPartition);
				currentPartition->pop_back();
			}
			return;
		}
		else if (currentLength == maxLength) return; //if we've used all allowed digits and haven't reached n yet, return

		//otherwise add all numbers possible via a loop. We can only add numbers that are less than or equal to
		//the number at the end of the current partition to maintain distinctiveness. For a little more efficiency
		//we can see what the maximum number possible is and start from there (for example, if we were trying to 
		//make partitions of 12 and our current partition was [11], then it wouldn't makes sense to try adding
		//any of the numbers 11 - 2 onto the back as 1 is the largest option here.
		int start_point = (n - currentValue < currentPartition->back()) ? n - currentValue : currentPartition->back();
		for (int i = start_point; i > 0; i--)
		{
			currentPartition->push_back(i); //add i to the back of the current partition
			getPartitions(n, all_partitions, maxNumber, maxLength, currentLength + 1, currentValue + i, currentPartition, true);
			currentPartition->pop_back(); //remove i from the back of the current partition before moving on
		}
	}
}
long long permutationsOfPartitions(int n, int l, int maximum, int minimum, bool quick_choose)
{
	//returns the number of distinct permutations for the partitions of n that have a length of l, 
	//where the maximum integer allowed is maximum and the least integer allowed is minimum. Maximum must be
	//greater than or equal to minimum, and less than or equal to n while minimum must be greater than or
	//equal to 1 and less than or equal to maximum.
	if (maximum == 0) maximum = n;
	else if (maximum > n) maximum = n;

	if (minimum == 0) minimum = 1;
	else if (minimum < 1) minimum = 1;
	else if (minimum > maximum) minimum = maximum;

	//This function utilizes generating functions to find the partitions.
	int target = n - minimum * l, primary = maximum - minimum + 1, flip = -1;
	int secondaryDenominator = target + primary, secondaryNumerator = l + secondaryDenominator - 1;
	long long answer = 0;

	//we get an equation of the form: [x^target]: (1 - x^primary)^l * (1 - x)^-l
	//for which we solve using binomial expansion
	for (int i = 0; i <= target / primary; i++)
	{
		if(quick_choose) answer += recursiveChoose(l, i) * recursiveChoose((secondaryNumerator -= primary), (secondaryDenominator -= primary)) * (flip *= -1);
		else answer += choose(l, i) * choose((secondaryNumerator -= primary), (secondaryDenominator -= primary)) * (flip *= -1);
	}
		
	return answer;
}

std::vector<int> nonRotatedPermutations(std::vector<int> numbers, int max_length)
{
	//numbers is a sorted set containing the digits that we're allowed to use
	int w = numbers[0], x = numbers[0], w_length = 1, mult = 1;
	std::vector<int> permutations;

	//keep adding w onto itself until we reach max_length
	while (true)
	{
		permutations.push_back(w);
		mult *= 10; //mult should point to digit right in front of MSB of w
		for (int i = 0; i < (max_length - w_length) / w_length; i++)
		{
			x += mult * w;
			mult *= MyPow(10, w_length);
		}

		if (max_length % w_length)
		{
			//add remaining digits if necessary
			w %= MyPow(10, max_length % w_length);
			w_length = max_length % w_length;
			x += mult * w;
			mult *= MyPow(10, w_length);
		}
		w_length = max_length;

		mult /= 10; //mult now points to the MSB of x

		//keep removing the most significant digit of x if it's equal to
		//the maximal number in the 'numbers' set
		int digits_removed = 0;
		while (x / mult == numbers.back())
		{
			x %= mult;
			mult /= 10;
			digits_removed++;
			w_length--;
			if (mult == 0) return permutations; //if we cancel all the digits out then we've found all permutations
		}

		//after removing the necessary digits, increment the most significant 
		//digit to the next digit chronologically in the 'numbers' set
		int first_digit = x / mult, next_digit;
		for (int i = 0; i < numbers.size() - 1; i++)
		{
			if (numbers[i] == first_digit)
			{
				next_digit = numbers[i + 1];
				break;
			}
		}

		w = x + mult * (next_digit - first_digit);
		x = w;
	}
}

template <typename T>
T BinomialMod(T n, T k, T m)
{
	//returns choose(n, k) % m

	//If m is a prime number this task is much easier so we call a different function
	//to do so
	if (m > n) return BinomialModLargePrime(n, k, m);

	//If m isn't prime then we use the generalized version of Lucas' Theorem.
	//The steps boil down to:
	//1. Find prime factors (and multiplicities) of m (i.e. m = 100 = 2^2 * 5^2)
	//2. Use generalized Lucas' Theorem to find all sub problems for each choose(n, k) mod p_i^e
	//3. Solve sub problems choose(n, k) using Fermat's little theorem
	//4. Use Chinese Remainder Theorem to combine the results

}

void modularMultiplicativeInverseRange(int n, long long mod, long long* inverses)
{
	//This method creates an array that holds the values [1/1, 1/2, 1/3, ...., 1/n] (% mod) using the Montgomery
	//batch inversion technique. The array is created using only a single division and 3n multiplications. There
	//are three times as many operations required for this function as there are to just take all of the
	//inverses individually, but the operations are much quicker than taking the inverses.

	inverses[0] = 1; inverses[1] = 1; //in case they aren't already, set 0 and 1 inverses to 1
	if (mod >= 3037000499 || n >= 3037000499)
	{
		//If either n or mod are greater than sqrt(2^{63} - 1) then it's possible to overflow
		//a signed long integer even though we're using modulus division. In these cases
		//we call a different version of this function which uses the ModMult() function
		//instead of normal multiplication to safely multiply without overflow. This is 
		//much slower than standard multiplication so we only use this method if it's 
		//necessary. Note: 3,037,000,499 = floor(sqrt(2^{63} - 1))
		modularMultiplicativeInverseRangeOverflowSafe(n, mod, inverses);
	}

	//TODO: The below commented out section was to split out cases where mod is a small number
	//so that I could modular divide the necessary values of i before multiplying them. After
	//doing some separate testing though, I couldn't find any instances where (a * (b % m)) % m
	//gave a different answer than, (a * b) % m. This implies that only one of the numbers in the
	//multiplication needs to be modulus divided and not both of them. I feel like this isn't correct
	//because everything I've read online says that for modular multiplication,
	//(a * b) % m == ((a % m) * (b % m)) % m. If things start acting fishy with this function 
	//then this will be my first smoking gun.
	
	//if (mod <= n)
	//{
	//	//if mod is a small number, like 7 for instance, we'll need to mod values before even
	//	//multiplying them. This is because (a * b) % m == ((a % m) * (b % m)) % m,
	//	//but the following isn't necessarily true (a * b) % m == (a * (b % m)) % m. When
	//	//a is smaller than m, then a % m is just a which is why we can skip the extra
	//	//modulus division.
	//	
	//	//compute successive modular inverses for values of i % mod
	//	for (long long i = 2; i < mod; i++) inverses[i] = (i * inverses[i - 1]) % mod;
	//	for (long long i = mod; i <= n; i++) inverses[i] = ((i % mod) * inverses[i - 1]) % mod;
	//}
	//else
	//{
	//	//compute successive modular inverses for values of i % mod
	//	for (long long i = 2; i <= n; i++) inverses[i] = (i * inverses[i - 1]) % mod;
	//}

	for (long long i = 2; i <= n; i++) inverses[i] = (i * inverses[i - 1]) % mod;
	inverses[n] = ModularMultiplicativeInverse(inverses[n], mod); //the sole division takes place on the last element

	for (long long i = n - 1; i >= 0; i--)
	{
		long long temp = inverses[i];
		inverses[i] = (inverses[i + 1] * (i + 1)) % mod;
		inverses[i + 1] = (inverses[i + 1] * temp) % mod;
	}
}
void modularMultiplicativeInverseRangeOverflowSafe(int n, long long mod, long long* inverses)
{
	//The same thing as the modularMultiplicativeInverseRange() function, however, checks to 
	//see if overflow of unsigned long integers will happen before multiplication so that it 
	//can safely do all multiplications.
	for (long long i = 2; i <= n; i++) inverses[i] = ModMult(i, inverses[i - 1], mod);
	inverses[n] = ModularMultiplicativeInverse(inverses[n], mod); //the sole division takes place on the last element

	for (long long i = n - 1; i >= 0; i--)
	{
		long long temp = inverses[i];
		inverses[i] = ModMult(inverses[i + 1], i + 1, mod);
		inverses[i + 1] = ModMult(inverses[i + 1], temp, mod);
	}
}

long long MyPow(long long x, unsigned long long p)
{
	if (p == 0) return 1;
	if (p == 1) return x;

	//T tmp = MyPow(x, p / 2)
	long long tmp = MyPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

long long primeFactorialInstances(long long k, long long p)
{
	//returns the number of times prime p appears in k!. For example
	//in 5! there are 3 instances of the prime 2 appearing:
	//5 * 4 * 3 * 2 * 1 --> = 5 * 3 * 2 * 2 * 2 * 1
	long long occurences = 0, u = p, t = k;
	while (u <= t)
	{
		occurences += t / u;
		u *= p;
	}
	return occurences;
}
long long binomialMod(long long n, long long m, long long mod, int isPrime)
{
	//This function returns choose(n, m) % mod. Depending on the value of mod there are a few
	//different ways to calculate this value. If mod is a prime number then the calculation 
	//is fairly straightforward. If mod isn't a prime number, but is only composed of 
	//primes raised to the first power (i.e. 70 = 2^1 * 5^1 * 7^1) we can calculate the answer
	//with a combination of Lucas' Theorem and the Chinese Remainder Theorem. If mod has
	//any primes raised to a power then we need to use a Generalized version of Lucas' Theorem.

	//Before checking anything, look to see if m > n, if so we just return 0
	if (m > n) return 0;

	//if we're not sure whether or not mod is a prime number we can just call a prime number
	//test on it. The number 2 is used to designate this option
	bool prime = false;
	if (isPrime == 2) prime = primeNumberTest(mod);
	else prime = isPrime;

	//If mod is a prime number then we can just solve the binomial % mod using Fermat's Little
	//Theorem
	if (prime) return binomialModPrime(n, m, mod);
	
	//If mod isn't a prime number then we need to break the problem down into sub problems.
	//We break mod down into it's prime factorization and then solve choose(n, m) % p^q
	//for each prime in the factorization (where q is the power of each prime).
	//We then use the Chinese Remainder Theorem on these different sub answers to get 
	//the final answer
	std::vector<long long> p_factors = PrimeFactors(mod);
	std::vector<std::pair<long long, long long> > chineseRemainderCongruences;

	int i = 0;
	long long current_prime = p_factors[0], prime_count = 0;
	while (i < p_factors.size())
	{
		if (p_factors[i] != current_prime)
		{
			//We found a different prime so we're ready to do our calculation for the existing prime.
			//If there's only one instance of the prime in question we use Lucas' Theorem to calculate
			//the binomial, otherwise we need to used the generalized version of Lucas' Theorem.
			if (prime_count == 1) chineseRemainderCongruences.push_back({ lucasTheorem(n, m, current_prime), current_prime });
			else chineseRemainderCongruences.push_back({ generalizedLucasTheorem(n, m, current_prime, prime_count), MyPow(current_prime, prime_count)});

			current_prime = p_factors[i];
			prime_count = 1;
		}
		else prime_count++;
		
		i++;
	}

	//Use the appropriate version of Lucas' Theorem one last time for the final prime in the array
	if (prime_count == 1) chineseRemainderCongruences.push_back({ lucasTheorem(n, m, current_prime), current_prime });
	else chineseRemainderCongruences.push_back({ generalizedLucasTheorem(n, m, current_prime, prime_count), MyPow(current_prime, prime_count) });

	//Finally, use the Chinese Remainder Theorem to combine all of the different partial
	//answers obtained above.
	return ChineseRemainderTheorem(0, 0, &chineseRemainderCongruences);
}
long long lucasTheorem(long long n, long long m, long long p)
{
	//Calculates choose(n, m) % p. This function only works if p is a prime number.
	//This function works by converting both n and m from base 10 into base p, i.e.:
	//n = n_0 + n_1*p + n_2 * p^2...
	//m = m_0 + m_1*p + m_2 * p^2...
	//Lucas' Theorem states that choose(n, m) is congruent to choose(n_i, m_i) mod p
	//for all of the n_i and m_i found when converting the numbers to base p. After
	//the main binomial is broken up into smaller binomials, these are each solved
	//using Fermat's Little Theorem

	//First, convert n and m to base p format. Since n is bigger then m, add any trailing
	//0's that are necessary to make m_p the same length as n_p
	std::vector<int> n_p = baseConversion(n, p), m_p = baseConversion(m, p);
	for (int i = 0; i < n_p.size() - m_p.size(); i++) m_p.push_back(0);

	long long answer = 1;
	for (int i = 0; i < n_p.size(); i++) answer = ModMult(answer, binomialModPrime((long long)n_p[i], (long long)m_p[i], p), p);

	return answer;
}
long long binomialModPrime(long long n, long long m, long long p, long long* factorials, long long* inverse_factorials)
{
	//Calculates choose(n, m) % p. This function only works if p is a prime and m < k.
	//This function can be called with a precalculated array of factorials and inverse factorials % mod
	//for quicker computation

	//In cases where m >= k, the advanced version of the function gets called
	if (m >= p) return binomialModePrimeAdvanced(n, m, p);

	if (factorials == nullptr)
	{
		long long numerator = 1, denominator = 1;
		for (long long i = n; i > n - m; i--) numerator = ModMult(numerator, i, p); //possible to overflow 64-bit so use ModMult()
		for (long long i = 2; i <= m; i++) denominator = ModMult(denominator, i, p); //possible to overflow 64-bit so use ModMult()

		return ModMult(numerator, ModularMultiplicativeInverse(denominator, p), p);
	}
	else return ModMult(factorials[n], ModMult(inverse_factorials[m], inverse_factorials[n - m], p), p);
}
long long binomialModePrimeAdvanced(long long n, long long m, long long p)
{
	//Calculates choose(n, m) % mod. This function only works if mod is a prime.

	//The degree of mod in the numerator must be equal to that of the denominator,
	//i.e. we need to be able to cancel out all instances of mod from choose(n, m).
	//For example, choose(5, 2) can be rewritten as (5 * 4 * 3 * 2 * 1)/((3 * 2 * 1)(2 * 1))
	//which, after cancelling out all prime numbers can in turn be written as (5 * 2).
	//If we were trying to find choose(5, 2) % 2 or choose(5, 2) % 5, the answer would just
	//be 0 as the binomial is divisible by mod.
	long long numerator_degree = primeFactorialInstances(n, p) - primeFactorialInstances(n - m, p);
	long long denominator_degree = primeFactorialInstances(m, p);
	if (numerator_degree > denominator_degree) return 0;

	long long numerator = 1, denominator = 1;
	for (long long i = n; i > n - m; i--)
	{
		long long cur = i;
		while (!(cur % p)) cur /= p;
		numerator = ModMult(numerator, cur, p); //possible to overflow 64-bit so use ModMult()
	}
	for (long long i = 2; i <= m; i++)
	{
		long long cur = i;
		while (!(cur % p)) cur /= p;
		denominator = ModMult(denominator, cur, p); //possible to overflow 64-bit so use ModMult()
	}

	return ModMult(numerator, ModularMultiplicativeInverse(denominator, p), p);
}
long long generalizedLucasTheorem(long long n, long long m, long long p, long long q)
{
	//this function returns choose(n, m) % p^q. It's based off of the following paper: https://web.archive.org/web/20170202003812/http://www.dms.umontreal.ca/~andrew/PDF/BinCoeff.pdf
	//Note: p^q must be able to fit into a long integer for this function to work

	//First calculate the arrays for n_j, m_j, r_j, N_j, M_j, R_j and e_j
	long long r = n - m, j = 0, mod = MyPow(p, q);
	bool mBiggerThanR = (m > r ? true : false); //needed to calculate e vector later

	//TEMP: See if this works, different calculation of e_0. All it does is 
	//look at the total number of p that exists in choose(n, m) after all
	//cancellation of primes has taken place. For example, in choose(8, 3)
	//there should be three 2's because choose(8, 3) = 8! / (5!*3!) = 
	//(8*7*6*5*4*3*2*1)/(5*4*3*2*1*3*2*1) = (8*7*6)/(3*2*1) = 8 * 7 = 7 * 2 * 2 * 2.
	//After all primes have been cancelled there are three two's remaining, 
	//so the value of e_0 should be 3.
	long long e_0 = 0, mult = p;
	while (mult <= n)
	{
		e_0 += n / mult;
		e_0 -= m / mult;
		e_0 -= r / mult;
		mult *= p;
	}

	std::vector<long long> n_j, m_j, r_j, N_j, M_j, R_j, e_j;

	//create vectors for n_j, m_j and r_j
	while (n >= p)
	{
		n_j.push_back(n % p);
		n /= p;
	}
	n_j.push_back(n);
	j = n_j.size();

	while (m >= p)
	{
		m_j.push_back(m % p);
		m /= p;
	}
	m_j.push_back(m);
	while (m_j.size() < n_j.size()) m_j.push_back(0);

	while (r >= p)
	{
		r_j.push_back(r % p);
		r /= p;
	}
	r_j.push_back(r);
	while (r_j.size() < n_j.size()) r_j.push_back(0);

	//Use the n_j, m_j and r_j arrays to create N_j, M_j and R_j arrays
	for (int i = 0; i < j; i++)
	{
		long long N_i = 0, M_i = 0, R_i = 0, pow = 1;
		for (int k = 0; k < q; k++)
		{
			if (k + i >= j) break;
			N_i += n_j[k + i] * pow;
			M_i += m_j[k + i] * pow;
			R_i += r_j[k + i] * pow;
			pow *= p;
		}
		N_j.push_back(N_i);
		M_j.push_back(M_i);
		R_j.push_back(R_i);
	}

	//Work backwards through the arrays to calculate e_j. The way that the above paper calculates 
	//e_j isn't entirely correct, instead of just comparing n to m, n should be compared to the 
	//larger of m and r.
	long long e = 0;
	for (int i = j - 1; i >= 0; i--)
	{
		if ((m_j[i] > n_j[i]) || (r_j[i] > n_j[i])) e_j.insert(e_j.begin(), ++e);
		else e_j.insert(e_j.begin(), e);
	}

	//With all of the arrays calculated we can calculate the answer
	long long numerator = 1, denominator = 1;

	//TODO: This is just a brute force calculation of all the (N!)_p terms. At some point
	//I should try and incorporate Theorems 2 and 3 from the above paper to calculate these
	//values more quickly. Implementing these theorems allows for the calculation of the (n!)_p
	//terms while only needed to go up to q*p
	//The numerator is composed of all the (N!)_p terms. The denominator is compose of both the 
	//(M!)_p and (R!)_p terms. The subscript p in this case means that we take the factorial of N, M and R
	//but omit any terms that are divisible by p.
	for (long long i = 0; i < j; i++)
	{
		for (long long k = 1; k <= N_j[i]; k++)
		{
			if (k % p == 0) continue;
			numerator = ModMult(numerator, k, mod);
		}
		for (long long k = 1; k <= M_j[i]; k++)
		{
			if (k % p == 0) continue;
			denominator = ModMult(denominator, k, mod);
		}
		for (long long k = 1; k <= R_j[i]; k++)
		{
			if (k % p == 0) continue;
			denominator = ModMult(denominator, k, mod);
		}
	}

	//Multiply the numerator by the modular inverse of the denominator, since all terms divisible
	//by p have been omitted, p^q will be coprime with both the numerator and denominator
	numerator = ModMult(numerator, ModularMultiplicativeInverse(denominator, mod, 0), mod);

	//TODO: The calculation of the e vector isn't correct. It seems rare that it would
	//actually be an issue, but there's a chance that the e_j[q-1] term could be off
	
	//Multiply the numerator by the +/-1 term from the equation
	if (p > 2 || q < 3)
	{
		//if p = 2 and q >= 3 the term is +1 so we ignore it
		if ((j >= q) && e_j[q - 1] % 2 == 1)
		{
			numerator *= -1;
			numerator = ((numerator % mod) + mod) % mod;
		}
	}

	//finally, multiply the numerator value by p^e_0, we then have our answer
	if (e_0) numerator = ModMult(numerator, MyPow(p, e_0), mod);

	return numerator;
}

//Other functions
std::vector<long long> mintageCount(long long total_mintage, std::vector<long long> highest_value)
{
	//makes a rough guess for mintage of coin varaints based on their red book values
	float x = 1, base = highest_value[0];
	for (int i = 1; i < highest_value.size(); i++) x += (base / highest_value[i]);
	x = total_mintage / x;
	for (int i = 0; i < highest_value.size(); i++) highest_value[i] = x * (base / highest_value[i]);

	return highest_value;
}

BigNum::BigNum()
{
	//default constructor for BigNum
	digits = 0;
	number_string = "";
}

BigNum::BigNum(std::string num)
{
	//Takes a string of numbers and converts it into a BigNum type
	digits = num.length();
	for (int i = digits - 1; i >= 0; i--) number.push_back(char_to_int(num[i]));
	number_string = num;
}

BigNum::BigNum(int zeros)
{
	//creates a BigNum type of all zeros with the specified length
	digits = zeros;
	for (int i = 0; i < digits; i++) number.push_back(0);
	UpdateNumberString();
}

std::vector<int> BigNum::GetVec()
{
	return number;
}

int BigNum::length()
{
	return digits;
}

void BigNum::Add(BigNum num)
{
	std::vector<int> new_number = num.GetVec();

	if (new_number.size() > number.size())
	{
		int yo = new_number.size() - number.size();
		for (int i = 0; i < yo; i++) number.push_back(0);
	}
	else if (new_number.size() < number.size())
	{
		int yo = number.size() - new_number.size();
		for (int i = 0; i < yo; i++) new_number.push_back(0);
	}

	int carry = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int digit = number[i] + new_number[i] + carry;
		if (digit >= 10)
		{
			carry = digit / 10;
			number[i] = digit % 10;
		}
		else
		{
			carry = 0;
			number[i] = digit;
		}
	}

	if (carry) number.push_back(carry);
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Add(std::vector<int> num)
{
	//takes a vector that's already in the proper form and adds it to a BigNum
	std::vector<int> new_number = num;

	if (new_number.size() > number.size())
	{
		int yo = new_number.size() - number.size();
		for (int i = 0; i < yo; i++) number.push_back(0);
	}
	else if (new_number.size() < number.size())
	{
		int yo = number.size() - new_number.size();
		for (int i = 0; i < yo; i++) new_number.push_back(0);
	}

	int carry = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int digit = number[i] + new_number[i] + carry;
		if (digit >= 10)
		{
			carry = digit / 10;
			number[i] = digit % 10;
		}
		else
		{
			carry = 0;
			number[i] = digit;
		}
	}

	if (carry) number.push_back(carry);
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Add(int num)
{
	std::vector<int> new_number = ConvertToVector(num);

	if (new_number.size() > number.size())
	{
		int yo = new_number.size() - number.size();
		for (int i = 0; i < yo; i++) number.push_back(0);
	}
	else if (new_number.size() < number.size())
	{
		int yo = number.size() - new_number.size();
		for (int i = 0; i < yo; i++) new_number.push_back(0);
	}

	int carry = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int digit = number[i] + new_number[i] + carry;
		if (digit >= 10)
		{
			carry = digit / 10;
			number[i] = digit % 10;
		}
		else
		{
			carry = 0;
			number[i] = digit;
		}
	}

	if (carry) number.push_back(carry);
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Add(long long num)
{
	std::vector<int> new_number = ConvertToVector(num);

	if (new_number.size() > number.size())
	{
		int yo = new_number.size() - number.size();
		for (int i = 0; i < yo; i++)
		{
			number.push_back(0);
		}
	}
	else if (new_number.size() < number.size())
	{
		int yo = number.size() - new_number.size();
		for (int i = 0; i < yo; i++)
		{
			std::cout << i << std::endl;
			new_number.push_back(0);
		}
	}

	int carry = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int digit = number[i] + new_number[i] + carry;
		if (digit >= 10)
		{
			carry = digit / 10;
			number[i] = digit % 10;
		}
		else
		{
			carry = 0;
			number[i] = digit;
		}
	}

	if (carry) number.push_back(carry);
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Multiply(BigNum num)
{
	std::vector<int> new_number = num.GetVec();
	int size;
	if (number.size() > new_number.size()) size = number.size(); //create a temporary BigNum of all zeros that's equal in length to the longer of the two numbers being multiplied
	else size = new_number.size();

	BigNum temp(size);
	//int carry = 0;
	int num_shift = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int new_num_shift = 0;
		for (int j = 0; j < new_number.size(); j++)
		{
			std::vector<int> temptemp = ConvertToVector(number[i] * new_number[j]);
			LeftShift(temptemp, new_num_shift + num_shift);
			temp.Add(temptemp);
			new_num_shift++;
		}
		num_shift++;
	}

	number = temp.GetVec();
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Multiply(std::vector<int> num)
{
	std::vector<int> new_number = num;
	int size;
	if (number.size() > new_number.size()) size = number.size(); //create a temporary BigNum of all zeros that's equal in length to the longer of the two numbers being multiplied
	else size = new_number.size();

	BigNum temp(size);
	//int carry = 0;
	int num_shift = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int new_num_shift = 0;
		for (int j = 0; j < new_number.size(); j++)
		{
			std::vector<int> temptemp = ConvertToVector(number[i] * new_number[j]);
			LeftShift(temptemp, new_num_shift + num_shift);
			temp.Add(temptemp);
			new_num_shift++;
		}
		num_shift++;
	}

	number = temp.GetVec();
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Multiply(int num)
{
	//always multiply with num on top and BigNum on the bottom
	std::vector<int> new_number = ConvertToVector(num);
	int size;
	if (number.size() > new_number.size()) size = number.size(); //create a temporary BigNum of all zeros that's equal in length to the longer of the two numbers being multiplied
	else size = new_number.size();

	BigNum temp(size);
	//int carry = 0;
	int num_shift = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int new_num_shift = 0;
		for (int j = 0; j < new_number.size(); j++)
		{
			std::vector<int> temptemp = ConvertToVector(number[i] * new_number[j]);
			LeftShift(temptemp, new_num_shift + num_shift);
			temp.Add(temptemp);
			new_num_shift++;
		}
		num_shift++;
	}

	number = temp.GetVec();
	digits = number.size();
	UpdateNumberString();
}

void BigNum::Multiply(long long num)
{
	std::vector<int> new_number = ConvertToVector(num);
	int size;
	if (number.size() > new_number.size()) size = number.size(); //create a temporary BigNum of all zeros that's equal in length to the longer of the two numbers being multiplied
	else size = new_number.size();

	BigNum temp(size);
	//int carry = 0;
	int num_shift = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int new_num_shift = 0;
		for (int j = 0; j < new_number.size(); j++)
		{
			std::vector<int> temptemp = ConvertToVector(number[i] * new_number[j]);
			LeftShift(temptemp, new_num_shift + num_shift);
			temp.Add(temptemp);
			new_num_shift++;
		}
		num_shift++;
	}

	number = temp.GetVec();
	digits = number.size();
	UpdateNumberString();
}

int BigNum::GetInt()
{
	std::vector<int> maximum = ConvertToVector(INT_MAX);
	TruncateZeros();
	int ans = 0;

	if (number.size() > maximum.size())
	{
		std::cout << "Can't convert BigNum to integer, too large." << std::endl;
		return -1; //number is too big to fit into an integer
	}
	else if (number.size() == maximum.size())
	{
		int smaller = 0;
		for (int i = number.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			if (number[i] > maximum[i])
				if (!smaller)
				{
					std::cout << "Can't convert BigNum to integer, too large." << std::endl;
					return -1; //number is too big to fit into an integer
				}

			if (number[i] < maximum[i]) smaller = 1;
			ans += number[i];
		}
		return ans;
	}
	else
	{
		for (int i = number.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			ans += number[i];
		}
		return ans;
	}
}

int BigNum::GetInt(std::vector<int> nums)
{
	std::vector<int> maximum = ConvertToVector(INT_MAX);
	int ans = 0;

	if (nums.size() > maximum.size())
	{
		std::cout << "Can't convert BigNum to integer, too large." << std::endl;
		return -1; //number is too big to fit into an integer
	}
	else if (nums.size() == maximum.size())
	{
		int smaller = 0;
		for (int i = nums.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			if (nums[i] > maximum[i])
				if (!smaller)
				{
					std::cout << "Can't convert BigNum to integer, too large." << std::endl;
					return -1; //number is too big to fit into an integer
				}

			if (nums[i] < maximum[i]) smaller = 1;
			ans += nums[i];
		}
		return ans;
	}
	else
	{
		for (int i = nums.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			ans += nums[i];
		}
		return ans;
	}
}

long long BigNum::GetLongInt()
{
	std::vector<int> maximum = ConvertToVector(LLONG_MAX);
	TruncateZeros();
	long long ans = 0;

	if (number.size() > maximum.size())
	{
		std::cout << "Can't convert BigNum to long integer, too large." << std::endl;
		return -1; //number is too big to fit into an integer
	}
	else if (number.size() == maximum.size())
	{
		bool smaller = 0;
		for (int i = number.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			if (number[i] > maximum[i])
				if (!smaller)
				{
					std::cout << "Can't convert BigNum to long integer, too large." << std::endl;
					return -1; //number is too big to fit into an integer
				}

			if (number[i] < maximum[i]) smaller = 1;
			ans += (long long)number[i];
		}
		return ans;
	}
	else
	{
		for (int i = number.size() - 1; i >= 0; i--)
		{
			ans *= 10;
			ans += (long long)number[i];
		}
		return ans;
	}
}

std::vector<int> BigNum::ConvertToVector(int num)
{
	//converts an int into a vector where index 0 = ones place, index 1 = tens place, etc.
	std::vector<int> number;

	while (num > 0)
	{
		number.push_back(num % 10);
		num /= 10;
	}

	return number;
}

std::vector<int> BigNum::ConvertToVector(long long num)
{
	//converts a long int into a vector where index 0 = ones place, index 1 = tens place, etc.
	std::vector<int> number;

	while (num > 0)
	{
		number.push_back(num % 10);
		num /= 10;
	}

	return number;
}

void BigNum::Print()
{
	TruncateZeros();
	if (number.size() == 0) return; //don't need to print out a blank line

	for (int i = number.size() - 1; i >= 0; i--) std::cout << number[i];
	std::cout << std::endl;
}

void BigNum::LeftShift(std::vector<int>& nums, int shift)
{
	for (int i = 0; i < shift; i++) nums.insert(nums.begin(), 0);
}

void BigNum::TruncateZeros()
{
	if (number.back() == 0);
	while (number.back() == 0) number.erase(number.end() - 1);

	digits = number.size();
	UpdateNumberString();
}

int BigNum::FirstXDigits(int x)
{
	//returns the first x digits of the BigNum
	if (x >= number.size()) return GetInt();
	std::vector<int> new_num;
	for (int i = number.size() - x; i < number.size(); i++) new_num.push_back(number[i]);

	return GetInt(new_num);;
}

int BigNum::LastXDigits(int x)
{
	//returns the last x digits of the BigNum
	if (x >= number.size()) return GetInt();
	std::vector<int> new_num;
	for (int i = 0; i < x; i++) new_num.push_back(number[i]);

	return GetInt(new_num);
}

void BigNum::UpdateNumberString()
{
	number_string = "";
	for (int i = number.size() - 1; i >= 0; i--) number_string += int_to_char(number[i]);
}

std::string BigNum::GetNumberString()
{
	return number_string;
}

std::string BigNum::GetNumberStringFirstXDigits(int x)
{
	if (x >= number_string.length()) return number_string;
	std::string new_string = "";
	for (int i = 0; i < x; i++) new_string += number_string[i];

	return new_string;
}

std::string BigNum::GetNumberStringLastXDigits(int x)
{
	if (x >= number_string.length()) return number_string;
	std::string new_string = "";
	for (int i = number_string.length() - x; i < number_string.length(); i++) new_string += number_string[i];

	return new_string;
}

int gcd(int a, int b)
{
	//new method utilizes modular division instead of subtraction
	//it's assumed that a is larger than b 
	if (!b) return a;
	else return gcd(b, a % b);
}

long long gcd(long long a, long long b)
{
	/*if (a == 0 or b == 0) return 0;
	if (a == b) return a;

	if (a > b) return gcd(a - b, b);
	return gcd(a, b - a);*/
	//new method utilizes modular division instead of subtraction
	//it's assumed that a is larger than b 
	if (!b) return a;
	else return gcd(b, a % b);
}

//long long extendedEuclideanGCD(long long a, long long b, long long* x, long long* y)
//{
//	//The extended Euclidean GCD algorithm.
//	//Computes x and y for the equation ax + by = gcd(a, b)
//
//	//This current version of the algorithm (which I made for the Chinese Remainder function above)
//	//assumes that a and b are coprime so gcd(a, b) is just 1.
//	// Base Case
//	if (a == 0)
//	{
//		*x = 0;
//		*y = 1;
//		return b;
//	}
//
//	long long x1, y1; // To store results of recursive call
//	long long gcd = extendedEuclideanGCD(b % a, a, &x1, &y1);
//
//	// Update x and y using results of
//	// recursive call
//	*x = y1 - (b / a) * x1;
//	*y = x1;
//
//	return gcd;
//}

long long extendedEuclideanGCD(long long a, long long b, long long* x, long long* y)
{
	//The extended Euclidean GCD algorithm.
	//Computes x and y for the equation ax + by = gcd(a, b)

	//This current version of the algorithm (which I made for the Chinese Remainder function above)
	//assumes that a and b are coprime so gcd(a, b) is just 1.
	*x = 0;
	*y = 0;
	long long u = 1, v = 0;
	while (a != 0)
	{
		long long q = b / a, r = b % a;
		long long m = *x - u * q, n = *y - v * q;
		b = a;
		a = r;
		*x = u;
		*y = v;
		u = m;
		v = n;
	}
	//gcd = b
	return 0;
}

bool coprime(int a, int b)
{
	if (gcd(a, b) == 1) return true;
	return false;
}