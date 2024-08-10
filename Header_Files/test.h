#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>
#include <Functions/montgomery_space.h>

#include <set>

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
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	
	const uint64_t mod = 17;
	MontgomerySpace space(mod);
	space.calculateInverses(mod - 1);
	
	/*for (int i = 1; i < mod; i++)
	{
		std::cout << "Inverse of " << i << "_m (mod " << mod << ") = " << space.getInverses()[i].value << "_m" << std::endl;
		std::cout << "Inverse of " << i << " (mod " << mod << ") = " << space.Revert(space.getInverses()[space.Transform(i).value]) << std::endl << std::endl;
	}*/

	//Calculate all possible divisions
	for (int i = 1; i < mod; i++)
	{
		MontgomeryNumber test = space.Transform(720720);
		MontgomeryNumber div = space.Transform(i);

		std::cout << space.Revert(test) << " / " << space.Revert(div) << " (mod " << mod << ") = " << space.Revert(space.Divide(test, div)) << std::endl;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}