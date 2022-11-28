#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <sstream>
#include <vector>

//XOR Decryption
std::pair<std::string, double> q59()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;

	//The given text file is only on a single line, read this line into a string with a
	//single readLine() call, convert it into a stringstream and then parse by separating 
	//out the commas
	std::ifstream inFile;
	std::string encryptedLetters;
	inFile.open("Resources/q59.txt");

	std::getline(inFile, encryptedLetters);
	std::stringstream encryptedStream(encryptedLetters);
	std::vector<char> encrypted_letters;
	
	while (std::getline(encryptedStream, encryptedLetters, ',')) encrypted_letters.push_back(std::stoi(encryptedLetters));

	char decryption_key[3] = { 0 }, answer_key[3] = { 0 };
	int longest_word = 100, average_expected_word_length = 10, characters_to_check = 100;
	int minimum_expected_spaces = characters_to_check / average_expected_word_length;

	for (char i = 97; i <= 122; i++)
	{
		decryption_key[0] = i;
		for (char j = 97; j <= 122; j++)
		{
			decryption_key[1] = j;
			for (char k = 97; k <= 122; k++)
			{
				decryption_key[2] = k;
				int spaces_found = 0, max_word_length = 0, current_word_length = 0;
				for (int x = 0; x < characters_to_check; x++)
				{
					if ((decryption_key[x % 3] ^ encrypted_letters[x]) == ' ')
					{
						spaces_found++;
						if (current_word_length > max_word_length) max_word_length = current_word_length;
						current_word_length = 0;
					}
					else current_word_length++;
				}

				if (spaces_found < minimum_expected_spaces) break;
				if (max_word_length < longest_word)
				{
					longest_word = max_word_length;
					answer_key[0] = decryption_key[0];
					answer_key[1] = decryption_key[1];
					answer_key[2] = decryption_key[2];
				}
			}
		}
	}

	//once the correct key is obtained, decrypt the whole message with it to get the final answer
	for (int i = 0; i < encrypted_letters.size(); i++) answer += answer_key[i % 3] ^ encrypted_letters[i];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 129448
	//ran in 0.0004219 seconds
}

//NOTES
//This is a really interesting problem as it looks like it will require a little logic as well as programming skill. The biggest issue that I see is that we don't know what the
//key is other than the fact that it contains three lower case letters. This leaves 26 * 26 * 26 = 17,576 possibilities for the correct answer key. This is a very small number
//and a computer can brute force this really quickly, however, what the computer isn't going to know is whether or not the applied key will result in real words. Some separate
//function is going to need to be written to check this. My first thought is that since every word in the message will most likely be separated by spaces then we can do a basic
//check on the first five words of the decrypted message. This basic check will make sure that everything in the word is a letter (if the wrong decryption key is used then there's
//a good chance that garbage characters will be in the middle of these words). It's not clear at this time whether or not the encrypted message is just a list of words, or if it's
//a message with punctuation. For now, just check that each character between the space characters is a letter and if the first five words pass this test then print out the words
//for a human to see along with the decryption key. I'm assuming that of the 17,576 possibilities only a very few of them will pass the test and it should be easy enough to sort out
//from there. Important to know for this program is that in ASCII lower case letters range from 97 to 122, upper case letters range from 65 to 90 and the space symbol has code 32.

//In the end it turned out there was only one encryption key that was able to pass my simple test, and this led to the answer. Out of curiosity I wanted to see the minimum number of words
//I could check and still get to the correct answer. I started originally with 5 words, but was able to get the same answer when checking only 4 words. Attempting to break the code
//when only looking at 3 words though didn't result in the right answer, so it seems that 5 was a good place to start off at. Checking 4 words instead of 5 didn't effect the run time at
//all, presumably because every attempt fails before 4 words other than the correct attempt.

//HACKERRANK UPDATE
/*
Checking back over my first solution, I actually got kind of lucky with how a found the answer. I decided to look at the first five words and make 
sure there weren't any symbols of any kind. Since the decoded message was a paragraph (and not just random english words like I thought it would
be) this meant that there was punctuation in the message (apostraphes, periods, commas, etc.). Luckily none of these symbols appeared before the 
first five words, however, it would be very possible for this to happen in a different message. In the HackerRank version you have to decipher 
different messages in each trial so I needed a different approach. In a standard paragraph there are spaces after all of the words, so one would 
expect there to be a good amount of space characters that appear. Furthermore, the spaces should be spread out at least somewhat evenly. I'm not 
sure what the average word length is in the English language, but I'd bet it's under 10 characters in length. By looking both at the number of 
space characters that appear, as well as the length of the longest word that's found it should be fairly easy to determine which key correctly 
deciphers the code.
*/