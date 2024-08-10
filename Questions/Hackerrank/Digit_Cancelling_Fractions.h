#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <iostream>
#include <map>

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
	}

	std::vector<std::pair<int, int> > findIntegerSolutions(int minimum_value)
	{
		//find integer solutions to the governing equation. 
		int potential_numerator = 0, potential_denominator = 0;
		std::vector<std::pair<int, int> > integer_solutions;
		if (x_val == 0) return integer_solutions;

		//Even if the governing equation doesn't have a Y or Z component we need to get to the inner portion 
		//of the loop at least once to make sure we solve for X. If there are Y and Z components they can be 
		//any integer value from 1 to 9.
		int y_max = (Y.first == 0) ? 1 : 9;
		int z_max = (Z.first == 0) ? 1 : 9;

		for (int y = 1; y <= y_max; y++)
		{
			for (int z = 1; z <= z_max; z++)
			{
				//calculate the value of x given the values of y, z and int_val
				if ((y * y_val + z * z_val + int_val) % x_val == 0)
				{
					int x = (y * y_val + z * z_val + int_val) / x_val;
					if (x > 0 && x < 10)
					{
						//This value of x works, however, we need to make sure it isn't one of the trivial cases before accepting it
						potential_numerator = x * X.first + y * Y.first + z * Z.first + int1.first * (k * coprime_pair.numerator % 10) + int10.first * (k * coprime_pair.numerator / 10 % 10) + int100.first * (k * coprime_pair.numerator / 100);
						potential_denominator = x * X.second + y * Y.second + z * Z.second + int1.second * (k * coprime_pair.denominator % 10) + int10.second * (k * coprime_pair.denominator / 10 % 10) + int100.second * (k * coprime_pair.denominator / 100);

						if (potential_numerator >= minimum_value && potential_denominator >= minimum_value) integer_solutions.push_back({ potential_numerator, potential_denominator });
					}
				}
			}
		}

		return integer_solutions;
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

	return equations;
}

std::pair<std::string, double> Digit_Cancelling_Fractions()
{
	auto run_time = std::chrono::steady_clock::now();
	std::map<int, std::vector<int>> winning_fractions; //used to make sure we don't double count
	int number_length, removed_digits;
	fraction final_fraction = { 0, 0 };

	std::cin >> number_length;
	std::cin >> removed_digits;

	std::vector<fraction> coprime_pairs;

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
					if (winning_fractions.find(solutions[s].first) != winning_fractions.end())
					{
						//make sure the denominator doesn't exist already
						if (std::find(winning_fractions[solutions[s].first].begin(), winning_fractions[solutions[s].first].end(), solutions[s].second) == winning_fractions[solutions[s].first].end())
						{
							winning_fractions[solutions[s].first].push_back(solutions[s].second);
							final_fraction.numerator += solutions[s].first;
							final_fraction.denominator += solutions[s].second;
						}
					}
					else
					{
						std::vector<int> new_numerator = { solutions[s].second };
						winning_fractions.insert({ solutions[s].first, new_numerator });
						final_fraction.numerator += solutions[s].first;
						final_fraction.denominator += solutions[s].second;
					}
				}
			}
		}
	}

	std::string answer = std::to_string(final_fraction.numerator) + " " + std::to_string(final_fraction.denominator);

	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//HackerRank score = 100.00/100.00
}