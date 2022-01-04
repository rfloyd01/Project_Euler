#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>
#include <vector>
#include <cmath>

//Maximum Integer Partition Product
std::pair<std::string, double> q374()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1; //answer could be initialized to no-pivot as there's only one case of that
	
	//TODO(HackerRank): if the goal number is one less than a triangular number, the maximum amount of digits in the chain will end up being
	//one less than what's actually possible. For example, if the goal number was 54 we could get there with [2, 3, 4, 5, 6, 7, 8, 9, 10]
	//currently I start the search with the highest possible pivot number and calculate everything based off of that which in this case
	//the highest pivot would be [2, 3, 4, 5, 6, 7, 8, 10] = 45 and ends up being 1 digit shorter than the physical maximum (where there's no pivot)
	//this shouldn't be a big deal for the Euler problem where it's known whether the goal number meets this criteria, however, it may come into play
	//for the HackerRank version where there will be multiple different goal numbers given.

	long long goal = 100000000000000;
	//long long goal = 100;
	long long max_length = (-3 + sqrt(9 + 8 * (goal - 1))) / 2.0, final_value, current_num = 0;
	
	long long start_number = (max_length + 1) * (max_length + 2) / 2; //start number is the second number in the final group (aka the number with the longest partition and highest pivot point). calculated from triangular number formula
	long long current_factorial = factorial(max_length + 1); //used for faster calculation of factorial
	long long pivot_stop = max_length - 1 - (goal - start_number) - 1; //when pivot reaches this location the maximum possible length decreases by 1
	long long double_pivot_start = max_length;
	if (max_length + start_number > goal) double_pivot_start--;

	//std::cout << max_length << ", " << pivot_stop << std::endl;
	//std::cout << max_length << ", " << start_number << std::endl;

	for (long long pp = max_length - 1; pp >= 0; pp--) //pp stands for pivot point you pervs
	{
		//before changing the "current_num" variable we need to check and see if the pivot location will cause the allowable maximum
		//length to change. This "shrink" effect will only every need to happen one time
		//when the pivot point get's lower than max_pivot - (goal_number - start_number). In the case of goal = 100, our start number
		//is 91 as this is the value when pivot is as far forwards as possibe. The max pivot location in this example is at index 11
		//of our array. 11 - 9 = 2, so as soon as the pivot becomes less than 2, i.e. the first time it hits 1 we need to subtract the 
		//final value of our cumulative multiplication. Keep track of our final value each iteration and when the time comes just
		//subtract it from the "current_num" variable.
		if (pp == pivot_stop) current_num -= final_value;

		//separate multiplication and division into two steps to avoid floating point errors
		current_num *= (pp + 3);
		current_num /= (pp + 2);

		//add new values
		current_factorial /= (pp + 2);
		long long new_values = current_factorial * (pp + 3) * (pp + 1); //calculating the factorial each iteration is slow, keep a running tally of the current factorial instead
		//long long new_values = factorial(pp + 1) * (pp + 3) * (pp + 1); //TODO(maybe): I might need to manually calculate factorials because of modular division. If so I'll need a much better factorial function
		current_num += new_values;

		//set final row value
		if (pp == (max_length - 1)) final_value = current_num; //set the first time
		else
		{
			final_value *= (pp + 3);
			final_value /= (pp + 2);
		}

		//calculate no pivot value by multiplying case where pivot is at the very end
		answer += new_values * (pp + 2) / (pp + 3);

		//calculate double pivot by cascading every number up by 1. This value should only be
		//calculated in the first iteration of loop if start_number + max_length < goal which is
		//why there's the double_pivot_start variable
		if (pp < double_pivot_start) answer += new_values * (pp + 2) / 2 * (pp + 4) / (pp + 3);

		//std::cout << current_num << std::endl;

		answer += current_num;

		//DEBUG: Print out all possible combinations with the current pivot point
		/*for (int i = pp; i < max_length; i++)
		{
			long long product = 1;
			int sum = 0;
			std::cout << '[';
			for (int j = 0; j < pp; j++)
			{
				std::cout << j + 2 << ", ";
				product *= (j + 2);
				sum += (j + 2);
			}
			for (int j = pp; j < i; j++)
			{
				std::cout << j + 3 << ", ";
				product *= (j + 3);
				sum += (j + 3);
			}
			product *= (i + 3);
			sum += (i + 3);
			std::cout << i + 3 << "]: " << sum << " = " << (i + 1) * product << std::endl;
		}
		std::cout << std::endl;*/
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.199284 seconds (this is current run time when goal is set to 10e14)
}

//NOTES
/*
initial thoughts:
At first when I saw this problem I thought it was going to be really hard, the number of integer partitions for each
number goes up really quickly as numbers get higher. The problem mentions though that each integer in the partition
must be distinct, which is great because it will really cut down the possible partitions that there are. My gut tells
me that the more integers you can squeeze into a partition the higher the product will be and
furthermore, the closer the numbers are together the higher the product will be. Let's look at the number 14, the partition
of length 3 that gives the lowest product is 1, 2, 11 with a product of 22 and the partition with the highest product
is 3, 5, 6 with a product of 90. So numbers closer together gave a largerproduct. Now let's look at partitions of length
4. The lowest product partition is 1, 2, 3, 8 with a product of 48. So the worst partition of 4 is better than the worst
partition of length 3. The best partition of length 4 is 2, 3, 4, 5 with a product of 120 which is better than the best
partition of length 3. So it seems that my gut reaction may be true, more numbers and close together numbers is what we
want. As a last initial thought, I think that the number 1 will
need to be avoided like the plague. Any partition that features a 1 will have an equivalent partition that's one integer shorter
but has a higher product. For example here's a partition of 6: 1, 2, 3 and it has a product of 6. If the 1 is merged
into the 3 it gives the partition 2, 4 which has a product of 8. The number 1 doesn't increase the product so it shouldn't be used at all.

Project Euler Solution(#1):
Before I got too far down the rabbit hole here I wanted to test whether or not my assummptions in the initial thoughts
section were correct. I wrote a down and dirty function to calculate all of the partitions of a number that don't
feature any repeated numbers and here's what it spits out for the first 10 numbers:
1:  [1] = 1
2:  [2] = 2
3:  [3] = 3
4:  [4] = 4
5:  [3, 2] = 6
6:  [4, 2] = 8
7:  [4, 3] = 12
8:  [5, 3] = 15
9:  [4, 3, 2] = 24
10: [5, 3, 2] = 30

This really makes it seem like my theory holds water, also there's a clear pattern here. The amount of integers in the
maximal partition will only increase when you get a number that can be expressed in the form 2, 3, 4....n. Well this
looks awfully familiar, it's just a triangle number without the one. If this is true then the first number with a maximal
partition of length 4 should be triangle(5) - 1 = 14 and the first to feature a maximal length of 5 would be triangle(6)
- 1 = 20. Here's what my program spits out for numbers 13 - 20:

13: [3, 4, 6] = 72
14: [2, 3, 4, 5] = 120
15: [2, 3, 4, 6] = 144
16: [2, 3, 5, 6] = 180
17: [2, 4, 5, 6] = 240
18: [3, 4, 5, 6] = 360
19: [3, 4, 5, 7] = 420
20: [2, 3, 4, 5, 6] = 720

So it looks like I was spot on. This means that we can just look at individual groups of numbers that have the same
maximal partition length and sum them all together. This partial sum can then just be multiplied by the partition
length. Do this for every group up to 10^14 (or whatever the limit is) and we should have our answer. Looking at each
of these groups a very clear pattern emerges from number to number. Since we can't have repeats, you just add 1 to the
lowest possible digit to get the maximal product for the next number. Looking at 16 for example, the maximal product
comes from the partition [2, 3, 5, 6]. To get to 17 we can add 1 to any of these numbers, although adding 1 to 2 would
yield 3 and we already have a 3 so that's not allowed. The same goes for 5 as it will give us a 6 which we already have.
So we can add the 1 to either the 3 or the 6. In terms of maximizing the product it'll be better to add it to the 3 because
the product will increase by a factor of 4/3, if we add the 1 to the 6 then the product will only increase by a factor
of 7/6. Adding the 1 to the 3 gives the partition [2, 4, 5, 6] and as shown above, that is indeed the maximal
partition for 17.

If we use the above method of adding 1 to the lowest possible number for a whole group it can be modeled via an
equation. Let's look at the group starting with the number 14, the first number in this group is the product of 14's
maximal partition which is 120 (the 5th triangle number). Each product after it can be obtained by multiplying this
first number with fractions (the number getting the 1 added to it becomes the denominator and the numerator is the value
after 1 is added):

14: x = 120
15: x*6/5 = 144
16: x*6/5*5/4 = x*6/4 = 180
17: x*6/5*5/4*4/3 = x*6/3 = 240
18: x*6/5*5/4*4/3*3/2 = x*6/2 = 360
19: x*6/5*5/4*4/3*3/2*7/6 = x*7/2 = 420

Since each maximal product can be expressed in terms of the first product in the group, the sum of all the products in
the group can be expressed as:

x = 120
6/6x + 6/5x + 6/4x + 6/3x + 6/2x + 7/2x = x(6(1/6 + 1/5 + 1/4 + 1/3 + 1/2) + 7/2)
										  120(6(20/120 + 24/120 + 30/120 + 40/120 + 60/120) + 7/2)
										  6(20 + 24 + 30 + 40 + 60) + 60 * 7 = 1,464

By summing the fraction 1/i from i = 2 to i = 6, multiplying that sum by 6 and then adding the final fraction which is
a bit of an oddball, we can multiply that whole value by x and get the sum of the maximal products for this entire
group. I don't know it off the top of my head but I feel like there's a good wayrgd xve to sum the sequence of 1/2 - 1/n.

A quick Google search led me to this wikipedia page (Harmonic Numbers). Unfortunately it doesn't appear to be as easy
to sum these numbers as I'd first hoped as it's a diverging sequence. It appears that there are a few ways to approximate
these sums though to a degree that should be good enough for this problem. This equation which is found at the bottom of
the calculations section on the wikipedia page:

Hn ~= ln(n) + y + 1/(2n) - sum(B2k / 2kn^(2k), for k = 1 to infinity)
y = Euler-Macheroni constant ~= 0.5772156649
Bk = Bernoulli numbers

This equation looks a little something like this when expanded:
Hn ~= ln(n) + y + 1/2n - 1/12n^2 + 1/120n^4....

The above equation looks a little intimidating but I'm sure that the Bernoulli sum part only needs to be taken out a few terms
in order to get a close enough approximation that works for this problem (I hope so anyway). I'm starting to think that the heart
of this problem has nothing to do with number partitions at all, and everything to do with these Harmonic numbers.

So, to summarize:
each group, n, can be summed with the following equation:
(n-1)! * (n * (harmonic(n) - 1) + (n + 1)/2)
and then this sum is just multiplied by n-2 as that's the number of integers in this partition group.

each group n starts at triangle(n - 1) - 1. This means the groups are as follows:
Group 1 = doesn't exist
Group 2 = 0-1
Group 3 = 2-4
Group 4 = 5-8
Group 5 = 9-13
Group 6 = 14-19
.....

group 4 calc test:
3! * (4 * 1.083333 + 2.5) = 41 * (4 - 2) = 82 (works!)

One last note here, the final group of numbers to check will most likely need to be calculated differently. This is because
the odds of the last number being a triangular number are pretty low. For example, if we were to check the sum of all numbers
up to 16, that would put us in the middle of group 6. This means that group 6 would need to be calculated like so:

partial group 6 calc test:
14: 2 * 3 * 4 * 5 = 120
15: 120 * 6/5 = 144
16: 144 * 5/4 = 180
120 * 6(1/6 + 1/5 + 1/4)
missing: 1/3 + 1/2 = harmonic[3]
120 * 6(harmonic[6] - harmonic[3])

5! * (6 * (1.45 - 0.8333)) *the n+1 / 2 bit is also excluded
5! * (6 * 0.6166667) = 444 = 120 + 144 + 180

The biggest hang up I have with this right now is that we'll need to do triangle(n) ~= 10e14 -> n ~= 14,142,000 different
calculations. 14 million should be possible to deal with, but I don't think that I'll be breaking any speed records. I'm a
little nervous too because I won't be dealing with exact numbers as the harmonic series is just an approximation of the sum.
The numbers will be big enough that they need to be modded, so I'll be truncating numbers which are built using approxiations.
I think what I may do instead of trying to generate these harmonic sums with that fancy equation, just do it the down and dirty
way by continuously adding 1/n and storing the values in an array. I feel like I'm maybe one or two little break throughs away from really solving this problem but hopefully what I have now is
good enough to at least get to the right answer and then reading the forum will shed some light.

Currently my answer for f(100) is 1683550844460 whereas the correct answer is 1683550844462. Clearly my thinking is correct here as
I'm only off by 2. My current algorithm runs in 0.0000603 seconds whereas a brute force approach takes about 6.5 seconds meaning this
algorithm is 100,000x faster. The fact that the answer is only off by 2 though makes me think that there's definitely some kind of floating
point error occuring. For the harmonic numbers I may need to keep them in their fractional form, however, this is going to be impossible
once the parts of the fraction exceed long integers. This is always a hard thing to do, but I think I need to rethink some things here.
Even though my approach is correct in theory, it may end up being too difficult to get to the actual answer this way.

Project Euler attempt(#2)
The good part about the first attempt is that I know for certain each answer follows a very specific pattern. This time around, instead
of organizing each number into groups based on the size of the number (i.e. first 4 numbers are a group, next 5 numbers are a group) I
want to try and create groups by pattern. For example, each triangle number will be an answer so therefore the triangle numbers can form
a group: max of 2 = [2], max of 5 = [2, 3], max of 9 = [2, 3, 4]... max of 10^14 ~= [2, 3, 4 ... ~10e7]. For each of these partitions that
makes up a triangle number, a number within that partition can be selected where we change every number above it to be 1 more than it's
current value. Looking at this partition [2, 3, 4, 5, 6] let's say we pick element 2 as our pivot point, then the partition would look like
this: [2, 3, 5, 6, 7] which is the correct partition for the number 23. We also could've picked element 4 as the pivot point to get the partition:
[2, 3, 4, 5, 7] which is the correct partition for 21. Will it actually be beneficial to order the partitions into these kinds of groups?
Well let's take a look at a few of the possible partitions with an element 2 pivot:
[2 * 3 * 5] + [2 * 3 * 5 * 6] + [2 * 3 * 5 * 6 * 7] + [2 * 3 * 5 * 6 * 7 * 8]...
30 * ([1] + [6] + [6 * 7] + [6 * 7 * 8])
30 * (1 + 6 * (1 + 7 * (1 + 8))) = 11,550

This almost resembles a continued fraction, but the opposite, each step is a multiplication instead of a division. We could continue the sequence
as long as we want and the continued multiplication should be of the same form. I'll need to do some research but maybe there's a good way to quickly
figure out the total sum for a continued multiplication like listed above. What happens if we look at a few of these sequences next to eachother? Here's
a pivot point of 0, 1 and 2 all next to each other:

[3] + [3 * 4] + [3 * 4 * 5] + [3 * 4 * 5 * 6] + [3 * 4 * 5 * 6 * 7] + [3 * 4 * 5 * 6 * 7 * 8]...
	  [2 * 4] + [2 * 4 * 5] + [2 * 4 * 5 * 6] + [2 * 4 * 5 * 6 * 7] + [2 * 4 * 5 * 6 * 7 * 8]...
				[2 * 3 * 5] + [2 * 3 * 5 * 6] + [2 * 3 * 5 * 6 * 7] + [2 * 3 * 5 * 6 * 7 * 8]...

If you were to ignore the very first element in the second line, each partial sum should be 4/3 more than its equivalent in the series below it. The same
goes when comparing line 2 to line 1. Ignoring the first element, each successive element will be 3/2 times more. In other words the above three lines could
be written like so (and reverse the order for good measure):

pivot @ 2 = x
pivot @ 1 = 4/3 * x + [2 * 4] = y
pivot @ 0 = 3/2 * y + [3] = z

If we were to start at the last possible pivot point and work our way backwards to a pivot point of 0 then the cumulative sum of each group can be found iteratively
fairly easily. The value of our starting point would just be (n - 1)! * (n + 1) where n = pivot_point + 2. The highest n would be one where only the final element of
the parition with the highest length has been increased by 1. Then we would just iterate downwards like so:

For example if max n = 10:
sum = 0
n = 10, current_value = 9! * 11, sum += current_value
n = 9, current_value *= 10/9, current_value += 8! * 10, sum += current_value
n = 8, current_value *= 9/8, current_value += 7! * 9, sum += current_value
....

Going like this should get you the answer (with the exception that the max partitionsof 1 and 4 would need to be added in manually. Approaching the problem in this way
seems better than my initial approach for 2 reasons. I don't need to calculate harmonic numbers which are from the addition of fractions. The highest fraction that I'll
need in this manner should be something on the scale of (10e7+1) / 10e7

More Notes for second attempt at problem:
-----------------------------------
Pretend that Goal = 100
[2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14] -> 100 = Goal with pivot at element 2 (max length = 12)

Any partition with its pivot point past 2 will be able to reach a length of 12
[2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14] ->  99 = Pivot at element 3
[2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14] ->  98 = Pivot at element 4
...........................................
[2, 3, 4, 5, 6, 7, 8,  9, 10, 11, 12, 14] ->  91 = Pivot at element 11

Any partition without a pivot (i.e. a triangular number - 1) can also have a max length of 12
[2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13] -->  90 = max length partition with no pivot = 12

Any partition with it's pivot point less than 2 can't reach the max length of 12
[2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14] -> 101 = length of 12 is too long
[2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13] ----->  87 = max length partition with pivot at 1
[3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13] ----->  88 = max length partition with pivot at 0

There's a final category where the pivot is @0 but the final element is also pivoted again.
This category represents the final element before the length of max partition increases.
[3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14] -----> 89 = max length partition with double pivot = 11

All partitions should be able to be found this way with the exception of 1 and 4 because
they're kind of wild cards, so 2 will need to be added to the final answer.

//strange pivots
no pivot  = 12 instances
dbl pivot = 10 instances

//less than max pivot
pivot @ 0 = 11 instances
pivot @ 1 = 10 instances

//standard pivots
pivot @ 2 = 10 instances
Pivot @ 3 =  9 instances
Pivot @ 4 =  8 instances
Pivot @ 5 =  7 instances
Pivot @ 6 =  6 instances
Pivot @ 7 =  5 instances
Pivot @ 8 =  4 instances
Pivot @ 9 =  3 instances
Pivot @ 10 =  2 instances
Pivot @ 11 =  1 instances

//final wild cards
1 and 4    =  2 instances

Grand Total = 100 instances of partitions for the numbers 1 - 100
----------------------------------------------
Moving through the standard pivots:
----------------------------------------------
Start at max, it will be most CPU intensive but this sum must be calculated:
1. Find base number = product of numbers below and including the pivot:
	a. [2 * 3 * 5] = 30
2. Do a repeated multiplication to get the product here:
	a. 30 * (1 + 6 * (1 + 7 * ( 1 + 8 *...(1 + 14))))))...) = 23,482,067,070
3. Move on to the next highest pivot (3 in this case)
4. With the pivot point 1 location further and the maximum length the same, each partition in this
   pivot group is just 4/5 the value of the previous pivot group. This means the overal sum of the
   pivot group will be 4/5 the sum of the previous pivot group as well (with an additional subtraction
   at the end to account for no instance of partition with length 3 in this pivot group):
	a. My guess is that total sum for this pivot group will be 4/5 * (23,482,067,070 - 30) = 18,785,653,632
	b. Double check estimate using same method as step 2a:
		i. Base case = [2 * 3 * 4 * 6] = 144
		11. 144 * (1 + 7 * ( 1 + 8 * ( 1 + 9 *...(1 + 14)))...) = 18,785,653,632 (double check works yeet)
5. Do this for every pivot value up to the maximum pivot value
	a. Value for pivot at x = (x+1)/(x+2) * (last_iteration_sum - (x)! * (x+2))
	b. sum of pivot at 4 = 5/6 * (18,785,653,632 - 144) [144 = 4!*6]
		i. Pivot at 4 = 15,654,711,240
		ii. double check: (2*3*4*5*7) * (1 + 8 * (1 + 9 *...(1 + 14))...) = 15,654,711,240 (yeet again)
	c. The only really intensive thing to do at each step is to calculate the value being subtracted
		i. Since this is iterative, the value of n should always be (n+1) * ((n+3)/(n+2)) times
		   more than the previous value
		ii. Test 1: n2 = 30  -> n3 = (3+1)*(6/5)*30  = 144
					Test 2: n3 = 144 -> n4 = (4+1)*(7/6)*144 = 840 [840 = 2*3*4*5*7]
					Works in both cases
		iii. It will be much more efficient to keep a running tally than to calculate the factorial
			 fresh each iteration

The above method is superior to original method:
1. There should be the same amount of steps:
	a. First calc features roughly sqrt(# of all partitions) steps in 1 go (maybe less if can find out formula
	   for repeated multiplication)
	b. Last calc will be a single partition
	c. Triangle(n) number of steps like first algorithm so timing should be at worst case equal
2. Don't need to calculate a factorial each step using iterative tracking of subtraction variable
	a. to be fair this probably could've been implemented in first algorithm as well
3. Shouldn't be any floating point errors, fractions are only multiplied and not added together
4. Maximal fraction should be on a scale of (~10^7+highest_pivot-1)/(~10^7+highest_pivot)
	a. this means that the highest fraction can be tracked by keeping the numerator and denominator
	   separate using int types, not even long int should be necessary
	b. in the original method the highest fraction would've needed probably hundreds of digits to
	   correctly track.
		i.  By the 10th iteration the fraction is 4861/2520 (1/2 + 1/3 + 1/4 ... + 1/10)
		ii. By the 20th iteration the fraction baloons to 40315631/15519504 (1/2 + 1/3 ... + 1/20)
		iii. By the time the fraction gets to the n = ~10^7 iteration the fraction would just be
			 absurd and untrackable (not easily anyway). Here, the int type as already almost
			 overflowed.
5. Since the fractions are able to be realistically tracked, performing modulo division to "shrink" numbers
   shouldn't be an issue.

It still feels like there should be a much better way to do this problem, however, the reservations I had before
are gone now. This should work without floating point issues and be able to sucessfully calculate (with modulo
division) high numbers. I still don't think it will be very fast, however.
------------------------------------
Addition of the multiplication of number of partitions for each partition
------------------------------------
I kind of got wrapped up in this new method and forgot to add a way to multiply each partition by the number of
integers in the actual partition. I think that this can be taken care of though in the very first step by slightly
changing the repeated multiplication. Let's look at an example where 23 is the goal value:

Minimum Normal Pivot Point is 2
[2, 3, 5, 6, 7]

Total for this group would be
5 * [2*3*5*6*7] + 4 * [2*3*5*6] + 3 * [2*3*5] = 6,300 + 720 + 90 = 7,110

Now let's look at the repeated multiplication without integer amount:
2*3*5 * (1 + (6) + (6*7))
30 (1 + 6 * (1 + 7))

Now let's do the same but include the integer amounts
2*3*5 * (3*(1) + 4*(6) + 5*(6*7))
30 * (3*1 + 6 * (4 + 5*7)) = 7,110

Hmm a little strange, let's throw a few more instances to help see the pattern

**Below was to double check the repeated multiplication formula
14!/4 = 21,794,572,800
/14   =  1,556,755,200
/13   =    119,750,400
/12 = 9979200
/11 = 907200
/10 = 90720
/9 = 10080
/8 = 1260
/7 = 180
/6 = 30
*/