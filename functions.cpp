#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <cmath>

//arrays with common values
int powers_of_two[11] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
long long powers_of_ten[11] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000 };

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

std::vector<int> PrimeFactors(int number)
{
	//returns a list of the prime factors of number, with repititions
	std::vector<int> p_facts;
	int i = 2;
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
		long long x = ModPow((long long)a[i], d, number, 0);
		if (x == 1 || x == (number - 1)) continue;

		for (int j = 0; j < (r - 1); j++)
		{
			x = ModPow(x, (long long)2, number, 0);
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
	if (new_pair)
	{
		pairs.push_back({ low });
	}

	//this portion advances to the next pair in the Faray sequence
	if (low.denominator + high.denominator <= maximum)
	{
		FaraySequence(maximum, pairs, { low.numerator + high.numerator, low.denominator + high.denominator }, high, true);
		FaraySequence(maximum, pairs, low, { low.numerator + high.numerator, low.denominator + high.denominator }, false);
	}
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
		if (maxNumber == 0) maxNumber = n;
		if (maxLength == 0) maxLength = n;

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
long long permutationsOfPartitions(int n, int l, int maximum, int minimum)
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
		answer += choose(l, i) * choose((secondaryNumerator -= primary), (secondaryDenominator -= primary)) * (flip *= -1);

	return answer;
}

template <typename T>
T BinomialMod(T n, T k, T m)
{
	//returns choose(n, k) % m
	if (m > n) return BinomialModLargePrime(n, k, m);
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
	/*if (a == 0 or b == 0) return 0;
	if (a == b) return a;

	if (a > b) return gcd(a - b, b);
	return gcd(a, b - a);*/

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

bool coprime(int a, int b)
{
	if (gcd(a, b) == 1) return true;
	return false;
}