#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <Header_Files/print.h>

#include <map>

//Cribbage
void printHand(std::vector<int>& hand)
{
	std::string hand_string = "";

	for (int i = 0; i < hand.size(); i++)
	{
		switch (hand[i])
		{
		case 1:
			hand_string += "A, ";
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			hand_string += std::to_string(hand[i]);
			hand_string += ", ";
			break;
		case 11:
			hand_string += "J, ";
			break;
		case 12:
			hand_string += "Q, ";
			break;
		case 13:
			hand_string += "K, ";
			break;
		}
	}
	hand_string.pop_back();
	hand_string.pop_back();

	std::cout << hand_string << std::endl;
}

bool bruteForceHandCheck(std::vector<int>& hand)
{
	//Iterate over the given hand and score all runs manually.
	//Also keep track of pairs so that the pair score can be deducted
	//from the current score (leaving only existing runs)
	long long currentCard = -1, nextCard = 0, currentRunMultiplier = 1, totalRunMultiplier = 1, currentRunLength = 0, currentPairLength = 0;
	long long totalCribbageScore = 0, totalFaceScore = 0;
	long long faceSums[16] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < hand.size(); i++)
	{
		nextCard = hand[i];

		if (nextCard == currentCard)
		{
			currentRunMultiplier++;
			currentPairLength++;
		}
		else
		{
			if (nextCard == (currentCard + 1))
			{
				totalRunMultiplier *= currentRunMultiplier;
				currentRunLength++;
			}
			else
			{
				if (currentRunLength >= 3)
				{
					totalCribbageScore += (currentRunLength * (totalRunMultiplier * currentRunMultiplier));
				}
				totalRunMultiplier = 1;
				currentRunLength = 1;
			}

			currentRunMultiplier = 1;

			//deduct current pair score from totalCurrentScore
			if (currentPairLength >= 2)
			{
				if (currentPairLength == 2) totalCribbageScore += 2;
				else if (currentPairLength == 3) totalCribbageScore += 6;
				else if (currentPairLength == 4) totalCribbageScore += 12;
			}
			
			currentPairLength = 1;

		}

		int nextCardValue = (nextCard >= 10) ? 10 : hand[i];
		totalFaceScore += nextCardValue;

		for (int j = 15; j >= nextCardValue; j--)
		{
			faceSums[j] += faceSums[j - nextCardValue];
		}

		currentCard = nextCard;
	}

	//Add final run/pair information
	if (currentRunLength >= 3)
	{
		totalCribbageScore += (currentRunLength * (totalRunMultiplier * currentRunMultiplier));
	}

	if (currentPairLength == 2) totalCribbageScore += 2;
	else if (currentPairLength == 3) totalCribbageScore += 6;
	else if (currentPairLength == 4) totalCribbageScore += 12;

	totalCribbageScore += (2 * faceSums[15]);

	return (totalCribbageScore == totalFaceScore);
}

std::map<std::vector<int>, int> createStandardTenHands(long long* pair_scores, long long* suite_scores)
{
	std::map<std::vector<int>, int> tenHands;

	for (int tens = 0; tens <= 4; tens++)
	{
		for (int jacks = 0; jacks <= 4; jacks++)
		{
			for (int queens = 0; queens <= 4; queens++)
			{
				for (int kings = 0; kings <= 4; kings++)
				{
					//Form for the tenHand array is:
					//{face score, cribbage score, suite multiplier}
					std::vector<int> tenHand = { 0, 0, 1};

					//First add the face score
					tenHand[0] = tens + jacks + queens + kings;

					if (tenHand[0] == 0) continue; //don't add the empty hand

					//Now calculate the cribbage score, start with pairs
					tenHand[1] += *(pair_scores + tens);
					tenHand[1] += *(pair_scores + jacks);
					tenHand[1] += *(pair_scores + queens);
					tenHand[1] += *(pair_scores + kings);

					//Now calculate the straight score. Also keep track of whether
					//the straight starts with 10
					if (tens > 0)
					{
						if (jacks > 0)
						{
							if (queens > 0)
							{
								int straight_score = 0;
								straight_score += (3 * tens * jacks * queens);
								if (kings > 0)
								{
									straight_score /= 3;
									straight_score *= (4 * kings);
								}
								tenHand[1] += straight_score;
							}
						}
						
					}
					else if (jacks > 0 && queens > 0 && kings > 0)
					{
						tenHand[1] += (3 * jacks * queens * kings);
					}

					//Create the suite multiplier for the hand
					tenHand[2] *= *(suite_scores + tens);
					tenHand[2] *= *(suite_scores + jacks);
					tenHand[2] *= *(suite_scores + queens);
					tenHand[2] *= *(suite_scores + kings);

					/*if (tenHand[0] == 90 && tenHand[1] == 92 && tenHand[2] == 24)
					{
						int yeeeet = 14;
					}*/

					//Add the hand to the map (if it doesn't exist yet),
					//or increment the existing value by 1.
					auto loc = tenHands.find(tenHand);
					if (loc == tenHands.end()) tenHands.insert({ tenHand, 1 });
					else loc->second++;
				}
			}
		}
	}

	return tenHands;
}

std::map<std::vector<int>, int> createTenHandsWithRuns(long long* pair_scores, long long* suite_scores)
{
	std::map<std::vector<int>, int> tenHands;

	for (int tens = 0; tens <= 4; tens++)
	{
		for (int jacks = 0; jacks <= 4; jacks++)
		{
			for (int queens = 0; queens <= 4; queens++)
			{
				for (int kings = 0; kings <= 4; kings++)
				{
					//Form for the tenHand array is:
					//{face score, pair score, run score, suite multiplier, run length starting with ten, run multiplier starting with ten}
					std::vector<int> tenHand = { 0, 0, 0, 1, 0, 1 };

					//First add the face score
					tenHand[0] = tens + jacks + queens + kings;
					if (tenHand[0] == 0) continue; //don't add the empty hand

					//Then calculate the pair score
					tenHand[1] += *(pair_scores + tens);
					tenHand[1] += *(pair_scores + jacks);
					tenHand[1] += *(pair_scores + queens);
					tenHand[1] += *(pair_scores + kings);

					//Now calculate the straight score. Also keep track of whether
					//the straight starts with 10
					if (tens > 0)
					{
						tenHand[4] = 1;
						tenHand[5] = tens;
						if (jacks > 0)
						{
							tenHand[4] = 2;
							tenHand[5] *= jacks;
							if (queens > 0)
							{
								tenHand[4] = 3;
								tenHand[5] *= queens;
								if (kings > 0)
								{
									tenHand[4] = 4;
									tenHand[5] *= kings;
								}
								tenHand[2] += tenHand[4] * tenHand[5];
							}
						}

					}
					else if (jacks > 0 && queens > 0 && kings > 0)
					{
						tenHand[2] += (3 * jacks * queens * kings);
					}

					//Create the suite multiplier for the hand
					tenHand[3] *= *(suite_scores + tens);
					tenHand[3] *= *(suite_scores + jacks);
					tenHand[3] *= *(suite_scores + queens);
					tenHand[3] *= *(suite_scores + kings);

					//Add the hand to the map (if it doesn't exist yet),
					//or increment the existing value by 1.
					auto loc = tenHands.find(tenHand);
					if (loc == tenHands.end()) tenHands.insert({ tenHand, 1 });
					else loc->second++;
				}
			}
		}
	}

	return tenHands;
}

std::vector<std::vector<std::vector<int>>> createTenHands(long long* pair_scores, long long* suite_scores)
{
	std::vector<std::vector<std::vector<int>>> tenHands;
	for (int i = 0; i < 17; i++)
	{
		std::vector<std::vector<int> > tenHandsByLength;
		tenHands.push_back(tenHandsByLength);
	}

	for (int tens = 0; tens <= 4; tens++)
	{
		for (int jacks = 0; jacks <= 4; jacks++)
		{
			for (int queens = 0; queens <= 4; queens++)
			{
				for (int kings = 0; kings <= 4; kings++)
				{
					//Form for the tenHand array is:
					//{face score, pair score, run score, suite multiplier, run length starting with ten,
					// run multiplier starting with ten, cribbage score, total hands}
					std::vector<int> tenHand = { 0, 0, 0, 1, 0, 1, 0, 1 };

					//First add the face score
					tenHand[0] = tens + jacks + queens + kings;
					if (tenHand[0] == 0) continue; //don't add the empty hand
					if (tenHand[0] == 2)
					{
						int yeeet = 67;
					}

					//Then calculate the pair score
					tenHand[1] += *(pair_scores + tens);
					tenHand[1] += *(pair_scores + jacks);
					tenHand[1] += *(pair_scores + queens);
					tenHand[1] += *(pair_scores + kings);

					//Now calculate the straight score. Also keep track of whether
					//the straight starts with 10
					if (tens > 0)
					{
						tenHand[4] = 1;
						tenHand[5] = tens;
						if (jacks > 0)
						{
							tenHand[4] = 2;
							tenHand[5] *= jacks;
							if (queens > 0)
							{
								tenHand[4] = 3;
								tenHand[5] *= queens;
								if (kings > 0)
								{
									tenHand[4] = 4;
									tenHand[5] *= kings;
								}
								tenHand[2] += tenHand[4] * tenHand[5];
							}
						}

					}
					else if (jacks > 0 && queens > 0 && kings > 0)
					{
						tenHand[2] += (3 * jacks * queens * kings);
					}

					//Add the pair score and the run score to get the cribbage score
					tenHand[6] = tenHand[1] + tenHand[2];

					//Create the suite multiplier for the hand
					tenHand[3] *= *(suite_scores + tens);
					tenHand[3] *= *(suite_scores + jacks);
					tenHand[3] *= *(suite_scores + queens);
					tenHand[3] *= *(suite_scores + kings);

					//Insert the current hand into the appropriate vector
					//such that the hands are ordered by cribbage score.
					//If the hand already exists then simply increment the counter
					//of the existing hand by 1.
					auto it = tenHands[tenHand[0]].begin();
					while (true)
					{
						if (it == tenHands[tenHand[0]].end())
						{
							tenHands[tenHand[0]].push_back(tenHand);
							break;
						}

						if (it->at(6) > tenHand[6])
						{
							tenHands[tenHand[0]].insert(it, tenHand);
							break;
						}
						else if (it->at(6) == tenHand[6])
						{
							//when the cribbage scores are equal then 
							//check to see if the hands are equal
							bool equal = true;
							for (int i = 0; i < 6; i++)
							{
								if (it->at(i) != tenHand[i])
								{
									equal = false;
									break;
								}
							}

							if (equal)
							{
								it->at(7)++;
							}
							else tenHands[tenHand[0]].insert(it, tenHand);
							break;
						}

						it++;
					}
				}
			}
		}
	}

	return tenHands;
}

bool createHands(long long current_face_score, long long current_cribbage_score, long long* remaining_face_scores, std::vector<int>& sum_counter, long long * pair_scores,
	long long* suite_scores, std::pair<long long, long long> current_run, int location, long long max_sum, long long suite_multiplier, long long* total, std::vector<int>& test_array, bool test,
	bool checkAnswer, std::map<std::vector<int>, int>& standard_ten_hands, std::map<std::vector<int>, int>& ten_hands_with_runs, std::vector<std::vector<std::vector<int>>>& ten_hands)
{
	//First, iterate through all options of adding the current card (i.e. we can add 1, 2, 3 or 4
	//of the current card)
	long long face_value = location >= 10 ? 10 : location;
	long long i = 1;
	for (; i <= 4; i++)
	{
		long long face_adder = i * face_value;

		//add the current card to the sum array
		for (int j = 15; j >= face_value; j--)
		{
			sum_counter[j] += sum_counter[j - face_value];
		}
		
		long long pair_score = *(pair_scores + i);
		long long fifteens_score = 2 * sum_counter[15];
		long long current_run_score = 0;
		if ((current_run.first + 1) >= 3) current_run_score = (current_run.first + 1) * (current_run.second * i);

		if (test)
		{
			test_array.push_back(location);
		}

		//Check the new cribbage vs. face score
		long long new_cribbage_score = pair_score + fifteens_score + current_run_score + current_cribbage_score;

		/*if(test)
		{
			if (bruteForceHandCheck(test_array))
			{
				printHand(test_array);
				*total += (suite_multiplier * (*(suite_scores + i)));
			}
		}*/

		if (current_face_score + face_adder == new_cribbage_score)
		{
			//Found a hit, count the total number of hands that can be created by altering the suites
			//of the cards in the current configuration
			if (test)
			{
				std::cout << (suite_multiplier * (*(suite_scores + i))) << ": ";
				printHand(test_array);
			}
			*total += (suite_multiplier * (*(suite_scores + i)));
		}
		else if ((current_face_score + face_adder + *(remaining_face_scores + location + 1)) < (new_cribbage_score - (4 - i) * face_value))
		{
			//Once the cribbage score exceeds the largest possible face score for the current
			//hand (i.e. if we were to add every card still left in the deck that's larger than
			//the current largest card) there's no need to continue.
			i++;
			break;
		}

		//Iterate through all ten hands and see if they can be added to the current hand
		//to get another hand where the face score equals the cribbage score
		if (location != 9)
		{
			int fifteen_face_adder = 10 - 2 * sum_counter[5];
			int cribbage_score_deficit = current_face_score + face_adder - new_cribbage_score;
			for (int j = 1; j < ten_hands.size(); j++)
			{
				//Based on the number of ways to make 5 with the current hand, we can calculate what the 
				//cribbage score of the ten hand needs to be as a factor of the number of cards making up
				//the ten hand. For every card in the ten hand, the difference between the face score and 
				//the cribbage score will change by a factor of 10 - 2 * [ways to make 5]
				int goal_cribbage_score = cribbage_score_deficit + fifteen_face_adder * j;

				//If the goal cribbage score is negative then move onto the next hand size as it won't be 
				//possible to add a ten hand of this length and have the cribbage score equal the face score.
				if (goal_cribbage_score < 0) continue;
				int run_difference = 0;

				if (location == 9)
				{
					//need to recalculte the maximum possible goal cribbage score so that we don't terminate
					//the search early
					int cards_with_four_instances = j / 4;
					int remainder_cards = j % 4;
					std::pair<int, int> run_adder = { cards_with_four_instances + (remainder_cards != 0), 0 };
					if (remainder_cards == 0) remainder_cards = 1;
					run_adder.second = MyPow(4, cards_with_four_instances) * remainder_cards;
					
					if (current_run.first + 1 + run_adder.first > 3)
					{
						run_difference = (current_run.first + 1 + run_adder.first) * (current_run.second * i * run_adder.second) - current_run_score;
					}
				}

				//Iterate over the ten hands. The hands are sorted by cribbage score so stop the loop if the 
				//gribbage score becomes greater than the goal cribbage score.
				for (int k = 0; k < ten_hands[j].size(); k++)
				{
					if (ten_hands[j][k][6] > (goal_cribbage_score + run_difference)) break;
					//else if (ten_hands[j][k][6] == goal_cribbage_score)
					else if ((ten_hands[j][k][2] + (location != 9) * ten_hands[j][k][3]) == (goal_cribbage_score - (location != 9) * run_difference))
					{
						//The current hand will work so add all permutations to the answer
						*total += (suite_multiplier * (*(suite_scores + i)) * ten_hands[j][k][3] * ten_hands[j][k][7]);
					}
				}
			}
		}
		//else
		//{
		//	//Hands with a 9 need to be treated differently as it's possible to have a run extend from 
		//	//the base hand into the part of the hand with cards greater than or equal to ten.
		//	for (auto it = ten_hands_with_runs.begin(); it != ten_hands_with_runs.end(); it++)
		//	{
		//		long long added_fifteens_score = 2LL * it->first[0] * sum_counter[5];
		//		if (it->first[4] == 0)
		//		{
		//			//The ten hand doesn't actually feature a 10 card so a continuous run isn't possible,
		//			//add scores normally
		//			if ((current_face_score + face_adder + 10LL * it->first[0]) == (new_cribbage_score + it->first[1] + it->first[2] + added_fifteens_score))
		//			{
		//				*total += (suite_multiplier * (*(suite_scores + i)) * it->first[3] * it->second);
		//			}
		//		}
		//		else
		//		{
		//			//Calculate the score with the run including the ten card
		//			long long new_run_score = 0;
		//			if ((current_run.first + 1 + it->first[4]) >= 3) new_run_score = (current_run.first + 1 + it->first[4]) * (current_run.second * i * it->first[5]);

		//			if ((current_face_score + face_adder + 10LL * it->first[0]) == (new_cribbage_score - current_run_score + new_run_score
		//				+ it->first[1] + added_fifteens_score))
		//			{
		//				*total += (suite_multiplier * (*(suite_scores + i)) * it->first[3] * it->second);
		//			}
		//		}
		//	}
		//}

		//if (location < 9)
		//{
		//	for (auto it = standard_ten_hands.begin(); it != standard_ten_hands.end(); it++)
		//	{
		//		long long added_fifteens_score = 2LL * it->first[0] * sum_counter[5];
		//		if ((current_face_score + face_adder + 10LL * it->first[0]) == (new_cribbage_score + it->first[1] + added_fifteens_score))
		//		{
		//			*total += (suite_multiplier * (*(suite_scores + i)) * it->first[2] * it->second);
		//		}
		//	}
		//}
		//else if (location == 9)
		//{
		//	//Hands with a 9 need to be treated differently as it's possible to have a run extend from 
		//	//the base hand into the part of the hand with cards greater than or equal to ten.
		//	for (auto it = ten_hands_with_runs.begin(); it != ten_hands_with_runs.end(); it++)
		//	{
		//		long long added_fifteens_score = 2LL * it->first[0] * sum_counter[5];
		//		if (it->first[4] == 0)
		//		{
		//			//The ten hand doesn't actually feature a 10 card so a continuous run isn't possible,
		//			//add scores normally
		//			if ((current_face_score + face_adder + 10LL * it->first[0]) == (new_cribbage_score + it->first[1] + it->first[2] + added_fifteens_score))
		//			{
		//				*total += (suite_multiplier * (*(suite_scores + i)) * it->first[3] * it->second);
		//			}
		//		}
		//		else
		//		{
		//			//Calculate the score with the run including the ten card
		//			long long new_run_score = 0;
		//			if ((current_run.first + 1 + it->first[4]) >= 3) new_run_score = (current_run.first + 1 + it->first[4]) * (current_run.second * i * it->first[5]);

		//			if ((current_face_score + face_adder + 10LL * it->first[0]) == (new_cribbage_score - current_run_score + new_run_score
		//				+ it->first[1] + added_fifteens_score))
		//			{
		//				*total += (suite_multiplier * (*(suite_scores + i)) * it->first[3] * it->second);
		//			}
		//		}
		//	}
		//}

		for (int j = location + 1; j <= 9; j++)
		{
			std::pair<long long, long long> run;
			if (j == (location + 1)) run = { current_run.first + 1, current_run.second * i };
			else
			{
				run = { 0, 1 };
			}
			if (!createHands(current_face_score + face_adder, current_cribbage_score + pair_score + ((run.first == 0) * current_run_score), remaining_face_scores,
				sum_counter, pair_scores, suite_scores, run, j, max_sum + face_adder, suite_multiplier * (*(suite_scores + i)), total,
				test_array, test, true, standard_ten_hands, ten_hands_with_runs, ten_hands))
			{
				//continue;
			}
		}
	}

	//decrement to the last successful attempt at adding a card
	i--;

	//remove all of the current card from the current run/sum_array/etc and then iterate to the next card.
	if (test)
	{
		for (int j = 0; j < i; j++) test_array.pop_back();
	}

	for (int k = 0; k < i; k++)
	{
		for (int j = 0; j <= (15 - face_value); j++)
		{
			sum_counter[j + face_value] -= sum_counter[j];
		}
	}

	return true;
}

std::pair<std::string, long double> q928 ()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	long long pair_scores[5] = { 0, 0, 2, 6, 12 }, suite_scores[5] = { 1, 4, 6, 4, 1 }, remaining_face_scores[15] = { 0 };
	long long max_face_score = 340;
	std::vector<int> test_array, sum_counter(16, 0);
	sum_counter[0] = 1;
	remaining_face_scores[1] = max_face_score;
	for (int i = 2; i <= 14; i++)
	{
		int remove = ((i - 1) >= 10) ? 10 : (i - 1);
		remaining_face_scores[i] = remaining_face_scores[i - 1] - (4 * remove);
	}

	std::map<std::vector<int>, int> standard_ten_hands = createStandardTenHands(&pair_scores[0], &suite_scores[0]);
	std::map<std::vector<int>, int> ten_hands_with_runs = createTenHandsWithRuns(&pair_scores[0], &suite_scores[0]);
	std::vector<std::vector<std::vector<int>>> ten_hands = createTenHands(&pair_scores[0], &suite_scores[0]);

	//for (int i = 1; i <=13; i++)
	for (int i = 1; i <= 9; i++)
		createHands(0, 0, &remaining_face_scores[0], sum_counter, &pair_scores[0], &suite_scores[0], { 0, 1 }, i, 0, 1, &answer, test_array,
			false, false, standard_ten_hands, ten_hands_with_runs, ten_hands);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//              1408573340
	//the answer is 81108001093
	//ran in 0.184828 seconds
}

//NOTES
/*
Here are some initial thoughts:

    1. Don't pay attention to suite when crafting hands. Once a type of winning hand has been found we can find the total hands of this form by looking at the amount of
	each card and multiplying by choose(4,n). For example, the question prompt gives that A,A,2,3,4,5 is a winning hand. Taking suite into account this means
	we can get a total of (4,2) * (4,1) * (4,1) * (4,1) * (4,1) = 6 * 4^4 = 1,536 different hands with this same form.
	2. Cribbage scores for individual items (i.e. a single pair, a single run, etc.) will have the tendency to be much lower than the face values for an 
	individual item. For example, the largest single pair is K + K which only gives 2 points but has a face score of 20. Likewise, the largest individual run
	is A-K and gives a cribbage score of 13 but has a face score of 85. The cribbage score will tend to go up drastically as we add more cards with the same
	value. Going back to the pair of kings, adding a third king will increase the cribbage score to 6 (tripling it) while the face score goes up to 30 
	(and increase by a factor of only 1.5). Adding a fourth king makes the cribbage score go to 12 (increasing by a factor of 6 over the original pair) while 
	the face value goes up to 40 (an two-fold increase). Basically, the point here is that by only adding an extra card or two, then combinatorically the 
	number of options may dramatically increase. My gut says that most of the "winning" hands will feature multiples of a few cards but not the majority.
	3. Pairs and 15's always give an even cribbage score. This means that anytime the face score is an odd number then there MUST be an odd numbered run
	which can be pulled off an odd number of ways (which can only happen if every card appearing in the run does so an odd number of times). For example,
	if we have the run 4,5,6 (face value of 15, cribbage value of 5) then we could get odd cribbage/face scores with any of the following combos:
	(4,4,4,5,6) [23, 15], (4,5,5,5,6,6,6) [37, 47] ... If we have a run with an even number of cards, or a run with an odd number of cards where even a single 
	of the cards has an even multiple, then the cribbage score is guaranteed to be even.
	4. Dealing with 15's might be a little tricky, but if all cards in the hand are an 8 or larger then making 15 will be impossible as any combination
	of cards will be guaranteed to be too big.

	Idea for breute force/recursive approach:
	-----------------------------------------
	Starting with Ace and working towards King, simply add cards one at a time at each level of the recursion and keep track of the effect it has on 
	both face and cribbage score. Any time adding a card causes the cribbage score to get large enough that the face score of all remaining cards can 
	no longer catch up then the current branch can be considered dead and we iterate to the next card. If the first time we attempt to add a new card
	we hit the failure condition then we go backwards in the recursion.

	To make things more efficient (hopefully) an array will be kept which counts the distinct number of ways to sum cards to a given face value. The
	purpose of this array will be to quickly increment the number of instances of 15. For example lets say our hand is made of of the cards 
	A,2,2,8. The sum array would look like this (of course using indices for the sum and not a map) [0 = 0ways, 1 = 1 ways, 2 = 2 ways, 3 = 2 ways, 4 = 1 way,
	5 = 1 ways, 6 = 0 ways, 7 = 0 ways, 8 = 1 way, 9 = 1 way, 10 = 2 ways, 11 = 2 ways, 12 = 1 way, 13 = 1 way .... 85 = 0ways]. Building out this 
	array should be easy enough, just start at the back and iterate forwards. Any time a non-zero number is encountered then incremenet the element at 
	current_element+card by the amount at the current index. Something like the following.

	empty hand = [1, 0, 0, 0..]
	A = [1, 1, 0, 0, 0, 0 ...]
	A,2 = [1, 1, 1, 1, 0, 0,...]
	A,2,2 = [1, 1, 2, 2, 1, 1,...]
	A,2,2,8 = [1,1,2,2,1,1,0,0,1,1,2,2,1,1,...]

	The cribbage score via 15's will simply be 2*array[15] which makes keeping track of this very simple. Like wise, a complex data structure can be 
	used for keeping track of the current run score. I'm thinking an array that holds pairs of numbers. In each pair the first number will represent 
	the total number of cards in the run while the second number represents the overall multiplier. For example, the run [4, 5, 6] would get a pair 
	of (3, 1) since the run has a length of 3 and only 1 way to do the run. The run, [4, 4, 5, 6, 6, 6] would get a pair of (3, 6) since the run 
	still has a length of 3 but now there are 2*1*3 = 6 different ways to create the run. The cribbage score from these runs can of course be obtained 
	by simply multiplying the two numbers together.

	I can't promise that an algorithm like this will be quick, however, it should be comprehensive. Since we iterate through all the cards 1 at a time 
	then it should create every eligible hand. I think it will take a little bit of slick coding to actually pull off (knowing when to back track the 
	recursion vs. go forwards for example) but I think it's very possible to get a solution from this.

	----
	Notes on the above method. It works pretty well but currently is doing a lot of double counting. This isn't a huge issue, I'm probably just messing
	up an index while back tracking so some debugging should snuff this out. I think I did find a potentially big speed up though. I noticed that 
	for runs involving the number 10, only a single form needs to be calculated. Since all the cards involved have the same value, they effect 15's in 
	the exact same manner. Here's an example. The hand [5, 10, 10, J, Q] has a face value of 45 and a cribbage score of 16. The hands [5, 10, J, J, Q],
	[5, 10, J, Q, Q], [5, J, J, Q, K], [5, J, Q, Q, K] and [5, J, Q, K, K] also have face scores of 45 and cribbage scores of 16. In all cases, the number 
	of ways to make 15 is identical (4) since it can only be done by adding one of the 10 cards to the 5 card. They all also feature a single pair of 
	cards with a value of 10, as well as a run of 3. Basically what this all boils down to is that there are multiple interchangeable hands with cards 
	that have a face value of 10. I think it would make sense to memoize all combinations of 10 cards and have the algorithm only recursively iterate 
	from A to 9 and simply add any of the 10 card combinations that will yield a working result. It will be a little tricky, especially where runs
	that extend into the 10 cards are concerned, however, it should add a pretty drastic speed up. With this change the recursion goes from a maximum
	depth of 52 to 36

	-----
	Data structure for 10+ cards

	So what's the best way to store the different types of ten-hands? To answer that let's look at the information we'll have from the cribbage hand
	with cards less than 10 first. We'll know the face score, the status of the current run, the total number of ways to sum the cards to all numbers
	less than or equal to 15 and the number of pairs. Pairs are easy to deal with since they don't effect each other so there shouldn't be a need to 
	keep track of individual pairs in the ten-hands. Dealing with 15s should also be easy. In the ten-hands themselves it will be impossible to make
	15 since the minimum sum of two cards is 20. Instead, we just need to know the total number of cards in the ten-hand as each 10 can be added to 
	the total number of ways to make 5 in the original cribbage hand to make a new 15.
	
	The trickiest part will be dealing with runs. Since there are only four types of 10 cards we can at most have a single run, i.e.
	[10, J, Q], [J, Q, K], [10, J, Q, K]. When dealing with the normal cribbage hands, as soon as a run ends (i.e. a card is skipped) we instantly 
	start keeping track of the next run. That shouldn't be necessary here since we know that under no circumstance will a second run occuring in 
	the ten hand score points. Because of this simply keep track of the first run starting from the lowest card in the ten-hand. The thing that will
	make things slightly tricky is if a run from the current cribbage hand actually extends into the ten-hand. This can only happen if the cribbage
	hand has a 9 in it and the ten-hand has a 10. Because of this, the data structure should separate hands with a 10 and those without. If the 
	cribbage hand has a 9 ten-hand has a 10 then the runs need to be combined before taking the total. For example, the hand [7, 8, 8, 9] would have 
	a run represented with {3, 2} meaning a run of length 3 with 2 ways to make it for 6 points. The ten-hand [10, 10, J, J, K] would have a run 
	representation of {2, 4}, or a run of length 2 with 4 ways to make it. Combining the two run structure would give {5, 8}, for a total run 
	score of 40 points. If we added a ten-hand with the same run structure, but no 10, i.e. [J, J, Q, Q] has a run representation of {2, 2} then 
	the final run score of the hand would found be calculating the two run representations separately instead of combining them. Since {2, 2} 
	doesn't actually score the total run score of the hand would be the original 6.

	Lot's of word vomit above so let's look at an example of how to combine ten-hands with the current cribbage hands made up of 9 or less.
	Let's say we have a hand of [A, A, 3, 4, 4, 7, 8]. This hand has a Face Score of 28 and a Cribbage Score of 20 and has 5 ways to make
	5. Since there are 5 ways to make 5, then every card in the ten-hand we want to add will create 5 more 15's for an added cribbage score of 
	10. So basically the Face Score of each card in the ten-hand will perfectly offset the added cribbage score from 15s. There's a discrepency
	of 8 points between the current Face and Cribbage scores which will have to be made up via pairs or runs in the ten-hand. This could be 
	done with a 3 of a kind of one card and a pair of another while making sure there's no run in the ten-hand. It doesn't matter if the 
	ten-hand has a 10 since the ciribbage hand doesn't have a 9, so we simply need to look at all ten-hands with runs of length 1 or 2 
	and a pair score of 8. Creating these hands manually and we get: [10, 10, 10, J, J], [10, 10, 10, Q, Q], [10, 10, 10, K, K],
	[10, 10, 10, J, J, K], [10, 10, 10, Q, Q, K], [10, 10, 10, J, K, K], [10, 10, 10, Q, K, K], [10, 10, J, J, J], [10, 10, Q, Q, Q],
	[10, 10, K, K, K], [10, 10, J, J, J, K], [10, 10, Q, Q, Q, K], [10, 10, J, K, K, K], [10, 10, Q, K, K, K], [10, J, J, J, K, K],
	[10, Q, Q, Q, K, K], [10, J, J, K, K, K], [10, Q, Q, K, K, K], [J, J, J, Q, Q], [J, J, J, K, K], [J, J, Q, Q, Q], [J, J, K, K, K],
	[Q, Q, Q, K, K], [Q, Q, K, K, K]. So 24 different hands of this type. Instead of going the pair and three of a kind route, we also could have a
	run of length 3 where one of the cards is paired. Here are all the examples of this type of hand: [10, 10, J, Q], [10, J, J, Q], [10, J, Q, Q],
	[J, J, Q, K], [J, Q, Q, K], [J, Q, K, K]. So 6 hands of this type. I don't think there are any other ways to have just 8 points via pairs
	and runs so it looks like 30 total ways we can add ten-hands to this one cribbage hand to meet our goal. Let's look at one of these complete
	hands just to confirm:

	[A, A, 3, 4, 4, 7, 8, 10, 10, Q, Q, Q, K]
	Pairs: [A, A] = 2, [4, 4] = 2, [10, 10] = 2, [Q, Q, Q] = 6 --> total = 12 points
	Runs: none --> total = 0 points
	15s: [A, 4, 10] * 8 = 16, [A, A, 3, 10] * 2 = 4, [A, 4, Q] * 12 = 24, [A, A, 3, Q] * 3 = 6, [A, 4, K] * 4 = 8, [A, A, 3, K] * 1 = 2,
	     [7, 8] * 1 = 2, [3, 4, 8] * 2 = 4, [4, 4, 7] * 1 = 2, [A, 3, 4, 7] * 4 = 8 --> total = 76
	Total Cribbage Score = 12 + 0 + 76 = 88
	Total Face Score = 1*2 + 3 + 4*2 + 7 + 8 + 10*6 = 88

	Ok great so that all worked out. In this instance all that we cared about was the ten-hand having it's own Cribbage Score of 8 since the 
	Face Score perfectly offset the 15's. Let's look at a different base hand where this won't be the case: [5, 5, 6, 7, 8]. In this hand 
	there are only two ways to make a 5, which means that every card in the ten-hand will add a Face Score of 10 and add to the 15's score
	by only 4, meaning that for each card in the ten-hand the face score will actually gain on the cribbage score by 6. The starting hand has 
	a Face Score of 31 and a Cribbage Score of 12. So there's a defecit of 17 points, and each added ten-card will increase this defecit.
	We'll actually need to use an equation to help find working ten-hands. 17 + 6 * number-of-ten-cards = ten-cribbage-score. So with only 
	one ten-card we'd need a cribbage score of 23, and with the max of 16 ten-cards we'd need a cribbage score of 113.
*/