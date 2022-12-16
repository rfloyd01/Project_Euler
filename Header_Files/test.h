#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <map>
#include <algorithm>

class GoverningEquation
{
public:
	//Variables defining the equation
	std::pair<int, int> X;
	std::pair<int, int> Y;
	std::pair<int, int> Z;
	std::pair<int, int> int100;
	std::pair<int, int> int10;
	std::pair<int, int> int1;

	//Variables that alter the equation
	fraction coprime_pair = { 0, 0 };
	int k = 0, x_val = 0, y_val = 0, z_val = 0, int_val = 0;

	GoverningEquation(std::pair<int, int> x, std::pair<int, int> y, std::pair<int, int> z, std::pair<int, int> one, std::pair<int, int> two, std::pair<int, int> three)
	{
		X = x;
		Y = y;
		Z = z;
		int1 = one;
		int10 = two;
		int100 = three;
	}

	void updateValues()
	{
		//Calculates x_val, y_val, z_val and int_val based on the pairs that 
		//define the equation as well as the current multiple of the  coprime_pair (k * coprime_pair).
		//Checks should be made elsewhere to make sure that k * coprime pair leads to the right
		//number of digits.

		//X goes on the left side of the equation
		//There will always be an X so no need to check for 0 value
		x_val = coprime_pair.denominator * X.first - coprime_pair.numerator * X.second;

		if (Y.first == 0) y_val = 0;
		else y_val = coprime_pair.numerator * Y.second - coprime_pair.denominator * Y.first;

		if (Z.first == 0) z_val = 0;
		else z_val = coprime_pair.numerator * Z.second - coprime_pair.denominator * Z.first;

		//There will always be an int1 value so no need to check for 0 value
		int int1_val = coprime_pair.numerator * int1.second * (k * coprime_pair.denominator % 10) - coprime_pair.denominator * int1.first * (k * coprime_pair.numerator % 10);

		int int10_val = 0;
		if (int10.first != 0)
			int10_val = coprime_pair.numerator * int10.second * (k * coprime_pair.denominator / 10 % 10) - coprime_pair.denominator * int10.first * (k * coprime_pair.numerator / 10 % 10);

		int int100_val = 0;
		if (int100.first != 0)
			int100_val = coprime_pair.numerator * int100.second * (k * coprime_pair.denominator / 100) - coprime_pair.denominator * int100.first * (k * coprime_pair.numerator / 100);

		int_val = int1_val + int10_val + int100_val;

		//std::cout << x_val << "X = " << y_val << "Y + " << z_val << "Z + " << int_val << std::endl;
	}

	std::vector<std::pair<int, int> > findIntegerSolutions(int minimum_value)
	{
		//find integer solutions to the governing equation. 
		int potential_numerator = 0, potential_denominator = 0;
		std::vector<std::pair<int, int> > integer_solutions;

		//Even if the governing equation doesn't have a Y or Z component we need to get to the inner portion 
		//of the loop at least once to make sure we solve for X. If there are Y and Z components they can be 
		//any integer value from 0 to 9.
		int y_max = (Y.first == 0) ? 0 : 9;
		int z_max = (Z.first == 0) ? 0 : 9;

		for (int y = 0; y <= y_max; y++)
		{
			for (int z = 0; z <= z_max; z++)
			{
				//calculate the value of x given the values of y, z and int_val
				if ((y * y_val + z * z_val + int_val) % x_val == 0)
				{
					int x = (y * y_val + z * z_val + int_val) / x_val;
					if (x > -1 && x < 10)
					{
						//This value of x works, however, we need to make sure it isn't one of the trivial cases before accepting it
						potential_numerator = x * X.first + y * Y.first + z * Z.first + int1.first * (k * coprime_pair.numerator % 10) + int10.first * (k * coprime_pair.numerator / 10 % 10) + int100.first * (k * coprime_pair.numerator / 100);
						potential_denominator = x * X.second + y * Y.second + z * Z.second + int1.second * (k * coprime_pair.denominator % 10) + int10.second * (k * coprime_pair.denominator / 10 % 10) + int100.second * (k * coprime_pair.denominator / 100);

						if (potential_numerator % 10 != 0 || potential_denominator % 10 != 0)
						{
							if (potential_numerator >= minimum_value && potential_denominator >= minimum_value)
							{
								//we've found a hit! just print for now
								integer_solutions.push_back({ potential_numerator, potential_denominator });
							}
						}
					}
				}
			}
		}

		return integer_solutions;
	}

	void printFraction()
	{
		//first find the maximum value in the fraction
		int max = 0, num = 0, den = 0;
		if (X.first > max) max = X.first;
		if (Y.first > max) max = Y.first;
		if (Z.first > max) max = Z.first;
		if (int1.first > max) max = int1.first;
		if (int10.first > max) max = int10.first;
		if (int100.first > max) max = int100.first;

		num = max;
		den = max;

		while (num > 0)
		{
			if (X.first == num) std::cout << 'X';
			else if (Y.first == num) std::cout << 'Y';
			else if (Z.first == num) std::cout << 'Z';
			else std::cout << '#';
			num /= 10;
		}
		std::cout << "/";
		while (den > 0)
		{
			if (X.second == den) std::cout << 'X';
			else if (Y.second == den) std::cout << 'Y';
			else if (Z.second == den) std::cout << 'Z';
			else std::cout << '#';
			den /= 10;
		}
		std::cout << std::endl;
	}
};

void recursiveEquationTransverse(int variable_number, int integer_number, std::vector<int>& used_powers_numerator, std::vector<int>& used_powers_denominator, std::vector<std::pair<int, int>>& ten_power_groups,
	std::vector<std::pair<int, int> >& variables, std::vector<std::pair<int, int> >& integers, std::vector<GoverningEquation>& equations, int location = 0, int level = 0)
{
	if (level == variable_number)
	{
		//we've added all the variables we need, iterate through the used_power vectors to figure out which 
		//integer values to add
		int index_n = 0, index_d = 0;
		for (int i = 0; i < integer_number; i++)
		{
			//numerator first
			for (; index_n < used_powers_numerator.size(); index_n++)
			{
				if (used_powers_numerator[index_n])
				{
					//we use the smallest number we come across
					integers[i].first = used_powers_numerator[index_n++]; //don't set element at index to 0, just skip it on next pass
					break;
				}
			}

			//denominator second
			for (; index_d < used_powers_denominator.size(); index_d++)
			{
				if (used_powers_denominator[index_d])
				{
					//we use the smallest number we come across
					integers[i].second = used_powers_denominator[index_d++]; //don't set element at index to 0, just skip it on next pass
					break;
				}
			}
		}

		//Create a new equation from the variables and integers vectors and add it to the equations vector
		GoverningEquation equation(variables[0], variables[1], variables[2], integers[0], integers[1], integers[2]);
		equations.push_back(equation);
		return;
	}

	for (int i = location; i < ten_power_groups.size(); i++)
	{
		//Check to see if either of the numerator or denominator value in ten_power_groups[i] has been used yet.
		//If neither has been used then this is a valid group
		if (used_powers_numerator[log10(ten_power_groups[i].first)] && used_powers_denominator[log10(ten_power_groups[i].second)])
		{
			//set the variable
			variables[level] = ten_power_groups[i];

			//update used vectors to reflect this variable
			used_powers_numerator[log10(ten_power_groups[i].first)] = 0;
			used_powers_denominator[log10(ten_power_groups[i].second)] = 0;

			//recursively go down to next level
			recursiveEquationTransverse(variable_number, integer_number, used_powers_numerator, used_powers_denominator, ten_power_groups, variables, integers, equations, i + 1, level + 1);

			//remove current variable from used vectors after returning from recursion
			used_powers_numerator[log10(ten_power_groups[i].first)] = ten_power_groups[i].first;
			used_powers_denominator[log10(ten_power_groups[i].second)] = ten_power_groups[i].second;
		}
	}
}

std::vector<GoverningEquation> createEquations(int total_digits, int removed_digits)
{
	//The number of digits being removed will be equal to the total number of variables in our equation.
	//i.e. 1 digit removed gives us X, 3 digits removed gives us X, Y and Z.
	int integer_digits = total_digits - removed_digits;
	std::vector<GoverningEquation> equations;
	std::vector<std::pair<int, int>> variables = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
	std::vector<std::pair<int, int>> integers = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
	std::vector<std::pair<int, int>> ten_power_groups;

	int ten_pow = 1;
	std::vector<int> ten_powers;
	for (int i = 0; i < total_digits; i++)
	{
		ten_powers.push_back(ten_pow);
		ten_pow *= 10;
	}

	//create all ways to choose two powers of 10 up to the limit, order matters here
	for (int i = 0; i < ten_powers.size(); i++)
	{
		for (int j = 0; j < ten_powers.size(); j++)
		{
			ten_power_groups.push_back({ ten_powers[i], ten_powers[j] });
		}
	}

	//choose all of the ways to pick 'removed_digit' number of groups from the 
	//ten_power_groups vector such that there's no overlap between values. This is 
	//ugly and could've been made recursive, but since there's only 1, 2 or 3 variables
	//it was easier to just write it like this.
	std::vector<int> used_powers_numerator = ten_powers, used_powers_denominator = ten_powers; //used to keep track of which 'places' have been used
	recursiveEquationTransverse(removed_digits, integer_digits, used_powers_numerator, used_powers_denominator, ten_power_groups, variables, integers, equations);
	
	//for (int i = 0; i < ten_power_groups.size(); i++)
	//{
	//	std::vector<int> used_powers_numerator = ten_powers, used_powers_denominator = ten_powers; //used to keep track of which 'places' have been used
	//	used_powers_numerator[log10(ten_power_groups[i].first)] = 0;
	//	used_powers_denominator[log10(ten_power_groups[i].second)] = 0;

	//	if (removed_digits == 1)
	//	{
	//		//std::cout << "{" << ten_power_groups[i].first << ", " << ten_power_groups[i].second << "}" << std::endl;
	//		variables[0] = ten_power_groups[i];

	//		//cycle through used numerator and denominator vectors to assign the smallest remaining values to the integers vector
	//		for (int a = 0; a < integer_digits; a++)
	//		{
	//			for (int b = 0; b < used_powers_numerator.size(); b++)
	//			{
	//				if (used_powers_numerator[b])
	//				{
	//					integers[a].first = used_powers_numerator[b];
	//					used_powers_numerator[b] = 0;
	//					break;
	//				}
	//			}
	//			for (int b = 0; b < used_powers_denominator.size(); b++)
	//			{
	//				if (used_powers_denominator[b])
	//				{
	//					integers[a].second = used_powers_denominator[b];
	//					used_powers_denominator[b] = 0;
	//					break;
	//				}
	//			}
	//		}
	//		GoverningEquation new_equation(variables[0], variables[1], variables[2], integers[0], integers[1], integers[2]);
	//		equations.push_back(new_equation);
	//	}
	//	else
	//	{
	//		for (int j = i + 1; j < ten_power_groups.size(); j++)
	//		{
	//			if (ten_power_groups[i].first != ten_power_groups[j].first && ten_power_groups[i].second != ten_power_groups[j].second)
	//			{
	//				std::vector<int> used_powers_numerator_state1 = used_powers_numerator, used_powers_denominator_state1 = used_powers_denominator; //create a 'saved state' of current vector
	//				used_powers_numerator[log10(ten_power_groups[j].first)] = 0;
	//				used_powers_denominator[log10(ten_power_groups[j].second)] = 0;
	//				if (removed_digits == 2)
	//				{
	//					/*std::cout << "{" << ten_power_groups[i].first << ", " << ten_power_groups[i].second << "}, " <<
	//						"{" << ten_power_groups[j].first << ", " << ten_power_groups[j].second << "}" << std::endl;*/
	//					variables[0] = ten_power_groups[i];
	//					variables[1] = ten_power_groups[j];

	//					//cycle through used numerator and denominator vectors to assign the smallest remaining values to the integers vector
	//					for (int a = 0; a < integer_digits; a++)
	//					{
	//						for (int b = 0; b < used_powers_numerator.size(); b++)
	//						{
	//							if (used_powers_numerator[b])
	//							{
	//								integers[a].first = used_powers_numerator[b];
	//								used_powers_numerator[b] = 0;
	//								break;
	//							}
	//						}
	//						for (int b = 0; b < used_powers_denominator.size(); b++)
	//						{
	//							if (used_powers_denominator[b])
	//							{
	//								integers[a].second = used_powers_denominator[b];
	//								used_powers_denominator[b] = 0;
	//								break;
	//							}
	//						}
	//					}
	//					GoverningEquation new_equation(variables[0], variables[1], variables[2], integers[0], integers[1], integers[2]);
	//					equations.push_back(new_equation);
	//					used_powers_numerator = used_powers_numerator_state1; //reset to state before adding second variable
	//					used_powers_denominator = used_powers_denominator_state1; //reset to state before adding second variable
	//				}
	//				else
	//				{
	//					for (int k = j + 1; k < ten_power_groups.size(); k++)
	//					{
	//						if (ten_power_groups[i].first != ten_power_groups[k].first && ten_power_groups[i].second != ten_power_groups[k].second)
	//						{
	//							if (ten_power_groups[j].first != ten_power_groups[k].first && ten_power_groups[j].second != ten_power_groups[k].second)
	//							{
	//								std::vector<int> used_powers_numerator_state2 = used_powers_numerator, used_powers_denominator_state2 = used_powers_denominator; //create a 'saved state' of current vector
	//								used_powers_numerator[log10(ten_power_groups[k].first)] = 0;
	//								used_powers_denominator[log10(ten_power_groups[k].second)] = 0;

	//								/*std::cout << "{" << ten_power_groups[i].first << ", " << ten_power_groups[i].second << "}, " <<
	//									"{" << ten_power_groups[j].first << ", " << ten_power_groups[j].second << "}, " <<
	//									"{" << ten_power_groups[k].first << ", " << ten_power_groups[k].second << "}" << std::endl;*/
	//								variables[0] = ten_power_groups[i];
	//								variables[1] = ten_power_groups[j];
	//								variables[2] = ten_power_groups[k];

	//								//cycle through used numerator and denominator vectors to assign the smallest remaining values to the integers vector
	//								for (int a = 0; a < integer_digits; a++)
	//								{
	//									for (int b = 0; b < used_powers_numerator.size(); b++)
	//									{
	//										if (used_powers_numerator[b])
	//										{
	//											integers[a].first = used_powers_numerator[b];
	//											used_powers_numerator[b] = 0;
	//											break;
	//										}
	//									}
	//									for (int b = 0; b < used_powers_denominator.size(); b++)
	//									{
	//										if (used_powers_denominator[b])
	//										{
	//											integers[a].second = used_powers_denominator[b];
	//											used_powers_denominator[b] = 0;
	//											break;
	//										}
	//									}
	//								}
	//								GoverningEquation new_equation(variables[0], variables[1], variables[2], integers[0], integers[1], integers[2]);
	//								equations.push_back(new_equation);
	//								used_powers_numerator = used_powers_numerator_state2; //reset to state before adding third variable
	//								used_powers_denominator = used_powers_denominator_state2; //reset to state before adding third variable
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	return equations;
}

std::pair<int, int> reduceFractionTest(std::vector<int> numerator, std::vector<int> denominator)
{
	int num = 1, den = 1;
	for (int i = 0; i < denominator.size(); i++)
	{
		for (int j = 0; j < numerator.size(); j++)
		{
			if (numerator[j] == denominator[i])
			{
				numerator[j] = 1;
				denominator[i] = 1;
				break;
			}
		}
	}

	for (int i = 0; i < denominator.size(); i++) den *= denominator[i];
	for (int i = 0; i < numerator.size(); i++) num *= numerator[i];

	return { num, den };
}

void otherFunction()
{
	std::vector<std::pair<int, int> > groups;
	std::vector<int> pows = { 1, 10, 100};

	int ways = 0, variables = 3;

	for (int i = 0; i < pows.size(); i++)
	{
		for (int j = 0; j < pows.size(); j++)
		{
			groups.push_back({ pows[i], pows[j] });
		}
	}

	//for (int i = 0; i < groups.size(); i++) std::cout << groups[i].first << ", " << groups[i].second << std::endl;
	for (int i = 0; i < groups.size(); i++)
	{
		if (variables == 1)
		{
			std::cout << "{" << groups[i].first << ", " << groups[i].second << "}" << std::endl;
			ways++;
		}
		else
		{
			for (int j = i + 1; j < groups.size(); j++)
			{
				if (groups[i].first != groups[j].first && groups[i].second != groups[j].second)
				{
					if (variables == 2)
					{
						std::cout << "{" << groups[i].first << ", " << groups[i].second << "}, " <<
							"{" << groups[j].first << ", " << groups[j].second << "}" << std::endl;
						ways++;
					}
					else
					{
						for (int k = j + 1; k < groups.size(); k++)
						{
							if (groups[i].first != groups[k].first && groups[i].second != groups[k].second)
							{
								if (groups[j].first != groups[k].first && groups[j].second != groups[k].second)
								{
									std::cout << "{" << groups[i].first << ", " << groups[i].second << "}, " <<
										"{" << groups[j].first << ", " << groups[j].second << "}, " <<
										"{" << groups[k].first << ", " << groups[k].second << "}" << std::endl;

									ways++;
								}
							}
						}
					}
				}
			}
		}
	}

	std::cout << ways << " total ways" << std::endl;
}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	std::map<int, std::vector<int>> winning_fractions; //used to make sure we don't double count

	int number_length = 4, removed_digits = 2, answer = 0;
	std::vector<fraction> coprime_pairs;
	fraction final_fraction = { 0, 0 };

	int lower_limit = MyPow(10, number_length - removed_digits - 1);
	int upper_limit = MyPow(10, number_length - removed_digits) - 1;

	FaraySequence(upper_limit, coprime_pairs);
	std::vector<GoverningEquation> equations = createEquations(number_length, removed_digits);

	//Try all multiples of each of the coprime pairs against every governing equation
	for (int i = 0; i < coprime_pairs.size(); i++)
	{
		int k = 0;
		while (true)
		{
			k++;
			if (k * coprime_pairs[i].numerator < lower_limit || k * coprime_pairs[i].denominator < lower_limit) continue; //increase k until both coprime numbers are the appropriate length
			if (k * coprime_pairs[i].numerator > upper_limit || k * coprime_pairs[i].denominator > upper_limit) break; //can't use this coprime pair anymore so go to next

			for (int j = 0; j < equations.size(); j++)
			{
				//update the equation with the appropriate k value and coprime pair
				equations[j].coprime_pair = coprime_pairs[i];
				equations[j].k = k;
				equations[j].updateValues();

				//after updating get all integer solutions to the new equation
				std::vector<std::pair<int, int> > solutions = equations[j].findIntegerSolutions(MyPow(10, number_length - 1));

				for (int s = 0; s < solutions.size(); s++)
				{
					//std::cout << solutions[s].first << "/" << solutions[s].second << " = " << k * coprime_pairs[i].numerator << "/" << k * coprime_pairs[i].denominator << std::endl;
					//std::cout << j << ", " << k << std::endl;
					//equations[j].printFraction();

					if (winning_fractions.find(solutions[s].first) != winning_fractions.end())
					{
						//make sure the denominator doesn't exist already
						if (std::find(winning_fractions[solutions[s].first].begin(), winning_fractions[solutions[s].first].end(), solutions[s].second) == winning_fractions[solutions[s].first].end())
						{
							winning_fractions[solutions[s].first].push_back(solutions[s].second);
							answer++;
							final_fraction.numerator += solutions[s].first;
							final_fraction.denominator += solutions[s].second;
						}
					}
					else
					{
						std::vector<int> new_numerator = { solutions[s].second };
						winning_fractions.insert({ solutions[s].first, new_numerator });
						answer++;
						final_fraction.numerator += solutions[s].first;
						final_fraction.denominator += solutions[s].second;
					}
				}
			}
		}
	}

	std::cout << "Final fraction is " << final_fraction.numerator << " " << final_fraction.denominator << std::endl;
	

	////Brute Force Approach
	//std::vector<std::vector<int> > prime_factorizations = { {0}, {1} };
	//int num, den, nnum, dden;
	//int final_numerator = 1, final_denominator = 1;

	//int viable_fractions = 0, hits = 0;
	//bool equations_used[10] = { false, false, false, false, false, false, false, false, false, false };

	//for (int i = 2; i < 1000; i++) prime_factorizations.push_back(PrimeFactors(i));

	//for (int i = 101; i < 1000; i++)
	//{
	//	//if (i % 10 == 0) continue;
	//	for (int j = i + 1; j < 1000; j++)
	//	{
	//		if (i % 10 == 0 && j % 10 == 0) continue;
	//		bool overlapping_digit = false;

	//		int i_digits[3] = {i / 100, i / 10 % 10, i % 10};
	//		int j_digits[3] = { j / 100, j / 10 % 10, j % 10 };
	//		
	//		//if ((i_digits[0] == j_digits[1]) && (i_digits[1] == j_digits[0])) //equation 1
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[2];
	//		//	dden = j_digits[2];
	//		//	equations_used[0] = true;
	//		//}
	//		//else if ((i_digits[0] == j_digits[2]) && (i_digits[1] == j_digits[0])) //equation 2
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[2];
	//		//	dden = j_digits[1];
	//		//	equations_used[1] = true;
	//		//}
	//		//else if ((i_digits[0] == j_digits[0]) && (i_digits[2] == j_digits[1])) //equation 3
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[1];
	//		//	dden = j_digits[2];
	//		//	equations_used[2] = true;
	//		//}
	//		//else if ((i_digits[0] == j_digits[2]) && (i_digits[2] == j_digits[1])) //equation 4
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[1];
	//		//	dden = j_digits[0];
	//		//	equations_used[3] = true;
	//		//}
	//		//else if ((i_digits[0] == j_digits[1]) && (i_digits[2] == j_digits[0])) //equation 5
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[1];
	//		//	dden = j_digits[2];
	//		//	equations_used[4] = true;
	//		//}
	//		//else if ((i_digits[0] == j_digits[2]) && (i_digits[2] == j_digits[0])) //equation 6
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[1];
	//		//	dden = j_digits[1];
	//		//	equations_used[5] = true;
	//		//}
	//		//else if ((i_digits[1] == j_digits[0]) && (i_digits[2] == j_digits[2])) //equation 7
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[0];
	//		//	dden = j_digits[1];
	//		//	equations_used[6] = true;
	//		//}
	//		//else if ((i_digits[1] == j_digits[1]) && (i_digits[2] == j_digits[0])) //equation 8
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[0];
	//		//	dden = j_digits[2];
	//		//	equations_used[7] = true;
	//		//}
	//		//else if ((i_digits[1] == j_digits[2]) && (i_digits[2] == j_digits[0])) //equation 9
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[0];
	//		//	dden = j_digits[1];
	//		//	equations_used[8] = true;
	//		//}
	//		//else if ((i_digits[1] == j_digits[2]) && (i_digits[2] == j_digits[1])) //equation 10
	//		//{
	//		//	overlapping_digit = 1;
	//		//	nnum = i_digits[0];
	//		//	dden = j_digits[0];
	//		//	equations_used[9] = true;
	//		//}
	//		std::vector<int> nnums, ddens;

	//		if (i_digits[0] == j_digits[0]) //equation 1
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i % 100);
	//			ddens.push_back(j % 100);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[0] == j_digits[1]) //equation 2
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i % 100);
	//			ddens.push_back(10 * j_digits[0] + j_digits[2]);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[0] == j_digits[2]) //equation 3
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i % 100);
	//			ddens.push_back(j / 10);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[1] == j_digits[0]) //equation 4
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(10 * i_digits[0] + i_digits[2]);
	//			ddens.push_back(j % 100);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[1] == j_digits[1]) //equation 5
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(10 * i_digits[0] + i_digits[2]);
	//			ddens.push_back(10 * j_digits[0] + j_digits[2]);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[1] == j_digits[2]) //equation 6
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(10 * i_digits[0] + i_digits[2]);
	//			ddens.push_back(j / 100);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[2] == j_digits[0]) //equation 7
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i / 10);
	//			ddens.push_back(j % 100);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[2] == j_digits[1]) //equation 1
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i / 10);
	//			ddens.push_back(10 * j_digits[0] + j_digits[2]);
	//			equations_used[0] = true;
	//		}
	//		if (i_digits[2] == j_digits[2]) //equation 1
	//		{
	//			overlapping_digit = 1;
	//			nnums.push_back(i / 10);
	//			ddens.push_back(j / 10);
	//			equations_used[0] = true;
	//		}

	//		//if there are no cases of two digits overlapping, check only for single digits

	//		if (overlapping_digit)
	//		{
	//			viable_fractions++;
	//			std::pair<int, int> original_fraction = reduceFractionTest(prime_factorizations[i], prime_factorizations[j]);
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
	//							final_numerator *= i;
	//							final_denominator *= j;
	//							//uncomment this code to see the actual fractions
	//							//std::cout << i << '/' << j << " = " << nnum << '/' << dden << std::endl;
	//						}
	//					}
	//					else
	//					{
	//						std::vector<int> new_numerator = { j };
	//						winning_fractions.insert({ i, new_numerator });
	//						hits++;
	//						final_numerator *= i;
	//						final_denominator *= j;
	//						//uncomment this code to see the actual fractions
	//						//std::cout << i << '/' << j << " = " << nnum << '/' << dden << std::endl;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//std::cout << "There were " << viable_fractions << " fractions tested and " << hits << " hits" << std::endl;
	//std::cout << "Equations used: ";
	//for (int i = 0; i < 10; i++) std::cout << equations_used[i] << " ";
	//std::cout << std::endl;

	//std::pair<int, int> ans = reduceFractionTest(PrimeFactors(final_numerator), PrimeFactors(final_denominator));
	//int answer = ans.second;
	
 //   //Good Approach
	//int answer = 0;

	//std::vector<fraction> coprime_pairs;
	//fraction final_fraction = { 1, 1 };
	//FaraySequence(99, coprime_pairs);

	////std::cout << coprime_pairs.size() << " coprime pairs" << std::endl;

	//////equations for removing two digits from a three digit number
	////std::vector<std::vector<int> > numerator_positions = { {100, 10, 1}, {100, 1, 10}, {10, 1, 100} }; //represents X, Y, int locations
	////std::vector<std::vector<int> > denominator_positions = { {100, 10, 1}, {100, 1, 10}, {10, 100, 1}, {10, 1, 100}, {1, 100, 10}, {1, 10, 100} }; //represents X, Y, int locations

	////equations for removing one digit from a three digit number
	//std::vector<std::vector<int> > numerator_positions = { {100, 10, 1}, {10, 100, 1}, {1, 100, 10} }; //represents X, int / 10, int % 10 locations
	//std::vector<std::vector<int> > denominator_positions = { {100, 10, 1}, {10, 100, 1}, {1, 100, 10} }; //represents X, int / 10, int % 10 locations
	
	//for (int i = 0; i < coprime_pairs.size(); i++)
	//{
	//	//for each coprime pair, we test each of the possible equations
	//	//std::cout << "coprime pair = " << coprime_pairs[i].numerator << ", " << coprime_pairs[i].denominator << std::endl;
	//	for (int nume = 0; nume < numerator_positions.size(); nume++)
	//	{
	//		//std::cout << "numerator change" << std::endl;
	//		for (int den = 0; den < denominator_positions.size(); den++)
	//		{
	//			//first create our governing equaion
	//			//int x_val = numerator_positions[nume][0] * coprime_pairs[i].denominator - denominator_positions[den][0] * coprime_pairs[i].numerator; //X value
	//			//int y_val = denominator_positions[den][1] * coprime_pairs[i].numerator - numerator_positions[nume][1] * coprime_pairs[i].denominator; //Y value
	//			//int int_val = (numerator_positions[nume][2] - denominator_positions[den][2]) * coprime_pairs[i].numerator * coprime_pairs[i].denominator; //integer value, goes on same side of equation as X value

	//			int x_val = numerator_positions[nume][0] * coprime_pairs[i].denominator - denominator_positions[den][0] * coprime_pairs[i].numerator; //X value
	//			//int y_val = denominator_positions[den][1] * (coprime_pairs[i].denominator / 10) * coprime_pairs[i].numerator - numerator_positions[nume][1] * (coprime_pairs[i].numerator / 10) * coprime_pairs[i].denominator; //int / 10 value
	//			//int int_val = denominator_positions[den][2] * (coprime_pairs[i].denominator % 10) * coprime_pairs[i].numerator - numerator_positions[nume][2] * (coprime_pairs[i].numerator % 10) * coprime_pairs[i].denominator; //int % 10 value

	//			//iterate through all possibilities of k
	//			for (int k = 1; k < 100; k++)
	//			{
	//				//std::cout << "Governing equation: " << x_val << "X + " << int_val * k << " = " << y_val << "Y --> coprime_pair = " << coprime_pairs[i].numerator << " - " << coprime_pairs[i].denominator << std::endl;
	//				//if (k * coprime_pairs[i].numerator > 10 || k * coprime_pairs[i].denominator > 10) break;
	//				if (k * coprime_pairs[i].numerator > 99 || k * coprime_pairs[i].denominator > 99) break;
	//				if (k * coprime_pairs[i].numerator < 10 || k * coprime_pairs[i].denominator < 10) continue;

	//				//the integer values must be calculated with k because of floor division
	//				int y_val = denominator_positions[den][1] * (k * coprime_pairs[i].denominator / 10) * coprime_pairs[i].numerator - numerator_positions[nume][1] * (k * coprime_pairs[i].numerator / 10) * coprime_pairs[i].denominator; //int / 10 value
	//				int int_val = denominator_positions[den][2] * (k * coprime_pairs[i].denominator % 10) * coprime_pairs[i].numerator - numerator_positions[nume][2] * (k * coprime_pairs[i].numerator % 10) * coprime_pairs[i].denominator; //int % 10 value

	//				

	//				/*if (coprime_pairs[i].numerator == 1 && coprime_pairs[i].denominator == 6 && nume == 2 && den == 4)
	//					std::cout << "Governing equation: " << x_val << "X + " << int_val * k << " = " << y_val << "Y --> coprime_pair = " << coprime_pairs[i].numerator << " - " << coprime_pairs[i].denominator << std::endl;*/

	//				//see if x results in an integer solution (allow 0 for now as it could be in the middle of a number more than 2 digits)
	//				if ((y_val + int_val) % x_val == 0)
	//				{
	//					int x = (y_val + int_val) / x_val;
	//					if (x > -1 && x < 10)
	//					{
	//						int winning_numerator = x * numerator_positions[nume][0] + (k * coprime_pairs[i].numerator / 10) * numerator_positions[nume][1] + (k * coprime_pairs[i].numerator % 10) * numerator_positions[nume][2];
	//						int winning_denominator = x * denominator_positions[den][0] + (k * coprime_pairs[i].denominator / 10) * denominator_positions[den][1] + (k * coprime_pairs[i].denominator % 10) * denominator_positions[den][2];
	//						
	//						

	//						//allow x to be zero in the case it get's added to the middle of a number, but we need to weed out the trivial case as well as cases where
	//						//0 gets added to the front
	//						if (winning_numerator % 10 == 0 && winning_denominator % 10 == 0) continue; //the trivial case
	//						if (winning_numerator < 100 || winning_denominator < 100) continue; //a 0 was added to the front

	//						
	//						/*if (winning_denominator < winning_numerator)
	//						{
	//							std::cout << "coprime pair = (" << coprime_pairs[i].numerator << ", " << coprime_pairs[i].denominator << "), " <<
	//								"Governing equation -> " << x_val << "x = " << k * (y_val + int_val) << ", k = " << k << std::endl;
	//							std::cout << winning_numerator << "/" << winning_denominator << std::endl;

	//							std::cout << y_val << std::endl;
	//						}*/
	//						
	//						//std::cout << winning_numerator << "/" << winning_denominator << " = " << coprime_pairs[i].numerator << "/" << coprime_pairs[i].denominator << ": Governing equation -> " << x_val << "X = " << k * (y_val + int_val) << std::endl;
	//						
	//						if (winning_fractions.find(winning_numerator) != winning_fractions.end())
	//						{
	//							//make sure the denominator doesn't exist already
	//							if (std::find(winning_fractions[winning_numerator].begin(), winning_fractions[winning_numerator].end(), winning_denominator) == winning_fractions[winning_numerator].end())
	//							{
	//								winning_fractions[winning_numerator].push_back(winning_denominator);
	//								answer++;
	//							}
	//						}
	//						else
	//						{
	//							std::vector<int> new_numerator = { winning_denominator };
	//							winning_fractions.insert({ winning_numerator, new_numerator });
	//							answer++;
	//						}
	//					}
	//				}

	//				//cycle through all values of x
	//				//for (int x = 1; x < 10; x++)
	//				//{
	//				//	if ((x_val * x + int_val * k) % y_val == 0)
	//				//	{
	//				//		//if ((yote * x + peety * k) / yeet > 0) std::cout << yeet << "Y = " << peety * k << " - " << -yote * x << "X --> " << coprime_pairs[i].numerator << "/" << coprime_pairs[i].denominator <<
	//				//		//	" * " << k << std::endl;
	//				//		int y = (x_val * x + int_val * k) / y_val;
	//				//		if (y < 10 && y > 0)
	//				//		{
	//				//			int winning_numerator = numerator_positions[nume][0] * x + numerator_positions[nume][1] * y + numerator_positions[nume][2] * coprime_pairs[i].numerator * k;
	//				//			int winning_denominator = denominator_positions[den][0] * x + denominator_positions[den][1] * y + denominator_positions[den][2] * coprime_pairs[i].denominator * k;

	//				//			if (winning_fractions.find(winning_numerator) == winning_fractions.end())
	//				//			{
	//				//				//make sure we only add unique entries
	//				//				winning_fractions.insert({ winning_numerator, winning_denominator });
	//				//				answer++; //keep a running tally of the fractions found

	//				//				//std::cout << "Governing equation: " << x_val << "X + " << int_val * k << " = " << y_val << "Y ---> ";
	//				//				//std::cout << "Equation combo: " << "num -> " << nume <<  ", den -> " << den << " ---> ";

	//				//				////printing first digit of numerator
	//				//				//if (numerator_positions[nume][0] == 100) std::cout << x;
	//				//				//else if (numerator_positions[nume][1] == 100) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].numerator * k;

	//				//				////printing second digit of numerator
	//				//				//if (numerator_positions[nume][0] == 10) std::cout << x;
	//				//				//else if (numerator_positions[nume][1] == 10) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].numerator * k;

	//				//				////printing third digit of numerator
	//				//				//if (numerator_positions[nume][0] == 1) std::cout << x;
	//				//				//else if (numerator_positions[nume][1] == 1) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].numerator * k;

	//				//				//std::cout << "/";

	//				//				////printing first digit of denominator
	//				//				//if (denominator_positions[den][0] == 100) std::cout << x;
	//				//				//else if (denominator_positions[den][1] == 100) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].denominator * k;

	//				//				////printing second digit of denominator
	//				//				//if (denominator_positions[den][0] == 10) std::cout << x;
	//				//				//else if (denominator_positions[den][1] == 10) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].denominator * k;

	//				//				////printing third digit of denominator
	//				//				//if (denominator_positions[den][0] == 1) std::cout << x;
	//				//				//else if (denominator_positions[den][1] == 1) std::cout << y;
	//				//				//else std::cout << coprime_pairs[i].denominator * k;

	//				//				//std::cout << " = " << coprime_pairs[i].numerator << "/" << coprime_pairs[i].denominator << std::endl;
	//				//			}
	//				//		}
	//				//	}
	//				//}
	//			}
	//		}
	//	}
	//}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}