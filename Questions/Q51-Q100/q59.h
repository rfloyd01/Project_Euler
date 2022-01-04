#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <vector>

//XOR Decryption
std::pair<std::string, double> q59()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	char letter = 'a'; //just use a random character to initialize the variable

	//first step is to copy the encrypted letters from the text file
	std::ifstream inFile;
	inFile.open("Resources/q59.txt");

	std::vector<char> encrypted_letters, decrypted_letters;

	//wasn't sure how to break the loop when EOF is reached so I just put an '@' character at the very end of the file to signify when to stop
	while (letter != '@')
	{
		char encrypted_letter = 0;
		while (true)
		{
			inFile >> letter;
			if (letter == ',' || letter == '@') break;
			encrypted_letter *= 10;
			encrypted_letter += (letter - 48);
		}

		encrypted_letters.push_back(encrypted_letter);
		decrypted_letters.push_back(0); //decrypted letters vector is being default initialized to the same size as the encrypted letters
	}

	char decryption_key[3] = { 0 };
	const int words_to_check = 5;
	std::string decrypted_words[words_to_check];

	for (char i = 97; i <= 122; i++)
	{
		for (char j = 97; j <= 122; j++)
		{
			for (char k = 97; k <= 122; k++)
			{
				decryption_key[0] = i;
				decryption_key[1] = j;
				decryption_key[2] = k;
				for (int x = 0; x < encrypted_letters.size(); x++) decrypted_letters[x] = (decryption_key[x % 3] ^ encrypted_letters[x]); //once decryption key is set, decrypt the message

				//after decryption put the first five words into the string array
				int position_counter = 0;
				bool cont = true;
				for (int x = 0; x < words_to_check; x++)
				{
					std::string current_word = "";
					while (decrypted_letters[position_counter] != ' ')
					{
						if (decrypted_letters[position_counter] < 65)
						{
							//break if a character less than 'A' is found
							cont = false;
							break;
						}
						else if ((decrypted_letters[position_counter] > 90) && (decrypted_letters[position_counter] < 97))
						{
							//break if a character between 'Z' and a'' are found
							cont = false;
							break;
						}
						else if (decrypted_letters[position_counter] > 122)
						{
							//break if a character greater than 'z' is found
							cont = false;
							break;
						}
						current_word += decrypted_letters[position_counter];
						position_counter++;
					}
					if (!cont) break; //break out of this loop if a non letter or space character is found
					decrypted_words[x] = current_word;
					position_counter++;
				}

				if (cont)
				{
					//Uncomment the below three lines to see what the decryption key is and the five words that cracked the code
					//for (int x = 0; x < 5; x++) std::cout << decrypted_words[x] << ", ";
					//std::cout << std::endl;
					//std::cout << "Decryption Key is: [" << decryption_key[0] << ", " << decryption_key[1] << ", " << decryption_key[2] << "]" << std::endl;

					for (int x = 0; x < decrypted_letters.size(); x++) answer += decrypted_letters[x];
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 129448
	//ran in 0.029874 seconds
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