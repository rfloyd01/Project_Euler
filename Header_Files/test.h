#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>
#include <Functions/modular_math.h>
#include <Header_Files/uint128_t.h>
#include <Header_Files/int_64x.h>

#include <set>

unsigned long long Karatsuba32Bit(const unsigned int& num1, const unsigned int& num2)
{
	//First check for trivial multiplications
	if (num1 == 0 || num2 == 0) return 0;
	else if (num1 == 1) return num2;
	else if (num2 == 1) return num1;

	int a = num1 >> 16, b = num1 & 0xFFFF, c = num2 >> 16, d = num2 & 0xFFFF;
	unsigned int Z2 = a * c, Z0 = b * d;

	//Create Z1 as an unsigned 128-bit integer by multiplying (B - A) and (C - D). If this 
	//multiplication results in a negative 64-bit number than any necessary leading 1 bits
	//are automatically added to Z1 due to how signed integers are casted into uint128_t.
	//This is the reason why A,B,C and D were created as signed integers as opposed to unsigned ones.
	int ba = b - a, cd = c - d;
	int bacd = ba * cd;

	unsigned long long Z1;
	if ((ba & 0x80000000) == (cd & 0x80000000) && (bacd & 0x80000000))
	{
		Z1 = unsigned int((b - a) * (c - d));
	}
	else if ((ba & 0x80000000) != (cd & 0x80000000) && !(bacd & 0x80000000))
	{
		Z1 = (b - a) * (c - d);
		if (bacd != 0 ) Z1 |= 0xFFFFFFFF00000000;
	}
	else
	{
		Z1 = (b - a) * (c - d);
	}

	//Adding Z2 and Z0 together could cause overflow since they are both positive 64-bit numbers.
	//Add them to Z1 individually as the uint128_t class can handle 64-bit additions without overflow.
	//Once Z2 and Z0 are added, Z1 will be guaranteed to be a positive number instead of a positive
	//representation of a negative number (i.e. it's lead bit will be a 0).
	Z1 += Z2;
	Z1 += Z0;

	//Right shift Z1 by 32 and then add Z0
	Z1 <<= 16;
	Z1 += Z0;

	//Add Z2 to the most significant 32-bits of Z1 (it should be possible to do this
	//without casting Z2 to an unsigned long long, this was just easier for now).
	Z1 += (unsigned long long)Z2 << 32;

	return Z1;
}

unsigned long long Karatsuba32BitSimple(const unsigned int& num1, const unsigned int& num2)
{
	//First check for trivial multiplications
	if (num1 == 0 || num2 == 0) return 0;
	else if (num1 == 1) return num2;
	else if (num2 == 1) return num1;

	int a = num1 >> 16, b = num1 & 0xFFFF, c = num2 >> 16, d = num2 & 0xFFFF;
	unsigned int Z2 = a * c, Z0 = b * d;

	//Create Z1 as an unsigned 128-bit integer by multiplying (B - A) and (C - D). If this 
	//multiplication results in a negative 64-bit number than any necessary leading 1 bits
	//are automatically added to Z1 due to how signed integers are casted into uint128_t.
	//This is the reason why A,B,C and D were created as signed integers as opposed to unsigned ones.
	unsigned long long Z1 = (b - a);
	Z1 *= (c - d);

	//Adding Z2 and Z0 together could cause overflow since they are both positive 64-bit numbers.
	//Add them to Z1 individually as the uint128_t class can handle 64-bit additions without overflow.
	//Once Z2 and Z0 are added, Z1 will be guaranteed to be a positive number instead of a positive
	//representation of a negative number (i.e. it's lead bit will be a 0).
	Z1 += Z2;
	Z1 += Z0;

	//Right shift Z1 by 32 and then add Z0
	Z1 <<= 16;
	Z1 += Z0;

	//Add Z2 to the most significant 32-bits of Z1 (it should be possible to do this
	//without casting Z2 to an unsigned long long, this was just easier for now).
	Z1 += (unsigned long long)Z2 << 32;

	return Z1;
}

bool testCRT(long long a, long long m, long long b, long long n, std::pair<long long, long long>* solution)
{
	//Given the following pair of congruences:
	//
	// x == a (mod m)
	// x == b (mod n)
	//
	//a single congruence for 'x' exists iff the gcd of m and n evenly divides a - b (i.e. (a - b) % gcd(m, n) = 0).
	//As long as that condition is set, this method will create a third congruence which holds true for both 
	//of the original congruences and store it in the solution variable. As an example, given the congruences x == 1 (mod 5) and 
	//x == 2 (mod 7), this method will return the combined congrence of x == 16 (mod 35)
	//which can be found to hold true for both of the original congruences.

	long long u, v;
	long long gcd = extendedEuclideanGCD(m, n, &u, &v);

	if ((a - b) % gcd != 0) return false;

	if (gcd == 1)
	{
		solution->second = m * n;
		solution->first = (a - m * u * (a - b)) % solution->second; //TODO: Overflow before modular division is possible here
	}
	else if ((a - b) % gcd == 0)
	{
		solution->second = m * n / gcd;
		solution->first = (a - m * u * (a - b) / gcd) % solution->second; //TODO: Overflow before modular division is possible here
	}

	return true;
}

void search(int n, int pos, std::pair<long long, long long> xy, std::vector<long long>& cand, std::vector<long long>& mods, std::set<long long>& ans)
{
	if (pos == n)
	{
		ans.insert(xy.first);
		return;
	}

	for (int i = pos; i < n; i++)
	{
		std::pair<long long, long long> newXY;
		testCRT(xy.first, xy.second, cand[i], mods[pos], &newXY);
		if (&newXY == nullptr) continue;

		long long temp = cand[pos];
		cand[pos] = cand[i];
		cand[i] = temp;

		search(n, pos + 1, newXY, cand, mods, ans);

		temp = cand[pos];
		cand[pos] = cand[i];
		cand[i] = temp;
	}
}

std::pair<std::vector<long long>, long long> findSolutions(long long n)
{
	std::pair<std::vector<long long>, long long> solutions;
	std::set<long long> ans;
	/*std::vector<long long> cand = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
	std::vector<long long> mods = { 36, 30, 24, 32, 28, 20, 18, 12, 16, 35, 34, 33, 27, 26, 22, 21, 15, 14, 10, 8, 6, 25, 9, 4, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2, 1 };*/

	std::vector<long long> cand = { 0, 1, 2, 3 };
	std::vector<long long> mods = { 4, 3, 2, 1 };

	search(n, 0, { 0, 1 }, cand, mods, ans);

	return solutions;
}

long long solve(long long n, long long m)
{
	std::pair<std::vector<long long>, long long> solutions = findSolutions(n);
	long long q = (m - 1) / solutions.first.size();
	long long r = (m - 1) % solutions.first.size();
	return q * solutions.second + solutions.first[r];
}

void recursiveSolve(int n, std::vector<long long>& moduli, std::vector<long long>& solutions, unsigned long long used, int a = 0, int m0 = 1, int level = 0)
{
	//The goal is to make sure that every number in the given range is 
	//divisible by one of the numbers from 1 to N. Once way we can achieve this 
	//is by keeping our range of numbers static, and shuffling around the numbers
	//that should evenly divide each one. Assuming that the first number in our 
	//range is X and the last number in the range is X + n - 1, we can create
	//a system of congruences which we can then attempt to solve using the 
	//Chinese Remainder Theorem. Here's how this would look in practice when n = 4:

	//[X % 1 == 0, (X+1) % 2 == 0, (X+2) % 3 == 0, (X+3) % 4 == 0]
	//[X % 1 == 0, (X+1) % 2 == 0, (X+2) % 4 == 0, (X+3) % 3 == 0]
	//[X % 1 == 0, (X+1) % 3 == 0, (X+2) % 2 == 0, (X+3) % 4 == 0]
	//....
	//[X % 4 == 0, (X+1) % 3 == 0, (X+2) % 2 == 0, (X+3) % 1 == 0]

	//The equations in each line can be simplified, for example, the 
	//equations in the third line above would be [X % 1 == 0, X % 3 == 2, X % 2 == 0, X % 4 == 1].
	//This method recursively tries to solve each system of congruences. By doing this 
	//recursively a large number of impossible congruences can be skipped by breaking out 
	//of the current level of the recursion when a certain combination is found to not work.
	if (level == n)
	{
		solutions.push_back(a);
		return;
	}

	for (int i = level; i <= n; i++)
	{
		int m1 = moduli[i];
		long long x, y;
		long long gcd = extendedEuclideanGCD(m0, m1, &x, &y);
		long long LCM = lcm(m0, m1);
		int b = gcd;
		unsigned long long digit = (1LL << (gcd + a - 1));
		while (digit < (1LL << n))
		{
			if (!(used & digit))
			{
				//solution.first = (a * x * m1 + (b - a) * m2 * m0) % solution.second;
				recursiveSolve(n, moduli, solutions, used | digit, (a * x * m1 + (b - a) * y * m0) % LCM, LCM, level + 1);
			}
			
			digit <<= gcd;
			b += gcd;
		}
	}
}

void mySolve(int n, unsigned long long usedModuli, std::pair<long long, long long> equation, std::set<long long>& solutions, long long level = 0)
{
	if (level == n)
	{
		solutions.insert((equation.first + equation.second) % equation.second);
		return;
	}

	for (long long i = 0; i < n; i++)
	{
		if ((1LL << i) & usedModuli) continue;

		std::pair<long long, long long> newEquation;
		int b = -level % (i + 1) + (i + 1);

		if (testCRT(equation.first, equation.second, b, i + 1, &newEquation))
		{
			if (newEquation.first < 0)
			{
				//Convert the first part of equation to a positive number
				newEquation.first = newEquation.first % newEquation.second + newEquation.second;
			}
			
			mySolve(n, usedModuli | (1LL << i), newEquation, solutions, level + 1);
		}
	}
}

std::pair<std::string, long double> test()
{          
	srand(std::chrono::steady_clock::now().time_since_epoch().count()); //set a new random generator seed
	long long answer = 0;

	auto run_time = std::chrono::steady_clock::now();
	
	unsigned long long a = -2, b = 2, n = -1;
	long long yo = MontgomeryMultiplication64Bit(a, b, n);

	uint128_t x = ~(0ULL) - 1, y = 2, m = ~(0ULL);
	unsigned long long yote = ((x * y) % m).getWords()[0];
	//std::cout << x / y << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}