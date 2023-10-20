#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>
#include <vector>
#include <cmath>
#include <random>

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	srand(run_time.time_since_epoch().count());

	//Assume that we always call a good coin first
	int game = 0, score = 0, total_flips = 0;
	while (game < 1000000)
	{
		int coin = rand() % 100 + 1, flips = 0;
		double current_expected_score = -15, good_odds = 0.5, bad_odds = 0.5;
		while (true)
		{
			double tails_odds = good_odds / 2.0 + bad_odds / 4.0;
			double heads_odds = 1.0 - tails_odds;

			double good_odds_with_tails = good_odds / 2.0 / tails_odds;
			double bad_odds_with_tails = bad_odds / 4.0 / tails_odds;
			double expected_score_with_tails = 0.0;

			if (good_odds_with_tails > bad_odds_with_tails) expected_score_with_tails = good_odds_with_tails * (19 - flips) - bad_odds_with_tails * (51 + flips);
			else expected_score_with_tails = bad_odds_with_tails * (19 - flips) - good_odds_with_tails * (51 + flips);

			double good_odds_with_heads = good_odds / 2.0 / heads_odds;
			double bad_odds_with_heads = 3.0 * bad_odds / 4.0 / heads_odds;
			double expected_score_with_heads = 0.0;

			if (good_odds_with_heads > bad_odds_with_heads) expected_score_with_heads = good_odds_with_heads * (19 - flips) - bad_odds_with_heads * (51 + flips);
			else expected_score_with_heads = bad_odds_with_heads * (19 - flips) - good_odds_with_heads * (51 + flips);

			//calculate the combined expected score when flipping the coin
			double combined_expected_score = tails_odds * expected_score_with_tails + heads_odds * expected_score_with_heads;

			if (combined_expected_score > current_expected_score)
			//if (expected_score_with_heads > current_expected_score || expected_score_with_tails > current_expected_score)
			{
				//We can increase our score by flipping the coin potentially so we do that here.
				int face = rand() % 100 + 1;
				flips++;
				total_flips++;

				if ((coin <= 50 && face <= 50) || (coin > 50 && face <= 75)) face = 0; //represents heads
				else face = 1; //tails

				//set the current expected score and coin odds to the values calculated above
				if (face == 0)
				{
					current_expected_score = expected_score_with_heads;
					good_odds = good_odds_with_heads;
					bad_odds = bad_odds_with_heads;
				}
				else
				{
					current_expected_score = expected_score_with_tails;
					good_odds = good_odds_with_tails;
					bad_odds = bad_odds_with_tails;
				}
			}
			else
			{
				//it doesn't make sense to flip so we make our guess based on which odds are currently
				//higher, the bad coin or the good coin
				if ((good_odds > bad_odds && coin <= 50) || (bad_odds > good_odds && coin > 50)) score += (20 - flips);
				else score -= (50 + flips);
				break;
			}
		}

		game++;
	}

	std::cout << "Expected Score = " << (float)score / (float)game << std::endl;
	std::cout << "Average Flips/game = " << (float)total_flips / (float)game << std::endl;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}