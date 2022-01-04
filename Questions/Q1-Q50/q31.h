#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h> //includes vpint()
#include <vector>

//Coin sums
std::pair<std::string, double> q31()
{
	auto run_time = std::chrono::steady_clock::now();
	std::vector<int> coin_values = { 1, 2, 5, 10, 20, 50, 100, 200 }, total_ways;
	for (int i = 0; i <= 200; i++) total_ways.push_back(0); total_ways[0] = 1;

	for (int i = 0; i < coin_values.size(); i++)
		for (int j = coin_values[i]; j < total_ways.size(); j++)
			total_ways[j] += total_ways[j - coin_values[i]];

	return { std::to_string(total_ways.back()), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 73682
	//ran in 0.0000552 seconds
}

//NOTES
//This is the classic example of a dynamic programming problem, basically a problem where you can get to the answer for a large
//question by solving a smaller question first. In the case of this problem, before finding all the ways to make 200p, first find all
//the ways to make 199p and then add a 1p coin, before finding all the ways to make 199p, find all the ways to make 198p and then
//add two 1p coins or a 2p coin... all the way on down to 1p. The key is to start with only thre 1p coin. You can make 1p with a 1p
//coin by adding a 1p coin on top of all the ways you can make zero (of which there's only one way). So there's only one way to make
//1p. Then ove on to 2p. You can make 2p by putting a 1p coin on all of the ways that there are to make 1p (which is 1), so there's
//also one way to make 2p using only 1p coins. Go on like this until you've made 200p using nothing but 1p coins. Then start using 
//2p coins. There's one way to make 2p using a 2p coin (which is by using the coin, duh) so add that to all of the ways there are to
//make 2p using 1p coins, for a grand total of two ways. Then move to 3p. Add a 2p coin on top of all the ways there are to make 2p,
//which is also one way, so there are two ways to make 3p using 1p and 2p coins. Continue like this until you've gotten all the way
//up to the 200p coin. I feel like this description doesn't quite capture the essence of what's happening, maybe come throuhg later
//and update this.
