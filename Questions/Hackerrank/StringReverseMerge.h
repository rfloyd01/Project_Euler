#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <map>

void MakeSubStrings(int len, int current_length, int location, std::string current_string, std::string& s, int* needed, std::string& answer, bool lesser)
{
    if (current_length == len)
    {
        answer = current_string;
        return;
    }

    for (int i = location; i >= 0; i--)
    {
        //breakout if there aren't enough letters left in s to make a full length substring
        if ((i + 1) < (len - current_length)) return;

        //see if it's possible to use the current letter, if not then continue to next iteration of loop
        if (*(needed + (s[i] - 97)))
        {
            //check to see whether choosing this letter would make lexicographic sense when comparing to current answer
            //if not then don't choose the letter. This will prevent the algorithm from going down unnecessary paths
            if ((((s[i] - 97)) > (answer[current_length] - 97)) && !lesser)
            {
                //the current letter we're testing comes lexicographically after the same letter of our current best answer
                //and none of the letters before this one are less than those in the answer. Therefore if we decide to use this
                //letter then no string we can make will beat our current answer lexicographically, so we continue
                //std::cout << s[i] << " vs. " << answer[current_length] << std::endl;
                //std::cout << current_string + s[i] << " vs. " << answer << std::endl << std::endl;
                continue;
            }
            else
            {
                //if we get here it means that a) we're in a lexicographically good position, or b) aren't yet good lexicographically
                //however, the current letter won't hurt our standing. This means that regardless this new letter should be added to
                //the current string
                
                *(needed + (s[i] - 97)) -= 1; //since we're using this letter, then 1 less will be needed in next iteration
                
                //std::cout << current_string + s[i] << ", " << answer << ", ";

                if (lesser || (((s[i] - 97)) == (answer[current_length] - 97)))
                {
                    //check to see if we're lexicographically good, if so then no need to set lesser variable
                    //likewise, if we're not lexicographically set and the current letter is the same as the answers letter, no cange
                    //is required to the lesser variable
                    //std::cout << lesser << std::endl;
                    MakeSubStrings(len, current_length + 1, i - 1, current_string + s[i], s, needed, answer, lesser);
                }
                else
                {
                    //in this case we're not lexicographically set yet but the current letter is less than the answers current letter
                    //which will change the lesser variable to true
                    //std::cout << lesser << std::endl;

                    //check to see if this is the first iteration, if so then don't update the lesser variable
                    if (answer[current_length] == '{') MakeSubStrings(len, current_length + 1, i - 1, current_string + s[i], s, needed, answer, lesser);
                    else MakeSubStrings(len, current_length + 1, i - 1, current_string + s[i], s, needed, answer, true);
                }
                *(needed + (s[i] - 97)) += 1; //put back the letter that we just used before moving on
            }
        }
    }

    return;
}

std::pair<std::string, double> ReverseMerge()
{
    auto run_time = std::chrono::steady_clock::now();

    std::cout << 4 % 2 << std::endl;
    std::cout << 2 % 4 << std::endl;
    std::string s = "aeiouuoiea", answer = "";

    int used_letters[26] = { 0 };
    for (int i = 0; i < s.length(); i++) used_letters[s[i] - 97]++;

    //since all the letters from A are used exactly twice, divide used_letters array by 2
    for (int i = 0; i < 26; i++) used_letters[i] /= 2;

    //create the worst possible lexicographic string that we can compare to as our answer without using any actual letters
    for (int i = 0; i < (s.length() / 2); i++) answer += '{';

    //answer needs to be initialized to the first possible iteration
    MakeSubStrings(answer.length(), 0, s.length() - 1, "", s, used_letters, answer, false);

    return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}
