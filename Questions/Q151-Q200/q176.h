#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <cmath>
#include <numeric>
#include <algorithm>

//Common Cathetus Right-angled Triangles
// Helper function to calculate the product of elements in a vector
int product(const std::vector<int>& vec) {
	return accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}

// Helper function to generate all combinations of size k
void combinations(const std::vector<int>& exponents, int k, int start, std::vector<int>& current, std::vector<std::vector<int>>& result) {
	if (current.size() == k) {
		result.push_back(current);
		return;
	}

	for (int i = start; i < exponents.size(); ++i) {
		current.push_back(exponents[i]);
		combinations(exponents, k, i + 1, current, result);
		current.pop_back();
	}
}

int calculate_permutations(const std::vector<int>& exponent_array) {
	int n = exponent_array.size();
	std::vector<int> grouped_permutations;

	for (int k = 1; k <= n; ++k) {
		std::vector<std::vector<int>> result;
		std::vector<int> current;
		combinations(exponent_array, k, 0, current, result);
		
		int total_k_group = 0;
		for (const auto& combo : result) {
			total_k_group += product(combo);
		}

		grouped_permutations.push_back(total_k_group);
	}

	int final_sum = 0;
	for (int i = 0; i < grouped_permutations.size(); ++i) {
		final_sum += pow(2, i) * grouped_permutations[i];
	}

	return final_sum;
}

// Backtracking function to find exponent array that yields the target result
void find_exponents(std::vector<int>& exponents, int target, std::vector<std::vector<int>>& solutions, int current_max = 1000000, int current_index = 0) {
	int result = calculate_permutations(exponents);

	// If result is too high, stop exploring this path
	if (result > target) {
		return;
	}

	// If the current exponent array gives the target result, store it
	if (result == target) {
		solutions.push_back(exponents);
	}

	// Start with the current value of exponents[current_index] and increment upwards
	int original_value = exponents[current_index];
	while (true) {
		if (exponents[current_index] < current_max) exponents[current_index]++;
		else break;
		if (current_index == 0 || exponents[current_index] <= exponents[current_index - 1]) {
			if (current_index + 1 < exponents.size()) {
				// Move to the next exponent
				find_exponents(exponents, target, solutions, exponents[current_index], current_index + 1);
			}
			else {
				// Reached the last exponent, check this configuration
				if (calculate_permutations(exponents) == target) {
					solutions.push_back(exponents);
				}
			}
		}

		// If the result exceeds the target, break the loop
		if (calculate_permutations(exponents) > target) {
			break;
		}
	}

	// Reset the value before backtracking
	exponents[current_index] = original_value;
}

std::pair<std::string, double> q176()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1;
	int target = 47547;
	
	std::vector<std::vector<int> > results;
	std::vector<int> primez = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 27 }; //for final multiplication

	//Check numbers that can be composed of 1 - 10 distinct
	//prime factors.
	for (int i = 10; i >= 1; i--)
	{
		std::vector<int> exponents(i, 1);
		find_exponents(exponents, target, results);

		if (results.size() > 0)
		{
			//The result that yields the lowest value will be composed of the 
			//most distinct primes whose exponents are closest together. Due to 
			//the way the search works, this value will be at the very end of 
			//the results array. 
			for (int i = 0; i < results.back().size(); i++) answer *= MyPow(primez[i], results.back()[i]);

			//Increment the exponent on the 2 by 1 which has the effect of 
			//making sure that chosen factors are only divisible by 4 or are odd.
			answer *= 2;
			break; //break out of loop as soon as answer is found
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 96818198400000
	//ran in 0.204274 seconds
}

//NOTES
//Notes have been transferred to project-floyd

//2^9 * 3^6 * 5^5 * 7^3 * 11^2
//Potential answers: [9, 6, 5, 3, 2]