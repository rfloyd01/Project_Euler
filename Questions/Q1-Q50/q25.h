#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/int_64x.h>
#include <cmath>

//1000-digit Fibonacci number
std::pair<std::string, double> q25()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1, digits = 1000;

	int_64x first(1), second(1);
	int goal_length = ((digits - 1) / log10(2)); //the number of binary digits required to have a number at least 999 digits long

	while (true)
	{
		if (GetLeadBitLocation(first) >= goal_length) break;

		first += second;
		second += first;

		answer += 2;
	}

	//Once the loop has been exited we'll have found the first number with at least 999 digits. Check to see if it's greater than
	//or equal to 1000 digits. If not then the next number is guaranteed to be due to how numbers in this sequence grow.
	if (first.getNumberString().length() < digits) answer++;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4782
	//ran in 0.0133452 seconds

	//NOTES
	//This is another one that probably could be improved with a non-brute force algorithm but the BigNum class has made this a pretty trivial problem. The largest theoretical size of a BigNum
	//object is the largest size of a std::vector<int> on a given system. This is a 64-bit system so the largest a vector of ints could be is about 2^64 elements which would yield a BigNum of about
	//18,446,744,073,709,551,616 digits long. The numbers in this question only reach 1000 digits in length so nowhere close to the maximum.

	//UPDATE 9/22/21
	//I wrote a new large number class called int_64x recently and wanted to go back and redo some Project Euler problems with it to see how it stacks up against my previous class (BigNum).
	//As I expected it solves this problem much quicker, however, the concept of this problem isn't really suited to the new class. The int_64x class keeps numbers in binary representation
	//and allows for them to grow to any size they want. Although it can do this really fast by utilizing built-in mathemetical operations, I'm not actually sure at this time what would be the
	//most efficient way to calculate the exact number of decimal digits from a binary number. Finding the largest power of 2 in the binary number is trivial and from that we can get a very close
	//estimate as to the number of digits (a 1000 digit decimal number would be on the scale of 10^1000 which would be 2^(1000 / log10(2)) = 2^3322, so a decimal number with at least 3322 bits
	//would do it. What I've done here is to set a goal for one less than 1000 and stop growing the Fibonacci sequence there. Our number at this point is guaranteed to have at least 999 digits,
	//but there's a chance that it will have more. Convert this number to a decimal string (which my new class is actually quite slow at) and see the length. If it's less than 1000 then due to
	//how quickly the Fibonacci sequence grows it's guaranteed to be the next number. The first loop runs in 0.0013 seconds (which is about 26x quicker than my original runtime for this problem),
	//but needing to convert the binary number to decimal to check the length adds on .009 seconds. All in all the run-time is 2.5x quicker now and I'll take any improvement I can get. If I can
	//find a better way to convert int_64x to decimal (which I'm positive I can) then the run-time will improve even more here.
}