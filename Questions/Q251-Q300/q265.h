#include <Header_Files/pch.h>
#include <iostream>
#include <cmath>

//Binary Circles
void recursiveCircleCreate(unsigned int used, int current_sub, unsigned int current, int n, int &pow, unsigned int* two_powers, long long& ans, int level = 0)
{
    if (level == (n-1))
    {
        ans += (current >> (pow - 1));
        return;
    }

    //try to add a 0
    if (!(*(two_powers + ((current_sub << 1) & (n - 1))) & used)) recursiveCircleCreate(used | *(two_powers + ((current_sub << 1) & (n - 1))), (current_sub << 1) & (n - 1), current << 1, n, pow, two_powers, ans, level + 1);

    //try to add a 1
    if (!(*(two_powers + (((current_sub << 1) + 1) & (n - 1))) & used)) recursiveCircleCreate(used | *(two_powers + (((current_sub << 1) + 1) & (n - 1))), ((current_sub << 1) + 1) & (n - 1), (current << 1) + 1, n, pow, two_powers, ans, level + 1);
}

std::pair<std::string, double> q265()
{
    auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

    unsigned int tp[32];
    for (int i = 0; i < 32; i++) tp[i] = pow(2, i);

    int limit = 5;
    recursiveCircleCreate(1, 0, 0, (int)pow(2, limit), limit, tp, answer);

    return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is 209110240768
    //ran in 0.0004185 seconds
}

//NOTES
/*
A little thought tells us two things about whatever binary strings we pick that can actually work. Since they need to start with all 0's then the 
very next digit will need to be a 1. If a 0 came as the first digit after the fist sub-string then both of the first sub-strings would be 000 which
doesn't work. Secondly, and for the same reason, the very last digit of the string must also be a 1. If our binary string ended in a 0 then the 
very last sub-string would be 000 so we don't want that.

My first idea is to start with a sub-string of 00000 and write up a recursive function that will attempt to add a 0 or 1 to the back of it. There will
be another binary number that keeps track of which sub-strings have already been seen. For example, if we've seen the substrings 00000, 00001 and
00011 which have binary values of 0, 1 and 3 respectively then our binary number for keeping track of sub-strings would read:
00000000000000000000000000001011, the one bit has been flipped in the 0, 1 and 3 columns. A 32-bit integer is necessary for this because 2^5 is
32 so that's how many distinct sub-strings we will be seeing.

In each level of the recursion I'll add a 0 to the back of the current substring (and then remove the front digit to keep it the appropriate length)
and see if this new sub-string is already contained in the 32-bit sub-string number. If not go to the next level of the recursion and keep track that
a 0 was added. If the sub-string IS already contained then skip it and change the newly added 0 at the back of the sub-string to a 1 and carry out
the same check. By doing this at every level of the recursion you should arrive to only correct answers.

This is a good example of the power of recursion. Taking a problem that would've been pretty tricky to try and code up and relegating it to a function
that only requires 5 lines of code.
*/