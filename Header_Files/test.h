#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>

int palindromeTestSixDigits(int y) {
    return 0;
}

std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	int lower_goal = 123456789 / 9, upper_goal = 987654321 / 9;

	int* start_values = new int[2]();
	int* growth_values = new int[2]();

	start_values[0] = 2; start_values[1] = 1;
	growth_values[0] = 1; growth_values[1] = 1;
	int arrayLength = 2, arrayGrowth = 1;

	for (int i = 2; i <= 10; i++)
	{
		//create arrays to hold new values
		int* new_growth = new int[arrayLength + arrayGrowth](), *new_start = new int[arrayLength + arrayGrowth](),*temp = growth_values;

		for (int j = arrayLength - 1; j >= 0; j--) new_growth[arrayLength - 1 - j] = start_values[j];
		for (int j = 1; j < arrayLength; j++) new_growth[j + arrayLength - 1] = start_values[j];

		for (int j = 0; j < arrayLength - 1; j++)
		{
			new_start[2 * j] = start_values[j];
			new_start[2 * j + 1] = start_values[j] + start_values[j + 1];
		}
		new_start[arrayLength + arrayGrowth - 1] = 1;

		//replace the original arrays with the newly created ones
		growth_values = new_growth;
		new_growth = temp;
		delete[] new_growth;

		temp = start_values;
		start_values = new_start;
		new_start = temp;
		delete[] new_start;

		//increase the arrayLength and arrayGrowth variables
		arrayLength += arrayGrowth;
		arrayGrowth *= 2;

		/*std::cout << "New row is complete: " << std::endl;*/
		/*for (int j = 0; j < arrayLength; j++) std::cout << start_values[j] << " ";
		std::cout << std::endl;*/
		/*for (int j = 0; j < arrayLength; j++) std::cout << growth_values[j] << " ";
		std::cout << std::endl;*/

		//At this point we've got the correct values for the next iteration of the sequence. Look at all of the
		//new numbers that were added and see if any of them will become 123,456,789 in future rows.
		for (int j = 1; j < arrayLength; j += 2)
		{
			if ((lower_goal - start_values[j]) % growth_values[j] == 0)
			{
				//if we've made it here than the current number will eventually hit 123,456,789. Now we need to find out if the number
				//directly to the left of this one will ever hit 987,654,321
				//std::cout << std::endl;
				/*std::cout << "Column " << j << ", starting with " << start_values[j] <<
					" that has growth of " << growth_values[j] << " will hit 123,456,789 in " << (lower_goal - start_values[j]) / growth_values[j] << " rows." << std::endl;
				std::cout << "Actual number is: " << MyPow(2, i) - 1 + MyPow(2, i - 1) + j << std::endl;*/

				//first, find out what the number directly to the left is
				int left_number = start_values[j - 1] + growth_values[j - 1] * ((lower_goal - start_values[j]) / growth_values[j]);
				//std::cout << "Left Number is: " << left_number << std::endl;

				//see if we'll ever hit 987,654,321 if we keep adding 123,456,789, to left_number.
				if (upper_goal < left_number) continue; //no point in checking if the current number is already greater than the goal

				if ((upper_goal - left_number) % lower_goal == 0) std::cout << "We've found a hit yaargghhhh" << std::endl;
				//else std::cout << "The final ratio is 123456789/" << left_number + lower_goal * ((upper_goal - left_number) / lower_goal) << std::endl;
			}
		}
		//std::cout << std::endl;
	}

	//123456789 and 987654321 aren't coprime with eachother, which means, we're actually looking for a different fraction
	//what we really want to find is the fraction 13717421/109,739,369
	/*int coprimes[2] = { 987654321 / 9, 123456789 / 9  };
	std::cout << "[" << coprimes[0] << ", " << coprimes[1] << "]" << std::endl;
	while (true)
	{
		if (coprimes[0] < coprimes[1]) coprimes[1] -= coprimes[0];
		else coprimes[0] -= coprimes[1];
		std::cout << "[" << coprimes[0] << ", " << coprimes[1] << "]" << std::endl;

		if (coprimes[0] == 1 || coprimes[1] == 1) break;
	}*/

	//std::vector<int> yeeet = { 1, 13717421 };

	//for (int i = 0; i < 8; i++)
	//{
	//	for (int j = 0; j < yeeet.size() - 1; j += 2)
	//	{
	//		yeeet.insert(yeeet.begin() + j + 1, yeeet[j] + yeeet[j + 1]);
	//	}
	//	//vprint(yeeet);
	//}

	//std::cout << yeeet.size() << std::endl;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 772843811
	//ran in 0.331892 seconds
}

//the answer is 2 ^ 13717420 - 1 + 256