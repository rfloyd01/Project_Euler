#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Eulercoin
std::pair<std::string, double> q700()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0, currentValue = 0, mod = 4503599627370517, currentEulerCoin = 1504170715041707;

	while (currentValue < mod)
	{
		answer += currentEulerCoin; //add the current euler coin to the answer

		//Starting at our current value, get as close to "mod" as possible using the current euler coin without surpassing
		//mod. From this point, the next Euler coin occurs if we advance forward by one more of the current Euler Coin
		currentValue += (mod - currentValue) / currentEulerCoin * currentEulerCoin;
		currentEulerCoin = (currentValue + currentEulerCoin) % mod;
	}


	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1517926517777556
	//ran in 0.0000036 seconds
}

//NOTES
/*
  Since we're using modular division here, there can be at most 4503599627370516 different Euler coins.

  12345 * n - 45678 * y <= set value
  12345 * n - setValue <= 45678 * y
  (12345 * n - setValue) / 45678 <= y

*/