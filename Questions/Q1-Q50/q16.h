#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes BigNum:, <vector>
#include <Header_Files/print.h>
#include <Header_Files/int_64x.h>

//Power digit sum


std::pair<std::string, double> q16()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	uint8_t digits[350] = { 0 };
	uint8_t two_mult[10] = { 0, 2, 4, 6, 8, 0, 2, 4, 6, 8 }; //ones place value when multiplying element by 2
	uint8_t carry[10] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }; //amount to be carried over when multiplying element by 2
	digits[0] = 1; //2^0 = 1 so start from this point
	uint8_t current_carry = 0, temporary_carry = 0; //will be used to hold the arithmatic carry value
	int significant_digit = 0; //keeps track of which element in the array currently holds the most significant digit

	for (int i = 1; i <= 1000; i++)
	{
		for (int j = 0; j <= significant_digit; j++)
		{
			temporary_carry = carry[digits[j]];
			digits[j] = two_mult[digits[j]];
			digits[j] += current_carry;
			current_carry = temporary_carry;
			if (digits[j] >= 10)
			{
				current_carry += (digits[j] / 10);
				digits[j] %= 10;
			}
		}

		while (current_carry > 0)
		{
			significant_digit++;
			digits[significant_digit] = current_carry % 10;
			current_carry /= 10;
		}
	}

	for (int i = 0; i <= significant_digit; i++) answer += digits[i];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1366
	//ran in 0.0001387 seconds
	//HackerRank Score = 100.00 / 100.00
}

//NOTES
//This question became fairly trivial after creating the BigNum class. Just start with the number 1, multiply it by 2 one thousand times and then add up the digits stored
//in BigNum's number vector. This was good enough to get the answer with a minimal amount of coding, however, .57 seconds isn't really that good. BigNum ultimately
//needs to go back and forth between strings and numbers a lot which isn't efficient so just coded up an array that will hold all of the digits and add them up at the end.
//Just from Googling 2^1000 you can see that it's a number with over 300 digits. Make an array of 8 bit integers with 350 elements to do all of the math in. Switching to this
//method sped up the run time considerably. Went from a time of .57 seconds down to .0000831 seconds which is a 6800x speedup! I'm sure there's a much more elegant way to write
//the algorithm than what I have above but whatever, I'm focusing more on quick algorithms for now than short algorithms. Frustratingly enough the sum of the digits in 2^999 is only
//different than 2^1000 by one. I had initially not made my loop stop at 999 instead of 1000 and was getting really confused as to how my answer was now off by 1.

//UPDATE 9/22/21
//I decided to take another crack at this problem with my newly created int_64x class. Well it creates the number 2^1000 and adds up all of the digits in 0.000009 seconds which is about
//9x quicker than the initial algorithm. Unfortunately it takes an additional 0.0004 seconds on top of that to actual convert the binary representation into a string so it ends up not being as
//quick as the initial algorithm. This one was always going to be tough to beat because the time is so good already, but if I can more efficiently convert int_64x int a decimal
//representation then I may be able to officially update this problem.
/*
int answer = 0;
int_64x number((unsigned long long)1, 1000); //create the number
std::string number_string = number.getNumberString(); //convert to string
for (int i = 0; i < number_string.length(); i++) answer += (number_string[i] - 48);
//ran in 0.0009092 seconds
*/

//HackerRank Update
//Just realized that instead of adding the number to itself 1000 times it would be way more efficient to just multiply it by itself. This way instead of going 2^1 -> 2^2 -> 2^3 -> 2^4 -> etc.
//you grow by a factor of 2 each time: 2^1 -> 2^2 -> 2^4 -> 2^8 -> etc. By multiplying the array by itself, we can get to 2^1024 in only 10 multiplications which is 100x less than the previous
//algorithm. What would be most efficient would be to multiply until we get 2^128 and then divide that by 2^3 to get a value of 2^125. From there we'd get 2^250 -> 2^500 -> 2^1000 through 
//multiplication. The question is, how to find the most efficient multiplication path given different values of n? Per the HackerRank question text our value of n can be anything from 1 - 10,000.
//Our worst case would then be 2^10,000. We can easily (because the exponent is a power of 2) calculate 2^8,192 and 2^16,364. We can get to 2^8192 in only 13 steps, but it would then take another
//~2000 steps to get to the exact exponent we want going 1 exponent at a time. What we can potentially do instead, is just save all of the arrays for each power of 2 up to the maximum value. The number
//10,000 has a binary representation of 0b0010 0111 0001 0000 which means that we can find 2^10,000 by multiplying (2^16 * 2^256 * 2^512 * 2^1024 * 2^8192). I like this now, we can calculate every 
//2^(power_of_2) that we need in just 13 multiplications, then to get any answer below 10,000 we'll need at most an additional 13 steps (8191 is our worst case with a binary represenatation of
//0b0001 1111 1111 1111). So our worst worst case would be 100 trials where every test is 8191 which would require 13 multiplications to create the initial arrays and then 13 * 100 = 1300 more steps
//to calculate 2^8191 on hundred times. This would be a grand total of only 1313 multiplications to calculate our worst case 100 times, whereas in my initial algorithm I required 1000 additions to calculate
//a non-worst case only a single time. This new algorithm will be much more efficient.

//I coded up what I thought was a pretty good multiplication algorithm, however, it ends up running slower than my addition algorithm even though it only goes through 13 iterations. This is actually
//kind of surprising to me, I guess addition is just that much quicker to implement than multiplication. It takes my code 8e-5 seconds to solve for 2^1000 so theoretically 2^10000 should take only 10 x
//longer. Carrying out 100 trials like this and my original code should still run in under a second.

/*
std::vector<int> one = { 2 }, two = { 2 };
for (int i = 0; i < 10; i++) arrayMultiply(one, one);
//vprint(one);

void arrayMultiply(std::vector<int> &arr1, std::vector<int> &arr2)
{
	//takes two vectors of integers and multiplies them together. Vectors need to be given in reverse order, i.e.
	//the most significant digits are at the end of the vectors. The result is stored in "arr1".
	//Example: 123 * 456 = 56,088 --> {3, 2, 1} * {6, 5, 4} = {8, 8, 0, 6, 5}

	//create a vector to store math in, it's size should be equal to the size of arr1 + arr2 as the multiplication
	//wont exceed that number of digits (i.e. 99,999 * 99,999 = 9,999,800,001, the maximum 5-digit number times the
	//maximum 5-digit number gives you a 10-digit number)
	std::vector<int> temp(arr1.size() + arr2.size(), 0);
	int carry = 0, mult = 0, adder = 0, location = 0;

	for (int i = 0; i < arr2.size(); i++)
	{
		//i represents the current place in the bottom number of the multiplication
		for (int j = 0; j < arr1.size(); j++)
		{
			//j represents our current location in the top number of the multiplication
			mult = arr2[i] * arr1[j]; //mult will either be a 1 or 2 digit number, these digits are added to running total until carry is 0;
			for (int k = 0; k < 2; k++)
			{
				adder = mult % 10;
				location = i + j + k;
				carry = adder + temp[location];
				temp[location] = carry % 10;
				while (carry >= 10)
				{
					carry /= 10;
					carry += temp[++location];
					temp[location] = carry % 10;
				}
				mult /= 10;
			}
		}
	}

	//transfer the value from temp on over to arr1
	arr1 = temp;

	//if there's a leading 0 then get rid of it
	if (!arr1.back()) arr1.pop_back();
}
*/