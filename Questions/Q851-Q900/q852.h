#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes coprime

#include <random>

struct ExpectedValueNode
{
	//Keeps track of various important odds for each state
	//of the game.
	double stay_expected_value = 0.0;
	double flip_expected_value = 0.0;
	double good_odds = 0.0;
	double bad_odds = 0.0;
	double heads_odds = 0.0;
	double tails_odds = 0.0;
};

double recursiveTriangleCreate(std::vector<std::vector<std::pair<double, double> > >& expected_value_triangle, double good_odds, double bad_odds, int location = 0, int flips = 0)
{
	//Make sure that the triangle is big enough to add these new odds
	if (expected_value_triangle.size() < (flips + 1)) expected_value_triangle.push_back({});

	//We always pick the highest probability outcome
	if (good_odds > bad_odds) expected_value_triangle[flips].push_back({ good_odds * (20 - flips) - bad_odds * (50 + flips), 0.0 });
	else expected_value_triangle[flips].push_back({ bad_odds * (20 - flips) - good_odds * (50 + flips), 0.0 });

	//base case for now, will need to tweak this
	if (flips == 50)
	{
		expected_value_triangle[flips].back().second = expected_value_triangle[flips].back().first; //give equal odds for staying and flipping at lowest level
		return expected_value_triangle[flips].back().first;
	}

	//recursively calcualte the expected value of continuing by weighting the value of flipping heads or tails.
	double heads_odds = good_odds * 0.5 + bad_odds * 0.75, tails_odds = good_odds * 0.5 + bad_odds * 0.25;

	//Calculate the odds for good and bad coins assuming a heads or tails is flipped
	double good_heads_odds = good_odds * 0.5, bad_heads_odds = bad_odds * 0.75;
	double good_tails_odds = good_odds * 0.5, bad_tails_odds = bad_odds * 0.25;
	double combined_heads_odds = good_heads_odds + bad_heads_odds, combined_tails_odds = good_tails_odds + bad_tails_odds;

	//heads odds first. Check to see if we've already calculated the odds of flipping a heads as there may be
	//multiple paths to certain spots in the triangle
	bool calcualtion_comlete = false;
	if (expected_value_triangle.size() >= (flips + 2))
	{
		if (expected_value_triangle[flips + 1].size() >= (location + 1))
		{
			//we've already calculated the necessary odds so take the greater of the two values
			double better_odds = expected_value_triangle[flips + 1][location].first > expected_value_triangle[flips + 1][location].second ? expected_value_triangle[flips + 1][location].first : expected_value_triangle[flips + 1][location].second;
			expected_value_triangle[flips][location].second += heads_odds * better_odds;
			calcualtion_comlete = true;
		}
	}

	if (!calcualtion_comlete)
	{
		//we need to recursively calculate new odds
		expected_value_triangle[flips][location].second += heads_odds * recursiveTriangleCreate(expected_value_triangle, good_heads_odds / combined_heads_odds, bad_heads_odds / combined_heads_odds, location, flips + 1);
	}

	//We now do the same thing for tails
	calcualtion_comlete = false;
	if (expected_value_triangle.size() >= (flips + 2))
	{
		if (expected_value_triangle[flips + 1].size() >= (location + 2))
		{
			//we've already calculated the necessary odds so take the greater of the two values
			double better_odds = expected_value_triangle[flips + 1][location + 1].first > expected_value_triangle[flips + 1][location + 1].second ? expected_value_triangle[flips + 1][location + 1].first : expected_value_triangle[flips + 1][location + 1].second;
			expected_value_triangle[flips][location].second += tails_odds * better_odds;
			calcualtion_comlete = true;
		}
	}

	if (!calcualtion_comlete)
	{
		//we need to recursively calculate new odds
		expected_value_triangle[flips][location].second += tails_odds * recursiveTriangleCreate(expected_value_triangle, good_tails_odds / combined_tails_odds, bad_tails_odds / combined_tails_odds, location + 1, flips + 1);
	}

	//After calcualting the expected value for flipping, return the higher value of guessing now or doing another flip
	if (expected_value_triangle[flips][location].first > expected_value_triangle[flips][location].second) return expected_value_triangle[flips][location].first;
	else return expected_value_triangle[flips][location].second;
}

double recursiveTriangleCreate(std::vector<std::vector<ExpectedValueNode> >& expected_value_triangle, double good_odds, double bad_odds, int max_flips, int location = 0, int flips = 0)
{
	//Make sure that the triangle is big enough to add these new odds
	if (expected_value_triangle.size() < (flips + 1)) expected_value_triangle.push_back({});

	//Calcualte the odds of flipping a heads or tails in our current state
	double heads_odds = good_odds * 0.5 + bad_odds * 0.75, tails_odds = good_odds * 0.5 + bad_odds * 0.25;

	//We always pick the highest probability outcome
	if (good_odds > bad_odds) expected_value_triangle[flips].push_back({ good_odds * (20 - flips) - bad_odds * (50 + flips), 0.0, good_odds, bad_odds, heads_odds, tails_odds });
	else expected_value_triangle[flips].push_back({ bad_odds * (20 - flips) - good_odds * (50 + flips), 0.0, good_odds, bad_odds, heads_odds, tails_odds });

	//base case for now, will need to tweak this
	if (flips == max_flips)
	{
		expected_value_triangle[flips].back().flip_expected_value = expected_value_triangle[flips].back().stay_expected_value; //give equal odds for staying and flipping at lowest level
		return expected_value_triangle[flips].back().stay_expected_value;
	}

	//Calculate the odds for good and bad coins assuming a heads or tails is flipped
	double good_heads_odds = good_odds * 0.5, bad_heads_odds = bad_odds * 0.75;
	double good_tails_odds = good_odds * 0.5, bad_tails_odds = bad_odds * 0.25;
	double combined_heads_odds = good_heads_odds + bad_heads_odds, combined_tails_odds = good_tails_odds + bad_tails_odds;

	//heads odds first. Check to see if we've already calculated the odds of flipping a heads as there may be
	//multiple paths to certain spots in the triangle
	bool calcualtion_comlete = false;
	if (expected_value_triangle.size() >= (flips + 2))
	{
		if (expected_value_triangle[flips + 1].size() >= (location + 1))
		{
			//we've already calculated the necessary odds so take the greater of the two values
			double better_odds = expected_value_triangle[flips + 1][location].stay_expected_value > expected_value_triangle[flips + 1][location].flip_expected_value 
				? expected_value_triangle[flips + 1][location].stay_expected_value : expected_value_triangle[flips + 1][location].flip_expected_value;
			expected_value_triangle[flips][location].flip_expected_value += heads_odds * better_odds;
			calcualtion_comlete = true;
		}
	}

	if (!calcualtion_comlete)
	{
		//we need to recursively calculate new odds
		expected_value_triangle[flips][location].flip_expected_value += heads_odds * recursiveTriangleCreate(expected_value_triangle, good_heads_odds / combined_heads_odds, bad_heads_odds / combined_heads_odds, max_flips,
			location, flips + 1);
	}

	//We now do the same thing for tails
	calcualtion_comlete = false;
	if (expected_value_triangle.size() >= (flips + 2))
	{
		if (expected_value_triangle[flips + 1].size() >= (location + 2))
		{
			//we've already calculated the necessary odds so take the greater of the two values
			double better_odds = expected_value_triangle[flips + 1][location + 1].stay_expected_value > expected_value_triangle[flips + 1][location + 1].flip_expected_value ?
				expected_value_triangle[flips + 1][location + 1].stay_expected_value : expected_value_triangle[flips + 1][location + 1].flip_expected_value;
			expected_value_triangle[flips][location].flip_expected_value += tails_odds * better_odds;
			calcualtion_comlete = true;
		}
	}

	if (!calcualtion_comlete)
	{
		//we need to recursively calculate new odds
		expected_value_triangle[flips][location].flip_expected_value += tails_odds * recursiveTriangleCreate(expected_value_triangle, good_tails_odds / combined_tails_odds, bad_tails_odds / combined_tails_odds, max_flips,
			location + 1, flips + 1);
	}

	//After calcualting the expected value for flipping, return the higher value of guessing now or doing another flip
	if (expected_value_triangle[flips][location].stay_expected_value > expected_value_triangle[flips][location].flip_expected_value) return expected_value_triangle[flips][location].stay_expected_value;
	else return expected_value_triangle[flips][location].flip_expected_value;
}


double recursiveExpectedValueCalculate(std::vector<std::vector<ExpectedValueNode> >& expected_value_triangle, int row = 0, int col = 0, double current_odds = 1.0)
{
	//After creating our expected value triangle above, we use this method to recursively drill down into it to calculate our exact expected value
	if ((row == (expected_value_triangle.size() - 1)) || (expected_value_triangle[row][col].stay_expected_value > expected_value_triangle[row][col].flip_expected_value))
	{
		//The base case, we've either reached the end of the triangle or reached a spot where the expected value from taking a guess
		//is higher than flipping
		return current_odds * expected_value_triangle[row][col].stay_expected_value;
	}
	
	//Recursively calculate the expected value by looking at the odds of flipping a heads or tails from the current position
	double expected_value = recursiveExpectedValueCalculate(expected_value_triangle, row + 1, col, current_odds * expected_value_triangle[row][col].heads_odds);
	return expected_value + recursiveExpectedValueCalculate(expected_value_triangle, row + 1, col + 1, current_odds * expected_value_triangle[row][col].tails_odds);
}

//Coins in a Box
std::pair<std::string, long double> q852()
{
	auto run_time = std::chrono::steady_clock::now();
	double answer = 0;

	//Randomize our random number generator for testing purposes
	//srand(run_time.time_since_epoch().count());

    /*std::vector<std::vector<std::pair<double, double> > > expected_values;
	recursiveTriangleCreate(expected_values, 0.5, 0.5);*/

	std::vector<std::vector<ExpectedValueNode> > expected_values;
	answer = recursiveTriangleCreate(expected_values, 0.5, 0.5, 125);
	
	/*std::cout << "Odds when starting at 50-50:" << std::endl;
	for (int i = 0; i < expected_values.size(); i++)
	{
		std::cout << i << " Flips:" << std::endl;
		for (int j = 0; j < expected_values[i].size(); j++)
		{
			std::cout << '(';
			for (int k = 0; k < i - j; k++) std::cout << 'H';
			for (int k = i - j; k < i; k++) std::cout << 'T';
			std::cout << ") {" << expected_values[i][j].stay_expected_value << ", " << expected_values[i][j].flip_expected_value << '}' << std::endl;
		}
		std::cout << std::endl;
	}*/

	//After creating the odds table, do randomized trials to see expected value
	//long long score = 0, game = 0;

	//for (; game < 1; game++)
	//{
	//	//Decide if we pull a good or bad coin
	//	int coin = rand() % 100 + 1;
	//	if (coin <= 50) coin = 0; //good coin
	//	else coin = 1; //bad coin

	//	int flip = 0;

	//	//Start at the top of the pyramid, and move through the pyramid based on whether a heads
	//	//or tails is flipped. We keep drilling down until we get to a location where the expected
	//	//value of flipping is <= the expected value of staying put.
	//	std::pair<int, int> location = { 0, 0 };
	//	bool guess_made = false;
	//	while (location.first < expected_values.size())
	//	{
	//		if (expected_values[location.first][location.second].stay_expected_value > expected_values[location.first][location.second].flip_expected_value)
	//		{
	//			//this is our condition for making a guess
	//			if (((expected_values[location.first][location.second].good_odds > expected_values[location.first][location.second].bad_odds) && (coin == 0)) || 
	//				((expected_values[location.first][location.second].bad_odds > expected_values[location.first][location.second].good_odds) && (coin == 1))) score += (20 - flip);
	//			else score -= (50 + flip);
	//			guess_made = true;
	//			break;
	//		}

	//		//If we didn't end the current game then we flip the coin
	//		flip++;
	//		int face = rand() % 100 + 1;
	//		double combined_odds;

	//		//Update the odds of the coin being good or bad based on the flip
	//		if ((coin == 0 && face <= 50) || (coin == 1 && face <= 75))
	//		{
	//			face = 0; //a heads was flipped
	//			/*bad_odds *= 0.75;
	//			combined_odds = good_odds * 0.5 + bad_odds;*/
	//		}
	//		else
	//		{
	//			face = 1; //a tails was flipped
	//			/*bad_odds *= 0.25;
	//			combined_odds = good_odds * 0.5 + bad_odds;*/
	//		}
	//		/*good_odds = good_odds * 0.5 / combined_odds;
	//		bad_odds = bad_odds / combined_odds;*/

	//		//Move down the pyramid depending on what was flipped.
	//		//We can only move straight down, or down and to the right one.
	//		if (face == 1) location.second++;
	//		location.first++;
	//	}

	//	if (!guess_made)
	//	{
	//		//we got to the end of the triangle without making a guess (this is unlikely, but still possible). Guess the
	//		//current highest odds.
	//		location.first--;

	//		if (((expected_values[location.first][location.second].good_odds > expected_values[location.first][location.second].bad_odds) && (coin == 0)) ||
	//			((expected_values[location.first][location.second].bad_odds > expected_values[location.first][location.second].good_odds) && (coin == 1))) score += (20 - flip);
	//		else score -= (50 + flip);
	//	}
	//}

	/*std::cout << "Experimental Expected Score = " << (float)score / (float)game << std::endl;
	std::cout << "Calculated Expected Score = " << final_expected_value << std::endl;*/

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
/*
----------------------------------------------------
* Trying to figure out the Ideal Strategy: Part 1:
----------------------------------------------------
My first instinct is that depending on the number of Good/Bad coins that are currently known in the box, there will be a certain magic number of times you can 
flip the next coin to maximize your odds of getting it correct. For example, at the start of the game there is a 50-50 shot of pulling a good coin or a bad coin
so it shouldn't matter if there's only 2 coins in the box or 50 coins, you should be able to tell after a set amount of flips in both scenarios. On the other extreme,
if there are only good coins left in the box then you'll always be able to tell after 0 flips.

A little exploration down this path and it seems that my hypothesis was wrong. Calculating the expected values by hand and I could never get anything above ~17 when 
utilizing a set number of flips. The best odds seemed to occur at 8 flips. It seems that instead you're better off using a more adaptive model based on the current odds.
For example, flipping the coin three times in a row and getting heads each time is a fairly strong indicator that the current coin is a bad coin and you can safely take
a guess. Flipping the coin three times and getting heads, heads, tails though isn't really a strong indicator in either direction so you should take more flips. The question
now though, is at what certainty is it most efficient to take a guess? Is it at 60% certainty, 75% certainty? Or does it depend? Most likely it won't be when the certainty
reaches a set amount and will instead depend on some other factors.

I think to play optimally you need to examine the current odds of getting something correct vs. the amount of points it's worth. At the very start of the game you have a 
50-50 shot of getting 20 points and -50 points when taking no flips, which results in an expected score of -15. At a later point in the game there may be 10 good coins left
and 40 bad coins, so without ever flipping the coin theres an 80-20 shot of being correct by guessing it's a bad coin. This leads to an expected score of 6. So by employing
the same strategy of not flipping the coin even once, our expected outcome is better. Using this same example of 10 good coins and 40 bad coins, we need to ask ourselves,
what is the maximum amount our expected score can increase by if we flip the coin one more time. If we knew that flipping the coin three times had a chance to increase
the odds of the bad coin from 80% to 90%, but also an equal chance of reducing our certainty in the bad coin to 70%, would this be worth it? Let's see, at three flips the
scores we can get change to 17 and -53. Assuming we have a 50-50 shot of increasing to 90% certainty and dropping to 70% certainty the expected scores would look like:
0.5 * (0.9 * 17 - 0.1 * 53) + 0.5 * (0.7 * 17 - 0.3 * 53) = 5 - 2 = 3. In the 90% certainty scenario the expected value increases to 10 which is nice, however, in the 70%
certainty scenario it drops to -4. So if these outcomes are equally as likely, the expected value here is 3, which isn't as high as when we don't flip the coin, so it wouldn't
be worth it to flip the coin three times in this case. This is obviously a hypothetical situation, but it may be worth exploring some real world scenarios.

Something else to consider, is what is the minimum certainty we need to have in our guess to make it worth our while? Since a correct guess gives less points than a wrong 
guess takes away, maybe we can calculate the percentage certainty we need in order to ensure our expected value is positive. At zero flips this would look like
20x - 50 (1 - x) = 0 ---> 20x + 50x = 50 --> 70x = 5 --> x = 0.7143. So as long as we're more than 71.43% certain in our guess, then our expected score with no coin flips will
be a positive number, which isn't bad. What about a different scenario, like on our 5th flip of the coin? By this flip the amount of points we can win goes down by 5 and 
the amount of points we can lose increases by 5, so presumably we'll need to be a lot more confident in our guess to go for it: 15x - 55 (1 - x) = 0 ---> 70x = 55 ---> x = 0.7857.
So we're going to need to be about 7% more confident in our answer for a fifth flip to be worth it. What about a 19th flip? x - 69 (1 - x) = 0 ---> 70x = 69 ---> x = 0.9857. So unless
we're pretty positive in our guess it makes no sense to go out to 19 flips. With 20 flips you can't even get any points so I don't see a scenario where going out there makes sense.
Just as a test, I'm going to write a brief script that keeps flipping a randomized coin and making an educated guess when the guess certainty is above the threshold for positive 
points and see how close it is to the optimal expected value in the question prompt of 0.558591 (where there's only a single coin that starts with a 50-50 chance of being 
good or bad).

----------------

Using the above strategy the expected score for a single coin with a 50-50 chance came out to ~-0.5, so clearly this isn't the optimal strategy. The main problem is that
the number of times you'd make it all the way to 20 flips before making a guess was too often, there needs to be some check in place that checks to see whether our odds
of making a guess before the 20th flip are good or not. If it seems like it isn't going to happen then we're better off making a guess where we currently are. Out of curiosity
I want to see what happens if we use a set percentage to make our guess instead of a moving percentage. The below table summarizes the expected score when using a set
percentage change:
      20f    10f
50%: -7.2, -7.23
55%: -7.25, -7.25
60%: -4.55, -4.5
65%: -4, -4.1
70%: -1.55, -2.075
75%: -0.45, -2.35
80%: 0.275, -3.3
85%: 0.015
90%: -1.1
95%: -3.4
100: -7.82

Pretty interesting here, the expected score was at it's highest if we make a guess as soon as we're over 80% certain. Any higher than this and the score goes down (most
likely because it's rare to get above an 80% chance so we end up going to 20 flips more often). Out of curiosity again, I want to see what happens if I limit the number
of flips to 10 instead of 20. This way we actually get points for a correct guess. I added these results (for the first half of odds choices) above and was surprised to see
that for the most part using a max of 10 flips hurt us more than it helped us. I'm now curious to see what happens if more than 20 flips are used, would it be helpful to
be guaranteed negative points if the points are only slightly negative as opposed to a higher chance of a very negative number? To see I put my original script inside of 
another loop that slowly increments the threshold for the percent chance at which we make a guess. I could then easily alter the number of maximum flips before this loop.
I have to say that the results surprised me a bit. If we put our win % chance to 85% and utilize a maximum of 40 flips (which is double the amount before we're guaranteed
negative points) the expected score per game goes all the way up to 0.68 which is actually higher than what the question prompt says. The interesting thing though is that
each time I run the test with these limits (each test is comprised of 1,000,000 games) the expected scores fluctaute a decent amount. With this number of tests I'd expect
there to be variances of maybe ~1%, however, I'm seeing variances ~16% between each test, which indicates that maybe there's something wrong with my methodology.

Either way, the data gathered above is interesting, but I doubt that a static limit for the odds we want is the way to go here. Just like using a static number of flips
also isn't the way to go. We need some way to dynamically look ahead of where we currently are and confirm beyond the shadow of a doubt that the expected score will never
go up from where we currently are in the game. Once we get to this position is when the guess needs to be made. To help figure out a way to do this it may be helpful to
look at higher starting percentages than 50-50. Let's imagine that we started a game with 50 of each coin, just like the actual question, and over the first 50 turns we've
somehow managed to pick 48 bad coins and only 2 good coins. This means that on the next turn we start of with a 4% chance of the next coin being bad and a 96% chance of
the coin being good. Normally, flipping a heads nudges the odds in favor of the coin being bad while flipping tails will nudge us towards the coin being good, but with 
odds as high as 96% without even flipping is it even worth it to get higher odds at the cost of some points? Using my current calculations (which I'm not even sure are
correct) it would look like this. With no flips we have an expected score of 0.96 * 20 - 0.04 * 50 = 17.2 points. If we choose to flip and get a tails (indicator for a 
good coin) the odds would change to good = 0.96 * 0.5 = 0.48, bad = 0.04 * 0.25 = 0.01 --> good = 0.48 / 0.49 -> 0.98, bad = 0.01 / 0.49 -> 0.02. So we increased our
odds by 2% with this extra flip, but was it worth it at the cost of one less point? 0.98 * 19 - 0.02 * 51 = 17.6 points. So in this case it was worth it, however, 
what would've happened if we had flipped a heads instead of a tails (indication of bad coin): good = 0.96 * 0.5 = 0.48, bad = 0.04 * 0.75 = 0.03 --> good = 0.48 / 0.51 -> 0.941,
bad = 0.03 / 0.51 -> 0.059 --> 0.941 * 19 - 0.059 * 51 = 14.87 points, so not worth the lost point in this case. So starting at our 96% chance we had a chance to go up
and a chance to go down, so let's see what these odds actually were. As was calculated above, we had a 49% chance for tails (improving our odds) and 51% chance for heads
(decreasing our odds) so the expected score when flipping in this scenario would really go down from 17.2 to 0.49 * 17.6 + 0.51 * 14.87 = 16.21. So the overall expected 
score goes down by a full point.

So maybe this kind of strategy will work better. After every single flip we need to ask ourselves if another flip has the potential to improve upon the current expected value.
Let's take this approach while using the easy example of only having a single coin. We start with 50-50 odds of the coin being good or bad. Expected score with no flips is
0.5 * 20 - 0.5 * 50 = -15. Looking at the case for a flip now, if we flip a heads then the odds change from 50-50 good-bad to 40-60 good-bad which changes our expected value
to 0.6 * 19 - 0.4 * 51 = -9, a solid imporvement over -15. If a tails gets flipped the good-bad odds change to 66-33 good-bad and the expected value becomes
0.66 * 19 - 0.33 * 51 = -4.33 which is an even more solid improvement. So regardless of what gets flipped our odds improve so we definitely want to flip the coin here. For
completeness sake though, in this scenario the odds of flipping heads is 62.5% while tails is only 37.5% for a combined expected value of 5/8 * -9 + 3/8 * -4.333 = -7.25.
Just to keep this going a bit more and look for obvious flaws, let's assume that tails was flipped in the first round. We already know from the calculations we just did that
this puts our expected score at -4.33 with a 66% chance that the coin is good and a 33% chance that it's bad. What happens now if we flip heads or tails? In the case of 
heads we get good_odds = 0.66 * 0.5 = 0.33, bad_odds = 0.33 * 0.75 = 0.25 --> good_odds = .33 / (.33 + .25) = 0.5714, bad_odds = .25 / (.33 + .25) = 0.4286 and an expected
value of 0.5714 * 18 - 0.4286 * 52 = -12 which is worse than our current chances. If a tails is flipped though the odds become, go = 0.66 * 0.5 = 0.33, bo = 0.33 * 0.25 = 0.0833
---> good_odds = .33 / (.33 + 0.0833) = 0.8, bad_odds = 0.2 and the expected value becomes 0.8 * 18 - 0.2 * 52 = 4. At the start of this turn the odds of flipping a tails
is 41.67% so the combined expected score when flipping the coin is 0.4167 * 4 - 0.5833 * 12 = -5.333. So in this case the combined expected score is lower than if we just
stay put, so we may as well guess now without flipping the coin. Unfortunately my gut is telling me this isn't the best way to go about things as the code I wrote found that
more flips usually worked out better. However, it's still worth it to code this up I think, it may just be that the scenario I laid out above isn't ideal for increasing the 
expected score that much, and other scenarios will.

After coding up the strategy above I ended up with an expected score of ~-4.2, which is woefully short of the 0.55 goal. I don't think the overall thinking behind this strategy
is wrong, however, it's clear that we're going to have to look out more than a single flip before deciding on whether or not to take a guess on the current turn or not. Maybe 
instead of comparing the combined expected score of a heads and tails flip, I'll just go ahead and flip the coin as long as one of the expected scores is higher than the 
current one. I'm assuming that this will help, but still fall short of the goal.

Alright that definitely didn't help. The expected score actually dropped to -6.33 here and needed an average of 24.25 flips / game (whereas the other strategy only needed
an average of 2.11 flips a game for a higher expected score).

For the first time in awhile on a Project Euler problem I'm drawing a complete blank on what to do here. I think what I might do is just look at all possible permutations
of coin flips going from 1 to about 25 flips and print out the expected value for each of these permutations. For example, the final chart will look like this:

Starting at 50-50 Probability for each coin:
0 Flips:
()-15.0

1 Flips:
(H) -9
(T) -4.33

2 Flips:
(HH) ...
(HT) ...
(TT) ...

There shouldn't be any difference between the same permutation as far as odds are concerned (i.e. the expected value when flipping HT and TH should be identical) which will
cut down on the number of tests to run. The one thing that I'm not currently sure about though, is how to take into account the current odds of what the coin is. For example,
when looking at the 20 flip case do I say, there's a 1/2^20 of this permutation occuring with a good coin and a 3^15/4^20 chance of it occuring with a bad coin, and each coin
has a 50-50 chance of being the actual coin so both of these odds get weighted equally? Or, do I need to say, the odds of flipping x amount of heads skews the odds in favor of
the bad coin, so there's no longer a 50-50 chance of each coin so I need to weight the odds of this particular permutation accordingly? Maybe I'll try both (at a lower flip 
amount) and see if there's any difference in the odds. Ideally there won't be, as it should all be baked in, but I'm not positive.

After implementing the above table I notice two things. First, my hopes appear to be correct in that starting with a 50-50 chance will carry over the chance at each level of
the coin being good or bad. The expected values up to 2 flips are identical to the ones I calculated earlier up above. Second, the way I've laid out the table almost makes a 
pyramid of values. Looking at the table like this I think I can see a way forward now. Basically, from any point in the pyramid, we look at all of the spots below it which we
can get to and ask, is it possible to have an expected value than our current location? It should actually be fairly easy to answer, we should be able to combine all of the values
that are reachable into a single expected value and store it in the some location of the pyramid. Doing this will give us a single snapshot for each spot in the pyramid that shows
the expected value of we choose a coin now vs. the expected value if we flip again. Whichever value is higher we do that action. This is more or less what I did above, however,
we're now going to be looking at all possible outcomes and not just the outcomes that are one flip away. Here's a snippet of the first few rows of the pyramid (really it's a triangle):

                                                    () -15
												   /      \
                                                  /        \
												(H)-9       (T)-4.333
												/    \     /   \
											   /      \   /     \
											(HH)-3.5   (HT)-12   (TT)4
											/   \     /    \     /   \
										   /     \   /      \   /     \
										(HHH)1 (HHT)-15.9  (HTT)-2.09  (TTT)9.22

From any location you can only move down and to the left, furthermore, the chances of moving in either direction aren't the same so they'll need to get weighted appropriately. This
should be able to be done with a recursive approach. I think this approach should finally get me on the right track here, but there are two issues that I see. First and foremost is,
how far down do I need to go? Presumably we'll need to keep adding rows until we get a row where each entry has a higher expected value when staying vs. continuing. The problem here
is that I need the bottom rows to already exist to get values for the upper rows. I'll need to do some experimenting to figure this out. The second issue is that this technique seems
fairly involved, and this is only for a single starting outcome, a 50-50 shot. For the actual problem there will be many starting points. There will be 50-50, 50-49, 50-47 ... 2-3. 
Basically all coprime pairs consisting of the numbers 1-50 will need their own pyramid here. For a good number of these combos it may make sense to never flip the coin (50-1 jumps out 
at me), but still this technique may prove to be too slow to get to the right answer in a reasonable time. Before I get ahead of myself though I'm going to at least use this technique 
for the single coin scenario to confirm it will work.

-------

Ok I think I've finally figured out the optimal strategy. Creating the triangle described above and recursively calculating the odds of staying vs. flipping for all options and then
running random number tests is given me expected values in the range of 0.45 to 0.66 which should even out to the 0.55 that's dictated by the problem. Phew it took a while but we
got here. To really confirm this though I don't want experimental values obtained from the random number generator. I should be able to just drill down into all possible "dead-ends"
of the triangle (locations where the expected value for staying is higher than the expected value of flipping) and add weight amounts of all of these expected values. The weighting
has to do with how likely we are to reach each dead end naturally.

-----

I spent about an hour creating an algorithm to recursively go through the triangle, figure out whether or not we're at a dead-end and calculate the expected value. After going through
this excercise I realized that my recursive method for building the triangle already returns the calculated expected value, dang I'm good! The value is a little lower than I expected,
but increasing the number of flips allowed is making it slowly converge to the correct value. It converges to the correct value of 0.558591 when raising the max flip amount to 125,
which is honestly much higher than expected. The good news is that the algorithm only takes 0.0006355 seconds to reach this value. The bad news is that I'm going to need to create
lots of different expected value triangles. To be exact I'll need.
*/