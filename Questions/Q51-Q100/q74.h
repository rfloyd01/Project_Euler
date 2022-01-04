#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Digit Factorial Chains
int newNumber(int n, int* fact)
{
	int nn = 0;
	while (n > 0)
	{
		nn += *(fact + n % 10);
		n /= 10;
	}
	return nn;
}

int createChains(int* chains, int* facts, int &circ, int current_num, int level)
{
	if (*(chains + current_num) != 0) //an already seen number has been encountered
	{
		if (*(chains + current_num) < 0) //a negative number denotes that a circular chain has just been found for the first time
		{
			circ = current_num;
			return level + *(chains + current_num);
		}
		else return *(chains + current_num); //an already discovered chain has been hit, don't alter the circular variable and just return existing value
	}
	else
	{
		*(chains + current_num) = -level; //mark current value with -level so when first duplicate is found we know how far away it is
		int val = createChains(chains, facts, circ, newNumber(current_num, facts), level + 1);
		if (circ == 0)
		{
			//this means the current number isn't in the circular part of the number chain so the chain length should be 1 more than the previous number in chain
			*(chains + current_num) = val + 1;
			return val + 1;
		}
		else
		{
			//the current number being looked at is in the circular part of the number chain so should have the same chain length as the previous number
			if (circ == current_num) circ = 0;
			*(chains + current_num) = val;
			return val;
		}
	}
}

std::pair<std::string, double> q74()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int* chain_lengths = new int[2177281];
	for (int i = 0; i < 2177281; i++) chain_lengths[i] = 0;
	int factorial[10] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 }; //save factorials for everything below 10 to avoid recalculating
	int circ = 0;

	for (int i = 1; i < 1000000; i++)
	{
		circ = 0; //this number needs to be reset to 0 every iteration
		createChains(chain_lengths, factorial, circ, i, 0);
		if (chain_lengths[i] == 60) answer++;
	}

	delete[] chain_lengths;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 402
	//ran in 0.0110103 seconds
}

//NOTES
//The first important thing to do for this probelm is find a potential ceiling and see whether or not all chain lengths can be stored in an array. The largest digit
//factorial is 9! = 362,880 therefore the largest chain produced with a number under 1,000,000 is 999,999 = 6 * 9! = 2,177,280 which is a 7 digit number. If we were
//to look at the largest 7 digit number, 9,999,999 we can see that it would chain to 7 * 9! = 2,540,160 which is also a 7 digit number. 2,177,280 seems like a logical
//choice for the length of our array as there should be no way to achieve a chain with a longer length than that. In order to actual discover the chains a recursive function
//can be used to just keep looping to the next number until a number that's already been seen before is found again. There are two kinds of numbers in this problem: numbers
//that are inside the circular part of a chain, and numbers that aren't. Any number that's inside the circular part of a chain should all have the same chain length (i.e.
//in the chain 169 -> 363601 -> 1454 -> (169) the chain length for each of those numbers will be 3). It's important to set these chain lengths to the same value because 
//you don't know which number might be found first in a different number's chain. If you were to set the chain values of the 169 chain to 3, 2 and 1 respectively (because
//looking at 1454 in that chain it only took one more number to see a value that's already been seen before) then when looking at the chain for 69 (69 -> 363600 -> 1454
// -> 169 -> 363601 -> (1454)) 69 would only have a value of 3 instead of it's real value of 5. This actual scenario wouldn't come up because 69 would be examined before
//169 so the 169 chain would actually be set off of 1454, but there will most definitely be scenarios where an issue will arise. Because of this, it's important to keep
//track in the recursive function whether the current number being looked at is in the circular part of the chain or if it isn't. This is accomplished with a reference
//variable named circ. Another thing to keep track of is when the recursive function comes across a number that's already been seen before, was the number found in the
//current call of the recursive function, or an earlier call. If the number in question was encountered during the current call it means a new circular chain has been discovered
//and needs to be set with the circ variable. If it wasn't found in the current function call than the setting of the circular chain numbers step must be skipped over.
//To keep track of this, negative values will be used instead of positive values for any number that hasn't been encountered before. This way if the recursive funciton
//comes across a negative number a new circular chain has been discovered and a positive number means an already discovered number has been found. With the above intricacies
//accounted for, the program find the answer in 11 milliseconds. This was a very interesting problem as it really made you think about things that could be potential bugs.
//Without a clever way to keep track of which numbers were part of the circular chain and which are outside there wouldn't really be a fast way to arrive at the answer.