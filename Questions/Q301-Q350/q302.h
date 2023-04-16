#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>
#include <algorithm>

//Strong Achilles Numbers

long long limit = 1e18;
int county = 0;
long long ans = 0;
std::vector<std::vector<int> > GCDs;
std::vector<long long> strong_achilles_numbers, strong_achilles_numbers1;
std::vector<std::vector<int> > standard_factors;

void q302PrimeFactors(long long n, std::vector<int>& prime_factorization, std::vector<int>& exponent_count, std::vector<int>& primes)
{
	//gives us the unique prime factors of n
	int i = 0, prime_count = 0;
	while (n > 1)
	{
		if (n % primes[i] == 0)
		{
			prime_factorization.push_back(primes[i]);
			while (n % primes[i] == 0)
			{
				prime_count++;
				n /= primes[i];
			}

			exponent_count.push_back(prime_count);
			prime_count = 0;
		}
		else i++;
	}
}

std::pair<std::pair<int, int>, std::pair<int, int>> FindTwoSmallestValues(std::vector<int> v)
{
	//returns the value and location of the two smallest values in the given array.
	//The smallest value is in position 0 and the next smallest is in position 1.
	//Due to the nature of this question, the minimum size of v will be 2 elements.
	std::pair<int, int> first_smallest, second_smallest;

	//initialize first and second smallest with the first two non-zero elements
	int found = 0, x = 0;
	while (found < 2)
	{
		if (v[x])
		{
			if (!found)
			{
				first_smallest.first = x;
				first_smallest.second = v[x++];
			}
			else
			{
				second_smallest.first = x;
				second_smallest.second = v[x++];
			}
			found++;
		}
		else x++;
	}

	for (int i = second_smallest.first; i < v.size(); i++)
	{
		//it's possible for some powers to be 0, these just represent the element isn't really there so are not counted
		if (!v[i]) continue;

		if (v[i] < first_smallest.second)
		{
			//what was originally the first smallest becomes the second smallest now
			second_smallest = first_smallest;

			//and the current number now becomes the second smallest
			first_smallest.first = i;
			first_smallest.second = v[i];
		}
		else if (v[i] < second_smallest.second)
		{
			second_smallest.first = i;
			second_smallest.second = v[i];
		}
	}

	return { first_smallest, second_smallest };
}

bool PerfectPowerTest(long long n, int lowest_power)
{
	//given that the smallest exponent in the prime factorization of n is lowest_power,
	//this function finds out if n is a perfect power or not.

	for (int i = 1; i < standard_factors[lowest_power].size(); i++)
	{
		long long root = pow(n, 1.0 / standard_factors[lowest_power][i]) + 0.0001; //add a small amount to ensure proper root calculation

		//the pow function can't be relied on for accurately raising a number to a power,
		//for example, it says that 189843750^2 = 36040649414062504 when in reality it is 36040649414062500.
		//Since this is the case just increment by factor amount and keep multiplying root against itself
		long long rebuild = root;
		for (int j = 1; j < standard_factors[lowest_power][i]; j++)
		{
			rebuild *= root;
		}
		if (rebuild == n) return true;
	}

	return false;
}

int VectorGDCFinder(std::vector<int> v, int ignore)
{
	//finds the GCD between all elements in v while ignoring the element at ignore
	int current_gcd;

	//set the current_gcd to the first non-zero term that isn't at element ignore
	for (int i = 0; i < v.size(); i++)
		if (i != ignore && v[i])
			current_gcd = v[i];

	for (int i = 0; i < v.size(); i++)
	{
		if (i == ignore || !v[i]) continue;

		for (int j = i + 1; j < v.size(); j++)
		{
			if (j == ignore || !v[j]) continue;

			int new_gcd = GCDs[v[i]][v[j]];
			if (new_gcd == 1) return 1;

			if (current_gcd <= 0) vprint(v);

			int combined_gcd = GCDs[current_gcd][new_gcd];
			if (combined_gcd == 1) return 1;

			current_gcd = combined_gcd;
		}
	}

	return current_gcd;
}

void recursiveExponentFill1(long long current_number, std::vector<int>& totient_primes, std::vector<int>& n_prime_powers, std::vector<int>& totient_prime_powers, long long& answer, int current_level = 0)
{
	//since the base prime powers are given at the start of this function, then increasing an exponent 
	//in n will increase the exponent in totient(n) by the exact same amount without effecting any of
	//the other exponents.

	if (current_level == totient_primes.size() - 1)
	{
		//we only ever to our GCD check at the bottom level of the recursion
		int n_exponent_gcd = VectorGDCFinder(n_prime_powers, current_level);
		int totient_exponent_gcd = VectorGDCFinder(totient_prime_powers, current_level);
		int max_exponent = log(limit / (double)current_number) / log(totient_primes[current_level]);

		long long mult = 1;
		
		if (n_exponent_gcd == 1 && totient_exponent_gcd == 1)
		{
			//if the gcd (excluding the final prime power) is 1 for both n and totient(n) then there's no restriction 
			//as to what exponent we can use so we just add max_exponent to the answer
			
			//answer += (max_exponent + 1);

			//TODO: Go one at a time to help debug
			for (int i = 0; i <= max_exponent; i++)
			{
				answer++;
				strong_achilles_numbers1.push_back(current_number * mult);

				mult *= totient_primes.back();
			}
		}
		else if (n_exponent_gcd == 1)
		{
			//the totient exponents don't have a GCD of 1 but n does. We need to keep incrementing our exponent by 1
			//and making sure the GCD is 1 each time
			for (int i = 0; i <= max_exponent; i++)
			{
				if (GCDs[totient_prime_powers[current_level] + i][totient_exponent_gcd] == 1)
				{
					answer++;
					strong_achilles_numbers1.push_back(current_number * mult);
				}
				mult *= totient_primes.back();
			}
		}
		else if (totient_exponent_gcd == 1)
		{
			//the n exponents don't have a GCD of 1 but totient(n) does. We need to keep incrementing our exponent by 1
			//and making sure the GCD is 1 each time
			for (int i = 0; i <= max_exponent; i++)
			{
				if (GCDs[n_prime_powers[current_level] + i][n_exponent_gcd] == 1)
				{
					answer++;
					strong_achilles_numbers1.push_back(current_number * mult);
				}
				mult *= totient_primes.back();
			}
		}
		else
		{
			//the gcd for n and totient(n) is more than 1, this means we need to check the GCD for both after each iteration
			for (int i = 0; i <= max_exponent; i++)
			{
				if (GCDs[n_prime_powers[current_level] + i][n_exponent_gcd] == 1)
				{
					if (GCDs[totient_prime_powers[current_level] + i][totient_exponent_gcd] == 1)
					{
						/*std::cout << i << std::endl;
						vprint(n_prime_powers);
						vprint(totient_prime_powers);*/
						answer++;
						strong_achilles_numbers1.push_back(current_number * mult);
					}
				}
				mult *= totient_primes.back();
			}
		}
	}
	else
	{
		//check to see if the prime we're looking at is in the prime factorization of n, if not just go to the
		//next level of the recursion
		if (!n_prime_powers[current_level])
		{
			recursiveExponentFill1(current_number, totient_primes, n_prime_powers, totient_prime_powers, answer, current_level + 1);
		}
		else
		{
			//calculate the maximum value for the exponent on the current prime.
			int max_exponent = log(limit / (double)current_number) / log(totient_primes[current_level]);
			long long new_current_number = current_number;

			for (int i = 0; i <= max_exponent; i++)
			{
				recursiveExponentFill1(new_current_number, totient_primes, n_prime_powers, totient_prime_powers, answer, current_level + 1);
				n_prime_powers[current_level]++;
				totient_prime_powers[current_level]++;
				new_current_number *= totient_primes[current_level];
			}

			//reset the exponent counts before going back to previous level of recursion
			n_prime_powers[current_level] -= (max_exponent + 1);
			totient_prime_powers[current_level] -= (max_exponent + 1);
		}
	}
}

void exponentFill(long long current_number, long long current_totient, std::vector<int>& primes, bool recursion = true, std::pair<std::pair<int, int>, std::pair<int, int>>* smallest_n_power = nullptr,
	std::pair<std::pair<int, int>, std::pair<int, int>>* smallest_t_power = nullptr, std::vector<int>* np_factors = nullptr, std::vector<int>* tp_factors = nullptr, std::vector<int>* np_powers = nullptr,
	std::vector<int>* tp_powers = nullptr, int current_level = 0)
{
	if (!recursion)
	{
		//The non-recursive part of this function is just to set up some key vectors to help with the exponents.
		//First we need to get the prime factorizations for the current number and totient, as well as the value of
		//their prime exponents
		std::vector<int> n_prime_factors, n_prime_powers;
		std::vector<int> t_prime_factors, t_prime_powers;

		q302PrimeFactors(current_number, n_prime_factors, n_prime_powers, primes);
		q302PrimeFactors(current_totient, t_prime_factors, t_prime_powers, primes);

		//It's possible for there to be primes in the totient which aren't in n itself. it'll be easier to keep track of primes if 
		//all the vectors are the same length and indices are the same, so for any primes in the totient vector that aren't in the n vector, just add a 0
		for (int i = 0; i < t_prime_factors.size(); i++)
		{
			if (t_prime_factors[i] != n_prime_factors[i])
			{
				n_prime_factors.insert(n_prime_factors.begin() + i, 0);
				n_prime_powers.insert(n_prime_powers.begin() + i, 0);
			}
		}

		//We also need to keep track of the exponent with the lowest value for both n and totient(n). The reason for this is
		//because we can tell if an integer is a power number simply by looking at the value the lowest exponent in its
		//prime factorization. A number can only have roots that evenly divide the lowest exponent in the prime factorization.
		//As an example the number 2^6 * 3^6 * 5^8 has a smallest exponent of 6. This means it's only possible for this number 
		//to have a square root, a cube root, or a 6th root. In this case the square root would be 2^3 * 3^3 * 5^4. Basically
		//the reason for keeping track of the prime powers is to figure out which roots we need to test to look for integers.
		std::pair<std::pair<int, int>, std::pair<int, int>> n_smallest_powers = FindTwoSmallestValues(n_prime_powers);
		std::pair<std::pair<int, int>, std::pair<int, int>> t_smallest_powers = FindTwoSmallestValues(t_prime_powers);

		/*std::cout << "n primes :"; vprint(n_prime_factors);
		std::cout << "n powers :"; vprint(n_prime_powers);
		std::cout << "smallest n powers are: " << n_smallest_powers.first.second << " and " << n_smallest_powers.second.second << std::endl;
		std::cout << "smallest n power is : " << n_smallest_power << std::endl;
		std::cout << "t primes :"; vprint(t_prime_factors);
		std::cout << "t powers :"; vprint(t_prime_powers);
		std::cout << "smallest t powers are: " << t_smallest_powers.first.second << " and " << t_smallest_powers.second.second << std::endl;
		std::cout << "smallest t power is : " << t_smallest_power << std::endl;
		std::cout << std::endl;*/

		//with the setup complete we now call the recursive part of the function, the starting level of the recursion must be set to the index of the first
		//non-zero prime in the n_prime_factors array
		int start_level = -1;
		while (!n_prime_factors[++start_level])
		{
			//intentionally blank
		}
		exponentFill(current_number, current_totient, primes, true, &n_smallest_powers, &t_smallest_powers, &n_prime_factors, &t_prime_factors, &n_prime_powers, &t_prime_powers, start_level);
	}
	else
	{
		//in the recursive part of the function we simply try and keep adding exponents one by one, checking to make sure that both 
		//n and totient(n) remain Achilles numbers. First calculate the maximum amount we can increase the exponent by for the prime
		//at the location current_level in the prime_factors vector
		int max_exponent = log(limit / (double)current_number) / log(np_factors->at(current_level));

		if (current_level == tp_factors->size() - 1)
		{
			//the base of our recursion happens at the last prime in the prime vector. Increment the final power 
			//as much as possible
			long long mult = 1;
			int lowest_n_power = smallest_n_power->first.second, lowest_t_power = smallest_t_power->first.second;
			int current_exponent = 0;

			for (int i = 0; i <= max_exponent; i++)
			{
				if (!PerfectPowerTest(current_number * mult, lowest_n_power))
				{
					if (!PerfectPowerTest(current_totient * mult, lowest_t_power))
					{
						ans++;
					}
				}

				mult *= np_factors->back();

				//see if the smallest power has changed in either n or totient(n)
				if (smallest_n_power->first.first == current_level)
				{
					if (lowest_n_power + 1 <= smallest_n_power->second.second) lowest_n_power++;
				}

				if (smallest_t_power->first.first == current_level)
				{
					if (lowest_t_power + 1 <= smallest_t_power->second.second) lowest_t_power++;
				}
			}
		}
		else
		{
			//make copies of the current_number, current_totient and smallest number pairs for a slightly easier time
			//reseting them when returning from recursion
			long long new_current_number = current_number;
			long long new_current_totient = current_totient;
			std::pair<std::pair<int, int>, std::pair<int, int>> new_smallest_n_power = *smallest_n_power;
			std::pair<std::pair<int, int>, std::pair<int, int>> new_smallest_t_power = *smallest_t_power;

			//before recursing down to the next level, find the next non-zero prime power and jump to it
			int next_power = 0;

			while (!np_factors->at(current_level + (++next_power)))
			{
				//intentionally blank
			}

			for (int i = 0; i <= max_exponent; i++)
			{
				//check to see if the next prime in the list is a 0 or not, if so then skip it
				exponentFill(new_current_number, new_current_totient, primes, true, &new_smallest_n_power, &new_smallest_t_power, np_factors, tp_factors, np_powers, tp_powers, current_level + next_power);
				np_powers->at(current_level)++;
				tp_powers->at(current_level)++;

				//after incrementing exponents, see if any smallest exponent values have changed
				if (new_smallest_n_power.first.first == current_level || new_smallest_n_power.second.first == current_level) new_smallest_n_power = FindTwoSmallestValues(*np_powers);
				if (new_smallest_t_power.first.first == current_level || new_smallest_t_power.second.first == current_level) new_smallest_t_power = FindTwoSmallestValues(*tp_powers);

				new_current_number *= np_factors->at(current_level);
				new_current_totient *= np_factors->at(current_level);
			}

			//reset the exponent counts before going back to previous level of recursion
			np_powers->at(current_level) -= (max_exponent + 1);
			tp_powers->at(current_level) -= (max_exponent + 1);
		}
	}
}

//void exponentFill1(int current_number, std::vector<int>& primes, std::vector<int>& prime_powers, std::vector<std::vector<int> >& prime_factorizations)
//{
//	
//	//county++;
//
//	//Any prime combo that makes it to this point is guaranteed to not have any primes raised to only 
//	//the first power in the totient. We still need to look at the GCD patterns for both n and totient(n)
//	//to see what exponents can be used.
//
//	//first, we need to come up with a set of equations that tells us how many of each prime there will be in 
//	//the totient based on what the exponents for the primes in n are.
//
//	std::vector<int> totient_primes = primes, totient_powers;
//	std::vector<std::vector<int> > totient_equations;
//	for (int i = 0; i < primes.size(); i++)
//	{
//		for (int j = 0; j < prime_factorizations[primes[i] - 1].size(); j++)
//		{
//			if (std::find(totient_primes.begin(), totient_primes.end(), prime_factorizations[primes[i] - 1][j]) == totient_primes.end())
//			{
//				totient_primes.push_back(prime_factorizations[primes[i] - 1][j]);
//			}
//		}
//	}
//
//	std::sort(totient_primes.begin(), totient_primes.end());
//
//	//find the base level of the exponents in the totient. From this base, increasing an exponent of n will cause the same 
//	//exponent to increase by the same amount in the totient and none of the other exponents will be effected
//	for (int i = 0; i < totient_primes.size(); i++) totient_powers.push_back(0);
//	for (int i = 0; i < primes.size(); i++)
//	{
//		//first, each prime power in n will decrease by 1 in the totient
//		for (int j = 0; j < totient_primes.size(); j++)
//		{
//			if (primes[i] == totient_primes[j])
//			{
//				totient_powers[j] = prime_powers[i] - 1;
//				break;
//			}
//		}
//
//		//then all prime factors of (prime - 1) will increase in the totient
//		for (int j = 0; j < prime_factorizations[primes[i] - 1].size(); j++)
//		{
//			int p_fact = prime_factorizations[primes[i] - 1][j];
//			for (int k = 0; k < totient_primes.size(); k++)
//			{
//				if (p_fact == totient_primes[k])
//				{
//					totient_powers[k]++;
//					break;
//				}
//			}
//		}
//	}
//
//	//once the base exponents for the totient are set we recursively increment the exponents upwards to find
//	//all posibilites where the GCD between each exponent of n is 1 and the GCD of each exponent in totient(n)
//	//is also 1. The length of prime_powers and totient_powers needs to be the same so insert 0's into 
//	//prime powers in the necessary locations.
//	int location = 0;
//	for (int i = 0; i < totient_primes.size(); i++)
//	{
//		if (totient_primes[i] != primes[location]) prime_powers.insert(prime_powers.begin() + i, 0);
//		else location++;
//	}
//
//	/*std::cout << "n primes :"; vprint(primes);
//	std::cout << "n powers :"; vprint(prime_powers);
//	std::cout << "t primes :"; vprint(totient_primes);
//	std::cout << "t powers :"; vprint(totient_powers);
//	std::cout << std::endl;*/
//
//	recursiveExponentFill1(current_number, totient_primes, prime_powers, totient_powers, ans);
//	//std::cout << "answer is now " << ans << std::endl << std::endl;
//}

void recursivePrimeSelect(long long current_number, long long current_totient, long long stranded_primes,
	std::vector<std::vector<int> >& prime_factorizations, std::vector<int>& primes, int location = 0, int level = 0)
{
	if (level > 1) //There needs to be more than 1 prime present for Achilles numbers, this happens at recursion levels greater than 1
	{
		//if there are any stranded primes that aren't included in the prime factorization then it won't be possible to make 
		//a strong Achilles number. It's easy enough to check this, if stranded primes evenly divides current primes then 
		//there are no extra primes present.
		if (current_number % stranded_primes == 0)
		{
			//This combo in theory works. Any primes that are still stranded need to be cubed instead of squared so we multiply the current 
			//number by the stranded_primes number to make up this difference. If we're still under the limit we can go ahead and find all 
			//the working combinations of exponents for the number.
			if (limit / current_number >= stranded_primes) exponentFill(current_number * stranded_primes, current_totient * stranded_primes, primes, false);
		}
	}

	for (int i = location; i < primes.size(); i++)
	{
		//Frst, see if the prime at the current location is small enough to add to our current number without exceding the limit.
		//We assume that this new prime will be the last one in the prime factorization and as such it would get cubed. If this prime 
		//is too large to be cubed when everything else is only squared then there's no more reason to increment higher.
		if (pow(limit / (double)current_number, 1.0 / 3.0) < primes[i]) break;

		//if the prime is small enough add its square to the current_number representation. The current_totient and stranded_primes each get increased
		//by the prime. Do this on two separate lines to avoid the primes (which are standard integers) from overflowing.
		current_number *= primes[i];
		current_number *= primes[i];

		int p_minus_one = primes[i] - 1;

		//create copies of the current state of the stranded primes and totient primes representations. Since some primes potentially get added
		//and some potentially get removed at the current level of the recursion, we can't rely on the recursion alone to get us back to the 
		//current state.
		long long original_stranded_primes = stranded_primes, original_totient = current_totient;

		//All primes are considered stranded when they're first introduced. Only when larger primes that can decompose into them get added
		//do the get "un-stranded". Because of this we increase both the totient and stranded primes by a factor of the current prime
		current_totient *= primes[i];
		stranded_primes *= primes[i];

		//iterate through the prime factorization of (primes[i] - 1) and see if 
		//it adds or removes anything from the stranded primes list

		for (int j = 0; j < prime_factorizations[p_minus_one].size(); j++)
		{
			int potential_new_prime = prime_factorizations[p_minus_one][j];

			if (stranded_primes % potential_new_prime != 0)
			{
				//the prime isn't in the stranded list, see if it exists in the current_totient (meaning it was once stranded but has already been unstranded)
				if (current_totient % potential_new_prime != 0)
				{
					//this is an entirely new prime so add it to both the totient and stranded list
					stranded_primes *= potential_new_prime;
					current_totient *= potential_new_prime;
				}
				else
				{
					//the prime was stranded at one point but no longer is, we increase the totient without changing
					//the stranded_primes
					current_totient *= potential_new_prime;
				}
			}
			else
			{
				//the prime is in the stranded list already, this means we will have at least two of this prime in the totient
				//so we remove it from the stranded primes representation and add it to the totient primes representation
				stranded_primes /= potential_new_prime;
				current_totient *= potential_new_prime;
			}
		}

		//once the stranded primes are dealt with we recurse down to the next level
		recursivePrimeSelect(current_number, current_totient, stranded_primes, prime_factorizations, primes, i + 1, level + 1);

		//after getting back from the recursion, we remove the current prime from the current_primes representation
		//and reset the totient primes and stranded primes representations before we altered them in the above loop
		current_number /= primes[i];
		current_number /= primes[i];
		current_totient = original_totient;
		stranded_primes = original_stranded_primes;
	}
}

//void recursivePrimeSelect1(long long current_number, std::vector<int>& current_primes, std::vector<int>& totient_primes, std::vector<int>& stranded_primes,
//	std::vector<std::vector<int> >& prime_factorizations, std::vector<int>& primes, bool* current_prime_factorization, int location = 0) 
//{
//	if (current_primes.size() > 1)
//	{
//		long long new_current_number = current_number;
//		bool good_to_go = true;
//
//		for (int i = 0; i < stranded_primes.size(); i++)
//		{
//			//if there are any stranded primes which aren't part of the original prime factorization of n
//			//then the totient won't be an Achilles number so we go back to the previous level of the recursion
//			if (!current_prime_factorization[stranded_primes[i]])
//			{
//				good_to_go = false;
//				break;
//			}
//			if (limit / new_current_number < stranded_primes[i]) return;
//
//			new_current_number *= stranded_primes[i];
//		}
//
//		if (good_to_go)
//		{
//			//This combo in theory works, call a separate function to count all the ways we can set the exponents to make Achilles numbers
//			//create a vector that represents the minimum value of the exponents and pass it to the exponent function
//			std::vector<int> base_exponents;
//			for (int i = 0; i < current_primes.size(); i++)
//			{
//				if (std::find(stranded_primes.begin(), stranded_primes.end(), current_primes[i]) != stranded_primes.end()) base_exponents.push_back(3);
//				else base_exponents.push_back(2);
//			}
//
//			//This combo in theory works, call a separate function to count all the ways we can set the exponents to make Achilles numbers
//			exponentFill1(new_current_number, current_primes, base_exponents, prime_factorizations);
//		}
//	}
//
//	for (int i = location; i < primes.size(); i++)
//	{
//		//frst, see if the prime at the current location is small enough to add to our current number without exceding the limit.
//		//We assume that all primes added will be the last, and therefore must be cubed
//		if (pow(limit / (double)current_number, 0.5) < primes[i]) break;
//
//		//if the prime is small enough add it to the current_prime list
//		current_primes.push_back(primes[i]);
//		current_prime_factorization[primes[i]] = true;
//		int p_minus_one = primes[i] - 1;
//
//
//		//create a copies of the totient primes and stranded primes list so we can reset it after checking each new prime
//		std::vector<int> original_stranded_primes = stranded_primes, original_totient_primes = totient_primes;
//		stranded_primes.push_back(primes[i]); //all primes will be stranded when first added until larger primes that can decompose into them get added
//
//		//iterate through the prime factorization of (primes[i] - 1) and see if 
//		//it adds or removes anything from the stranded primes list
//		for (int j = 0; j < prime_factorizations[p_minus_one].size(); j++)
//		{
//			int potential_new_prime = prime_factorizations[p_minus_one][j];
//
//			//check to see if the current prime factor is in the stranded primes list
//			bool stranded = false;
//			auto prime_location = std::find(stranded_primes.begin(), stranded_primes.end(), potential_new_prime);
//			if (prime_location == stranded_primes.end())
//			{
//				//the prime isn't in the stranded list, see if it exists in the totient list (meaning it was once stranded but has already been unstranded)
//				if (std::find(totient_primes.begin(), totient_primes.end(), potential_new_prime) == totient_primes.end())
//				{
//					//this is a new stranded prime so add it to the list
//					stranded_primes.push_back(potential_new_prime);
//				}
//			}
//			else
//			{
//				//the prime is in the stranded list already, this means we will have at least two of this prime in the totient
//				//so we remove it from the stranded list and add it to the totient list
//				stranded_primes.erase(prime_location);
//				totient_primes.push_back(potential_new_prime);
//			}
//		}
//
//		//once the stranded primes are dealt with we recurse down to the next level
//		recursivePrimeSelect1(current_number * primes[i] * primes[i], current_primes, totient_primes, stranded_primes, prime_factorizations, primes, current_prime_factorization, i + 1);
//
//		//after getting back from the recursion, we remove the current prime from the current_primes vector
//		//and reset the totient primes and stranded primes vector
//		current_primes.pop_back();
//		current_prime_factorization[primes[i]] = false;
//		totient_primes = original_totient_primes;
//		stranded_primes = original_stranded_primes;
//	}
//}

//void recursivePrimeSelect(long long current_number, std::vector<int>& current_primes, std::vector<int>& totient_primes, std::vector<int>& stranded_primes,
//	std::vector<std::vector<int> >& prime_factorizations, std::vector<int>& primes, bool* current_prime_factorization, int location = 0)
//{
//	if (current_primes.size() > 1)
//	{
//		bool good_to_go = true;
//
//		for (int i = 0; i < stranded_primes.size(); i++)
//		{
//			//if there are any stranded primes which aren't part of the original prime factorization of n
//			//then the totient won't be an Achilles number so we go back to the previous level of the recursion
//			if (!current_prime_factorization[stranded_primes[i]])
//			{
//				good_to_go = false;
//				break;
//			}
//		}
//
//		if (good_to_go)
//		{
//			//This combo in theory works, call a separate function to count all the ways we can set the exponents to make Achilles numbers
//			//create a vector that represents the minimum value of the exponents and pass it to the exponent function
//			std::vector<int> base_exponents;
//			for (int i = 0; i < current_primes.size(); i++)
//			{
//				if (std::find(stranded_primes.begin(), stranded_primes.end(), current_primes[i]) != stranded_primes.end()) base_exponents.push_back(3);
//				else base_exponents.push_back(2);
//			}
//			exponentFill(current_number, current_primes, base_exponents, prime_factorizations);
//		}
//	}
//
//	for (int i = location; i < primes.size(); i++)
//	{
//		//frst, see if the prime at the current location is small enough to add to our current number without exceding the limit.
//		//We assume that all primes added will be the last, and therefore must be cubed
//		if (pow(limit / (double)current_number, 1.0 / 3.0) < primes[i])
//		{
//			//Adding the new prime (which must be cubed) causes us to exceed the limit.
//			//This doesn't mean that it's impossible to use this number though (or even higher
//			//numbers potentially). There's a chance that higher primes added will have their 
//			//(p-1) values bring current primes in the factorization from a cube down to a sqaure
//			//which can get us back under the limit.
//
//			//The only time that we can declare it truly impossible to add higher primes is if 
//			//all cubed numbers are converted to squares and the new prime still makes us exceed
//			//the limit
//			break; //square_current assumes that only the last number is cubed
//		}
//
//		//if the prime is small enough add it to the current_prime list
//		current_primes.push_back(primes[i]);
//		current_prime_factorization[primes[i]] = true;
//		int p_minus_one = primes[i] - 1;
//		long long temp_current_number = current_number * primes[i] * primes[i] * primes[i];
//		
//
//		//create a copies of the totient primes and stranded primes list so we can reset it after checking each new prime
//		std::vector<int> original_stranded_primes = stranded_primes, original_totient_primes = totient_primes;
//		stranded_primes.push_back(primes[i]); //all primes will be stranded when first added until larger primes that can decompose into them get added
//
//		//iterate through the prime factorization of (primes[i] - 1) and see if 
//		//it adds or removes anything from the stranded primes list
//		for (int j = 0; j < prime_factorizations[p_minus_one].size(); j++)
//		{
//			int potential_new_prime = prime_factorizations[p_minus_one][j];
//
//			//check to see if the current prime factor is in the stranded primes list
//			bool stranded = false;
//			auto prime_location = std::find(stranded_primes.begin(), stranded_primes.end(), potential_new_prime);
//			if (prime_location == stranded_primes.end())
//			{
//				//the prime isn't in the stranded list, see if it exists in the totient list (meaning it was once stranded but has already been unstranded)
//				if (std::find(totient_primes.begin(), totient_primes.end(), potential_new_prime) == totient_primes.end())
//				{
//					//this is a new stranded prime so add it to the list
//					stranded_primes.push_back(potential_new_prime);
//				}
//			}
//			else
//			{
//				//the prime is in the stranded list already, this means we will have at least two of this prime in the totient
//				//so we remove it from the stranded list and add it to the totient list
//				stranded_primes.erase(prime_location);
//				totient_primes.push_back(potential_new_prime);
//
//				/*if (temp_current_number / potential_new_prime == 0)
//				{
//					vprint(current_primes);
//					std::cout << temp_current_number << std::endl;
//				}*/
//
//				temp_current_number /= potential_new_prime; //we no longer have to cube the prime in the makeup of n, it can be squared
//				
//			}
//		}
//
//		//once the stranded primes are dealt with we recurse down to the next level
//		recursivePrimeSelect(temp_current_number, current_primes, totient_primes, stranded_primes, prime_factorizations, primes, current_prime_factorization, i + 1);
//
//		//after getting back from the recursion, we remove the current prime from the current_primes vector
//		//and reset the totient primes and stranded primes vector
//		current_primes.pop_back();
//		current_prime_factorization[primes[i]] = false;
//		totient_primes = original_totient_primes;
//		stranded_primes = original_stranded_primes;
//	}
//}

std::pair<std::string, long double> q302()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	int max_prime = pow(limit / 4.0, 1.0 / 3.0);
	std::vector<std::vector<int> > prime_factorizations = AllPrimeFactors(max_prime); //takes 0.114752 seconds to generate the prime factorizations
	std::vector<int> current_primes, totient_primes, stranded_primes, primez = primes(max_prime);

	//max expected exponent is 64, memoize all factors (prime or not) for numbers up to this number
	standard_factors.push_back({});
	for (int i = 1; i <= 64; i++) standard_factors.push_back(getFactors(i));

	recursivePrimeSelect(1, 1, 1, prime_factorizations, primez);

	return { std::to_string(ans), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1170060
	//ran in 1.57469 seconds
}

//Notes
/*
	there are 106,896 working combos for primes, currently it takes 47 seconds to find all of these
	I'm sure I can speed this up quite a bit with some thought
	For reference the first combo that works is [2, 3] and the last combo that works is [3529, 4051]

	After editing the recursive function so that it cubes any prime in the prime factorization that doesn't get an extra
	prime from decompositions (not just the last prime) there are now only 20,260 workable prime combos which takes
	12.6 seconds to find all of them. This is much better. I think I can still greatly improve the recursive function if
	I can get rid of the need to copy arrays so many times.

	I updated the algorithm again so that it automatically cubes numbers until they become unstranded (after all, when each prime is
	first added there's nothing larger than it so it will always get cubed at the base of the recursion). Only when a prime becomes
	unstranded can it be squared instead of cubed. Doing this brought the number of workable combos down slightly to 19,624 but decreased
	the runtime by a factor of 60x to only 0.2 seconds. I'm nervous though because I feel like there should be the same number of
	combos as before and there aren't. It's possible that there are cases where adding a single prime on the end can remove multiple
	cubed primes and this new algorithm would miss this.

	I did end up a few instances where the second iteration of the rerusive algorithm found correct hits which the third iteration doesn't
	for example [2, 3, 5, 7, 11, 13, 113]. This is because the 113 has a 7 in its decomposition so the 7 goes to a square from a cube.I'm sure 
	there's something I can do which will give me a happy medium between the two algorithms but for now, until I solve the problem the 
	first time, I'm just going with the second algorithm. Since the third one is so much quicker though and only generates correct answers
	I'm going to use it while working on the exponent selection algorithm.

	------

	The algorithm currently works when the limit is 10^4 and 10^8, but not when it's 10^18. Current runtime when limit is 10^18
	is 64 seconds which is just beyond a passing grade. After working on a better solution I realized that one of my functions used 
	a regular integer instead of a long integer. Changing the variable type led to the correct answer, although the runtime is still 
	pretty bad so I'm going to keep working on this for a bit. I think the best speed up would come from somehow getting rid of the 
	need to use vectors for storing all the primes currently being used.

	After a little thought I realized that instead of passing vectors of primes in the first recursive function, I can just use a single 
	integer. All of the prime arrays getting manipulated feature only 1 of each prime (as the prime power array is created at the end of 
	the recursion). Since each array holds distinct primes, it can simply be expressed as an integer, and then re-extracted from the 
	prime factors array when needed.

	In cunjuntion with the above paragraph I had an epiphany. One of the reasons I kept the primes separated in vectors in the first 
	place was because I needed to test whether or not the GCD's of their exponents was 1. Well in reality the only prime power that matters 
	is the smallest one. If the smallest exponent power is a 2, then the smallest power number this could possibly be is a square. For example
	if we're looking at a number with a prime factorization of 2^6*3^3*5^4, then at most this could be a cubic number because of the 3^3 term.
	Instead of checking whether the GCD of all the exponents is 1 (which would entail calling the GCD() method 3 times in the worst case) I can 
	just see if the cube root is an integer to verify if it's a power number or not. Using another example, 2^6*3^6*5^8. The smallest exponent
	in this case is 6, and this isn't a 6th power. It is, however, a perfect square. So what we do here is see if the given number is a power 
	of any of the factors of 6. So if we checked to see if this is a square, a cube, or a 6th power we would've gotten a hit for a square. This 
	should be much quicker and easier than the complicated GCD check I implemented.

	-----

	After a lot of debugging I've got the faster algorithm working. I definitely made a few mistakes along the way, but by far the biggest issue 
	was the built in pow() function returning wrong numbers. Both when taking roots (i.e. 216^(1/3) = 5.99999 instead of 6) and when giving 
	powers (i.e. 189843750^2 = 36040649414062504 instead of 36040649414062500). Everytime I use this method I forget that it's not precise enough 
	to be correct all the time.
*/