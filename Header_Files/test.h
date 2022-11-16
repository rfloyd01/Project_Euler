#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <fstream>


std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;

	//fill up an array with values from 16^0 all the way through 16^12 for easy construction of hexadecimal numbers
	int suite_values[4] = { 1, 2, 4, 8 }; //suite order will be heart, club, diamond, spade
	long long card_values[13], counter = 1;
	for (long long i = 0; i < 13; i++)
	{
		card_values[i] = counter;
		counter *= 16;
	}

	//read through text file and add hexadecimal representations of each hand to the player hand arrays
	std::ifstream inFile;
	inFile.open("Resources/q54.txt");
	char value, suite; //just use a random character to initialize the variable
	std::string current_game;
	long long player_one_hand_score = 0;

	while (std::getline(inFile, current_game))
	{
		for (int player = 0; player < 2; player++)
		{
			long long hand = 0;
			for (int card = 0; card < 5; card++)
			{
				long long current_card = 0;

				value = current_game[3 * card + 15 * player];
				suite = current_game[3 * card + 15 * player + 1];

				if (value <= '9') current_card = card_values[value - '2'];
				else if (value == 'T') current_card = card_values[8];
				else if (value == 'J') current_card = card_values[9];
				else if (value == 'Q') current_card = card_values[10];
				else if (value == 'K') current_card = card_values[11];
				else current_card = card_values[12];

				if (suite == 'H') current_card *= suite_values[0];
				else if (suite == 'C') current_card *= suite_values[1];
				else if (suite == 'D') current_card *= suite_values[2];
				else current_card *= suite_values[3];

				hand = hand | current_card; //add the card into the hand
			}

			if (!player) player_one_hand_score = scoreHand(hand, card_values);
			else
			{
				//now that the each hand been saved in the proper format, compare the score for players 1 and 2
				//for each hand
				if (player_one_hand_score > scoreHand(hand, card_values)) answer++;
			}
		}
	}
	inFile.close(); //close file after reading all of the hands
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 132.52756426
	//ran in 0.198797 seconds
}

//current best ratio for N=250 is 133.00456453