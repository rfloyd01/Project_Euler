#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Counting Fractions
void FindAnswer(std::vector<int>& prims, long long* triangle, int& limit, long long& ans, int current_location, int current_number, int level)
{
	if (level > 0)
	{
		//chip away at current answer using the inclusion-exclusion principle
		long long num = *(triangle + (limit / current_number));
		if (level % 2 == 1) ans -= num;
		else ans += num;
	}
	for (int i = current_location; i < prims.size(); i++)
	{
		if ((long long)current_number * (long long)prims[i] > (long long)limit) return; //the long long typecasts are to prevent integer overflow
		FindAnswer(prims, triangle, limit, ans, i + 1, current_number * prims[i], level + 1);
	}
}

std::pair<std::string, double> q72()
{
	auto run_time = std::chrono::steady_clock::now();
	int limit = 1000000;
	long long answer = (long long)limit * (long long)(limit + 1) / 2 - 1; //start with a value of triangle(1000000) - 1 so that the totient of 1 isn't counted

	long long* triangle = new long long[limit / 2 + 1]; triangle[0] = 0;//an array to store triangle numbers
	for (int i = 1; i < limit / 2 + 1; i++) triangle[i] = triangle[i - 1] + i;

	std::vector<int> prims = primes(limit);
	FindAnswer(prims, triangle, limit, answer, 0, 1, 0);

	delete[] triangle;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 303963552391
	//ran in 0.0136116 seconds
}

//NOTES
//A little thought about this problem and it can be seen that all we really need to do is find the sum
//of the totient function for every number from 2 to 1,000,000. Having used a sieve method of finding
//the totient on problem 70, it would be easy enough to just copy that code and sum up the answer for
//each number. I feel like there has to be a more efficient way though. Since we're summing totients
//over a large range and don't necessarily need any individual totient it would be nice if a pattern could
//be found and exploited. My first thought for such a method: if you were to write the numbers from 
//1 to n for n = 1 to 6 on a separate line it would look like this:
//1
//1, 2
//1, 2, 3
//1, 2, 3, 4
//1, 2, 3, 4, 5
//1, 2, 3, 4, 5, 6
//This is a total of 21 numbers. Now let's look at all of the numbers divisible by 2 (i.e. 2, 4 and 6).
//If you look at these three lines, any number that's divisible by 2 can by eliminated from the totient
//of n because it's coprime. Removing numbers divisiible by 2 leaves: [2: 1], [4: 1, 3], [6: 1, 3, 5].
//In other words, looking at the first prime number of 2, every other line has half of it's numbers 
//removed. Of the 21 initial numbers, 2 + 4 + 6 = 12 numbers were considered for removal and half of them
//were removed. Well 2 + 4 + 6 is the same thing as 2 * (1 + 2 + 3) which is 2 times the third triangle
//number. In short, triangle(3) * 2 / 2 = triangle(3) numbers were removed in total. Now if you were to
//list all the numbers from 1 to n for n = 1 to 1,000,000 on separate lines, there would be a total of
//triangle(1,000,000) total numbers written out. If we apply the same method above for removing numbers
//from this list, the total amount of numbers left after going through all of the primes should be
//equal to the sum of the totient for each number. The final equation would look something like:
//answer = triangle(1000000), for (n = 2 - 1000000) answer -= triangle(1000000 / n)

//The above code would work beautifully except for one detail. Numbers that are divisible by multiple
//primes are going to try and remove numbers that have already been removed. Looking at the number
//6, when we divide it by 2 we're getting rid of the numbers 2, 4 and 6. Now when we divide 6 by 3 which
//is the next prime we're effectively removing 3 and 6. The number 6 is being removed twice. This would
//leave 6 with a calculated totient of 6 - 3 - 2 = 1 instead of the actual value of 2. What we need to do
//is add back 1 for the overlapping number. This phenomenon is known as the inclusion exclusion 
//principle. The opposite thing will happen for numbers that are divisible by 3 distinct primes. Looking
//at the number 30 (which is the first number divisible by 3 distinct primes): 2 removes 2, 4,
//6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28 and 30. 3 removes 3, 6, 9, 12, 15, 18, 21, 24, 27 and 30
//and 5 removes 5, 10, 15, 20, 25 and 30. Numbers put back by 2 and 3 overlapping are 6, 12, 18, 24 and 30
//, numbers put back by 2 and 5 overlapping are 10, 20 and 30 and numbers put back by 3 and 5 overlapping
//are 15 and 30. So of the 30 original numbers, 15 + 10 + 6 = 31 numbers were removed from individual 
//primes and 5 + 3 + 2 = 10 numbers were put back because of double counting. 30 - 31 + 10 = 9, however,
//totient(30) = 8. This is because we didn't look at numbers divsible by 3 primes. The number 30, which
//is divisble by 2, 3 and 5 got removed three times from individual primes, and then added back 3 times 
//for overlapping of two primes. It now needs to be removed 1 final time for the overlapping of 3 primes.
//In short, anything that's divisible by an odd number of primes gets removed from the total and anything
//divisible by an even number of primes get's added back.

//The best approach to this would be to make a vector of vectors, where element n is a vector containing
//all of the numbers below 1,000,000 which are comprised of n distinct primes. If the limit was 100 
//instead of 1,000,000 the vector would look like this: {{}, {2, 3, 5, 7, 11, ... 97}, {6, 10, 14, 15 ...
//95}, {30, 42, 66, 70, 78}}. The total would start at triangle(100) = 5050. Iterating through every element
//of the even numbered vectors you would subtract triangle(floor(100 / n)) from the total and all of the
//odd numbered vectors you would add triangle(floor(100/n)) back to the total to get the final answer.
//In the example with 100, the vector of vectors has a total of 60 numbers contained which means that
//instead of doing multiple division on 100 distinct numbers (probably works out to around 200 divisions)
//we are only doing a single division and a multiplication for 60 numbers which should be at least 40%
//quicker. Looking at 1,000,000, instead of doing multiple divisions on 1,000,000 numbers we're now
//doing a single division on 608,004 numbers so again it should be a bit quicker than just the standard
//sieve method. When all is said and done the correct answer is found with the include-exclude method
//in ~35 milliseconds while the seive method used in other problems took ~70 milliseconds. So not really
//a drastic improvement but twice as fast is still pretty good nevertheless.

//This writeup has already gotten pretty long but one more thing to add in here. While doing this problem
//I found a way to improve the sieve for obtaining the totient of every number from 2 - n. It requires
//no division at all, just subtraction which should offer a pretty hefty speedup as subtraction is much
//quicker than division on a computer. The theory is that if you look at a certain number, n, and list out
//all of the possible fractions with n in the denominator, any number that evenly divides into n will 
//be able to replace one of those fractions with it's own denominator. Looking at the number 10 as an 
//example. Listing out all of the fractions with 10 in the denominator yields: 1/10, 2/10, 3/10, 4/10, 
//5/10, 6/10, 7/10, 8/10, 9/10, and 10/10. The only numbers that divide 10 are 2 and 5 so looking at all
//of the possible fractions with 2 and 5 in the denominator we get 1/2, 1/5, 2/5, 3/5 and 4/5. The theory
//is that every single one of those fractions for 2 and 5 will take over one of the unreduced fractions
//for 10. Listing out the fractions for 10 again we see that it's true: 1/10, 1/5, 3/10, 2/5, 1/2, 3/5,
//7/10, 4/5 and 9/10 (we ignore 10/10 because it equals 1). All of the unreduced fractions make up the 
//totient of 10, i.e. (1, 3, 7 and 9). Now moving forward in a sieve like manner, any number that's evenly
//divisible by 10 will have 4 of it's unreduced fractions taken over by 1/10, 3/10, 7/10 and 9/10. 
//Starting from the number 2, the totient of any number can be found this way. Here's the code for the
//function:
//
//int* totient = new int[limit + 1];
//for (int i = 2; i < limit + 1; i++) totient[i] = i - 1;
//
//for (int i = 2; i < limit + 1; i++)
//{
//	compare += totient[i];
//	for (int j = i + i; j < limit + 1; j += i) totient[j] -= totient[i];
//}
//
//delete[] totient;
//
//start with an array where every element, i,  is set to i-1 and then from there just keep chipping
//away at the value. By the time the i loop reaches a certain element, that element will already have
//it's totient calculated.