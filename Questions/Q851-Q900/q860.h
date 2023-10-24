#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>

#include <random>

//Gold and Silver Coin Game
std::pair<std::string, long double> q860()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//First create modular multiplicative inverses for all factorials up to 9898! mod 989898989.
	//(I have a separate method that does this for successive integers, but not successive factorials)
	const int total_stacks = 9898;
	long long mod = 989898989, * inverses = new long long [total_stacks + 1]();

	//The Montgomery batch inversion technique is used for calculating the inverses
	inverses[0] = 1;
	inverses[1] = 1;
	for (long long i = 2; i <= total_stacks; i++)
	{
		inverses[i] = (i * inverses[i - 1]) % mod;
	}
	long long max_factorial = inverses[total_stacks]; //We also save 9898! mod 989898989 as it will be used in all calculations
	inverses[total_stacks] = ModularMultiplicativeInverse(inverses[total_stacks], mod); //the sole division takes place on the last element

	for (long long i = total_stacks - 1; i >= 0; i--)
	{
		long long temp = inverses[i];
		inverses[i] = (inverses[i + 1] * (i + 1)) % mod;
	}

	//First calculate the number of fair games that exist using criteria 1. There must be an even number 
	//of stacks for this criteria to work
	int full_stacks = total_stacks / 2, hybrid_stacks = 0;
	if (!(total_stacks % 2))
	{
		while (full_stacks >= 0)
		{
			long long ways = max_factorial;
			for (int i = 0; i < 2; i++)
			{
				ways *= inverses[full_stacks];
				ways %= mod;
			}

			for (int i = 0; i < 2; i++)
			{
				ways *= inverses[hybrid_stacks];
				ways %= mod;
			}

			full_stacks -= 2;
			hybrid_stacks += 2;
			answer += ways;
		}

		//each of hte calculated ways can have its full colored stacks and hybrid stacks swapped
		answer *= 2;
	}

	//Now calculate the number of fair games that exist using criteria 2
	for (int extra_color = 2; extra_color < total_stacks; extra_color += 2)
	{
		int favored_stacks = extra_color / 2;
		int favored_hybrid_stacks = 2 * extra_color; //this is the number of hybrid stacks that favor the color with less coins

		//There are (total_stacks - extra_color / 2 - hybrid_stacks) stacks that need to be filled out. These
		//remaining stacks have the same rules applied to them as in criteria 1, i.e., equal number of solid 
		//stacks of each color and hybrid stacks favoring one color. This can't be achieved with an odd number 
		//of stacks
		int remaining_stacks = total_stacks - favored_stacks - favored_hybrid_stacks;
		if (remaining_stacks % 2) continue;

		full_stacks = remaining_stacks / 2;
		hybrid_stacks = 0;

		while (full_stacks >= 0)
		{
			long long ways = max_factorial;
			ways *= inverses[full_stacks + favored_stacks]; //add the single colored stacks that there are more of
			ways %= mod;

			ways *= inverses[full_stacks]; //add the single colored stacks that there are less of
			ways %= mod;

			ways *= inverses[favored_hybrid_stacks + hybrid_stacks]; //add the hybrid stacks that there are more of
			ways %= mod;

			ways *= inverses[hybrid_stacks]; //add the hybrid stacks that there are more of
			ways %= mod;

			full_stacks -= 1;
			hybrid_stacks += 1;
			answer += 2 * ways; //the color with more coins can be reversed, doubling the possibilities
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 2424840693838490 (current wrong answer)
	//ran in 0.0130322 seconds
}

//NOTES
/*
I worked out the strategy and rules for creating a fair game elsewhere, but what it boils down to is this. The optimal strategy is to do moves with the following precedence:

1. Take pieces of yours with the other players pieces on top, this forces them to effectively lose a turn.
2. Take pieces of yours stacked on top of another players, this prevents them from stealing a turn from you.
3. Take individual pieces of yours stacked on top of other pieces of yours, or take pieces of yours with nothing stacked on top. Never take a full stack of your own pieces.

In order for a game to be fair it must meet two criteria:

1. There's an equal number of silver and gold pieces, and the number of hybrid stacks (1 silver and 1 gold piece) with gold on the bottom is equal to the number
   of hybrid stacks with silver on the bottom. This can't be achieved with an odd number of stacks.
2. There's more pieces of one color, but the number of hybrid stacks with the lesser color on the bottom is double the piece defecit. For example, if there are 
   12 gold pieces and 8 silver pieces at the beginning then there must be 8 hybrid stacks with silver on the bottom (because 2 * (12 - 8) = 8. Building on this,
   the remaining stacks in the game that don't add to the piece defecit or the hybrid stacks making the defecit back up must conform to rule 1 here.

With the rules laid out above for creating a fair game, you can calculate F(10) as follows:
Games conforming to rule 1:
No hybrid stacks = 10!/(5!5!) = 252 ways
2 hybrid stacks = 10!/(4!4!1!1!) = 6,300 ways
4 hybrid stacks = 10!/(3!3!2!2!) = 25,200 ways
6 hybrid stacks = 10!/(2!2!3!3!) = 25,200 ways
8 hybrid stacks = 10!/(1!1!4!4!) = 6,300 ways
10 hybrid stacks = 10!/(5!5!) = 252 ways
Total = 63,504 ways

Games conforming to rule 2:
4 extra pieces of 1 color and 8 hybrid stacks favoring the other color = 2 * 10!/(8!2!) = 90

F(10) = 63,504 + 90 = 63,594.

As long as there aren't any other ways to create a fair game that I'm missing (which to be fair is possible, the minimum amount of stacks required to 
make the first fair game with an uneven number of pieces is 5) then calculating F(9898) should hopefully be fairly straight forwards. Currently I have a 
method for calculating large binomials mod a prime number, however, most of the calculations I'll be doing aren't going to be pure binomials. Looking over 
the code I have for that method though, it seems that I just keep multiplying by the numbers that make up each part of the factorial and take the modulus
after each multiplication, so it's nothing too fancy. Things will go quicker if I precalculate the modular multiplicative inverses of all factorials from 
1! to 9896! mod 989898988 (which is a prime number, making calculations easier).

test: 2451659829562
*/