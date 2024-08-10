#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <Header_Files/print.h>

#include <algorithm>

//Divisible Ranges
std::vector<std::vector<long long>> createBinaryCycles(long long n)
{
	std::vector<std::vector<long long> > cycles = { {}, {} };

	for (long long i = 2; i <= n; i++)
	{
		std::vector<long long> cycle;
		long long start = 1LL << (i - 1LL);
		for (long long j = 2 * i; j <= n; j += i)
		{
			start <<= i;
			start |= (1LL << (i - 1LL));
		}

		start &= ~(-1LL << (n - 1LL)); //remove leading bit of n-bit number
		start <<= 1LL;
		start |= 1LL;

		for (int j = 0; j < i; j++)
		{
			cycle.insert(cycle.begin(), start);
			start &= ~(-1LL << (n - 1LL)); //remove leading bit of n-bit number
			start <<= 1LL;
		}

		//For all odd rows, remove any bits that would fall into an even column.
		//This effectively doubles the cycle for all odd rows so copy all existing
		//layouts first.
		if (i % 2 == 1)
		{
			for (int j = 0; j < 2 * i; j++)
			{
				if (j < i) cycle.push_back(cycle[j]);
				cycle[j] &= (cycle[j] ^ cycles[2][j % 2]);
			}
		}
		
		cycles.push_back(cycle);
	}

	return cycles;
}

bool recursivePathFinderBinary(long long usedDigits, int lastUsedDigit, long long firstNumberOfCycle, std::vector<std::vector<long long> >& cycles,
	int maxDigits, int currentLevel, int levelSkip = 0, int stopDigit = 1)
{
	if (lastUsedDigit < stopDigit) return true;

	//Odd cycles are longer than even cycles, so use the length of the given array when using modular division
	long long currentCycle = cycles[lastUsedDigit][(firstNumberOfCycle - 1) % cycles[lastUsedDigit].size()];

	long long digit = 1LL;
	while (digit < (1LL << maxDigits))
	{
		if (((currentCycle & digit) == digit) && ((usedDigits & digit) == 0))
		{
			//We only care that a solution exists, not how many, so as soon as a working solution
			//is found we return.
			if (recursivePathFinderBinary(usedDigits | digit, lastUsedDigit - 1 - levelSkip, firstNumberOfCycle, cycles, maxDigits, currentLevel + 1 + levelSkip, levelSkip, stopDigit))
			{
				return true;
			}
		}
		digit <<= 1;
	}

	return false;
}

std::vector<long long> workingStartNumbersFromEvenRows(std::vector<std::vector<long long> >& allCycles)
{
	int n = allCycles.size() - 1;
	long long startNumber = 2;

	std::vector<int> usedNumbers;
	for (int i = 2; i <= n; i += 2) usedNumbers.push_back(i);

	if (usedNumbers.size() == 1) return { false, true };

	std::vector<long long> workingStartNumbers = { 1 };

	while (true)
	{
		unsigned long long ore = 0;
		bool stop = true;
		//TODO: This can be cleaned up
		for (int i = 4; i <= n; i += 2)
		{
			int cycleIndex = (startNumber - 1) % i;
			ore |= allCycles[i][cycleIndex];
			stop = (stop && (cycleIndex == 0));
		}

		if (stop)
		{
			break;
		}

		//We only care about the number of 0-bits in the even numbered columns.
		//If the furthest right column represents an odd number do a right shift 
		//before starting to count the 0-bits.
		int shifts = 0;
		if ((startNumber % 2) == 1)
		{
			ore >>= 1;
			shifts++;
		}

		int zerosFound = 0, firstZero = 0;
		bool works = true;

		while (shifts < n)
		{
			if (!(ore & 1LL))
			{
				zerosFound++;
				if (zerosFound == 1) firstZero = shifts;
				else
				{
					works = false;
					break;
				}
			}
			ore >>= 2;
			shifts += 2;
		}

		if (!works)
		{
			startNumber += (firstZero + 1); //skip ahead until the first 0 bit leaves the grid
			continue;
		}

		//If the binary OR test passes, perform a brute force search to see if this is a viable
		//starting number
		if (recursivePathFinderBinary(0, n - (n % 2), startNumber, allCycles, n, 1, 1, 2))
		{
			workingStartNumbers.push_back(startNumber);
		}

		startNumber++;
	}

	return workingStartNumbers;
}

void workingOddNumberFinder(std::vector<std::vector<long long> >& allCycles, std::vector<long long>& workingEvenNums, long long adder,
	std::vector<long long>& totalWorkingStartNumbers)
{
	int n = allCycles.size() - 1;
	int halfN = n / 2;
	bool evenN = (n % 2 == 0);

	for (int index = 0; index < workingEvenNums.size(); index++)
	{
		long long i = workingEvenNums[index] + adder;
		long long ore = 0;
		for (int j = 3; j <= n; j += 2) ore |= allCycles[j][(i - 1) % allCycles[j].size()];

		//There need to be at least n/2 bits in the binary form of ore,
		//if the aren't then this starting number will result in failure and
		//shouldn't be added to the final list.
		int onesFound = 0;
		while (ore > 0)
		{
			if (ore & 1) onesFound++;
			ore >>= 1;
		}

		int goal = (halfN - 1);
		if (n % 2 != 0) goal = (halfN + i % 2 - 1);
		
		if (onesFound >= goal)
		{
			if (recursivePathFinderBinary(0, n, i, allCycles, n, 1, 0, 3))
			{
				totalWorkingStartNumbers.push_back(i);
			}
		}
	}
}

std::pair<std::string, long double> q896 ()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	
	int n = 36;

	//Memoize all possible binary representations of each grid row
	std::vector<std::vector<long long> > cycles = createBinaryCycles(n);

	long long evenRowPeriod = lcmOverRange(2, n, 2);
	long long totalPeriod = lcmOverRange(2, n, 1);
	long long adder = 0;

	std::vector<long long> workingEvenNums = workingStartNumbersFromEvenRows(cycles), totalWorkingStartNumbers;
	
	while (totalWorkingStartNumbers.size() < n)
	{
		workingOddNumberFinder(cycles, workingEvenNums, adder, totalWorkingStartNumbers);
		adder += evenRowPeriod;
	}

	answer = totalWorkingStartNumbers[n - 1];

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 274229635640
	//ran in 5.86784 seconds
}

//NOTES
/*
It took me quite awhile to understand what the question even wanted from us. The way it's written makes it sound like, as long as the n-th term in the
range is divisible by n then we're good. This would of course make the problem trivial as any range of length n would work. I had to look at a separate 
forum to realize what the question wants is to rearrange the range so that every number in the range is divisible by its index in the range. For example,
the sequnce 5, 2, 3, 4 works because 5 % 1 = 0, 2 % 2 = 0, 3 % 3 = 0 and 4 % 4 = 0. This makes more sense, and understandably makes the question much
harder.

My first observation here is that prime numbers larger than N will only ever be able to go in the very first slot of the sequence. This might not end up
being a big deal, because primes get more and more spread out as the number get larger. My next observation is that whatever number is divisible by N will
always need to go at the end of the sequence. This one is also kind of a no brainer, since only 1 in every N numbers will actually be divisible by N. With this
in mind, each number that's divisible be N will have exactly N chances to be a part of a working sequence. For example, when N is 4, there are only 4 sequences
that can be made which include 4: [1,2,3,4], [2,3,4,5], [3,4,5,6], [4,5,6,7]. As can be seen from the problem prompt, some of these ranges work but not
all of them.

I'm having and interesting idea on how to solve this problem. Create N binary numbers of length N. The first number will represent each number in the sequence
that's divisible by 1, the second will represent all numbers divisible by 2, etc. So pretending that our sequence is [1, 2, 3, 4] the 4 binary numbers
would be:

1111 (all numbers divisible by 1)
0101 (2 and 4 divisible by 2)
0010 (only 3 divisible by 3)
0001 (only 4 divisible by 4)

The goal is to pick only a single digit from each column and have the final binary number be 1111. In this case the answer is obvious. Looking at the first
range that has no answer though ([4,5,6,7]) here's what the binary looks like:

4567
-----
1111
1010
0010
1000

In the 2nd and 3rd columns we can see that 5 and 7 only fulfill the divisible by 1 requirement, so the combination isn't possible. Since the row which represents
divisibility by 1 is all 1's, it might make sense to omit it. Doing that and we see:

4567
----
1010
0010
1000

If we were to do a binary AND on these three numbers then we'd get 1010. Since only a single row was omitted and there's two gaps to fill here, it goes
to follow that there's no solution. If we keep sliding our range over by 1 and doing a binary AND on the three rows we get the following:

5678   6789   78910
----   ----   ----
0101   1010   0101
0100   1001   0010   
0001   0010   0100
....   ....   ....
0101   1011   0111

So the range of [5..8] has no solution because it has two zeros, but the ranges of [6..9] and [7..10] each only have a single 0 and therefore should both
work. Working with the binary a bit we can see that the two solutions are [7,6,9,8] and [7,10,9,8]. These seems like an easy enough path to a solution, we
simply keep track of these 3 binary numbers, left shift them by 1, and AND them all together in each iteration. As long as the solution of this AND only contains
a single (or no) 0's the solution will work. It's easy to see too, that with this limited number of binary numbers being left shifted that the pattern will
eventually reset itself and we'll have a closed solution for which patterns of length 4 will work. A little modular math tells us that the pattern should reset
in 12 left-shift cycles. So if the pattern starts at [1,2,3,4] it will reset at [13,14,15,16], sure enough, both of these patterns work without the need to
shuffle any of the digits around.

So this is all and good when there's only 4 digits, but will it hold up with more? My gut tells me no. As nice as it is to simply binary AND all these numbers
together, the one glaring omission here is that there's no way to prevent multiple digits from the same number filling into our answer. 

To find out let's take a moderate loop up and try with 6 digits. Here's
the first few iterations of the left-shift sequence:

123456   234567   345678   456789
------                     ------
010101                     101010
001001                     001001
000100                     100010
000010                     010000
000001                     001000
                           ......
						   111011
                          [7,4,9,8,5,6]
						  [7,8,9,4,5,6]

-----
Changing my train of thought a bit here as I'm not sure there's a good way to do this purely with binary numbers like I was hoping.
So I showed above that when N is 4, there is a pattern of acceptable answers that will repeat every 12 shifts of the window. Going back
to the problem, when N is 36 how many shifts are there before the pattern resets? Well for all the binary numbers to perfectly line up again
we need to shift over by a number that each number from 1-36 evenly divides. Multiplying the maximum power each prime below 36 can be raised to
without exceeding 36 we see:

2^5 * 3^3 * 5^2 * 7 * 11 * 13 * 17 * 19 * 23 * 27 * 29 * 31 = 3,898,895,928,127,200

Ok that's much bigger than I was hoping for. I was hoping I could simply brute force each series in a complete cycle and then just extrapolate
from there. This approach would work on smaller numbers, but 36 is gonna be too much. That's ok though, I may just be better of focusing on
my original approach
-----

Here's an outline for an algorithm that could potentially work.
For each iteration:
1. Check that there aren't two prime numbers (larger than N)
    a. If there are, shift forwards until the smaller prime is gone then check again.
2. Put any numbers into slots which MUST be used. This would be any of the binary numbers with only a single digit
   or any column in the binary numbers with only a single digit.
   a. Looking at the example for 456789 above, the bottom two rows and the right most column would qualify.
3. When a digit gets selected in a row, remove it as a digit from all other rows, then recurse downwards.
   When coming back from recursion, add that digit back wherever necessary.

----
I wrote a pseudo brute-force algorithm for creating the binary numbers, cycling through them, and finding paths that work. For smaller
numbers this works very quickly, however, for larger numbers (like 36) I basically can't get through a single recursion when there isn't a
solution. So there's two things I need to do here, improve the recursive algorithm, and skip over ranges that I know have no hope of working.
In terms of the later, I know that a range won't work if it contains two prime numbers that are larger than N. Sure enough, the first recursion 
I get stuck on for n=36 is when both 37 and 41 appear in the range. Doing a little more digging into the spacing of prime numbers, it's actually
somewhat rare to find primes with large gaps between them. The first two primes that have a gap of at least 36 between them is 9551 and 9587.
A little more thinking shows that I don't actually need consecutive primes with a gap of 36, and group of three primes where the gap between 
prime 1 and prime 3 is 36 will work out fine. As long as this gap exists, then we can set the first number of our range to be directly after
the first prime and roll the window forward until just before the third prime. If we do this, then the only prime within the current range will
be prime number 2. I still don't think this will be enough to solve the problem, but let's see what happens if instead of iteration the window up 1
number at a time, I jump to these windows that will only have a single prime number.

After coding this up I can say it definitely helps, however, not nearly enough on its own. For instance, it takes 14.5 seconds to solve when 
n = 15 without the prime jumping, and 5.8 seconds with the prime jumping. So definitely faster, but not the orders of magnitude that I need. This 
isn't the end of the world though. Currently the algorithm I have for adding digits is very sloppy (iterating over each binary number and checking
whether or not there's a 1-bit or 0-bit at each location before trying to add). I should be able to come up with something more elegant here. I
intitially decided to used binary numbers because I figured it would make it easy to add digits with bitwise operations, however, I'm not really
going that route anymore. Instead, I could just create a vector for each binary number containing the indexes where the 1's are located. For
example the number 0b00101101 would get the vector [0, 2, 3, 5] since there are 1's at 2^0, 2^2, 2^3 and 2^5 respectively. I'll still need to use a 
loop, however, this will get the iterations down from a minimum of n. For reference, all numbers greater than n / 2 can have a maximum of two 1-bits
so this will definitely cut down on the number of iterations. The maximum iterations will occur when looking at numbers divisible by 2 which will 
have exactly floor(n, 2) iterations. By my estimate, the total number of loop iterations should drop from ~n^2 to ~n which is great, however, it's
important to point out that on the vast majority of these iterations no logic is actually happening. The iterations where nothing happens are over
quickly obviously, it's just that there are a lot. So this approach is simply cleaning things up, which I still don't think will be remotely enough
to solve the problem.

I was afraid this might happen, after taking the time to code up the same solution using vectors instead of binary numbers the run time went from
5.8 seconds to 10.2 seconds. So that was a waste of time. After all this work it seems like I may just need to change up my approach entirely.
----

The only place I can really think to start is by trying to analyze whether or not a range will work by looking at the makeup of prime factors
for all numbers in the range. When I was looking at the case of n=4, by looking at how the binary numbers representing divisibility shifted as 
the range increased, it became clear that the pattern for working ranges would repeat whenever the starting number of the range increased by 
12. This is because the smallest number divisible by 1, 2, 3 and 4 is 12. I also so that the value of this number when n is 36 is much larger,
3,898,895,928,127,200 to be exact. This number is larger enough though, that I would be shocked if we didn't find 36 working ranges before
hitting it. It's easy enough to see when all of the binary numbers will line up back to their original starting positions, however, I wonder if
there's also a quick way to see the next time all the binary numbers will line up so that there are no gaps so to speak.

Thinking more about primes, and in particular, coprime numbers, it can be seen that eventually, even if there are two numbers which are coprime
then the binary bits in their respective rows will eventually line up. Take the numbers 5 and 7, which both evenly divide the number 35. If we set
n to 7 and move our range to start at 35, here are the first few sequences of binary numbers for 4,5,6 and 7 

  35 36 37 38 ...  34 35 36 37 
--------------     -----------
4  0  1  0  0       0  0  1  0
5  1  0  0  0       0  1  0  0
6  0  1  0  0       0  0  1  0
7  1  0  0  0       0  1  0  0

You can see that the 1 bit for row 5 and 7 line up with each other as long 35 remains in the window. Since 7 is the largest number and will only ever 
have a single bit present that means it NEEDS the 35, so the 5 would need to go on either 30, or 40. From this we can conclude that starting the 
range on the numbers 31, 32 or 33 will make it impossible to find a winning combo as 35 will be the only number useable by 7 and 5 and they can't both
use it. It may be possible to further weedle down numbers we can't start with by looking at all combinations of number greater than n/2. Since it isn't
possible for any of these numbers to have more than 2 active bits at a time, then there's higher chances to cancel out larger groups of numbers where we
can start our range.

Focusing on coprime numbers again, numbers that are coprime with each other are always going to overlap (i.e. if a number is divisible by 8, it will also always be divisible by 4 and 2).
Let's look at the highly divisible number of 30, it's divisible by 1, 2, 3, 5, 6, 10, 15 and 30. Ignoring the one, this means any time there's an active
bit in row 30 of the above tables, bits in the corresponding column will also be active for rows 2, 3, 5, 6, 10 and 15. If n is 30, then at most there will 
only be 2 active bits for 15, so as soon as we select the 30 bit, it will always leave us with a single bit to be used for 15. Now looking at a number which
evenly divides both of these, like 5, this means of the 30/5 = 6 possible flags, only 4 should ever be usable. Now add in the fact that larger numbers which
are also divisible by 5 and are only likely to have a single flag active (20 and 25), this cuts the total bits that we can every select for 5 down to 2. What
I'm trying to get at here, is it may be possible to eliminate many numbers as starting points at a time by knowing that at any given point, almost all flags
for the lower numbers will be taken up. What we can do from here is look at these lower prime numbers and quickly find instances where the few free binary bits
they can actually use overlap with bits from rows that they aren't coprime with. For example, 5 and 7 are coprime and typically their bits won't overlap, but as
was seen in the above paragraph, once the number 35 comes into play then one of the 2 free bits for the 5 can potentially be stolen by the 7./

----
My mind has kind of been going all over the place with this problem, but I think it's going to be more about eliminating numbers that don't work as opposed
to finding the ones that do (as I've seen from doing smaller values of n, there are a lot more instances that don't work as opposed to those that do). I've
already found that a lot of options can be eliminated by taking into account ranges that contain two prime numbers. Next, I'm going to focus on numbers that
usually only have 1 active bit. In the case of n=36, 36 will always have a single active bit, 35 will have 1 active bit when the starting number != 0 mod 35,
etc. So if the starting number of the range is 35 * 36 + 1 = 1261, then both rows 35 and 36 will only have a single bit that overlaps for the next 34 numbers
(a starting number of 1295). The only way we could pick a working combination in this range is if there's no prime number in the current range.

Now look at the first instance where 34 and 36 overlap and 34 only has a single bit (34 * 36 + 2 = 1226). So likewise, any range starting between the numbers
1226 and 1259 are overwhelmingly likely to not work. If you do this exercise all the way down to 18 (36 / 2) then you'll see that a majority of the numbers
between 648 and 1295 can be eliminated. Thinking a little further, won't this scenario just repeat once you get to a range starting with 36 * 18 * 2 = 1296,
aka the very next number? I'm now starting to think that a majority of options can be elimiated simply by focusing on numbers divisible by n/2 through n. If you
were to make a list of all numbers that are factors of the numbers n/2 through n, and now that ranges can only have these numbers in very specific locations,
combined with the fact that none of these ranges work if there's more than 1 prime in them, and we might be able to take out large swaths of numbers at a time 
here.

Looking at a smaller example, n = 6, the numbers we want to focus on are 4, 5 and 6. Numbers that feature 2 or more of these numbers as factors are
[12, 15, 18, 20, 24, 30, 36, 45, ...]. Running my brute force algorithm for n = 6 shows that ranges starting with: 1, 2, 3, 4, 5, 20, 23, 24, 25, 30, 31 and 32
all work. So 12, 15 and 18 all got skipped over from the above array, however, 20, 24, 30 and 36 all seem to be ok. Hmmm, not quite what I was looking for. 
What's going on for the case when we start at 20?

   20 21 22 23 24 25
   -----------------
2   1  0  1  0  1  0
3   0  1  0  0  1  0
4   1  0  0  0  1  0
5   1  0  0  0  0  1
6   0  0  0  0  1  0

So the bit in the 6th row matches up with one in the 4th row, however, there's another bit in the 4th row which can be used, ok so that makes sense. When verifying
whether a number can work in a certain slot these secondary bits need to be taken into account. In the case of 24, since 4 and 6 are two number away from each other
it means that 24 would in theory be safe in slot 1, 2, 5, or 6 but not in slots 3 or 4, meaning that starting numbers of 21 and 22 wouldn't work. In other words,
we want to eliminate numbers fitting the equation k(4 * 6) - 3 and k(4 * 6) - 2 as viable starting options (unless there were no prime numbers within a certain
distance). Let's just double check that this works on the next 2 instances of the equation, i.e. 45 and 46

   45 46 47 48 49 50   46 47 48 49 50 51
   -----------------   -----------------
4   0  0  0  1  0  0    0  0  1  0  0  0
6   0  0  0  1  0  0    0  0  1  0  0  0

Everything seems to checkout here, there's only a single instance of the bit in row 4 and it lines up with the bit in row 6. Since the number 47 appears
(which is prime and needs to be selected for row 1), I can tell without looking at the rest of the rows that these ranges can't possibly work. The equations
for pure overlap between 5 and 6 would be k(5 * 6) - [1 through 4]. Testing this out, the first instances would be 26, 27, 28 and 29

   26 27 28 29 30 31 . 29 30 31 32 33 34
   -----------------   -----------------
5   0  0  0  0  1  0    0  1  0  0  0  0
6   0  0  0  0  1  0    0  1  0  0  0  0

This also looks correct. I think in general for a given number a, which is greater than floor(n / 2), you can eliminate from the list of valid starting numbers
for ranges kna - [(n - a) through (a - 1)]. Kind of a strange formula, but it should eliminate a lot of possibilities for us. We could probably take this a step
further if we wanted. For example, we know that when the starting number is of the form 24k - [0, 1, 4 or 5] that the bits from row 4 and 6 will overlap, but
there will be an extra bit open from the 4. What if we then decided to lock this bit down with a bit from row 5? Things are getting a touch complicated now, but
there should be 4 equations we could use for this purpose depending on if we choose to subtract 0, 1, 4 or 5 in the above equation. Choosing the -5 for now, this
means the two active four bits will be at starting number + 1 and starting number + 5 (4 away from each other) and the active 6 bit will be 5 from the start (because
we chose the -5 option). This means that the bit in row 5 must be 1 away from the starting number. So if our starting number is of the form 24k - 5, then the 5 bit 
will be found with the equation 24k (mod 5) = 4. The first time we actually see this happen is when k = 1, meaning our starting number is 19. Here's what the 
table looks like for rows 4, 5 and 6 when starting at 19 and 20.

   19 20 21 22 23 24  20 21 22 23 24 25
   -----------------  -----------------
4   0  1  0  0  0  1   1  0  0  0  1  0
5   0  1  0  0  0  0   1  0  0  0  0  1
6   0  0  0  0  0  1   0  0  0  0  1  0

So when starting at 19, we can see that it's impossible to choose a bit in row 4. When starting at 20 though, an extra bit on row 5 comes into play making it so we
can still choose the first bit in row 4. So if we want to cancel out an extra 4 bit with a 5 bit there are only two actual equations that will do this instead of
4 (this make sense actually, since the difference between 4 and 5 is one and the 4 bit will only be in slots 1, 2, 5 or 6, it means we can only have a single 5 bit
if it's in slots 2 or 5). 

Putting some of these last thoughts together, if we want to perfectly cancel out bits in the 4th row with bits in the 6th row then we use the equations 24k - 2 and 24k - 3.
If we want to have 1 bit from the 6th row cancel out 1 bit from the 4th row, and then all bits from row 5 perfectly cancel the remaining bit in row 4 we'd need the equations
24k (where 6k % 5 = 4) and 24k - 5 (where 6k % 5 = 1). Phew, that was a workout and this is only scratching the surface a little bit. What if I wanted to have 5 and 6 match,
and cancel the extra 5 bit with a single 4 bit. Or what if the 6 doesn't overlap with the 4 or 5, but they each only have a single bit and overlap with each other? There's 
clearly a lot of scenarios here and this is only when n = 6, trying to imagine the case when n = 36 is already hurting my brain a little bit. Well, I've already seen that
a lot of starting positions can be canceled out even when only focusing on two numbers and coming up with the equations for two numbers isn't bad at all. It seems that fewer
numbers will be eliminated when focusing on 3 numbers, but it should still be a decent amount and the equations shouldn't be that hard to come up with. Once you start focusing
on more than 3 numbers, the possibilities getting cancelled out shouldn't be that many, and the equations are going to get even more complicated (I'm talking about layered 
modular arithmetic here). I think the best bang for my buck will be sticking to combos of 2 numbers (with perfect cancellation) or 3 numbers (where 1 number has 2 bits that get
cancelled and the other 2 numbers only have 1). If come up with an algorithm that can easily eliminate starting numbers for these scenarios, combined with all the numbers
that get eliminated because there's more than 1 prime in the range, I think will hopefully reduce the search space enough to get an answer quickly.

------

So, coming up with general equations for the above paragraph. Well, I already figured out equations for the scenario of two numbers perfectly cancelling a single digit when
one of the digits is n. I think I can extrapolate this to any number between n and floor(n/2 + 1) though. The farthest any bits can be before shifting will add another one
is entirely dependent on the smaller number and n, so the general equation should just be.

kab - [(b - 1) through (n - b)] where a <= n, b < a and k > 0.

Just to test this out, instead of cancelling 4 and 6 like I did up above let's pretend n is 11. I'm going to cancel 7 and 9 so the equation set would be 63k - [6 through 4].
Setting k to 1 and I'd expect to get perfect cancellation in rows 6 and 9 when the starting number is 57, 58 and 59

   57 58 59 60 61 62 63 64 65 66 67  ... 59 60 61 62 63 64 65 66 67 68 69
   --------------------------------      --------------------------------
7   0  0  0  0  0  0  1  0  0  0  0       0  0  0  0  1  0  0  0  0  0  0
9   0  0  0  0  0  0  1  0  0  0  0       0  0  0  0  1  0  0  0  0  0  0

This looks correct, ok that wasn't too bad. The next part will be a little trickier though since it depends on three numbers. First, we need to decide which number is going to
have two bits that need cancelling, call it c. Of the other two numbers, the larger will be a and the smaller will be b. The first order of business is to do the opposite of the
general equation, that is, calculate values where a only has 1 bit and c is guranteed to have 2. It may not be immediately obvious, but in order for this to work then c needs to 
be smaller than both a and b. Just looking at two letters, a and c, for c to have two bits the product kac needs to be outside the 'safe zone' of row c while kac needs to be inside
the 'safe zone' of row a. The larger a number is, the larger it's safe zone is. So if c is larger than a, it will have a larger safe zone, making it impossible for kac to fall outside
of c's zone will simultaneously being in a's zone. The same logic then extends to kbc, meaning that c MUST be less than a and b. Anyway, with that said here are the equations:

kac > 

---

Changing train of thought a little bit again, after spending a lot of time above on how to best cancel out numbers that are greater than n/2, it didn't really give the speedup I was
hoping for. The amount of possibilities that are cancelled out by looking at the gap of successive primes far outweights the numbers cancelled out when cancelling out numbers greater
than n/2. My next idea has to do with looking at the even numbers. If we only look at even numbers, the cycle for when they all reset should be lower than when every single number resets:

2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36  --> 2^5 * 3^2 * 5 * 7 * 11 * 13 * 17 = 24,504,480

This is still a pretty big number, however, it's magnitudes smaller than 3,898,895,928,127,200 which is the length of the complete cycle for all numbers 1-36. At a length of under 25 million
it should be possible to make every binary combination of the even numbers only and see how many iterations are actually possible. Since only even numbers are being checked, that means
the final binary should be of the form 101010... or 01010101... Excluding the number 2 (because it will fill all of these gaps), we should be able to do a binary OR of all numbers (which is
quick) and if the final number has more than a single gap, the combo isn't possible.

After coming up with a down and dirty method for checking this I found that in the cycle of ~25 million for even numbers only, there's only 642432 combinations that actuuall work.
This is a mere 2.6% of all possible start numbers that don't get eliminated from this check.

Extending this logic further, what about looking at numbers divisible by 3? Then our cycle would be 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36  --> = 2^3 * 3^3 * 5 * 7 * 11 = 83,160
which is magnitudes less than when looking at only even numbers. Per the same method I calculated useable numbers divisible by 2, when looking at numbers divisible by 3 only 6,624 of
the 83,160 work as valid start numbers which is about 8%. So not as much of a reduction as the even numbers, but we've still eliminated 92% of starting numbers in the range. I could keep
expanding this to any number that evenly divides 36, but just to put things into perspective, how many starting numbers are getting eliminated over a large range? When n is 36 and we're
looking at a maximum range from 1 to 10,000,000 here is how many starting numbers get eliminated from each of the three operations I've looked at so far:

1. Ranges without two primes in them: 10,000,000 - 3,069,691 = 6,930,309 eliminated
2. Ranges that work with all numbers divisible by 2: 10,000,000 - 642,432 = 9,357,568 eliminated
3. Ranges that work with all numbers divisible by 3: 10,000,000 - 796,544 = 9,203,456 eliminated

There's obviously going to be some overlap between these three different things, so to see the total number of useable starting numbers we need to look at the intersection of
the three different methods. The result is better than I would've hoped for (assuming my logic is correct). Out of an initial pool of 10,000,000 numbers where our range could
start, eliminating all numbers with the above three methods takes this pool of useable numbers down to a mere 3,192 which is 99.97% reduction!
---

The above logic is definitely getting me close. When n is 36 I'm able to use a search range into the billions and the code doesn't even take 10 seconds to run, however, I'm not
finding a single hit after the first few. For reference, when n = 30 the first starting numbers for working ranges are 1, 2, 3, 4, 5, 6, 7, 376576202, 376576203, 376576204 ...
You can see that there's a pretty sizeable jump between working combinations. There are two options I can see right now. Either there's a slight flaw in my logic and I'm 
inadvertently skipping numbers that work, or, the gaps between working combos really is massive and I need to add some more ranges I can skip in order to reduce the total number
of values being searched.

Focusing on option one first, I tested my current algorithm against a pure brute force one and found something interesting. The values I got for each algorithm were identical
as long as the test number n was divisible by 6. If it wasn't, then I was getting slightly different values. I'm going to chalk this up to the fact that the logic I used for figuring
out working groups for even numbers only and every third number only works because the number of binary bits that the lowest number is guaranteed to fit in is the exact same as 
the amount of numbers <= n that are divisible by that number. A little confusing when written down, so here's an example.

   21 22 23 24 25 26
   -----------------
2   0  1  0  1  0  1
4   0  0  0  1  0  0
6   0  0  0  1  0  0

In this scenario, n = 6. We can see that the 2nd row will always have three binary bits no matter how it gets shifted. This is complemented by the fact that there are exactly three
even numbers <= 6. The three columns with active bits in row 2 will have to be chosen for 2, 4 and 6 in some fashion. It's this fact that allows us to very quickly binary OR rows
4 and 6 together to see if we've oversatured any of the three possible columns. On the flipside though, if n isn't divisible by 2, say n=7, then it would be possible for there to be 4 
binary bits in the second row:

   20 21 22 23 24 25 26
   --------------------
2   1  0  1  0  1  0  1
4   1  0  0  0  1  0  0
6   0  0  0  0  1  0  0

In this scenario the logic I'm using won't necessarily be correct and I may be eliminating valid possibilities. The way my logic is written is that since there are 4 columns that can
have a 2-bit in them, then all other even rows beside 2 need to fill 4-1 = 3 of these columns. The above scenario shows that isn't possible as the bits in row 4 and 6 can only fill
two of these binary bits, however, the above example has two solutions 2=22,4=20,6=24 and 2=26,4=20,6=24. This is definitely a shortcoming of my current algorithm, however,
I think fixing it should be straight forward enough. A simple change to the way I count the number of columns that need to be filled and now my quick algorithm matches the
brute force algorithm, great. The bad news is that I still can't find a single answer when n=36 beyond the numbers 1 through 5, so I'm going to need a way to further reduce the
search space.

As of right now, I think the biggest weak point of my algorithm is how much memory it requires. Basically, I create multiple separate arrays which hold possible starting locations.
For example, numbers that haven't been eliminated by looking at even factors only, and numbers that haven't been eliminated by looking at factors divisible by 3 are each stored in 
separate arrays. As was shown above, these array are cyclic with the cycle length depending on the prime factors of the rows being searched. The even rows had a cycle of ~24 million
and every third row had a cycle of ~80k. When I increase the length of my search space into the billions, each of these arrays will also grow and each hold hundreds of millions of
numbers. When these two arrays are finished being created, I then create a third array out of the union of the two original arrays. Basically, the third array only contains numbers
which appear in both of the other arrays. This final array is MUCH smaller than the two original. For perspective, when the n=36 and I put a hard limit at 1,000,000,000 for starting
numbers the sizes of the arrays are: divisble by 2 = 26,216,910, divisible by 3 = 79,653,712, union = 1,914,453. The final array is less than 1/10 the size of the 2 array and less than
1/40th the size of the 3 array. For further perspecitive, the sizes of the 2 and 3 arrays without adding extra cycles is only 642,432 and 6,624 respectively. It would be great I could
think of some algorithm to take these two much smaller arrays and combine them (knowing the cycle of each) to create the third array. I suppose this could potentially be done
algebraically.

When n=36 the cycle for the 2 array is 24,504,480 and the 3 array is 83,160. Taking two random non-matching numbers from each array, lets say two[18] = 407 and three[56] = 597. In theory
we should be able to find instances for these numbers that will "meet up" in the future with the equation x * 24504480 + 407 = y * 83160 + 597. Since both x and y need to be integers
this can be viewed as a linear Diophantine equation. If we were to choose a random number from the 2 array ('a') and a random number from the 3 array ('b') then the following would
need to be true to have integer solutions for x and y:

GCD(24504480, 83160) = 27720
(b - a) % 27720 = 0

A linear scan through both arrays can be carried out to choose pairs of (a, b) where the above congruence holds true. For each of these working pairs, the base solution
(x0, y0) for the Diophantine equation needs to be obtained, and once that solution is found, all other integer solutions can be found using:

x_k = x_0 - 83160
y_k = y_0 - 24504480
---

I've made good strides on this problem so far, but I'm going to need one final breakthrough to actually solve it I think. My biggest breakthrough so far has been realizing that
I can eliminate all starting numbers where the even numbers can't be "stuffed" into the even columns. Basically what this breakthrough boils down to is that even numbers can't
divide odd numbers, therefore all of the even columns must be occupied by even rows. This approach let me easily determine all starting numbers which can be eliminated from
1 to 24504480 which I can then extend indefinitely due to the cycle restarting at 24504481.

Directly stemming from this discovery, and admitedly something I didn't realize until just now, since all of the even columns must be occupied by even rows it means that only
odd rows can occupy the odd columns. This doesn't seem like a huge discovery at first, however, it's possible for odd numbers to divide even numbers (ie. 10 % 5 = 0). This means
that the odd rows are going to have much less oportunity to actually get used. As an example:

  30 31 32 33 34 35 36 37 38 39
3  -  x  x  o  x  x  -  x  x  o
5  -  x  x  x  x  o  x  x  x  x
7  x  x  x  x  x  o  x  x  x  x
9  x  x  x  x  x  x  -  x  x  x

The x's represent no divisibility, the -'s represent that divisibility is there, however, the space can't be used because on of the even columns will ultimately need to occupy
the space and the o's represent working locations. Looking above, we can see that from the 9 row alone that this starting location will be impossible. It would be nice to test
all numbers that get elimiated from this new phenomenon, however, the complete cycle length of the odd numbers less than 36 is a whopping 4,512,611,027,925 which is going to 
be out of reach. To sate my curiosity though, I can test all starting numbers in the same range as the even numbers cycle (24,504,480) and see how many numbers get eliminated.
My gut says that it will be quite a lot. After running the algorithm, of the first 24,504,480 numbers checked a mere 7,141 were usable as starting numbers. This is a reduction
of 99.97% of all possibilities which is massive. For reference, when looking at the even rows only there are 642,431 valid starting numbers which is only a 97.38% reduction of
all posibilities. If I were to create an array featuring numbers that only exist in the evens only and odds only arrays it would undoubtedly be the answer that I'm looking for.
The only problem is that the cycle for the odd numbers only is very large, even calculating working values up to 24 million takes around 2 seconds. This should scale linearly with
n, so I can extrapolate that going up to 4 trillion would take about 92 hours lol. Maybe there's a way I can speedup my current algorithm though? Even if I could speed it up 100x
(which I don't think I can) it would take almost a full hour to run so this may not work. I think I'm on the right track though with focusing on odd numbers, I just need to focus
on something slightly different maybe.

I just remembered something from the algorithm I created to generate all working start numbers between the evens only and every third number array. A working starting number
could only be generated if the difference between the number in each array was of the form k * GCD(LCM(even numbers), LCM(every third number)). My current dilemma is that there
are simply too many numbers to check in the odds only rows, however, if I can limit the numbers I need to check based on which numbers exist in the much smaller evens only array
we may be in business. The LCM for the evens only array is 24,504,480 while the LCM for the odds only array is 4,512,611,027,925. The GCD between these two numbers is 765,765.
So to find working solutions to the problem I need to take known numbers from the evens only array (call them x) and see if numbers of the form x + 765765k are valid solutions to
the odds only array. Do this for all combinations that are less than the cycle length of the odd array and I should have all the numbers I need. Unfortunately there will still be 
a lot of numbers to check, by my rough estimate it will be (~4trillion - ~24million) / 765765 * 642431 = 3,355,740,444,726. So I'm able to elimiate close to a trillion numbers,
however, since there were around 4 trillion to check right off the bat this isn't as helpful as is it seems.

----
My current algorithm was finding lots of numbers which worked in the case of only evens and only odds, however, couldn't find a solution at these numbers. After taking a bit more
of a deep dive I can see that there are plenty of instances where although the binary slots all get filled, there will be a certain odd row which is never choosable because its
only bit falls into an even column. Other odd rows make up the short fall by having multiple bits so the overall bits used are adequate. I think I can reduce the solution space
further in a few ways. 

First and foremost, when I add numbers to the list of potential starting numbers it's only based on my quick binary OR search. The above scenario has made me realize that just 
because all binary slots get filled, doesn't mean that the given solution is actually possible. What I should be doing is carry out my brute force search for any start number
which passes the binary OR test. I won't be testing all the rows from 1-n in this brute force search, I'll only be testing the even rows or the odd rows depending on which set
I'm looking at. This will take a little bit longer up front, however, it may drastically reduce my search space. For example, the even rows have over 600,000 starting numbers
that pass the binary OR test. It may be possible that only 1 in 10 of these start numbers would then pass the brute force test (maybe wishful thinking).

On a slightly different note, for the odd rows. Looking at the rows which are larger than n/2, these rows can have at most 2 binary bits lit up, however, for a majority of the
starting numbers they will have only a single bit lit up. Due to the revalation I had that binary bits in odd rows will never be selected in even columns, this means that there
should be large swathes of starting numbers which we can rule out simply because one of the odd rows will have 0 useable options. For example, when n = 36 and the starting number
is 36 - 70 we will instantly know that we should fail because the only column divisible by 35 will be 70. Since this is an even column it can't be occupied by an odd row. Compound
this with all of the other odd rows and the odds that we can actually select every odd row greater than n/2 should be pretty darn slim. The LCM of all odd numbers between 18 and 36
is around 20 billion. It won't be quick, however, this is small enough where the whole cycle can be checked. Once this whole cycle is calculated it can then be used to much
quickly check the cycle for all odd numbers. The total cycle is around 4 trillion, however, if there are only a handful of numbers which need to be checked out of every 20 billion,
then the search space should be able to be reduced enough that an exhaustive search can be carried out. Just to help myself keep track of what to do going forward.

1. Use brute force algorithm on all starting numbers which pass the even row OR test to reduce the even row starting numbers
    a. After applying this (with almost no additional run time cost), the 
	   amount of working starting numbers is just 3,455 out of the initial 24 millions possibilities. This means that
	   99.9859% of starting numbers are now eliminated purely by looking at the even rows.
	b. Since there are now so few possible starting points when simply using the even rows I decided to simply run my 
	   standard algorithm. It turns out this was enough to solve the problem! Finally! It takes 68 seconds to run though
	   which is awful so I'm still going to implement items 2-4 below which should greatly reduce this runtime.
2. Find all valid starting numbers by looking at odd rows greater than n/2
3. Find all valid odd row starting numbers using step 2 and the cycle from step 2 and a brute force of all start numbers that pass the OR check.
4. All numbers in the even and odd arrays should be valid solutions, find all common numbers to these arrays using the GCD technique
*/