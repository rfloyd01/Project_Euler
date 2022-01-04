#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes powers_of_two[]
#include <map>
#include <vector>
#include <bitset>

//Fractions involving the number of different ways a number can be expressed as a sum of powers of 2
struct BaseBinaryNumber
{
	long long ones;
	long long zeros;
	long long ways;

	BaseBinaryNumber(long long one, long long zero)
	{
		ones = one;
		zeros = zero;
		ways = 1 + ones * zeros;
	}
};

void recursiveBinaryConstruct(long long goal1, long long goal2, std::vector<BaseBinaryNumber>& binary_groups, std::vector<int> current_number, std::vector<long long> equations)
{
	//I don't like using a vector to hold the value of our current number is it involves adding and removing things between recursive calls which isn't very quick.
	//I plan to (hopefully) solve the problem and then when it's time to reduce the run time I'll rethink a better way to do this.
	if (equations[1] == goal1)
	{
		//base case 1, the desired base case. Now check to see if n-1 will produce goal2
		std::cout << "Answer found with the following combination: ";
		vprint(current_number);
		return;
	}
	else if (equations[1] > goal1)
	{
		//base case 2, the bad base case. Return without doing anything
		return;
	}
	else
	{
		//haven't reached our base yet, keep drilling down
		for (int i = 0; i < binary_groups.size(); i++)
		{
			/*
			I got a little long winded in the above paragraphs so basically to summarize, when tacking a new group onto the back of an existing one there's two important equations we need:
            1. Total ways new combo group     = (total_ways_current_group * total_ways_new_group) + ways_current_group_ends_in 1
            2. Ways new combo group ends in 1 = (total_ways_current_group * ways_new_group_ends_in_1) + total_ways_current_group_ends_in_1
            (the total_ways_current_group variable will need to be saved as a temp value as it's needed for both calcs)
			*/
			int current_one_ways = equations[0];
			int current_total_ways = equations[1];

			//add next number to chain
			current_number.push_back(binary_groups[i].ones);
			current_number.push_back(binary_groups[i].zeros);

			//update equations
			equations[1] = (current_total_ways * binary_groups[i].ways) + equations[0];
			equations[0] += (current_total_ways * binary_groups[i].ones);

			recursiveBinaryConstruct(goal1, goal2, binary_groups, current_number, equations);

			//remove last number from chain
			current_number.pop_back();
			current_number.pop_back();

			//reset equations
			equations[0] = current_one_ways;
			equations[1] = current_total_ways;
		}
	}
}

std::pair<std::string, double> q175()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	int max_binary_length = 10, goal1 = 1000, goal2 = 17;
	std::vector<BaseBinaryNumber> binary_groups;
	for (int i = 2; i <= max_binary_length; i++)
	{
		for (int j = 1; j < i; j++)
		{
			BaseBinaryNumber bin(i - j, j);
			binary_groups.push_back(bin);
		}
	}

	std::vector<long long> starting_equation = { 0, 1 };
	std::vector<int> current_number;
	//recursiveBinaryConstruct(goal1, goal2, binary_groups, current_number, starting_equation);

	/*for (int i = 0; i < binary_groups.size(); i++)
	{
		std::cout << "{" << binary_groups[i].ones << ", " << binary_groups[i].zeros << "}" << std::endl;
	}*/

	/*const int limit = 100;
	long long* ways = new long long[limit + 1]();
	ways[0] = 1;

	int two_pow = 1;
	while (two_pow <= limit)
	{
		for (int i = limit - two_pow; i >= 0; i--)
		{
			ways[i + two_pow] += ways[i];
			if ((i + two_pow + two_pow) <= limit) ways[i + two_pow + two_pow] += ways[i];
		}
		two_pow *= 2;
	}

	for (int i = 0; i <= limit; i++) std::cout << i << "(" << std::bitset<8>(i) << "): " << ways[i] << std::endl;
	std::cout << std::endl;

	delete[] ways;*/

	std::vector<int> facts = PrimeFactors(123456789);
	vprint(facts);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 126461847755
	//ran in 0.0008542 seconds
}

//NOTES
//Initial Thoughts:
/*
My first thought here was to write out all of the equations for numbers 1 - 10 and see if there were any noticeable patterns. It was pretty easy to see early on that using a dynamic approach allows for 
the creation of all the equations. Iterating through powers of 2 (going up from 1, 2, 4, etc.) in one loop, and then working backwards through the array in an inner loop yields:

int two_pow = 1, limit = 10;
while (two_pow <= limit)
{
	for (int i = limit - two_pow; i >= 0; i--)
	{
		ways[i + two_pow] += ways[i];
		if ((i + two_pow + two_pow) <= limit) ways[i + two_pow + two_pow] += ways[i];
	}
	two_pow *= 2;
}

0's [0, -, -, -, -, -, -, -, -, -, -]
1's [0, 1, 1+1, -, -, -, -, -, -, -, -]
2's [0, 1, 1+1 2, 2+1, 2+1+1 2+2, 2+2+1, 2+2+1+1, -, -, -, -]
4's [0, 1, 1+1 2, 2+1, 2+1+1 2+2 4, 2+2+1 4+1, 2+2+1+1 4+1+1 4+2, 4+2+1, 4+2+1+1 4+2+2 4+4, 4+2+2+1 4+4+1, 4+2+2+1+1 4+4+1+1 4+4+2]
8's [0, 1, 1+1 2, 2+1, 2+1+1 2+2 4, 2+2+1 4+1, 2+2+1+1 4+1+1 4+2, 4+2+1, 4+2+1+1 4+4 4+4+2 8, 4+2+2+1 4+4+1 8+1, 4+2+2+1+1 4+4+1+1 4+4+2 8+2 8+1+1]

In the short code a wrote to do this I didn't actually have the equations written out, just numbers to represent the total number of equations in each slot:
0's [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
1's [1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]
2's [1, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0]
4's [1, 1, 2, 1, 3, 2, 3, 1, 3, 2, 3]
8's [1, 1, 2, 1, 3, 2, 3, 1, 4, 3, 5]

Although it's possible to get the appropriate number of equations dynbamically pretty easily, we're going to end up going to very high numbers so using an array (at least in this implementation)
won't be possible. I studied the numbers for a little bit to see if there's any kind of clear pattern and I really didn't see anything too clear. There were a few things though. Each succesive power
of 2 has one more equation than the last. Looking at the above arrays you can see that 1 = 1 equation, 2 = 2 equations, 4 = 3 equations and 8 = 4 equations. This pattern continues for each power of
2. Also, everything directly before a power of 2 only has 1 possible equation (with the exception of 1 which is itself a power of 2 but comes directly before the number 2). 3 has 1 equation and 7 has
1 equation. I don't show it above but this is true for every number 2^(n) - 1 that I saw all the way up to 500. Although I'm sure that there's a pattern, I wasn't really getting anywhere just looking
at the total number of equations.

Next I decided to start looking at binary representations of the numbers and this is where I started to get some traction.
*/

//First Solution Attempt
/*
The answer to this question is a binary representation of a number so I figured that I'd need to look at the binary representations at some point, might as well start off now. Comparing the binary 
representation for the first 10 numbers to the total number of equations for each of those numbers you start to see some clear patterns. Every number has a distinct binary representation which utilizes only 
distinct powers of 2. i.e. 6 can be written in binary as 0b0110 which is the same as saying 2 + 4. Well for this problem we're allowed to use two of each power of 2 instead of only 1. So instead of each
of our numbers 1-10 being represented as a distinct binary number, it will instead be the addition of 2 binary numbers.

One of the ways to create 6 is by adding 1 and 5, well in binary this looks like:

0001 +
0101
====
0110

Looking at all the binary digits we used the above equation becomes 1 + 1 + 4 = 6 which is indeed one of the acceptable equations I came up with in my initial thoughts. Looking at the problem with this frame
of mind changes things up. We're now looking for the number of distinct ways we can add smaller binary numbers to get our desired number where the columns are distinct. For example, the following 2 equations
are the same thing from a binary standpoint even though the numbers are physically different:

0101100 (44) +
0110101 (53)
=======
1100001 (97)

0110100 (52) +
0101101 (45)
=======
1100001 (97)

In the above two equations we add four distinct numbers and get 97 in both cases, however, in both cases the binary equation looks like this: 1 + 4 + 4 + 8 + 16 + 32 + 32. The reason the equations are the same
is that the number of 1's and 0's in each column of the equation is the same, it's just that in the columns that feature both a 1 and a 0 the positions have been switched in some cases. Seeing the equations written
out like this made me think of things slightly differently. Instead of just randomly trying to guess all of the combinations of 1's and 0's for each column, we can actually create them by shifting 1's down from
their highest potential location. Let's look at 97 again, but with the highest possible binary represntation (i.e. each bit in the first number of the equation is shifted as far left as possible):

1100001 (97) +
0000000 (0)
=======
1100001 (97)

As long as there isn't a 1 directly to the left of a given bit, then we can "shift" the bit to the right by flipping the bit from 0 to 1 in both numbers of the equation directly to the right. In the given equation
set above there's only a single 1 that matches this criterion, the one in the 32's place. So we set it to 0 and then flip the bit in the 16's place for both of the equations to get:

1010001 (81) +
0010000 (16)
=======
1100001 (97)

No we're given two options. We can either flip the bits to the right of the 16's place, or, we can flip the bit currently in the 64's place. In this scenario it's better to flip the highest bit so that all possible
options are explored so:

0110001 (49) +
0110000 (48)
=======
1100001 (97)

We just carry out this same procedure until we can't go any further, i.e. we hit the positive bit located in the 1's place of the top number in the equation. We learn a few things from this"
1. We need to split our starting binary number into different groups. A group counts as something that starts with a 1 and ends in a 0. For example given the binary number 0b 110001101000110
   we can see that there are 4 distinct groups: group 1 = 11000, group 2 = 110, group 3 = 1000 and group 4 = 110. It's worth noting that the final group is the only one that's allowed to end in
   a 1 bit (this happens when the number is odd), all other groups must end in a 0. Furthermore, no group can start with a 0.
2. The total number of ways to distinctly order a group is just equal to 1 + #_of_1s * #_of_0s. For example the binary number 0b110 (6) is made of only 1 group. The number of equations we can get
   from this group and thus for the whole number = 1 + 2 * 1 = 3 which matches what I saw above. 
3. Binary groups that are next to each other in a larger number can "interconnect" with eacother when the last bit of the first group is a 1. This means that if we look at all the different ways to
   have the final bit of the first group be a 1, there are exactly that many ways in which we can "connect" the two groups which wouldn't be possible if the last bit wasn't a 1. This observation is
   a little confusing sounding so here's what it looks like:

   Start number is 10010 (18) which has group 1 - 100 and group 2 - 10. There are 1 + 2*1 = 3 ways to order group 1 and 1 + 1*1 = 2 ways to order group 2. 3 * 2 = 6 so you would think that it goes to
   follow that there are 6 total equations for the number 18, this is not true though, 18 has 7 total equations. The final option comes when we choose to "interconnect" group 1 and group 2. There's
   only a single way to have group 1 end in a 1 bit : 001 + 011. Since there's only 1 way to end in a 1 bit, there's only 1 additional equation

4. The number of ways to have a binary group end in a 1 bit is equal to the total number of 1's that are in the equation. For example if our binary group was 111000 then there are three different ways
   to have this group end in a 1 bit: 110001 + 000111, 101001 + 001111 and 011001 + 011111.
5. We can only interconnect groups in 1 direction, from the lower group number to the higher group number. The reason for this is that the ability to interconnect depends on the final bit of the lower
   numbered group being set to a 1.
6. The total number of ways to make equations for a given number is to look at all the possibilities for each group and all of the possible ways to interconnect groups. Here's an in-depth example:

   Starting number = 10101010 (170)
   Group 1 = 10 (6, 2 total ways to shift bits)
   Group 2 = 10 (6, 2 total ways to shift bits)
   Group 3 = 10 (6, 2 total ways to shift bits)
   Group 4 = 10 (6, 2 total ways to shift bits)

   Total ways with no interconnecting = 2 * 2 * 2 * 2 = 16
   Ways when only interconnecting groups 1-2 = (1) * 2 * 2  = 4
   Ways when only interconnecting groups 2-3 = 2 * (1) * 2  = 4
   Ways when only interconnecting groups 3-4 = 2 * 2 * (1)  = 4
   Ways when interconnecting groups 1-2 and 3-4 = (1) * (1) = 1
   Ways when interconnecting groups 1-2-3 = (1 * 1) * 2 = 2
   Ways when interconnecting groups 2-3-4 = 2 * (1 * 1) = 2
   Ways when interconnecting groups 1-2-3-4 = (1 * 1 * 1) = 1

   Grand Total = 34 ways

   A quick double check with my brute force code from the initial impressions section proves that this is true. It's great to have a way to find the number of equations for a given number given
   its binary representation as opposed to needing to build up dynamically. This will allows us to find the answer for really large numbers. The biggest caveat here is that the more binary groups
   we have the more ways there are to choose groups to interconnect. Given a number with N binary groups there will always be 2^(N - 1) ways to choose which groups get interconnected.
   For example, in a number with 10 different binary groups there would be 2^(10 - 1) = 512 distinct ways to choose our interconnect
   groups. Or, as seen above for the case of 170 which has 4 groups there were 2^(4 - 1) = 8 ways to choose groups which can be verified looking at the lines above.
   The more groups that there are the more compicated it will be to calculated all of these interconencts.

So I now have a good way to figure out the total number of equations given any individual number. What's great about this, is that the reverse is true. Given a goal number, we can just construct binary
numbers as desired to reach that goal. Let's look at the given example where we want to find a number n where f(n) = 13 and f(n-1) = 17. Let's first focus on f(n) = 13. We don't know how many binary
groups will ultimate make our answer, so let's just start with 1 group and work our way upwards. For there to only be 1 binary group then the number must be of the form 1110000... (basically a group
of all 1's followed by all 0's). For a single group we have #_equations = 1 + #1's * #0's which means that in order to get 13 equations we need #1s x #0s to equal 13 - 1 = 12. If we look at the factors
of 12 and apply them to our 1's and 0's this should give us the different ways to have f(n) = 13 with only a single group. These ways are:

1,12 = 1000000000000 (4,096) and 1111111111110 (8,190)
2,6 = 11000000 (192) and 11111100 (252)
3,4 = 1110000 (112) and 1111000 (120)

My brute force algorithm confirms that each of these numbers has 13 different equations, however, f(n-1) doesn't equal 17 for any of the given numbers. This means that we need to move up to 2 binary groups.
With 2 binary groups we have that the total number of equations = group1ways * group2ways + interconnect1-2. So the number of ways to get 13 equations like this would be 13 * 1 + 0, 12 * 1 + 1, 11 * 1 + 2
... 1 * 13 + 0. So at 2 groups there's already quite a number of equations that we can work with. It worth noting that the only way you can have 0 possible ways to interconnect it when the higher number
of the 2 groups is composed of all 1's. This is because there's no where for any of these 1's to "shift" thus making an interconnect impossible. So looking at the first equation here, 13 * 1 + 0, with only
1 way to make group 2 and an interconnect value of 0 means that group 2 must be comprised of all 1's. So now our question is how can we make 13 from just group 1. Well, we know the answer to this because we
just looked at all the ways to make 13 equations with a single group, there were a total of 6 ways. We want to find the smallest value of N so it goes to show that we should pick the smallest value from up above
which would be 1111000 (120). The smallest value for group 2 here would just be 1, so tak it onto the end to get 11110001 = (241). Lo and behold, that's the answer, however, we could've added any number of 1's
onto the back and still gotten f(n) values of 13 (111100011, 1111000111 ... 11110001111111111 all have 13 equations).

This has shown us something important, we can dynamically build up our binary numbers one binary group at a time. Let's go back to the above example of 170 (0b10101010) which even at only 4 binary groups was
somewhat of a handful to calculate. Starting with the first binary group of 0b10 we know that there are only 2 possible equations. Well let's just go ahead and stick the next binary equation 0b10 onto the back
of that. We multiply the number of ways by 2 (because that's the ways of creating the new binary group) and then we add the number of ways to interconnect the new group to the back of the existing group (which
in this case is only 1). So we have 2 * 2 + 1 = 5 (which is correct for 0b1010). Let's add group 3 now. We multiply the current number of ways by 2 again, and add 1 to get 5 * 2 + 1 = 11. This isn't correct though
as there should be 13 ways to get 42 (0b101010). A little digging shows that the combined second group (0b1010) actually has TWO ways that it can end in a 1 (and thus interconnect to the new group of 0b10 in the
third iteration). So instead of looking at the number of ways that the most recent group can end in a 1, we need to look at the number of ways that the entire chain can end in a 1. This is a little trickier to
pull off but not too bad. 0b1010 has three different ways to end in a 1, and thus 2 ways in which it doesn't so to get the number of equations for the next number we calculate: (ends in 1 but doesn't connect
x new group ways) + (ends in 1 but DOES connect [only 1 way for new group to be set up and allow earlier group to connect]) + (doesn't end in 1 x new_group ways) = (3 * 2) + (3 * 1) + (2 * 2) = 13. Before going 
onto the next group, we need to figure out how many ways this new combined group ends in a 1.

There's a few ways for the newest group tacked onto the end to end in a 1. First, we can not interconnect with the group before and have the new group to end in 1. In this case there are ways_to_make_current_group
* number_of_1s_in_new_group ways to do this. In our example that makes 5 ways. The second way we can do this is force the interconnect (i.e. first group ends in 1 and second group starts with 1) and then look at all
the ways the second group can also end with a !. I won't go through it here, but there ends up being only a single way to have the second group both start with a 0 and end with a 1, so the number we're looking for here
is really just the total number of ways to have the first group end with a 1. To recap, that means to find the number of ways the current number can end in 1, it's total number of ways for previous group times
the number of ways for new group to end in 1 (regardless of interconnect) + the number of ways for previous group to end in 1. In our example, the group 0b1010 had 3 ways to end in 1 and 2 ways to end in 0. This means
that the group 0b101010 will have 5 * 1 + 3 * 1 = 5 total ways to end in a 1 and 8 ways of ending in a 0. These values should be stored somewhere in our dynamic array.

Now looking at adding our final group of 0b10. We have (ends in 1 but doesn't connect x new group ways) + (ends in 1 but DOES connect [only 1 way for new group to be set up and allow earlier group to connect]) +
(doesn't end in 1 x new_group ways) = (8 * 2) + (8 * 1) + (5 * 2) = 34 ways. Another way to look at the above written equation would be (total_ways_current_group * total_ways_new_group) + (ways_current_group_ends_in 1).
That would just give us 13 * 2 + 8 = 34 which is the same but more compact. Before moving on we need to again figure out the number of these 34 ways in which we can end with a 1. As per the above equation it should be
13 * 1 + 8 = 21. Just to take this one step further, 0b1010101010 should have 34 * 2 + 21 = 89 total equations and 34 * 1 + 21 = 55 ways to end in a 1. My brute force algorithm confirms this. If the final group here had
been 0b11 instead of 0b10 this would keep the total ways at 34 because any group composed of all 1's has no way of interconnecting and also only has 1 way to arrange the bits. The brute force algorithm confirms that 
0b1010101011 (683) only has 34 total equations. Again, it's only possible to have a group of all 1's at the very end of a number which indicates oddness.

I got a little long winded in the above paragraphs so basically to summarize, when tacking a new group onto the back of an existing one there's two important equations we need:
1. Total ways new combo group     = (total_ways_current_group * total_ways_new_group) + ways_current_group_ends_in 1
2. Ways new combo group ends in 1 = (total_ways_current_group * ways_new_group_ends_in_1) + total_ways_current_group_ends_in_1
(the total_ways_current_group variable will need to be saved as a temp value as it's needed for both calcs)

To show how this would work dynamically in the case where we keep adding 0b10 to the back:

Assume that we start with an empty group, i.e. 0b0
{ends_in_1, total_ways} ---> 0b0 would start at {0, 1}

add 0b10 (new overall group is 0b10):
temp current_ways = array[1] = 1;
{0, (current_ways*2) + 0}
{(current_ways*1) + 0, 2}
{1, 2} //This tells us that 0b10 has 2 total ways, 1 of which ends in a 1

add 0b10 (new overall group is 0b1010):
temp current_ways = array[1] = 2;
{1, (current_ways*2) + 1}
{(current_ways*1) + 1, 5}
{3, 5} //This tells us that 0b1010 has 5 total ways, 3 of which end in a 1

add 0b10 (new overall group is 0b101010):
temp current_ways = array[1] = 5;
{3, (current_ways*2) + 3}
{(current_ways*1) + 3, 13}
{8, 13} //This tells us that 0b1010 has 5 total ways, 3 of which end in a 1

These numbers can be seen to coincide with my manual calculations above. With the correct way to calculate the increase to total equations when adding sub-groups to the back of a larger group this problem has gotten
much easier. So what I propose to do is create a vector that contains every possible binary group. There's obviously an infinite amount here, but, we can't have a single group that yields f(n) > 123456789 so this becomes
our limit. The largest possible binary number we'll have to deal with is --> 1 + #1's * #0's = 123456789 --> #1's = 123456788 and #0's = 1. This number is obviously ginormous, but what we can do is come up with a
shorthand version by listing the number of consecutive 1's followed by the consecutive 0's (which I think the final answer needs to be in anyway). [1, 123456788] makes sense to me for representing the number of 0's and
1's in a given number (with the number of 0's listed at index 0 and the number of 1's listed at index 1 for obvious reasons). The question now becomes, how many of these base groups to we put into our array? There's going
to be an insanely large number of possible base groups (even though they're easy to make in theory). There's at least one binary number of length 61,728,394 up to a length of 123,456,788 that will yield an f(n) that's
less than 123456789. The moment we combine one of these really large groups with a smaller group though, f(n) will be greater than or equal to our goal. What I'm thinking of doing is limiting the length of these base
binary numbers to some limit, say 100. I'd be shocked if the final answer had stretches where there were 100 binary digits between the first 1 of the group, the first 0, and then the next 1. It's theoretically not
impossible for this to happen, it would just surprise me from a statistics standpoint. So I'm going to start with a limit of 100, if I can't find an answer this limit can go up to 1,000 and so on and so forth.

Setting this limit of 100 for the longest base binary group means that there's less than 5,000 total base numbers to deal with which should be easy to iterate over as a vector. Just keep combining all possible combinations
until 1 combination's f(n) grows larger than the limit, when this happens it's no longer in the running. I think a good way to approach this method of trying every option would be to do it recursively. On a final note,
the order of our binary groups actually makes a difference. For example 0b11000100 (196) and 0b10011000 (152) feature the same binary groups (0b11000 and 0b100), however, 196 has 25 equations while 152 only has 24.
Because of this the recursive search will start at index 0 for each new level of the function.

On another final note, since f(n) is ~1/9the size of f(n-1) and they're only 1 away from eachother I'm going to assume that n is an odd number and n-1 is an even number. I'm only going to look at groups that don't end
in a 1 and try to make 123,456,789. Once I've done this a 1 bit can be tacked onto the end of the number to make it odd but maintain the same number of equations. To get from n to n-1 at this point we just need to turn
the newly tacked on 1 into a 0 which will always make the total equations go up. If this isn't enough for us to reach 987,654,321 then we can tak on another one until we exceed the denominator value.
*/

//Attempt 2
/*
I felt pretty good about the approach laid out above, however, after coding it up it quickly became apparent that the approach is going to be way too slow. There are just too many combinations of the base binary groups
to realistically iterate over. Capping the base group length at 100 binary digits gives us ~5000 different building blocks so as soon as we get up to 2 combinations there are 25,000,000 and 3 combinations shoots up to
125,000,000,000 combinations. Most likely the answer will be a composed of many different sub groups combined so there's just no way to check them all these way unfortunately. Even if we were to cap the length of the binary
groups to 10 digits (which I don't think will be nearly long enough), by the time we combine 5 different groups there's already ~312,000,000 different combinations. To be honest I'm kind of surprised it didn't jump out
at me sooner just how quickly the total number of combinations would shoot up. I'm not ready to abandon the concept of combining these binary groups just yet though. Knowing that there are these groups of binary numbers
that can be connected and will allow us to find the number of equations for any number we want should be instrumental in solving this problem.

Somewhere up above I showed that it was possible to construct numbers with a set f(n) value by using a single binary group. Well these should be true for f(n) that's made out of any number of binary groups, not just a 
single group. We can take this a step further and try and impose limitations onto what n can be by using the fact that we also need f(n-1) to equal a set number. Let's look at the given example again. We're trying to find
a value of N where f(n) = 13 and f(n-1) = 17. A little studying of the amount of equations for each number shows us that any even number will ALWAYS have more equations than either of the odd numbers aroundd it. It doesn't
take too much thought to see why this is the case, but here's a small sample that drives this home:

79(01001111): 3
80(01010000): 14
81(01010001): 11
82(01010010): 19
83(01010011): 8
84(01010100): 21
85(01010101): 13
86(01010110): 18
87(01010111): 5
88(01011000): 17
89(01011001): 12
90(01011010): 19

Looking at the above you can see that 80 has more equations than 79 and 81, 82 has more equations than 81 or 83, and so on. Actually, now that I look at this snippet something just jumped out at me which hadn't before.
The total number of equations for an even number appears to be the sum of the number of equations for the odd numbers on either side of it. This holds true for each of the numbers written above. If this holds true for
every single number then we should be able to come up with a recurrence relation between all of these numbers. The question right now though is, is there an easy way to tell the number of equations for a given odd number
based on the numbers around it?

A little more investigation into patterns of the numbers has shown me another interesting thing. When looking at the number of equations for a group of numbers that stretches from 2^(n)-1 through 2^(n+1)-1, the amount of
equations reverses itself. To show case this, here's the equations for 2^3 - 1 up to 2^4 - 1:

7(00000111): 1
8(00001000): 4
9(00001001): 3
10(00001010): 5
11(00001011): 2
12(00001100): 5
13(00001101): 3
14(00001110): 4
15(00001111): 1

So we get 1, 4, 3, 5, 2, 5, 3, 4, 1. If we look at the numbers on either end of the chain we have 1, which makes sense as these numbers are composed
of only 1 bits. If we look at the number right in the middle of them it has 2 equations, which is 1 + 1. If we now look at the number directly in the middle of the 1 and the 2 on both sides we get a 3 (1 + 2 = 3). There's
definitely a pattern emergin here. My guess is that if we look at the group of numbers from 2^4 - 1 through 2^5 - 1 it will look like this:

indices: [15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
add odd #'s: [ 1,  0,  4,  0,  3,  0,  5,  0,  2,  0,  5,  0,  3,  0,  4,  0,  1]
add evens:   [ 1,  5,  4,  7,  3,  7,  4,  6,  2,  6,  4,  7,  3,  7,  4,  5,  1]

Running my brute force algorithm shows that my hunch was spot on
actual: [ 1,  5,  4,  7,  3,  8,  5,  7,  2,  7,  5,  8,  3,  7,  4,  5,  1]

To get entire groups all we need to do is just keep adding adjacent numbers and expanding our array:

2^0 - 1 [1, 1] 2^1 - 1
2^1 - 1 [1, 2, 1] 2^2 - 1
2^2 - 1 [1, 3, 2, 3, 1] 2^3 - 1
2^3 - 1 [1, 4, 3, 5, 2, 5, 3, 4, 1] 2^4 - 1
2^4 - 1 [1, 5, 4, 7, 3, 8, 5, 7, 2, 7, 5, 8, 3, 7, 4, 5, 1] 2^5 - 1
2^5 - 1 [1, 6, 5, 9, 4, 11, 7, 10, 3, 11, 8, 13, 5, 12, 7, 9, 2, .....] 2^6 - 1
2^6 - 1 [1, 7, 6, 11, 5, 14, 9, 13, 4, 15, 11, 18, 7, 17, 10, 13, 3, 14, 11, 19, 8, 21, 13, 18, 5, 17, 12, 19, 7, 16, 9, 11, 2 ....] 2^7 - 1

This is just the Faray sequence but were only looking at the denominators, very very interesting. If we were to keep expanding this array, eventually we'd start to get elements that are equal to 123456789 which is our goal.
If we go even further then we'll also eventually get an element with a value of 123456789 where the element directly before it is 987654321. For that to happen, since we know that the even numbers are just the sum of the
odd numbers around it, n-2 will need to have a value of 987,654,321 - 123,456,789 = 864,197,532. Out of curiosity I'm going to code up a brute force implementation that will keep expanding this array of numbers until
we get our first value that's equal to at least 123,456,789. To do this I'm just going to use a singly linked list as it won't be required to resize it over and over again. Also, since the array is mirrored after the middle
element then there's no need to look beyond that element, we can just initialize our array as [1, 2] and it will yield only the first half. My gut tells me it'll take a pretty long time until we get to a number that high,
considering after 5 iterations our largest number will only be 13.

After creating the Singly Linked List class it looks like it will take about 45 iterations to first get the value of 987,654,321 in our list. At this stage the list will have a size of roughly 10 trillion nodes, so, it
isn't really going to be effective to traverse through this list. It'd be one thing if we were guaranteed that our hit happened after only 45 iterations, but my gut tells me that it will take more iterations before
we manage to get 987,654,321 to appear directly in front of 123,456,789. So we're going to need to do something a little more slick than just keep adding numbers together.

If we look at the columns formed when these patterns are placed on top of eachother we can notice... more patterns!

1 [1, 2]
2 [1, 3, 2]
3 [1, 4, 3, 5, 2]
4 [1, 5, 4, 7, 3, 8, 5, 7, 2]
5 [1, 6, 5, 9, 4, 11, 7, 10, 3, 11, 8, 13, 5, 12, 7, 9, 2]
6 [1, 7, 6, 11, 5, 14, 9, 13, 4, 15, 11, 18, 7, 17, 10, 13, 3, 14, 11, 19, 8, 21, 13, 18, 5, 17, 12, 19, 7, 16, 9, 11, 2]

The first column will always have a value of 1. Every column that starts with a 2 (aka the last column in any individual row) will see the number directly beneath it increase by 1. This appears to continue on into infinity.
No this is where things get interesting. Looking at every column you can see that successive rows add the same number, that is to say each column can be represented by a linear equation. Here are the column adders
from what I've seen so far:

[0, 1, 1, 2, 1, 3, 2, 3, 1, 4, 3, 5, 2, 5, 3, 4, 1 ....] very interesting. This is exactly the pattern for the number of equations that can be used to make N (indexed properly if we remove the lead 0). So essentially, we
can use the pattern itself to propogate the pattern?? I think there's something deeper happening here that I'm missing. What's interesting about this though is that if we can figure out the first number tha appears on the
top of each column, then we can figure out if (and when) each column will end up having a value of 123,456,789 or 987,654,321. For example if we look at the first non-zero column where the numbers increase by 1 each time
it can be seen that we'll hit 123,456,789 equations when n = 2^123,456,788. Well that sounds very familiar, I figure out that this number would yield the right number of equations back up in the section for attempt 1. The 
second non-zero column will have 123,456,789 equations when n = 2^123,456,789 + 1. This was also figured out in the section above, by left shifting a number and setting the first bit to 1, we won't change the number of equations
at all. The third column starts at 5 and increases by 2 every time which means that we'll reach 123,456,789 equations when N = 2^61,728,395 + 3. You should be able to tell pretty easily if a column will actually hit
123,456,789 or not using a little modulus division.

We can actually get very very close to the correct answer if we start in the row where the first number is 2^9 - 1. The very last number of this row will be a 2, as it is with every row, so to get to a value of 123,456,789
from here we need to stay in the same column and move downwards by 123,456,787 rows. If we look at the number right before this, we will see that it has a value of 17 and increase in value by 8 every time we go down a row.
This means that after going down 123,456,787 rows the value in the this column will be 987,654,313 which is only 8 off of our answer!! If we go down one more row then the value will be 987,654,321 which is exactly what we
need. Unfortunately if we take that to be our f(n-1) value then f(n) will have a value of 123,456,790 wich is 1 too much, are you kidding me?? N in this case is 2^123,456,796 + 2^8 - 1. In terms of the how we're 
supposed to phrase the final answer this number would be represented by (1, 123456788, 8). So when our binary represntation of N is (1, 123456788, 8) then the fraction we get for f(n)/f(n-1) is 123456790/987654321 dammm so
close.

It seems that one way we could solve this problem is to compare the "growth" of successive columns. Comparing what the growth is (the growth being the difference in numbers of the same column) for the two columns and knowing
the start values will allow us to quickly check with an equation whether or not we'll have a match. If we call our first number X0 and our second number Y0 (these numbers will be right next to eachother in the same
row) and the difference in growth between the 2 is G (growth can be seen by comparing X1/X0, X2/X1, etc.) then we can calculate which row R of our table will need to be at for the value of X to be 987654321 and for Y to be
123456789. Ok I'm confusing myself just be reading this past paragraph, here's what the equation actually looks like. 

Looking back to the example I did two paragraphs ago where the difference in growth of the two columns was 8 (I chose this as a growth number because 987654321 / 123456789 ~= 8) we can see that the row n where we have the 
desired values will be dictated by the two equations:

8*n + X = 987,654,321
  n + Y = 123,456,789

this reduces down to:
X = 9 + 8*Y

So if we're going to hit the goal numbers the difference between X and Y must be as dictated by that final equation. This means that the following values would work for [X, Y] : [17, 1], [2, 25], [3, 33] ...
When I did the above example the starting value for X was 17 but the starting value for Y was 2 instead of 1, this is why our fraction was off by 1. Let's look at another example just picking random columns, 11 and 12 sound
good to me. Column 11 has a growth of 5 (as seen by going from 13 to 18) whereas column 12 has a growth of 2 (as seen by going from 5 to 7). The growth ratio G is then 5/2 and our governing equation are:

5/2*n + X = 987,654,321
    n + Y = 123,456,789

2X = 1,358,024,697 + 5Y

Not nearly as clean as the other set of equations I found, but it holds. This means that if our starting value of Y is 5 (as is the case for column 12 in row 5) then for this to work the value directly before the 5
would need to be muuuuuch much higher, a value of 679,012,361. This means that there won't be any row in the existence of these two columns where the value in 
*/