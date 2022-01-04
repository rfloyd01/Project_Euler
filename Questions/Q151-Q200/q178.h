#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <map>
#include <vector>

//Step Numbers
std::pair<std::string, double> q178()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 0;
	const int limit = 10000;
	std::map<int, std::vector<long long>> step_numbers;

	//First create the map with empty vectors
	int shifter = 1;
	for (int i = 1; i <= 10; i++)
	{
		int mover = 0;
		for (int j = 0; j <= (10 - i); j++) step_numbers.insert({ shifter << mover++, std::vector<long long>(10, 0) });
		shifter = 2 * shifter + 1;
	}

	//we need to initialize the map with 1 digit step numbers before iterating over it
	for (int i = 1; i < 10; i++) step_numbers[1 << i][i] = 1;

	//with the map created we just need to do 39 iterations over it to get the step numbers
	//under 10^40
	for (int i = 2; i <= limit ; i++)
	{
		//in every iteration, we must traverse the whole map
		//also, I don't like this but we must create a blank, temporary map to store math in
		//TODO: At some point think of a better way to increase numbers without needing a blank map
		std::map<int, std::vector<long long>> temp;
		for (auto iter = step_numbers.begin(); iter != step_numbers.end(); iter++) temp.insert({ iter->first, std::vector<long long>(10, 0) });

		for (auto iter = step_numbers.begin(); iter != step_numbers.end(); iter++)
		{
			//for each group, we need to check all 10 elements of vector
			for (int j = 0; j < 10; j++)
			{
				if (!iter->second[j]) continue; //only care about non-zero values;
				
				//if j is 0 or 9 then we only have 1 option of number to add on to the end (1 or 8)
				//any other number can have 2 numbers added to it
				if (j == 0 ) temp[iter->first | powers_of_two[1]][1] += iter->second[j];
				else if (j == 9) temp[iter->first | powers_of_two[8]][8] += iter->second[j];
				else
				{
					temp[iter->first | powers_of_two[j - 1]][j - 1] += iter->second[j];
					temp[iter->first | powers_of_two[j + 1]][j + 1] += iter->second[j];
				}
			}
		}

		//copy values from temp map into step_numbers map
		//TODO: would this be quicker to do by just using a pointer?
		step_numbers = temp;

		//To get the answer just add all of the numbers stored in the vector at element[0b1111111111] of the map
		for (int i = 0; i < 10; i++) answer += step_numbers[0b1111111111][i];
	}
	//ran in 0.0008542 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 126461847755
	//ran in 0.0008542 seconds
}

//NOTES
//Initial Thoughts:
/*
So my first thought here is that this problem looks ripe for a dynamic approach. We're looking at numbers that are less than 10^40 which itself is a 41 digit number so we should
only need 40 total iterations. Our numbers must be pandigital which means that the minimum amount of digits required is 10. If we were just looking for all step numbers then this problem would
be pretty trivial, but adding the pandigital restraint definitely adds a layer of complexity.
*/

//Euler Solution
/*
Forgetting about the pandigital requirement for now, let's just focus on building step numbers. Every digit other than 0 and 9 has two different numbers that it can step to (0 must step up to
1 and 9 must step down to 8). For example, 4 can step up to 5 or step down to 3, 7 can step up to 8 or step down to 6, and so on and so on. We can use an array of length ten to keep track of our
current step numbers where the number stored in each array element corresponds to the number of step numbers that end with that digit. So in our first step where we only have 1 digit the array would
look like this:

[0, 1, 1, 1, 1, 1, 1, 1, 1, 1]

The reason that 0 doesn't have a value is that no number can start with 0. For the next step we would first look at the numbers stored in 0 and 9, there's only 1 digit that we can add to each of these
so we copy their respective values into elements 1 and 8 respectively (it probably makes sense to use a second array for this copy operation). We then cycle through numbers 1 - 8 and copy their values into
the elements both directly before and directly after them. Here's what this would look like:

array (after iteration 1) = [0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
copy_array =                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

copy_array[1] = array[0];
copy_array[8] = array[9];

//copy_array now looks like this [0, 0, 0, 0, 0, 0, 0, 0, 1, 0]

for (int i = 1; i < 9; i++)
{
    copy_array[i - 1] += array[i];
	copy_array[i + 1] += array[i];
}

//after i = 1 copy_array looks like: [1, 0, 1, 0, 0, 0, 0, 0, 1, 0]
//after i = 2 copy_array looks like: [1, 1, 1, 1, 0, 0, 0, 0, 1, 0]
//after i = 3 copy_array looks like: [1, 1, 2, 1, 1, 0, 0, 0, 1, 0]
//after i = 4 copy_array looks like: [1, 1, 2, 2, 1, 1, 0, 0, 1, 0]
//after i = 5 copy_array looks like: [1, 1, 2, 2, 2, 1, 1, 0, 1, 0]
//after i = 6 copy_array looks like: [1, 1, 2, 2, 2, 2, 1, 1, 1, 0]
//after i = 7 copy_array looks like: [1, 1, 2, 2, 2, 2, 2, 1, 2, 0]
//after i = 8 copy_array looks like: [1, 1, 2, 2, 2, 2, 2, 2, 2, 1]

Here's what the array looks like after the third iteration:
[1, 3, 3, 4, 4, 4, 4, 4, 3, 2]

According to our array, after the third iteration there should be 32 3-digit step numbers. Let's confirm that:

Ends in 0: 210
Ends in 1: 101, 121, 321
Ends in 2: 212, 232, 432
Ends in 3: 123, 323, 343, 543
Ends in 4: 234, 434, 454, 654
Ends in 5: 345, 545, 565, 765
Ends in 6: 456, 656, 676, 876
Ends in 7: 567, 767, 787, 987
Ends in 8: 678, 878, 898
Ends in 9: 789, 989

Ok so it looks like everything checks out here. If we were to carry out this same procedure 37 more times we'd effectively have every single step number less than 10^40 saved in this array.
Unfortunately there's more to the problem than this. Our step numbers also need to be pandigital, i.e. feature every digit from 0-9 at least one time. It's possible to hit our first pandigital
number after the 9th iteration (the only number we could do this with is 9876543210), or, it's also possible to have a number only become pandigital on the last iteration (such as
2101234567878787878787878787878787878789). In order to track whether or not our numbers are pandigital I think it would make sense to put them into groups depending on which numbers they don't
currently contain. Once a number no longer has any missing digits it can be considered to be pandigital.

My first thought on tracking digits not contained within a number was to look at all the possible ways this could be. For example, we can be missing 10 digits (only 1 possible way), we can be missing
9 digits (10 possible ways), 8 digits (10 choose 8 = 45 ways) .... all the way on down to missing 0 digits (only 1 possible way). This is obviouisly just the 10th row of Pascal's Triangle which means that
there are 2^10 = 1,024 possibilities here. So we need to not only keep track of these 1,024 ways to count the digits missing from our numbers, but we also still need to keep track of the final digit
for our numbers as well. This means we need some kind of array that has 1,024 x 10 = 10,240 elements in it. I think this will be manageable to create and manipulate, I've used arrays with over a million
elements before so this doesn't seem that bad by comparison. By the the 40th iteration is complete, our answer should be the sum of all 10 numbers stored in the "Missing no Numbers" portion of our
array.

At first I was thinking that overflow of long integers might be possible, although now that I think about it, the ceiling for possible step numbers of n digits should be less than 10 x 2^(n-1). You can
kind of see why this is the case when looking at the array above for 2 digit step numbers. Since we're only going out to 40 digit numbers, 10 x 2^39 = 5,497,558,138,880 which can definitely fit into
a long integer. Furthermore, our array will be partitioned into roughly 10,000 elements so the following limit split over all of these elements means that on average each element will have a value of
roughly 549,755,814 in the worst case. I might be wrong in this analysis, so if things start acting funky I could always just use my int_64x class.

To actually implement this structured array, every element is going to itself be an array of length 10. Each index of the main array will be a binary representation of which numbers are currently
contained wihtin the 10 digit array. For example, the array at index 999 (which is 0b1111100111 in binary) will contain step numbers that are only missing the numbers 3 and 4
The following numbers would be examples: 1012156789098, 98765210. As I write this out it occurs to me that some combinations of missing digits aren't actually possible to create with step numbers.
For example, it isn't possible to only be missing 3 and 4. You would also need to be missing 0-2 or 5-9. A little thought shows that you can't have any binary representation that features 1's on
both sides of 0's. 0b1111000000, 0b1100011111 or 0b0000001111 are all possible while 0b0011001100, 0b0011000000 or 0b0101010001 aren't possible. Since the 1's represent the digits that the numbers
currently contain then it isn't possible for there to be a gap in the 1's, otherwise the number wouldn't be considered a step number. What this means is that instead of 1,024 possible "shapes" for our
step numbers there's really only 56 of them so this will cut down on our runtime and memory requirements by 20 fold! We go from an array of size 10,240 all the way down to 560, that's much better. I think
at this size I might just use a map instead of an array to store the numbers. The keys stored in the map will be the binary numbers, and the binary numbers can be created by a series of left shifting
(for example the binary number 0b0011100000 will be created by the following (0b0000000111 << 5) )
*/

//Conclusions
/*
I got the answer correct on the first try! It runs in just under a millisecond so I feel pretty good about it, although I feel like there's room for improvement. In every iteration I need to create a new
map of the same dimensions but with 0's for every value. This is because I couldn't quickly think of a way to account for double counting if I just overwrote values of the current map. If I can think of
a good way to do this then the need for creating a new map in every iteration will go away. I get away with it here becuase there are only 40 total iterations, if we had to do millions of iterations then
this wouldn't be very efficient.
*/