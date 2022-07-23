#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes permutationsOfPartitions()
#include <iostream>

//Top Dice
long long permutationsOfPartitionsMod(long long n, long long l, long long maximum, long long minimum, long long mod, long long* facts)
{
	//the same as my standard permutationsOfPartitions() function, but uses the BinomialModLargePrime() function
	//instead of the standard choose() function.
	if (maximum == 0) maximum = n;
	else if (maximum > n) maximum = n;

	if (minimum == 0) minimum = 1;
	else if (minimum < 1) minimum = 1;
	else if (minimum > maximum) minimum = maximum;

	//This function utilizes generating functions to find the partitions.
	long long target = n - minimum * l, primary = maximum - minimum + 1, flip = -1;
	long long secondaryDenominator = target + primary, secondaryNumerator = l + secondaryDenominator - 1;
	long long answer = 0;

	if (secondaryNumerator <= 0) return 1; //there's only one way to choose from 0

	/*if (n == 60 && l == 5 && minimum == 3)
	{
		std::cout << target << ", " << primary << ", " << maximum << ", " << minimum << std::endl;
	}*/
	//we get an equation of the form: [x^target]: (1 - x^primary)^l * (1 - x)^-l
	//for which we solve using binomial expansion
	for (long long i = 0; i <= target / primary; i++)
	{
		long long numb = (BinomialModLargePrime(l, i, mod, facts) * BinomialModLargePrime((secondaryNumerator -= primary), (secondaryDenominator -= primary), mod, facts)) % mod * (flip *= -1);
		//std::cout << BinomialModLargePrime((long long)l, i, (long long)mod, facts) << " * " << BinomialModLargePrime(secondaryNumerator, secondaryDenominator, (long long)mod, facts) << " % " << mod << " = " << (BinomialModLargePrime((long long)l, i, (long long)mod, facts) * BinomialModLargePrime(secondaryNumerator, secondaryDenominator, (long long)mod, facts)) % mod << std::endl;
		//std::cout << (9 * 377348994) << std::endl;
		//std::cout << "binomal(" << secondaryNumerator << ", " << secondaryDenominator << ") % 1000000007 = " << BinomialModLargePrime(secondaryNumerator, secondaryDenominator, mod, facts) << std::endl;
		answer = (answer + numb) % mod;
	}

	return answer;
}

std::pair<std::string, double> q240()
{
    auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

    //long long goal = 250, dice_sides = 50, number_top_dice = 50, total_dice = 10000;
	//long long goal = 70, dice_sides = 12, number_top_dice = 10, total_dice = 20;
	long long goal = 15, dice_sides = 6, number_top_dice = 3, total_dice = 5;
	long long modulus = 1000000007;

	//initialize an array for factorials % 1,000,000,007
	//the maximum factorial we will need is equal to n + the number of top dice
	long long* factorials = new long long[total_dice + 1]();
	factorials[0] = 1;
	for (int i = 1; i <= total_dice; i++) factorials[i] = factorials[i - 1] * i % modulus;

	//minimum_minimum represents the smallest possible value for the minimum top die
	int minimum_minimum = goal - (number_top_dice - 1) * dice_sides;
	if (minimum_minimum <= 0) minimum_minimum = 1;

	//maximum_minimum represents the largest possible value for the minimum top die
	int maximum_minimum = goal / number_top_dice;
	
	//Memoize ways to split dice into two sections
	long long* ways_to_choose = new long long[total_dice + 1]();
	for (long long i = 0; i <= total_dice; i++) ways_to_choose[i] = BinomialModLargePrime(total_dice, i, modulus, factorials);
	//for (int i = 0; i <= number_top_dice; i++) std::cout << total_dice << "! / " << i << "!" << total_dice - i << "! = " << ways_to_choose[i] << std::endl << std::endl;
	//for (int i = 0; i <= number_top_dice; i++) ways_to_choose[i] = (factorials[total_dice] / (factorials[i] * factorials[total_dice - i]));

	for (long long least_top_die_value = minimum_minimum; least_top_die_value <= maximum_minimum; least_top_die_value++)
	//for (int least_top_die_value = minimum_minimum; least_top_die_value < minimum_minimum + 1; least_top_die_value++)
	{
		//calculate both the most and least number of least valued top die we can have
		//for the current least top die value
		int fewest;
		int most;

		if (least_top_die_value * number_top_dice == goal)
		{
			//the equation for setting the fewest and most possible top dice for the current least top die value
			//breaks down if our goal number can only be reached by having all the top dice be the maximum number.
			//For example if the goal was 120, we could only get there if every Top Die was 12.
			fewest = number_top_dice;
			most = number_top_dice;
		}
		else
		{
			//these equations should work in all other cases
			fewest = (number_top_dice * (least_top_die_value + 1)) - goal;
			most = ((number_top_dice * dice_sides) - goal) / (dice_sides - least_top_die_value);
			if (fewest <= 0) fewest = 1;
			if (most > dice_sides) most = number_top_dice;
		}

		for (long long amount_of_least_top_dice = fewest; amount_of_least_top_dice <= most; amount_of_least_top_dice++)
		//for (int amount_of_least_top_dice = fewest; amount_of_least_top_dice < fewest + 1; amount_of_least_top_dice++)
		{
			//std::cout << "Smallest top die = " << least_top_die_value << ", amount = " << amount_of_least_top_dice << std::endl;
			//First we calculate the total ways to shuffle the dice that are less than or equal to the least valued top die
			long long number_lower_dice = total_dice - number_top_dice + amount_of_least_top_dice;
			long long small_shuffle = (ModPow(least_top_die_value, number_lower_dice, modulus, 1) - ModPow(least_top_die_value - 1, number_lower_dice, modulus, 1)) % modulus;

			//factorials[number_lower_dice] / (factorials[i] * factorials[number_lower_dice - i])
			for (long long i = 1; i < amount_of_least_top_dice; i++)
			{
				
				/*if (least_top_die_value == 4 && amount_of_least_top_dice == 5)
				{
					long long b = BinomialModLargePrime(number_lower_dice, i, modulus, factorials) * ModPow(least_top_die_value - 1, number_lower_dice - i, modulus, 1) % modulus;
					std::cout << small_shuffle  -  b << " MOD 1,000,000,007 = " << (small_shuffle - b) % modulus << std::endl;
					std::cout << -12351625 % modulus << std::endl;
				}*/
				//in c++ the modulus operator can give us a negative value which we don't want, so when dealing with subtraction,
				//instead of doing: (A - B) % M === (A % M - B % M) % M we need to do: (A - B) % M === A % M - B % M + M
				small_shuffle = (small_shuffle - (BinomialModLargePrime(number_lower_dice, i, modulus, factorials) * ModPow(least_top_die_value - 1, number_lower_dice - i, modulus, 1) % modulus)) + modulus;
			}
			//std::cout << "small shuffle = " << small_shuffle << std::endl;
			//Next we calculate the total ways to shuffle the top dice that are greater than the least valued top die
			long long large_shuffle = permutationsOfPartitionsMod(goal - amount_of_least_top_dice * least_top_die_value, number_top_dice - amount_of_least_top_dice, dice_sides, least_top_die_value + 1, modulus, factorials);

			//std::cout << "large shuffle = " << large_shuffle << std::endl;
			//We multiply the ways to individually shuffle the upper and lower dice with the overall number of ways to shuffle
			//the dice together
			//std::cout << "overall shuffle = " << ways_to_choose[number_top_dice - amount_of_least_top_dice] << std::endl;
			//std::cout << ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus << " ways to order them.\n" << std::endl;
			answer = (answer + ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus) % modulus;

			//std::cout << "MTD = " << least_top_die_value;
			//std::cout << ", # of MTD = " << amount_of_least_top_dice;
			//std::cout << ", Ways % MOD = " << ((ways_to_choose[number_top_dice - amount_of_least_top_dice] * large_shuffle) % modulus * small_shuffle) % modulus << std::endl << std::endl;
		}
	}
	//ran in 0.000304 seconds

	delete[] factorials;
	delete[] ways_to_choose;
    return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is 7448717393364181966
    //ran in 0.0004609 seconds
}

//NOTES
//This problem is like a combinatroics problem within a combinatorics problem. Basically what you
//want to do is to first look at all of the ways to create partitions of 70 with a maximum number
//of 12 and a maximum length of 10. These partitions represent the sum of our top dice. For each one
//of these combinations we then want to fill the other 10 of the 20 dice with numbers that are less
//than or equal to the lowest value top dice. For example, one of the options for the ten top dice
//is [12, 12, 12, 12, 5, 4, 4, 3, 3, 3]. The other 10 dice can only utilize the numbers 1-3. If a 4
//were to be used then it would replace one of the 3's in the top dice vector and now the sum of the
//top dice woud be 71 instead of 70. Now let's look at the full array of dice, where anything not
//tagged as a top die is represented with a o. The vector looks like this:

//[12, 12, 12, 12, 5, 4, 4, 3, 3, 3, o, o, o, o, o, o, o, o, o, o]

//There are 20!/(10! * 4! * 3! * 2! * 1!) = 2,327,925,600 different ways to distinctly shuffle the
//above vector. Here are 5 of those ways:

//[12, 12, 12, 12, 5, 4, 4, 3, 3, 3, o, o, o, o, o, o, o, o, o, o]
//[12, o, o, o, o, o, o, o, o, o, o, 12, 12, 12, 5, 4, 4, 3, 3, 3]
//[o, 5, 4, 12, o, 12, 12, o, 3, 4, o, o, 3, o, o, o, 3, o, o, 12]
//[12, o, o, o, o, o, o, o, 3, 3, 4, 0, 3, 4, 5, 12, o, 12, 12, o]
//[o, o, o, o, o, o, o, o, o, o, 3, 3, 3, 4, 4, 5, 12, 12, 12, 12]

//What we need to do is to look at these 2,327,925,600 ways and then multiply each one by 3 ^ 10, which
//represent the number of ways to distinctly fill in the holes with digits 1-3. This would lead to a
//total of 137,461,678,754,400 ways to roll 20 dice where the top 10 are equal to 70, for this 1 way
//of making 70. There's a caveat, however, because these ways aren't all distinct. This is because 
//when we select the number 3 to fill one of the gaps, it's interchangeable with the locations of any
//top dice that are equal to 3 as well. For example let's pretend the Top dice were [5, 4, 3] and we
//only had 5 dice to play with, so our vector could be shuffled as both [5, 4, 3, o, o] or 
//[5, 4, o, 3, o]. Looking at the first vector, we could make the first circle a 2 and the second circle
//a 1 which would give us [5, 4, 3, 2, 1]. If we do the same to the second vector then we get the
//distinct formation of [5, 4, 2, 3, 1]. Now instead of 2 and 1 what if we chose 3 and 1? If we did
//that then both vectors would be [5, 4, 3, 3, 1] which aren't distinct from eachother. 

//To mitigate the issue of double counting instances where the lowest top dice are swapped with the
//highest non-top dice we need to add another step. We need to independently do trials for each 
//possible lowest die value and possible number for this lowest die. Let's start with the highest
//possible value of our lowest top die which is 7. There's only one way to have the smallest top
//die be 7:

//[7, 7, 7, 7, 7, 7, 7, 7, 7, 7, o, o, o, o, o, o, o, o, o, o]

//There will be 11 iterations to get the complete contribution to the answer from the above set of
//top dice. In the first iteration we choose 0 of the 10 gaps to be equal to 7, therefore all 10 gaps
//will have a value of 1-6. The distinct permutations from doing this would be 20!/(10! * 10!) *
//6 ^ 10. In the second iteration we force one of the gaps to be the number 7 and the other 9 gaps
//can have a value between 1-6. The distinct permutations from doing this is now 20!/(11! * 9!) *
//6 ^ 9. On the last iteration we force all 10 gaps to equal 7 which leads to only 20!/20! = 1
//distinct permutation. So in the case of our lowest top die being equal to 7 there were 11 different
//values to calculate. If we look at the next possible lowest value, 6, there are actually over 30
//different ways to have the smallest top die be 6. Some of them feature only one 6, others may have
//four 6's. Each one of those 30+ ways to choose the lowest top die with a value of 6 will need 11
//distinct iterations to figure out the correct contribution. So doing a loop from i = 0 to i < 11
//the contribution of each iteration will be 20!/((# of smallest top die in top die set + i)! * (10 -
//# of smallest top die in top die set)! * (10 - i)!) * (smallest top die value - 1) ^ (10 - i). So
//all we need to do is look at every permutation of 70 that is 10 digits long with a maximum value
//of 12, and then for each of those permutations we need to run 10 iterations of converting non-top
//dice to be the same as the lowest top die value. But how to find all correct the permutations of 70?

//The best way to find the permutations that we want is by using something called a generating function.
//I don't quite understand all of the theory behind generating functions, but what I do know is that with
//a little algebra we can quickly find crazy combinations that would take other algorithms thousands of
//times longer to calculate. Let's pretend we want to find all the ways to have 3 numbers sum up to 10
//where the maximum number allowed is a 5 and the minimum number allowed is a 1. The generating
//function to find this would be (x + x^2 + x^3 + x^4 + x^5)^3. Since there are 3 digits we multiply
//the polynomial by itself three times. The exponents on the x's represent the numbers that are allowed
//to be used. Multiplying out the above equation yields: 

//x^15 + 3 x^14 + 6 x^13 + 10 x^12 + 15 x^11 + 18 x^10 + 19 x^9 + 18 x^8 + 15 x^7 + 10 x^6 + 6 x^5 + 3 x^4 + x^3

//Since we're interested in the amount of ways to make 10, we need to look at the coefficient for x^10.
//At first glance it's hard to see how this helps. We can see that x^10 has a coefficient of 18 but is that
//really the number of ways to sum to 10 with 3 digits and using the numbers 1-5? Let's see

//1.  5, 4, 1
//2.  5, 1, 4
//3.  4, 5, 1
//4.  4, 1, 5
//5.  1, 5, 4
//6.  1, 4, 5
//7.  5, 3, 2
//8.  5, 2, 3
//9.  3, 5, 2
//10. 3, 2, 5
//11. 2, 5, 3
//12. 2, 3, 5
//13. 4, 4, 2
//14. 4, 2, 4
//15. 2, 4, 4
//16. 4, 3, 3
//17. 3, 4, 3
//18. 3, 3, 4

//It actually works, and here's why this method can be so powerful, it not only gives all of the
//partitions of 10 with the 3 digits between 1 and 5, but it also tells us each of the permutations
//of these partitions as well! Furthermore, we can restrict which numbers are allowed to be used
//for each digit. If we wanted to see the same permutations as above, but with the restriction that
//the second digit HAS to be the number 1, then we can change our generating function to allow this.
//The generating function would become: (x + x^2 + x^3 + x^4 + x^5) * (x) * (x + x^2 + x^3 + x^4 + x^5)
//= x * (x + x^2 + x^3 + x^4 + x^5)^2 = x^11 + 2 x^10 + 3 x^9 + 4 x^8 + 5 x^7 + 4 x^6 + 3 x^5 + 2 x^4 + x^3.
//The coefficient for x^10 is 2, and looking at the above permutations you can see that there are only
//two ways that have the second digit as 1 (permuations 2 and 4). This opens up a new realm of possibilities
//for this problem. We can now treat the 20 dice as 3 distinct groups. Group 1 is dice larger than the
//smallest top die (these will all be included in the top dice), group 2 will be dice equal to the
//lowest top die (can be split between the top dice and non-top dice) and group 3 will be anything
//less than the lowest top die (these will all be in the non-top dice). Each of these groups can be
//represented with different symbols. Going back to an earlier dice representation, let's change it 
//so that group 1 are x's, group 2 is the lowest top die number and group 3 are o's:

//[12, 12, 12, 12, 5, 4, 4, 3, 3, 3, o, o, o, o, o, o, o, o, o, o] will turn into:
//[ x,  x,  x,  x, x, x, x, 3, 3, 3, o, o, o, o, o, o, o, o, o, o]

//The first vector has 20!/(10! * 4! * 3! * 2! * 1!) = 2,327,925,600 distinct ways to shuffle it while
//the second vector has a much more manageable 20!/(10! * 7! * 3!) = 22,170,720. Now this is where the power
//of the generating function comes in. In the above vector, 3 of the top dice are equal to 3 which means 
//the other 7 must sum up to 61. Utilizing a generating function we can instantly find both all of the
//distinct ways to pick 7 numbers greater than 3 and less than or equal to 12 which sum up to 61, and
//also all of the distinct permutations of those numbers. To find this we just need to look at the 61st
//coefficient of the function (x^4 + x^5 + x^6 + x^7 + x^8 + x^9 + x^10 + x^11 + x^12)^7. So looking
//at the 61st coefficient of that equation gives all of the different ways to fill in the x's in the
//above vector. We already know that we can fill in the circles in 2^10 different ways as we don't care
//about the sum of the bottom 10 dice and we can only use numbers less than the lowest top die which is
//3. This means that in total there are polcoeff((x^4 + x^5 + x^6 + x^7 + x^8 + x^9 + x^10 + x^11 + x^12)^7, 61)
// * 2 ^ 10 * 20!/(10! * 7! * 3!) different ways to fill in the [x, 3, o] vector above where the top 10 dice
//sum to 70. We still need to iterate through this function 11 times (starting with 0 lowest top dice in 
//the bottom 10 and going all the way to 10 in the bottom 10), however, the top dice greater than the
//lowest top die don't change for any of these iterations. So the polcoeff() function only needs to be
//evaluated once and can then just multiply the sum of the parts of the equation that do change over the
//11 iterations. We carry out this algorithm for every possible lowest top die and every possible amount
//of lowest top die and we get our answer in under a milisecond.

//HackerRank Version
//A version of this same question exists on the website HackerRank but with much higher restrictions. There
//are a total of 10,000 dice instead of 20, the dice are 50 sided instead of 12 and there are 50 top dice.
//My current algorithm would work for this problem with the new restrictions, however, 
//I currently don't have a great implementation of the polcoeff() function. I just manually carry out all
//of the multiplications needed to expand the generating function to it's final form. It's possible to 
//find the nth coefficient in a much more efficient manner though. The nth coefficient can be found
//instantly by taking the nth-derivative of the generating function multiplying it by (start_number - 1)!
//divided by n! and then evaluating the equation for x = 0. So for example, to evaluate the answer for
//polcoeff((x^4 + x^5 + x^6 + x^7 + x^8 + x^9 + x^10 + x^11 + x^12)^7, 61) we would take the 61st derivative
//of the generating function, multiply it by 3! / 61! and then plug in 0 for x. This is possible to do by
//rearranging the generating function into partial fractions where we can easily find the n-th derivate. 
//Currently I don't personally know a good way to do this, however, some math languagues like Pari/GP have
//built in functions for this. Dealing with a 50 digit generating function where the numbers 1 - 50 can be
//used in each digit would require millions of additions to carry out succesfully which would in turn 
//require a lot of time. Not to mention there's close to 100,000 ways to have 50! / (x! * y! * x!) where
//x + y + x = 50. If I had a a good implementation of the polcoeff function I could astronomically reduce
//the run time, so until I find a good way to do that I'll leave the HackerRank version of this question
//alone.

//DEBUGGING NOTES
//I had a floating point issue when using the built-in pow() function that was causing my answer to
//be off by very small amounts. It took literal hours of debugging to eventually solve by casting a
//single value to a long long. I didn't want to lose all of the stuff that I wrote to attempt to
//debug so I'll paste it in down here

/*
void getLowCombos(int smallest, int length, std::vector<int> current_set, std::vector<std::vector<int> >& combos, int level = 0)
{
	if (level == length)
	{
		combos.push_back(current_set);
		return;
	}

	for (int i = smallest; i > 0; i--)
	{
		if (i > current_set.back()) continue;
		current_set.push_back(i);

		getLowCombos(smallest, length, current_set, combos, level + 1);
		current_set.pop_back(); //remove last element upon return from lower level of function
	}

	return;
}

void getHighCombos(int smallest, int largest, int goal, int length, std::vector<int> current_set, int current_value, std::vector<std::vector<int> > & combos, int level = 0)
{
	if (level == length)
	{
		if (current_value == goal) combos.push_back(current_set);
		return;
	}

	for (int i = largest; i > smallest; i--)
	{
		if (current_set.size() == 0)
		{
			current_set.push_back(i);
		}
		else
		{
			if (i > current_set.back()) continue;
			if (current_value + i > goal) continue;
			current_set.push_back(i);
		}

		getHighCombos(smallest, largest, goal, length, current_set, current_value + i, combos, level + 1);
		current_set.pop_back(); //remove last element upon return from lower level of function
	}

	return;
}

long long SanityCheck(int goal, int smallest, int number_of_smallest, int top_dice, int total_dice, int max)
{
	std::vector<std::vector<int> > combos, good_combos;
	getHighCombos(smallest, max, goal - smallest * number_of_smallest, top_dice - number_of_smallest, {}, 0, combos, false);

	for (int i = 0; i < combos.size(); i++)
	{
		for (int j = 0; j < number_of_smallest; j++) combos[i].push_back(smallest);
	}
	for (int i = 0; i < combos.size(); i++) getLowCombos(smallest, total_dice - top_dice, combos[i], good_combos);

	if (smallest == 3 && number_of_smallest == 1)
	{
		for (int i = 0; i < combos.size(); i++)
		{
			for (int j = 0; j < combos[i].size(); j++) std::cout << combos[i][j] << " ";
			std::cout << std::endl;
		}
	}

	long long answer = 0, current_count = 0, max_combo = factorial(total_dice);
	for (int i = 0; i < good_combos.size(); i++)
	{
		std::vector<int> distinct;
		int current = good_combos[i][0];
		long long this_answer = max_combo;
		for (int j = 0; j < good_combos[i].size(); j++)
		{
			if (good_combos[i][j] == current) current_count++;
			else
			{
				distinct.push_back(current_count);
				current = good_combos[i][j];
				current_count = 1;
			}
		}
		distinct.push_back(current_count);
		current_count = 0;


		for (int j = 0; j < distinct.size(); j++) std::cout << distinct[j] << " ";
		std::cout << std::endl;


for (int j = 0; j < distinct.size(); j++) this_answer /= factorial(distinct[j]);
answer += this_answer;
	}

	//std::cout << "Sanity Check for i = " << smallest << " and j = " << number_of_smallest << ": " << answer << std::endl;
	return answer;
}

long long semiSanityCheck(int goal, int smallest, int number_of_smallest, int top_dice, int total_dice, int max)
{
	std::vector<std::vector<int> > combos;
	getHighCombos(smallest, max, goal - smallest * number_of_smallest, top_dice - number_of_smallest, {}, 0, combos, false);


	if (smallest == 3 && number_of_smallest == 1)
	{
		for (int i = 0; i < combos.size(); i++)
		{
			for (int j = 0; j < combos[i].size(); j++) std::cout << combos[i][j] << " ";
			std::cout << std::endl;
		}
	}

	long long answer = 0, current_count = 0, max_combo = factorial(top_dice - number_of_smallest);
	for (int i = 0; i < combos.size(); i++)
	{
		if (combos[i].size() == 0)
		{
			std::cout << "yote" << std::endl;
			continue;
		}
		std::vector<int> distinct;
		int current = combos[i][0];
		long long this_answer = max_combo;
		for (int j = 0; j < combos[i].size(); j++)
		{
			if (combos[i][j] == current) current_count++;
			else
			{
				distinct.push_back(current_count);
				current = combos[i][j];
				current_count = 1;
			}
		}
		distinct.push_back(current_count);
		current_count = 0;


		for (int j = 0; j < distinct.size(); j++) std::cout << distinct[j] << " ";
		std::cout << std::endl;


		for (int j = 0; j < distinct.size(); j++) this_answer /= factorial(distinct[j]);
		answer += this_answer;
	}

	//std::cout << "Sanity Check for i = " << smallest << " and j = " << number_of_smallest << ": " << answer << std::endl;
	return answer;
}
*/

/*
Apparently I solved this problem back in 2015. I don't remember doing that but I'm assuming my logic was similar to the first half of
my notes above, before I started doing anything with the generating functions. I wrote this version of code for the Project-Floyd
website as I think it reflects how I would've done things in the past
std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0; //answer could be initialized to no-pivot as there's only one case of that

	std::vector<std::vector<std::vector<int> > > partitions;
	getPartitions(70, partitions, 12, 10);
	long long twentyFact = factorial(20); //this number will get used a lot so only calculate it once

	for (int i = 0; i < partitions[10].size(); i++)
	{
		int minimalTopDice = 0;
		int minimalTopDieValue = partitions[10][i].back();

		//First count the number of minimal top dice
		for (int j = 9; j >= 0; j--)
		{
			if (partitions[10][i][j] > minimalTopDieValue) break;
			minimalTopDice++;
		}

		//Then calculate the number of permutations of non-minimal top dice
		long long maximalTopDicePermutations = factorial(10 - minimalTopDice);
		int currentNumber = partitions[10][i][0], sameNumberCount = 0;
		for (int j = 0; j < (10 - minimalTopDice); j++)
		{
			if (partitions[10][i][j] == currentNumber) sameNumberCount++;
			else
			{
				maximalTopDicePermutations /= factorial(sameNumberCount);
				sameNumberCount = 1;
				currentNumber = partitions[10][i][j];
			}
		}
		maximalTopDicePermutations /= factorial(sameNumberCount); //make sure to divide out the last number

		long long totalDicePermutations = 0;
		long long commonFact = factorial(10 - minimalTopDice);

		//Now that we have the permutations for the Maximal Top Dice, loop through the 11 different possibilities
		//for total number of additional minimal Top Dice we can add
		for (int j = 0; j < 11; j++)
			totalDicePermutations += (twentyFact / (factorial(minimalTopDice + j) * factorial(10 - j) * commonFact) * pow(minimalTopDieValue - 1, 10 - j));

		answer += totalDicePermutations * maximalTopDicePermutations;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7448163872691144859
	//ran in 0.0053879 seconds
}
*/