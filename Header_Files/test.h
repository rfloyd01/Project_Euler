#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <map>
#include <algorithm>

//class GoverningEquation
//{
//public:
//	//Variables defining the equation
//	std::pair<int, int> X;
//	std::pair<int, int> Y;
//	std::pair<int, int> Z;
//	std::pair<int, int> int100;
//	std::pair<int, int> int10;
//	std::pair<int, int> int1;
//
//	//Variables that alter the equation
//	fraction coprime_pair = { 0, 0 };
//	int k = 0, x_val = 0, y_val = 0, z_val = 0, int_val = 0;
//
//	GoverningEquation(std::pair<int, int> x, std::pair<int, int> y, std::pair<int, int> z, std::pair<int, int> one, std::pair<int, int> two, std::pair<int, int> three)
//	{
//		X = x;
//		Y = y;
//		Z = z;
//		int1 = one;
//		int10 = two;
//		int100 = three;
//	}
//
//	void updateValues()
//	{
//		//Calculates x_val, y_val, z_val and int_val based on the pairs that 
//		//define the equation as well as the current multiple of the  coprime_pair (k * coprime_pair).
//		//Checks should be made elsewhere to make sure that k * coprime pair leads to the right
//		//number of digits.
//
//		//X goes on the left side of the equation
//		//There will always be an X so no need to check for 0 value
//		x_val = coprime_pair.denominator * X.first - coprime_pair.numerator * X.second;
//
//		if (Y.first == 0) y_val = 0;
//		else y_val = coprime_pair.numerator * Y.second - coprime_pair.denominator * Y.first;
//
//		if (Z.first == 0) z_val = 0;
//		else z_val = coprime_pair.numerator * Z.second - coprime_pair.denominator * Z.first;
//
//		//There will always be an int1 value so no need to check for 0 value
//		int int1_val = coprime_pair.numerator * int1.second * (k * coprime_pair.denominator % 10) - coprime_pair.denominator * int1.first * (k * coprime_pair.numerator % 10);
//
//		int int10_val = 0;
//		if (int10.first != 0)
//			int10_val = coprime_pair.numerator * int10.second * (k * coprime_pair.denominator / 10 % 10) - coprime_pair.denominator * int10.first * (k * coprime_pair.numerator / 10 % 10);
//
//		int int100_val = 0;
//		if (int100.first != 0)
//			int100_val = coprime_pair.numerator * int100.second * (k * coprime_pair.denominator / 100) - coprime_pair.denominator * int100.first * (k * coprime_pair.numerator / 100);
//
//		int_val = int1_val + int10_val + int100_val;
//
//		//std::cout << x_val << "X = " << y_val << "Y + " << z_val << "Z + " << int_val << std::endl;
//	}
//
//	std::vector<std::pair<int, int> > findIntegerSolutions(int minimum_value)
//	{
//		//find integer solutions to the governing equation. 
//		int potential_numerator = 0, potential_denominator = 0;
//		std::vector<std::pair<int, int> > integer_solutions;
//		if (x_val == 0) return integer_solutions;
//
//		//Even if the governing equation doesn't have a Y or Z component we need to get to the inner portion 
//		//of the loop at least once to make sure we solve for X. If there are Y and Z components they can be 
//		//any integer value from 1 to 9.
//		int y_max = (Y.first == 0) ? 1 : 9;
//		int z_max = (Z.first == 0) ? 1 : 9;
//
//		for (int y = 1; y <= y_max; y++)
//		{
//			for (int z = 1; z <= z_max; z++)
//			{
//				//calculate the value of x given the values of y, z and int_val
//				if ((y * y_val + z * z_val + int_val) % x_val == 0)
//				{
//					int x = (y * y_val + z * z_val + int_val) / x_val;
//					if (x > 0 && x < 10)
//					{
//						//This value of x works, however, we need to make sure it isn't one of the trivial cases before accepting it
//						potential_numerator = x * X.first + y * Y.first + z * Z.first + int1.first * (k * coprime_pair.numerator % 10) + int10.first * (k * coprime_pair.numerator / 10 % 10) + int100.first * (k * coprime_pair.numerator / 100);
//						potential_denominator = x * X.second + y * Y.second + z * Z.second + int1.second * (k * coprime_pair.denominator % 10) + int10.second * (k * coprime_pair.denominator / 10 % 10) + int100.second * (k * coprime_pair.denominator / 100);
//
//						if (potential_numerator >= minimum_value && potential_denominator >= minimum_value) integer_solutions.push_back({ potential_numerator, potential_denominator });
//					}
//				}
//			}
//		}
//
//		return integer_solutions;
//	}
//
//	void printFraction()
//	{
//		//first find the maximum value in the fraction
//		int max = 0, num = 0, den = 0;
//		if (X.first > max) max = X.first;
//		if (Y.first > max) max = Y.first;
//		if (Z.first > max) max = Z.first;
//		if (int1.first > max) max = int1.first;
//		if (int10.first > max) max = int10.first;
//		if (int100.first > max) max = int100.first;
//
//		num = max;
//		den = max;
//
//		while (num > 0)
//		{
//			if (X.first == num) std::cout << 'X';
//			else if (Y.first == num) std::cout << 'Y';
//			else if (Z.first == num) std::cout << 'Z';
//			else std::cout << '#';
//			num /= 10;
//		}
//		std::cout << "/";
//		while (den > 0)
//		{
//			if (X.second == den) std::cout << 'X';
//			else if (Y.second == den) std::cout << 'Y';
//			else if (Z.second == den) std::cout << 'Z';
//			else std::cout << '#';
//			den /= 10;
//		}
//		std::cout << std::endl;
//	}
//};
//
//void recursiveEquationTransverse(int variable_number, int integer_number, std::vector<int>& used_powers_numerator, std::vector<int>& used_powers_denominator, std::vector<std::pair<int, int>>& ten_power_groups,
//	std::vector<std::pair<int, int> >& variables, std::vector<std::pair<int, int> >& integers, std::vector<GoverningEquation>& equations, int location = 0, int level = 0)
//{
//	if (level == variable_number)
//	{
//		//we've added all the variables we need, iterate through the used_power vectors to figure out which 
//		//integer values to add
//		int index_n = 0, index_d = 0;
//		for (int i = 0; i < integer_number; i++)
//		{
//			//numerator first
//			for (; index_n < used_powers_numerator.size(); index_n++)
//			{
//				if (used_powers_numerator[index_n])
//				{
//					//we use the smallest number we come across
//					integers[i].first = used_powers_numerator[index_n++]; //don't set element at index to 0, just skip it on next pass
//					break;
//				}
//			}
//
//			//denominator second
//			for (; index_d < used_powers_denominator.size(); index_d++)
//			{
//				if (used_powers_denominator[index_d])
//				{
//					//we use the smallest number we come across
//					integers[i].second = used_powers_denominator[index_d++]; //don't set element at index to 0, just skip it on next pass
//					break;
//				}
//			}
//		}
//
//		//Create a new equation from the variables and integers vectors and add it to the equations vector
//		GoverningEquation equation(variables[0], variables[1], variables[2], integers[0], integers[1], integers[2]);
//		equations.push_back(equation);
//		return;
//	}
//
//	for (int i = location; i < ten_power_groups.size(); i++)
//	{
//		//Check to see if either of the numerator or denominator value in ten_power_groups[i] has been used yet.
//		//If neither has been used then this is a valid group
//		if (used_powers_numerator[log10(ten_power_groups[i].first)] && used_powers_denominator[log10(ten_power_groups[i].second)])
//		{
//			//set the variable
//			variables[level] = ten_power_groups[i];
//
//			//update used vectors to reflect this variable
//			used_powers_numerator[log10(ten_power_groups[i].first)] = 0;
//			used_powers_denominator[log10(ten_power_groups[i].second)] = 0;
//
//			//recursively go down to next level
//			recursiveEquationTransverse(variable_number, integer_number, used_powers_numerator, used_powers_denominator, ten_power_groups, variables, integers, equations, i + 1, level + 1);
//
//			//remove current variable from used vectors after returning from recursion
//			used_powers_numerator[log10(ten_power_groups[i].first)] = ten_power_groups[i].first;
//			used_powers_denominator[log10(ten_power_groups[i].second)] = ten_power_groups[i].second;
//		}
//	}
//}
//
//std::vector<GoverningEquation> createEquations(int total_digits, int removed_digits)
//{
//	//The number of digits being removed will be equal to the total number of variables in our equation.
//	//i.e. 1 digit removed gives us X, 3 digits removed gives us X, Y and Z.
//	int integer_digits = total_digits - removed_digits;
//	std::vector<GoverningEquation> equations;
//	std::vector<std::pair<int, int>> variables = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
//	std::vector<std::pair<int, int>> integers = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
//	std::vector<std::pair<int, int>> ten_power_groups;
//
//	int ten_pow = 1;
//	std::vector<int> ten_powers;
//	for (int i = 0; i < total_digits; i++)
//	{
//		ten_powers.push_back(ten_pow);
//		ten_pow *= 10;
//	}
//
//	//create all ways to choose two powers of 10 up to the limit, order matters here
//	for (int i = 0; i < ten_powers.size(); i++)
//	{
//		for (int j = 0; j < ten_powers.size(); j++)
//		{
//			ten_power_groups.push_back({ ten_powers[i], ten_powers[j] });
//		}
//	}
//
//	//choose all of the ways to pick 'removed_digit' number of groups from the 
//	//ten_power_groups vector such that there's no overlap between values. This is 
//	//ugly and could've been made recursive, but since there's only 1, 2 or 3 variables
//	//it was easier to just write it like this.
//	std::vector<int> used_powers_numerator = ten_powers, used_powers_denominator = ten_powers; //used to keep track of which 'places' have been used
//	recursiveEquationTransverse(removed_digits, integer_digits, used_powers_numerator, used_powers_denominator, ten_power_groups, variables, integers, equations);
//
//	return equations;
//}
//
//std::pair<int, int> reduceFractionTest(std::vector<int> numerator, std::vector<int> denominator)
//{
//	int num = 1, den = 1;
//	for (int i = 0; i < denominator.size(); i++)
//	{
//		for (int j = 0; j < numerator.size(); j++)
//		{
//			if (numerator[j] == denominator[i])
//			{
//				numerator[j] = 1;
//				denominator[i] = 1;
//				break;
//			}
//		}
//	}
//
//	for (int i = 0; i < denominator.size(); i++) den *= denominator[i];
//	for (int i = 0; i < numerator.size(); i++) num *= numerator[i];
//
//	return { num, den };
//}
//
//void recursiveBruteForceAllEquations(int total_numbers, int numbers_to_remove, std::vector<int>& sub_set, std::vector<std::vector<int> >& sub_sets, int used = 0, int level = 0)
//{
//	//need all unordered subsets of choose(total_numbers, numbers_to_remove).
//	//For example (3, 2) would give the following subsets: (0, 1), (0, 2), (1, 0), (1, 2), (2, 0) and (2, 1).
//	//We can't choose the same number twice so (0, 0), (1, 1) and (2, 2) are excluded
//	if (level == numbers_to_remove)
//	{
//		//base case, add the subset to overall list of subsets
//		sub_sets.push_back(sub_set);
//		return;
//	}
//
//	for (int i = 0; i < total_numbers; i++)
//	{
//		if (!(used & powers_of_two[i]))
//		{
//			
//			//haven't used this index yet
//			sub_set.push_back(i);
//			recursiveBruteForceAllEquations(total_numbers, numbers_to_remove, sub_set, sub_sets, used | powers_of_two[i], level + 1);
//			sub_set.pop_back();
//		}
//	}
//}
//
//bool bruteForceCommonDigits(int numerator, int denominator, std::vector<int> num_indices, std::vector<int> den_indices)
//{
//	//check to see if the digits at the specified locations are the same in the numerator and denominator
//	for (int i = 0; i < num_indices.size(); i++)
//	{
//		int nume = numerator / MyPow(10, num_indices[i]) % 10;
//		int den = denominator / MyPow(10, den_indices[i]) % 10;
//		if (nume != den || nume == 0) return false;
//	}
//	return true;
//}
//
//std::pair<int, int> bruteForceCancelDigits(int numerator, int denominator, std::vector<int> num_cancel, std::vector<int> den_cancel)
//{
//	int new_num = 0, new_den = 0, mult = 1, stop = log10(numerator);
//	for (int i = 0; i <= stop; i++)
//	{
//		bool add = true;
//		for (int j = 0; j < num_cancel.size(); j++)
//		{
//			if (num_cancel[j] == i)
//			{
//				add = false;
//				break;
//			}
//		}
//		if (add)
//		{
//			new_num += (numerator % 10) * mult;
//			mult *= 10;
//		}
//		numerator /= 10;
//	}
//
//	mult = 1;
//	for (int i = 0; i <= stop; i++)
//	{
//		bool add = true;
//		for (int j = 0; j < den_cancel.size(); j++)
//		{
//			if (den_cancel[j] == i)
//			{
//				add = false;
//				break;
//			}
//		}
//		if (add)
//		{
//			new_den += (denominator % 10) * mult;
//			mult *= 10;
//		}
//		denominator /= 10;
//	}
//
//	return { new_num, new_den };
//}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	std::map<int, std::vector<int>> winning_fractions; //used to make sure we don't double count
	int answer = 0, number_length = 2, removed_digits = 1;
	fraction final_fraction = { 0, 0 };

	////New Brute Force
	//std::vector<int> sub_set;
	//std::vector<std::vector<int> > sub_sets;

	//recursiveBruteForceAllEquations(number_length, removed_digits, sub_set, sub_sets);

	//std::vector<std::vector<int> > prime_factorizations = { {0}, {1} };
	//for (int i = 2; i < MyPow(10, number_length); i++) prime_factorizations.push_back(PrimeFactors(i));

	//for (int i = 0; i < sub_sets.size(); i++)
	//{
	//	for (int j = 0; j < sub_sets.size(); j++)
	//	{
	//		std::cout << "Equations " << i << ", " << j << std::endl;
	//		//sub_sets[i] is the equation for our numerator and sub_sets[j] is the equation for the denominator
	//		//For each combination of equations we need to try every possible fraction for removal of numbers.
	//		for (int num = MyPow(10, number_length - 1); num < MyPow(10, number_length); num++)
	//		{
	//			for (int den = num + 1; den < MyPow(10, number_length); den++)
	//			{
	//				//see if we can actually cancel out the appropriate digits
	//				if (bruteForceCommonDigits(num, den, sub_sets[i], sub_sets[j]))
	//				{
	//					//The reduced form of the fraction before cancelling any digits
	//					std::pair<int, int> original_fraction = reduceFractionTest(prime_factorizations[num], prime_factorizations[den]);
	//					std::pair<int, int> reduced_fraction = bruteForceCancelDigits(num, den, sub_sets[i], sub_sets[j]);

	//					if (original_fraction == reduceFractionTest(prime_factorizations[reduced_fraction.first], prime_factorizations[reduced_fraction.second]))
	//					{
	//						if (winning_fractions.find(num) != winning_fractions.end())
	//						{
	//							//make sure the denominator doesn't exist already
	//							if (std::find(winning_fractions[num].begin(), winning_fractions[num].end(), den) == winning_fractions[num].end())
	//							{
	//								winning_fractions[num].push_back(den);
	//								answer++;
	//								final_fraction.numerator += num;
	//								final_fraction.denominator += den;
	//								std::cout << num << "/" << den << std::endl;
	//							}
	//						}
	//						else
	//						{
	//							std::vector<int> new_numerator = { den };
	//							winning_fractions.insert({ num, new_numerator });
	//							answer++;
	//							final_fraction.numerator += num;
	//							final_fraction.denominator += den;
	//							std::cout << num << "/" << den << std::endl;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	////Good Approach
	//std::vector<fraction> coprime_pairs;

	//int lower_limit = MyPow(10, number_length - removed_digits - 1);
	//int upper_limit = MyPow(10, number_length - removed_digits) - 1;

	//FaraySequence(upper_limit, coprime_pairs);
	//std::vector<GoverningEquation> equations = createEquations(number_length, removed_digits);

	////Try all multiples of each of the coprime pairs against every governing equation
	//for (int i = 0; i < coprime_pairs.size(); i++)
	//{
	//	int k = 0;
	//	while (true)
	//	{
	//		k++;
	//		//if (k * coprime_pairs[i].numerator < lower_limit || k * coprime_pairs[i].denominator < lower_limit) continue; //increase k until both coprime numbers are the appropriate length
	//		if (k * coprime_pairs[i].numerator > upper_limit || k * coprime_pairs[i].denominator > upper_limit) break; //can't use this coprime pair anymore so go to next

	//		for (int j = 0; j < equations.size(); j++)
	//		{
	//			//update the equation with the appropriate k value and coprime pair
	//			equations[j].coprime_pair = coprime_pairs[i];
	//			equations[j].k = k;
	//			equations[j].updateValues();

	//			//after updating get all integer solutions to the new equation
	//			std::vector<std::pair<int, int> > solutions = equations[j].findIntegerSolutions(MyPow(10, number_length - 1));

	//			for (int s = 0; s < solutions.size(); s++)
	//			{
	//				//std::cout << solutions[s].first << "/" << solutions[s].second << " = " << k * coprime_pairs[i].numerator << "/" << k * coprime_pairs[i].denominator << std::endl;
	//				//std::cout << j << ", " << k << std::endl;
	//				//equations[j].printFraction();

	//				if (winning_fractions.find(solutions[s].first) != winning_fractions.end())
	//				{
	//					//make sure the denominator doesn't exist already
	//					if (std::find(winning_fractions[solutions[s].first].begin(), winning_fractions[solutions[s].first].end(), solutions[s].second) == winning_fractions[solutions[s].first].end())
	//					{
	//						winning_fractions[solutions[s].first].push_back(solutions[s].second);
	//						answer++;
	//						final_fraction.numerator += solutions[s].first;
	//						final_fraction.denominator += solutions[s].second;
	//						//std::cout << solutions[s].first << "/" << solutions[s].second << " = " << k * coprime_pairs[i].numerator << "/" << k * coprime_pairs[i].denominator << std::endl;
	//					}
	//				}
	//				else
	//				{
	//					std::vector<int> new_numerator = { solutions[s].second };
	//					winning_fractions.insert({ solutions[s].first, new_numerator });
	//					answer++;
	//					final_fraction.numerator += solutions[s].first;
	//					final_fraction.denominator += solutions[s].second;
	//					//std::cout << solutions[s].first << "/" << solutions[s].second << " = " << k * coprime_pairs[i].numerator << "/" << k * coprime_pairs[i].denominator << std::endl;
	//				}
	//			}
	//		}
	//	}
	//}

	//std::cout << "Final fraction is " << final_fraction.numerator << " " << final_fraction.denominator << std::endl;
	

	////Old Brute Force Approach
	//std::vector<std::vector<int> > prime_factorizations = { {0}, {1} };
	//int hits = 0;

	//for (int i = 2; i < 10000; i++) prime_factorizations.push_back(PrimeFactors(i));

	//for (int i = 1001; i < 10000; i++)
	//{
	//	for (int j = i + 1; j < 10000; j++)
	//	{
	//		bool overlapping_digit = false;
	//		std::vector<int> nnums, ddens;

	//		/*int i_digits[3] = {i / 100, i / 10 % 10, i % 10};
	//		int j_digits[3] = { j / 100, j / 10 % 10, j % 10 };*/
	//		int i_digits[4] = { i / 1000, i / 100 % 10, i / 10 % 10, i % 10 };
	//		int j_digits[4] = { j / 1000, j / 100 % 10, j / 10 % 10, j % 10 };

	//		if (i_digits[0] == j_digits[0]) //equation 1
	//		{
	//			if (i_digits[0]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i % 1000);
	//				ddens.push_back(j % 1000);
	//			}
	//		}
	//		if (i_digits[0] == j_digits[1]) //equation 2
	//		{
	//			if (i_digits[0]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i % 1000);
	//				ddens.push_back(100 * j_digits[0] + j % 100);
	//			}
	//		}
	//		if (i_digits[0] == j_digits[2]) //equation 3
	//		{
	//			if (i_digits[0]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i % 1000);
	//				ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
	//			}
	//		}
	//		if (i_digits[0] == j_digits[3]) //equation 4
	//		{
	//			if (i_digits[0]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i % 1000);
	//				ddens.push_back(j / 10);
	//			}
	//		}
	//		if (i_digits[1] == j_digits[0]) //equation 5
	//		{
	//			if (i_digits[1]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + i % 100);
	//				ddens.push_back(j % 1000);
	//			}
	//		}
	//		if (i_digits[1] == j_digits[1]) //equation 6
	//		{
	//			if (i_digits[1]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + i % 100);
	//				ddens.push_back(100 * j_digits[0] + j % 100);
	//			}
	//		}
	//		if (i_digits[1] == j_digits[2]) //equation 7
	//		{
	//			if (i_digits[1]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + i % 100);
	//				ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
	//			}
	//		}
	//		if (i_digits[1] == j_digits[3]) //equation 8
	//		{
	//			if (i_digits[1]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + i % 100);
	//				ddens.push_back(j / 10);
	//			}
	//		}
	//		if (i_digits[2] == j_digits[0]) //equation 9
	//		{
	//			if (i_digits[2]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
	//				ddens.push_back(j % 1000);
	//			}
	//		}
	//		if (i_digits[2] == j_digits[1]) //equation 10
	//		{
	//			if (i_digits[2]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
	//				ddens.push_back(100 * j_digits[0] + j % 100);
	//			}
	//		}
	//		if (i_digits[2] == j_digits[2]) //equation 11
	//		{
	//			if (i_digits[2]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
	//				ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
	//			}
	//		}
	//		if (i_digits[2] == j_digits[3]) //equation 12
	//		{
	//			if (i_digits[2]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
	//				ddens.push_back(j / 10);
	//			}
	//		}
	//		if (i_digits[3] == j_digits[0]) //equation 13
	//		{
	//			if (i_digits[3]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i / 10);
	//				ddens.push_back(j % 1000);
	//			}
	//		}
	//		if (i_digits[3] == j_digits[1]) //equation 14
	//		{
	//			if (i_digits[3]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i / 10);
	//				ddens.push_back(100 * j_digits[0] + j % 100);
	//			}
	//		}
	//		if (i_digits[3] == j_digits[2]) //equation 15
	//		{
	//			if (i_digits[3]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i / 10);
	//				ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
	//			}
	//		}
	//		if (i_digits[3] == j_digits[3]) //equation 16
	//		{
	//			if (i_digits[3]) //can't remove 0's
	//			{
	//				overlapping_digit = 1;
	//				nnums.push_back(i / 10);
	//				ddens.push_back(j / 10);
	//			}
	//		}

	//		if (overlapping_digit)
	//		{
	//			std::pair<int, int> original_fraction = reduceFractionTest(prime_factorizations[i], prime_factorizations[j]);
	//			
	//			for (int w = 0; w < nnums.size(); w++)
	//			{
	//				std::pair<int, int> new_fraction = reduceFractionTest(prime_factorizations[nnums[w]], prime_factorizations[ddens[w]]);

	//				if ((original_fraction.first == new_fraction.first) & (original_fraction.second == new_fraction.second))
	//				{
	//					if (winning_fractions.find(i) != winning_fractions.end())
	//					{
	//						//make sure the denominator doesn't exist already
	//						if (std::find(winning_fractions[i].begin(), winning_fractions[i].end(), j) == winning_fractions[i].end())
	//						{
	//							winning_fractions[i].push_back(j);
	//							hits++;
	//							final_fraction.numerator += i;
	//							final_fraction.denominator += j;

	//							std::cout << i << '/' << j << " = " << nnums[w] << '/' << ddens[w] << std::endl;
	//						}
	//					}
	//					else
	//					{
	//						std::vector<int> new_numerator = { j };
	//						winning_fractions.insert({ i, new_numerator });
	//						hits++;
	//						final_fraction.numerator += i;
	//						final_fraction.denominator += j;

	//						std::cout << i << '/' << j << " = " << nnums[w] << '/' << ddens[w] << std::endl;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//std::cout << final_fraction.numerator << " " << final_fraction.denominator << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//N = 3, r = 1 brute force test cases
/*
if (i_digits[0] == j_digits[0]) //equation 1
			{
				overlapping_digit = 1;
				nnums.push_back(i % 100);
				ddens.push_back(j % 100);
				equations_used[0] = true;
			}
			if (i_digits[0] == j_digits[1]) //equation 2
			{
				overlapping_digit = 1;
				nnums.push_back(i % 100);
				ddens.push_back(10 * j_digits[0] + j_digits[2]);
				equations_used[0] = true;
			}
			if (i_digits[0] == j_digits[2]) //equation 3
			{
				overlapping_digit = 1;
				nnums.push_back(i % 100);
				ddens.push_back(j / 10);
				equations_used[0] = true;
			}
			if (i_digits[1] == j_digits[0]) //equation 4
			{
				overlapping_digit = 1;
				nnums.push_back(10 * i_digits[0] + i_digits[2]);
				ddens.push_back(j % 100);
				equations_used[0] = true;
			}
			if (i_digits[1] == j_digits[1]) //equation 5
			{
				overlapping_digit = 1;
				nnums.push_back(10 * i_digits[0] + i_digits[2]);
				ddens.push_back(10 * j_digits[0] + j_digits[2]);
				equations_used[0] = true;
			}
			if (i_digits[1] == j_digits[2]) //equation 6
			{
				overlapping_digit = 1;
				nnums.push_back(10 * i_digits[0] + i_digits[2]);
				ddens.push_back(j / 100);
				equations_used[0] = true;
			}
			if (i_digits[2] == j_digits[0]) //equation 7
			{
				overlapping_digit = 1;
				nnums.push_back(i / 10);
				ddens.push_back(j % 100);
				equations_used[0] = true;
			}
			if (i_digits[2] == j_digits[1]) //equation 1
			{
				overlapping_digit = 1;
				nnums.push_back(i / 10);
				ddens.push_back(10 * j_digits[0] + j_digits[2]);
				equations_used[0] = true;
			}
			if (i_digits[2] == j_digits[2]) //equation 1
			{
				overlapping_digit = 1;
				nnums.push_back(i / 10);
				ddens.push_back(j / 10);
				equations_used[0] = true;
			}
*/

//N = 3, r = 2 brute force test cases
/*
if ((i_digits[0] == j_digits[1]) && (i_digits[1] == j_digits[0])) //equation 1
			{
				if (i_digits[0] && i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[2]);
					ddens.push_back(j_digits[2]);
				}
			}
			if ((i_digits[0] == j_digits[2]) && (i_digits[1] == j_digits[0])) //equation 2
			{
				if (i_digits[0] && i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[2]);
					ddens.push_back(j_digits[1]);
				}
			}
			if ((i_digits[0] == j_digits[0]) && (i_digits[2] == j_digits[1])) //equation 3
			{
				if (i_digits[0] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[1]);
					ddens.push_back(j_digits[2]);
				}
			}
			if ((i_digits[0] == j_digits[2]) && (i_digits[2] == j_digits[1])) //equation 4
			{
				if (i_digits[0] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[1]);
					ddens.push_back(j_digits[0]);
				}
			}
			if ((i_digits[0] == j_digits[1]) && (i_digits[2] == j_digits[0])) //equation 5
			{
				if (i_digits[0] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[1]);
					ddens.push_back(j_digits[2]);
				}
			}
			if ((i_digits[0] == j_digits[2]) && (i_digits[2] == j_digits[0])) //equation 6
			{
				if (i_digits[0] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[1]);
					ddens.push_back(j_digits[1]);
				}
			}
			if ((i_digits[1] == j_digits[0]) && (i_digits[2] == j_digits[2])) //equation 7
			{
				if (i_digits[1] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[0]);
					ddens.push_back(j_digits[1]);
				}
			}
			if ((i_digits[1] == j_digits[1]) && (i_digits[2] == j_digits[0])) //equation 8
			{
				if (i_digits[1] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[0]);
					ddens.push_back(j_digits[2]);
				}
			}
			if ((i_digits[1] == j_digits[2]) && (i_digits[2] == j_digits[0])) //equation 9
			{
				if (i_digits[1] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[0]);
					ddens.push_back(j_digits[1]);
				}
			}
			if ((i_digits[1] == j_digits[2]) && (i_digits[2] == j_digits[1])) //equation 10
			{
				if (i_digits[1] && i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i_digits[0]);
					ddens.push_back(j_digits[0]);
				}
			}
*/

//N = 4, r = 1 brute force test cases
/*
if (i_digits[0] == j_digits[0]) //equation 1
			{
				if (i_digits[0]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i % 1000);
					ddens.push_back(j % 1000);
				}
			}
			if (i_digits[0] == j_digits[1]) //equation 2
			{
				if (i_digits[0]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i % 1000);
					ddens.push_back(100 * j_digits[0] + j % 100);
				}
			}
			if (i_digits[0] == j_digits[2]) //equation 3
			{
				if (i_digits[0]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i % 1000);
					ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
				}
			}
			if (i_digits[0] == j_digits[3]) //equation 4
			{
				if (i_digits[0]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i % 1000);
					ddens.push_back(j / 10);
				}
			}
			if (i_digits[1] == j_digits[0]) //equation 5
			{
				if (i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + i % 100);
					ddens.push_back(j % 1000);
				}
			}
			if (i_digits[1] == j_digits[1]) //equation 6
			{
				if (i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + i % 100);
					ddens.push_back(100 * j_digits[0] + j % 100);
				}
			}
			if (i_digits[1] == j_digits[2]) //equation 7
			{
				if (i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + i % 100);
					ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
				}
			}
			if (i_digits[1] == j_digits[3]) //equation 8
			{
				if (i_digits[1]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + i % 100);
					ddens.push_back(j / 10);
				}
			}
			if (i_digits[2] == j_digits[0]) //equation 9
			{
				if (i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
					ddens.push_back(j % 1000);
				}
			}
			if (i_digits[2] == j_digits[1]) //equation 10
			{
				if (i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
					ddens.push_back(100 * j_digits[0] + j % 100);
				}
			}
			if (i_digits[2] == j_digits[2]) //equation 11
			{
				if (i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
					ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
				}
			}
			if (i_digits[2] == j_digits[3]) //equation 12
			{
				if (i_digits[2]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(100 * i_digits[0] + 10 * i_digits[1] + i_digits[3]);
					ddens.push_back(j / 10);
				}
			}
			if (i_digits[3] == j_digits[0]) //equation 13
			{
				if (i_digits[3]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i / 10);
					ddens.push_back(j % 1000);
				}
			}
			if (i_digits[3] == j_digits[1]) //equation 14
			{
				if (i_digits[3]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i / 10);
					ddens.push_back(100 * j_digits[0] + j % 100);
				}
			}
			if (i_digits[3] == j_digits[2]) //equation 15
			{
				if (i_digits[3]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i / 10);
					ddens.push_back(100 * j_digits[0] + 10 * j_digits[1] + j_digits[3]);
				}
			}
			if (i_digits[3] == j_digits[3]) //equation 16
			{
				if (i_digits[3]) //can't remove 0's
				{
					overlapping_digit = 1;
					nnums.push_back(i / 10);
					ddens.push_back(j / 10);
				}
			}
*/