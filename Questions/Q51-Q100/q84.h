#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Functions/functions.h>
#include <vector>

//Monopoly Odds
std::pair<std::string, double> q84()
{
	auto run_time = std::chrono::steady_clock::now();
	std::string answer = "";

	int dice_sides = 4;
	double odds = 1.0 / (dice_sides * dice_sides);
	std::vector<double> single_odds, double_odds;

	//initialize the odds of rolling certain numbers based on the number of dice sides
	for (int i = 0; i <= dice_sides * 2; i++)
	{
		single_odds.push_back(0.00);
		double_odds.push_back(0.00);
	}

	for (int i = 1; i <= dice_sides; i++)
	{
		for (int j = 1; j <= dice_sides; j++)
		{
			if (i == j) double_odds[i + j] += odds;
			else single_odds[i + j] += odds;
		}
	}

	//initialize state matrix to all zeros
	double state_matrix[120][120];
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++) state_matrix[i][j] = 0;

	int new_state; //a variable for keeping track of the next state of the game

	//go through each state of the matrix and calculate the odds of landing on other states
	for (int state = 0; state < 120; state++)
	{
		for (int single = 3; single < single_odds.size() - 1; single++)
		{
			//start at 3 because it's the lowest possible number with two distinct die and stop 1 before the end as the final number is
			//only possible if a double is rolled

			new_state = (state + single) % 40; //mod 40 because no doubles are rolled therefore we need to be in the no doubles states

			//check to see if 'Go to Jail' space was landed on
			if (new_state == 30) state_matrix[state][10] += single_odds[single];

			//check to see if a Chance space was landed on, need individual if statements as certain cards will send you
			//to a location thats dependant on which Chance space you're currently on (7, 22, 36)
			else if (new_state == 7 || (new_state == 22 || new_state == 36))
			{
				if (new_state == 7) //CH1
				{
					//next railroad cards
					state_matrix[state][15] += single_odds[single] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][12] += single_odds[single] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][4] += single_odds[single] / 16.0; // 1/16 chance of picking this card
				}
				else if (new_state == 22) //CH2
				{
					//next railroad cards
					state_matrix[state][25] += single_odds[single] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][28] += single_odds[single] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][19] += single_odds[single] / 16.0; // 1/16 chance of picking this card
				}
				else //CH3
				{
					//next railroad cards
					state_matrix[state][5] += single_odds[single] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][12] += single_odds[single] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][33] += single_odds[single] / 16.0; // 1/16 chance of picking this card
				}

				//the rest of the Chance cards don't depend on the space so can be calculated at the same time
				//Advance to GO card
				state_matrix[state][0] += single_odds[single] / 16.0;
				//Advance to Jail card
				state_matrix[state][10] += single_odds[single] / 16.0;
				//Advance to C1 card
				state_matrix[state][11] += single_odds[single] / 16.0;
				//Advance to E3 card
				state_matrix[state][24] += single_odds[single] / 16.0;
				//Advance to H2 card
				state_matrix[state][39] += single_odds[single] / 16.0;
				//Advance to R1 card
				state_matrix[state][5] += single_odds[single] / 16.0;
				//No movement card drawn, 6/16 chance
				state_matrix[state][new_state] += 6.0 * single_odds[single] / 16.0;
			}

			//Next need to see if a Community Chest Space is landed on
			else if (new_state == 2 || (new_state == 17 || new_state == 33))
			{
				//Advance to GO card
				state_matrix[state][0] += single_odds[single] / 16.0;
				//Advance to Jail card
				state_matrix[state][10] += single_odds[single] / 16.0;
				//No movement card drawn, 14/16 chance
				state_matrix[state][new_state] += 14.0 * single_odds[single] / 16.0;
			}

			//if the new space is standard then just stay there
			else state_matrix[state][new_state] += single_odds[single];
		}

		//next go through the scenario for when a double is rolled
		for (int dub = 2; dub <= double_odds.size(); dub += 2) //only check even numbers as there are no double odds
		{
			//calculate next state
			new_state = state + 40 + dub;

			//add a line about controlling wrapping around back to 0 when crossing GO
			if ((state % 40 + dub) >= 40) new_state -= 40;

			//check to see if this is the third double rolled in a row
			if (new_state >= 120)
			{
				state_matrix[state][10] += double_odds[dub]; //go to jail weenie
				continue; ///TODO: this could be done with a break statement but don't feel like it for now
			}

			//do the same checks as above for go to jail, community chest and chance
			//check to see if 'Go to Jail' space was landed on, this will reset the double counter
			if (new_state % 40 == 30)
			{
				state_matrix[state][10] += double_odds[dub];
			}

			//check to see if a Chance space was landed on, need individual if statements as certain cards will send you
			//to a location thats dependant on which Community Space chest you're currently on (7, 22, 36)
			else if ((new_state % 40 == 7) || ((new_state % 40 == 22) || (new_state % 40 == 36)))
			{
				if (new_state % 40 == 7) //CH1
				{
					//next railroad cards
					state_matrix[state][15 + 40 * (new_state / 40)] += double_odds[dub] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][12 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][4 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
				}
				if (new_state % 40 == 22) //CH2
				{
					//next railroad cards
					state_matrix[state][25 + 40 * (new_state / 40)] += double_odds[dub] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][28 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][19 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
				}
				if (new_state % 40 == 36)
				{
					//next railroad cards
					state_matrix[state][5 + 40 * (new_state / 40)] += double_odds[dub] / 8.0; // 2/16 chance of picking this card
					//next utility card
					state_matrix[state][12 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
					//back three spaces card
					state_matrix[state][33 + 40 * (new_state / 40)] += double_odds[dub] / 16.0; // 1/16 chance of picking this card
				}

				//the rest of the Chance cards don't depend on the space so can be calculated at the same time
				//Advance to GO card
				state_matrix[state][40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//Advance to Jail card
				state_matrix[state][10] += double_odds[dub] / 16.0; //player always pays to get out of jail which resets doubles counter
				//Advance to C1 card
				state_matrix[state][11 + 40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//Advance to E3 card
				state_matrix[state][24 + 40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//Advance to H2 card
				state_matrix[state][39 + 40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//Advance to R1 card
				state_matrix[state][5 + 40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//No movement card drawn, 6/16 chance
				state_matrix[state][new_state] += 6.0 * double_odds[dub] / 16.0;
			}

			//Next need to see if a Community Chest Space is landed on
			else if ((new_state % 40 == 2) || ((new_state % 40 == 17) || (new_state % 40 == 33)))
			{
				//Advance to GO card
				state_matrix[state][40 * (new_state / 40)] += double_odds[dub] / 16.0;
				//Advance to Jail card
				state_matrix[state][10] += double_odds[dub] / 16.0; //player always pays to get out of jail which resets doubles counter
				//No movement card drawn, 14/16 chance
				state_matrix[state][new_state] += 14.0 * double_odds[dub] / 16.0;
			}

			//if the new space is standard then just stay there
			else state_matrix[state][new_state] += double_odds[dub];
		}
	}

	//Now the Markov probability matrix is set so rolls can be simulated by taking a vector which 
	//represents the starting position of the game ([1, 0, 0, 0, 0....0] because the game starts with the
	//player at state 0
	double state_probability[120] = { 0 };
	state_probability[0] = 1; //A game of Monopoly starts on GO which is state 0

	//Was getting the wrong answer until I realized that I created the Transpose of the probability matrix
	//by mistake. Instead of changing all of the above code I just create the real matrix here by transposing
	//the existing matrix
	double transp[120][120];
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++)
			transp[i][j] = state_matrix[j][i];

	//Multiply state_probability vector by the probability matrix until values converge
	for (int i = 0; i < 20; i++) MatrixVectorMult(&transp[0][0], state_probability, 120);

	//Create a vector of all the spaces in order of their probabilities to get the final answer
	std::vector<std::pair<double, std::string> > final_probabilities;
	for (int i = 0; i < 40; i++)
	{
		//an indivisual space probability will be the sum of the current space and the two double state spaces
		double p = 100 * (state_probability[i] + state_probability[i + 40] + state_probability[i + 80]);
		std::string state = std::to_string(i);
		if (i < 10) state = '0' + state;
		std::pair<double, std::string> yeet = { p, state };
		bool placed = false;

		for (int j = 0; j < final_probabilities.size(); j++)
		{
			if (p >= final_probabilities[j].first)
			{
				final_probabilities.insert(final_probabilities.begin() + j, yeet);
				placed = true;
				break;
			}
		}
		if (!placed) final_probabilities.push_back(yeet);
	}

	//Concatenate the final answer
	answer += final_probabilities[0].second;
	answer += final_probabilities[1].second;
	answer += final_probabilities[2].second;

	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 101524
	//ran in 0.0003058 seconds
}

//NOTES
//I originally wanted to just simulate a game of Monopoly (well millions of games) and after each turn
//of each game I would tally up which space has the piece on it. Gien enough games this method should
//wor out just fine because we don't need the exact percentages, we just need to know the top 3 spaces.
//This problem should also be able to be solved via a Markov Chain. In a true Markov chain the current
//state of the system (in this case our board) doesn't depend on the state of the system at the previous
//step. This is almost true for the Monopoly problem, the only wrinkle is the senario in which rolling
//3 doubles in a row will send you to jail. Going to jail in this method does indeed depend on the outcome
//of the previous two states of the system, however, if we consider the board to be in a different state
//depending on the number of doubles rolled we can get around this. A piece can be on any one of 40 different
//spaces at the end of a turn, furthermore, ther can currently be 0, 1 or 2 doubles in a row that have been
//rolled. This means that there are 40 * 3 = 120 different states that we can be in. We can use a 120 x 120
//matrix to represnt the odds of travelling from one state to another (i.e. M[2][10] will represent the
//probability of moving from state 2 to state 10). States 0-39 will represent each space on the board
//with no doubles being rolled, 40-79 will be the same spaces but with 1 double rolled and 80-119 will be
//with two doubles rolled. If you were on space 10 (Jail) and rolled a 4 on one die and a 6 on the other
//then you would move to state 20 (Free Parking), however, if you were to roll a 5 on one die and a 5 on
//the other as well you would move to state 60 instead (this represents free parking with one double being
//rolled). Likewise if you were on state 50 (Jail w/ one double) and rolled a 4 and 6 you would go to state
//20, but if you rolled a 5 and 5 you would go all the way to state 100 (Free Parking w/ two doubles). It 
//isn't possible to stay in the single double or double double states. You will either go up 40 states
//or down by 40 or 80 states. Once all of the odds have been calculated, add up the percentages for all 
//numbers that are equal to eachother mod 40 as these represent the same locations. The three spaces with
//the highest percentages should be the answer.

//In order to not have to calculate odds over and over two separate vectors have been created which store
//the odds of rolling a given number with two dice. One vector holds the odds without doubles and the other
//vector has the odds for doubles only. For quick reference the states for Go_to_Jail are 30, 70 and 110, the
//states for Community Chest are 2, 17, 33, 42, 57, 73, 82, 97 and 113, and the states for Chance are 7, 
//22, 36, 47, 62, 76, 87, 102 and 116. Also for reference, Community Chest has a 1/16 chance to send you to
//GO (0, 40, 80) or Jail (10, 50, 90) and 14/16 chance to stay on the space while Chance has a 1/16 chance
//to send you to GO (0, 40, 80), Jail (10, 50, 90), C1 (11, 51, 91), E3 (24, 64, 104), H2(39, 79, 119),
//R1 (5, 45, 85), Next Railway (state dependant), Next Railway (state dependant), Next Utility (state
//dependant) or go back 3 squares (state dependant) with a 6/16 chance to stay on the same spot. The railway
//spots are R1 (5, 45, 85), R2 (15, 55, 95), R3 (25, 65, 105) and R4 (35, 75, 115) while the Utilities are
//U1 (12, 52, 92) and U2 (28, 68, 108).

//The Markov chain approach ended up working great. The majority of the code is dedicated to creating the
//probability matrix, but once that is complete the answer is converged upon very quickly (only 20 iterations
//are needed to arrive at the correct answer). Not only does the Markov approach work quickly, but it will
//also arrive at the exact same answer every time. If I had decided to simulate actual games of Monopoly
//then lots of iterations would be needed to arrive at the correct answer (probably in the millions) and
//even then there's no guarentee the answer would be the same every time. Coming up with the final matrix
//for the Markov chain method currently doesn't look elegant here and takes up a lot of lines of code so at
//some point I'd like to come back and really clean this code up, I feel like it should be possible to get it
//down to at least 75 lines or so.