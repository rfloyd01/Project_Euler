#pragma once

#include <Header_Files/pch.h>
#include <fstream>

//Poker Hands
long long scoreHand(long long hand, long long* base_sixteen)
{
	//creates a 13 digit hexadecimal number which represents the score for the hand
	//each digit of the number (from least significant to most significant) represents the following:
	//[high card 1, high card 2, high card 3, high card 4, high card 5, first pair, second pair, three of a kind, straight, flush, full house, four of a kind, straight flush]

	bool flush = true, straight = false; //assume a flush has been found until proven otherwise and that a straight hasn't been found
	long long hand_score = 0;
	uint8_t highest_card = 0, lowest_card = 0, number_of_high_cards = 0, cards_found = 0, suits_found = 0;

	//start with Ace and work all the way down to 2
	long long score_compare = 0xF0000000000000;
	for (uint8_t i = 0; i < 13; i++)
	{
		score_compare >>= 4; //move on to the next digit
		uint8_t card_score = ((score_compare & hand) >> (48 - 4 * i)); //only need 4 of these 8 bits

		if (card_score) //if the hand has 1 or more of card type i
		{
			if (!cards_found) highest_card = (13 - i); //this is the first type of card found and thus is the highest

			//first examine the suit of the card
			if (suits_found) //this is not the first card being looked at
			{
				if ((suits_found | card_score) != suits_found) flush = false;
			}
			else (suits_found = card_score);

			if (card_score == 15) //represent a four of a kind
			{
				hand_score = (hand_score | ((13 - i) * base_sixteen[11]));
				flush = false; //if anything other then a single card is found then a flush can be eliminated from possiblity
				cards_found += 4;
			}
			else if (card_score == 14 || card_score == 13 || card_score == 11 || card_score == 7)  //represent a three of a kind
			{
				hand_score = (hand_score | ((13 - i) * base_sixteen[7]));
				flush = false; //if anything other then a single card is found then a flush can be eliminated from possiblity
				cards_found += 3;
			}
			else if (card_score == 8 || card_score == 4 || card_score == 2 || card_score == 1) //represent a card without a pair, aka a high card
			{
				//every new high card found counts less and less towards overall score
				hand_score = (hand_score | (((13 - i) * base_sixteen[4]) >> (number_of_high_cards * 4)));
				number_of_high_cards++;
				cards_found++;
			}
			else
			{
				//a pair has been found. Store it in first pair to start off, if a second pair is found then store it in second pair
				//and then switch places between low and high pairs. This is because two pairs needs to beat a single pair no matter how high the pair is
				//i.e. a pair of 2's and a pair of 3's will beat a pair of aces
				if (hand_score & 0xF00000)
				{
					int high_pair = (hand_score & 0xF00000);
					hand_score = (hand_score | (high_pair << 4)); //move the first pair over by 4 bits as it is now the higher of two pairs instead of a single pair
					hand_score = (hand_score ^ high_pair); //remove high pair from original low pair location
					hand_score = (hand_score | ((13 - i) * base_sixteen[5])); //add new pair to low pair location
				}
				else hand_score = (hand_score | ((13 - i) * base_sixteen[5]));
				flush = false; //if anything other then a single card is found then a flush can be eliminated from possiblity
				cards_found += 2;
			}

			if (cards_found == 5) //this is the last card to be found and thus is the lowest in the hand, no need to keep searching for lower cards
			{
				lowest_card = 13 - i;
				break;
			}
		}
	}
	//once all cards have been scanned, check whether any straights, flushes , straight flushes or full houses have occured
	
	//check for straights first, a straight will always have it's highest card be 4 away from it's lowest card. If this is true and there aren't any kind of pairs
	//then we have a straight. Search in order of rareness to help speed things up
	if ((highest_card - lowest_card) == 4)
	{
		if (!(hand_score & 0xF00000)) //There isn't a pair
		{
			if (!(hand_score & 0xF0000000)) //There isn't a three of a kind
			{
				if (!(hand_score & 0xF00000000000)) //There isn't a four of a kind
				{
					hand_score = (hand_score | (highest_card * base_sixteen[8])); //log the highest number in the straight
					straight = true;
				}
			}
		}
	}

	//now we check to see if we have a flush. This was already checked as cards were individually being processed above. We don't care what the suit is so a score of
	//1 is applied in the flush location
	if (flush) hand_score = (hand_score | base_sixteen[9]);

	//Check to see if we have both a pair and a triple. If so then add a 1 to the full house digit of the answer
	if ((hand_score & 0xF0000000) && (hand_score & 0xF000000)) hand_score = (hand_score | base_sixteen[10]);

	//Finally we check to see if we have a straight flush, it doesn't matter if it's a royal flush or just a normal straight flush, the highest card in the flush will
	//already be captured in the above steps
	if (straight && flush) hand_score = (hand_score | base_sixteen[12]);

	//finally we can return the score for the hand
	return hand_score;
}

std::pair<std::string, double> q54()
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

	//create two arrays to hold all of the hands for each player
	long long* player_one_hands = new long long[1000];
	long long* player_two_hands = new long long[1000];

	//read through text file and add hexadecimal representations of each hand to the player hand arrays
	std::ifstream inFile;
	inFile.open("Resources/q54.txt");
	char letter = 'a'; //just use a random character to initialize the variable

	for (int i = 0; i < 1000; i++) //1000 lines to read
	{
		for (int j = 0; j < 2; j++) //2 players to read hands for
		{
			long long hand = 0;
			for (int k = 0; k < 5; k++) //5 cards per hand
			{
				long long card = 0;
				for (int l = 0; l < 2; l++) //3 characters to read per card including ' ' and '\n'
				{
					inFile >> letter;

					if (l == 0) //card value is read first
					{
						if (letter == '2') card = card_values[0];
						else if (letter == '3') card = card_values[1];
						else if (letter == '4') card = card_values[2];
						else if (letter == '5') card = card_values[3];
						else if (letter == '6') card = card_values[4];
						else if (letter == '7') card = card_values[5];
						else if (letter == '8') card = card_values[6];
						else if (letter == '9') card = card_values[7];
						else if (letter == 'T') card = card_values[8];
						else if (letter == 'J') card = card_values[9];
						else if (letter == 'Q') card = card_values[10];
						else if (letter == 'K') card = card_values[11];
						else card = card_values[12];
					}
					else
					{
						if (letter == 'H') card *= suite_values[0];
						else if (letter == 'C') card *= suite_values[1];
						else if (letter == 'D') card *= suite_values[2];
						else card *= suite_values[3];
					}
				}
				hand = hand | card; //add the card into the hand
			}
			if (j == 0) player_one_hands[i] = hand;
			else player_two_hands[i] = hand;
		}
	}
	inFile.close(); //close file after reading all of the hands

	//Was curious to see how long it takes to physically read the poker hands so added the below print line, is there a way to improve the read time? It seems a little high
	//std::cout << "Time to read poker hands from file is: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 << " seconds." << std::endl;
	
	//now that the hands have been saved in the proper format, compare the score for players 1 and 2 for each hand
	long long player_one_hand_score, player_two_hand_score;
	int value_to_check = 8;
	for (int i = 0; i < 1000; i++)
	{
		player_one_hand_score = scoreHand(player_one_hands[i], card_values);
		player_two_hand_score = scoreHand(player_two_hands[i], card_values);
		if (player_one_hand_score > player_two_hand_score) answer++;
	}

	//make sure to delete hand arrays off of the heap
	delete[] player_one_hands;
	delete[] player_two_hands;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 376
	//ran in 0.0013582 seconds
}

//NOTES
//Seems like a fairly straightforward problem. The only thing that I'm not sure of is what happens if two players have the exact same hand, i.e. player 1 and player 2
//both have a straight flush with 2, 3, 4, 5, 6 but the suites are different? The problem text doesn't mention this scenario so I'm just going to assume that it doesn't
//pop up during the course of the problem. In order to more easily track the hands I will use a 13 digit hexadecimal number. The reason for using hex is because there are 16
//possible ways in which a player can own a certain card. Looking at the number 2 for example a player can have ([none], [2h], [2c], [2d], [2s], [2h, 2c], [2h, 2d], [2h, 2s],
//[2c, 2d], [2c, 2s], [2d, 2s], [2h, 2c, 2d], [2h, 2c, 2s], [2h, 2d, 2s], [2c, 2d, 2s], [2h, 2c, 2d, 2s]). A hexadecimal number can be represented with 4 bits so for all 13
//suites we need 4 * 13 bits = 52 bits which is large but can fit into a c++ long integer type. Once all of the poker hands are stored into long integers it should be fairly
//easy to see what kind of hand a player has. A straight would have 5 consecutive non-zero digits in the hex number while a flush would feature 5 of the same digit. 5 identical
//digits in a row would be a straight flush and a royal flush would be 5 identical digits in a row at the very end of the hex number. In terms of scoring each hand I'm going
//to create a different hexadecimal number that's 13 digits long where each digit represent one of the types of poker hands [high card 1, high card 2, high card 3, high card 4,
//high card 5, first pair, second pair, three of a kind, straight, flush, full house, four of a kind, straight flush]. For example a score of 10030200000 would represent
//a full house with triple 3's and double 2's. A score of B8742 would represent a Jack high, followed by 8 high, followed by 7 high... Certain categories are just a true or false,
//such as full house, flush and straight flush, but the rest of the categories will get a number which represent the value of the card. For example a pair of 5's would get a 5 in
//the first pair place. A two pair with 5's and 7's would get a 5 in the first pair place and a 7 in the second pair place. The higher value goes second.By putting the hand score
//into a hexadecimal number it should be very easy to compare hands. Whichever hand has the higher hexadecimal value will win.

//This has ended up being much more frustrating than I thought it would be. By using hex numbers I'm able to sort and score each hand really quickly which is great, however, it
//makes the readability when debugging terrible. I think there's one fringe case that I'm not accounting for and it's hard to tell at a glance what it is because the only thing
//printing onto the screen is hexadecimal numbers. After debugging for like 3 hours I realized that I was scoring single pairs in the two pair digit of the card score. This meant
//that if player 1 had a single pair that was something high like an Ace, it was beating player 2 when they had a two pair of something lower, like 2's and 3's for example. After
//changing the way I count pairs vs. two pairs the code finally worked. The whole reason for using hexadeimal here was to get the code to run quickly, in hindsight though, since there
//are only 1000 hands to check it would've been much easier to just use vectors to keep track of scores for each hand. After getting the answer I could've then tried to update my code
//to use hexadecimal and I think it would've been much easier to debug. Anyway, I'm happy that the problem is over. It runs in .0014 seconds but of that time, .00115 of the seconds
//are needed to read the text file with all the poker hands so to actually score every hand with my algorithm only takes ~.00025 seconds so I'm happy with that. If had tried to use
//vectors to store things instead of single numbers the code would probably be much slower.