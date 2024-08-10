#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>

#include <bitset>
#include <map>

//Touch-screen Password

//void recursivePasswordCreate(int current_digit, std::vector<std::vector<int> >& blockers, int* total_passwords, int max_level, std::string password_string = "", int used_digits = 1, int current_level = 0)
//void recursivePasswordCreate(int current_digit, std::vector<std::vector<int> >& blockers, int* total_passwords, int max_level, int used_digits = 1, int current_level = 0)
void recursivePasswordCreate(int current_digit, std::vector<std::vector<int> >& blockers, int* total_passwords, int max_level, int used_digits = 1, int current_level = 0)
{
	//passwords need to be 2 digits or more
	if (current_level >= 2)
	{
		(*total_passwords)++;
		//std::cout << password_string.substr(0, password_string.size() - 2) << std::endl;
	}

	//base of recursion happens once we go beyond a 9-digit password
	if (current_level == max_level) return;

	for (int i = 1; i <= max_level; i++)
	{
		//Make sure the current digit isn't in the password already
		if ((1 << i) & used_digits) continue;

		//before selecting the current digit, make sure the path to it isn't blocked.
		//if ((used_digits & blockers[current_digit][i]) == blockers[current_digit][i]) recursivePasswordCreate(i, blockers, total_passwords, max_level, password_string + std::to_string(i) + "->", used_digits | (1 << i), current_level + 1);
		if ((used_digits & blockers[current_digit][i]) == blockers[current_digit][i]) recursivePasswordCreate(i, blockers, total_passwords, max_level, used_digits | (1 << i), current_level + 1);
	}
}

long long recursivePasswordCreateWithMemoization(int current_digit, std::vector<std::vector<int> >& blockers, std::map<std::pair<int, int>, long long>& memoize, int max_digit, int used_digits = 0, int current_level = 0)
{
	//The base of the recursion happens once the current password uses all the 
	//possible digits
	if (current_level == max_digit) return 1;

	//Before doing anything, check to see if the current combination
	//of used digits and current digit has been memoized. If so simply
	//return the memoized value.
	if (memoize.find({ used_digits, current_digit }) != memoize.end()) return memoize.at({ used_digits, current_digit });

	//Iterate through each digit from 1 to max digit, attempting to add the digit
	//to the current password.
	long long total_passwords = 1;
	for (int i = 1; i <= max_digit; i++)
	{
		//Make sure the current digit isn't in the password already
		if ((1 << (i - 1)) & used_digits) continue;

		//before selecting the current digit, make sure the path to it isn't blocked.
		//if ((used_digits & blockers[current_digit][i]) == blockers[current_digit][i]) recursivePasswordCreate(i, blockers, total_passwords, max_level, password_string + std::to_string(i) + "->", used_digits | (1 << i), current_level + 1);
		if ((used_digits & blockers[current_digit][i]) == blockers[current_digit][i])
		{
			total_passwords += recursivePasswordCreateWithMemoization(i, blockers, memoize, max_digit, used_digits | (1 << (i - 1)), current_level + 1);
		}
	}

	std::pair<int, int> memo_combo = { used_digits, current_digit };
	memoize.emplace(memo_combo, total_passwords);
	return total_passwords;
}

long long recursivePasswordCreateWithArrayMemoization(int current_digit, std::vector<std::vector<int> >& blockers,
	std::vector<std::vector<long long>>& memoize, int max_digit, int used_digits = 0, int current_level = 0)
{
	//The base of the recursion happens once the current password uses all the 
	//possible digits
	if (current_level == max_digit) return 1;

	//Before doing anything, check to see if the current combination
	//of used digits and current digit has been memoized. If so simply
	//return the memoized value.
	if (memoize[current_digit][used_digits]) return memoize[current_digit][used_digits];

	//Iterate through each digit from 1 to max digit, attempting to add the digit
	//to the current password.
	long long total_passwords = 1;
	for (int i = 1; i <= max_digit; i++)
	{
		//Make sure the current digit isn't in the password already
		if ((1 << (i - 1)) & used_digits) continue;

		//before selecting the current digit, make sure the path to it isn't blocked.
		if ((used_digits & blockers[current_digit][i]) == blockers[current_digit][i])
		{
			total_passwords += recursivePasswordCreateWithArrayMemoization(i, blockers, memoize, max_digit, used_digits | (1 << (i - 1)), current_level + 1);
		}
	}

	memoize[current_digit][used_digits] = total_passwords;
	return total_passwords;
}

std::pair<std::string, long double> q879()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	const int grid_size = 4;
	const int total_digits = grid_size * grid_size;
	const int password_permutations = (int)(pow(2, total_digits + 1) + 0.00001);

	//Write out numbers 0-16 in binary form for easier use later
	int b[17] = { 0, 0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000,
		0b100000000, 0b1000000000, 0b10000000000, 0b100000000000, 0b1000000000000,
		0b10000000000000, 0b100000000000000, 0b1000000000000000
	};

	//Create an multi-dimensional array of binary numbers representing blocked
	//paths. For example, if we start at the number 1, we can't go directly to
	//the number 3 because it's blocked by the number 2
	std::vector<std::vector<int> > blocked_digits_two = { {0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0}
	};

	std::vector<std::vector<int> > blocked_digits_threet = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, b[2], 0, 0, 0, b[4], 0, b[5]},
		{0, 0, 0, 0, 0, 0, 0, 0, b[5], 0},
		{0, b[2], 0, 0, 0, 0, 0, b[5], 0, b[6]},
		{0, 0, 0, 0, 0, 0, b[5], 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, b[5], 0, 0, 0, 0, 0},
		{0, b[4], 0, b[5], 0, 0, 0, 0, 0, b[8]},
		{0, 0, b[5], 0, 0, 0, 0, 0, 0, 0},
		{0, b[5], 0, b[6], 0, 0, 0, b[8], 0, 0}
	};

	std::vector<std::vector<int> > blocked_digits_four = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, b[2], b[3] | b[2], 0, 0, 0, 0, b[5], 0, b[6], 0, b[9] | b[5], 0, 0, b[11] | b[6]},
		{0, 0, 0, 0, b[3], 0, 0, 0, 0, 0, b[6], 0, b[7], 0, b[10] | b[6], 0, 0},
		{0, b[2], 0, 0, 0, 0, 0, 0, 0, b[6], 0, b[7], 0, 0, 0, b[11] | b[7], 0},
		{0, b[3] | b[2], b[3], 0, 0, 0, 0, 0, 0, 0, b[7], 0, b[8], b[10] | b[7], 0, 0, b[12] | b[8]},
		{0, 0, 0, 0, 0, 0, 0, b[6], b[7] | b[6], 0, 0, 0, 0, b[9], 0, b[10], 0},
		{0, 0, 0, 0, 0, 0, 0, 0, b[7], 0, 0, 0, 0, 0, b[10], 0, b[11]},
		{0, 0, 0, 0, 0, b[6], 0, 0, 0, 0, 0, 0, 0, b[10], 0, b[11], 0},
		{0, 0, 0, 0, 0, b[7] | b[6], b[7], 0, 0, 0, 0, 0, 0, 0, b[11], 0, b[12]},
		{0, b[5], 0, b[6], 0, 0, 0, 0, 0, 0, 0, b[10], b[11] | b[10], 0, 0, 0, 0},
		{0, 0, b[6], 0, b[7], 0, 0, 0, 0, 0, 0, 0, b[11], 0, 0, 0, 0},
		{0, b[6], 0, b[7], 0, 0, 0, 0, 0, b[10], 0, 0, 0, 0, 0, 0, 0},
		{0, 0, b[7], 0, b[8], 0, 0, 0, 0, b[11] | b[10], b[11], 0, 0, 0, 0, 0, 0},
		{0, b[9] | b[5], 0, 0, b[10] | b[7], b[9], 0, b[10], 0, 0, 0, 0, 0, 0, 0, b[14], b[15] | b[14]},
		{0, 0, b[10] | b[6], 0, 0, 0, b[10], 0, b[11], 0, 0, 0, 0, 0, 0, 0, b[15]},
		{0, 0, 0, b[11] | b[7], 0, b[10], 0, b[11], 0, 0, 0, 0, 0, b[14], 0, 0, 0},
		{0, b[11] | b[6], 0, 0, b[12] | b[8], 0, b[11], 0, b[12], 0, 0, 0, 0, b[15] | b[14], b[15], 0, 0}
	};

	/*long long recursive_answer = 0;
	std::map<std::pair<int, int>, long long> memoized_passwords;
	recursive_answer += recursivePasswordCreateWithMemoization(1, blocked_digits_four, memoized_passwords, total_digits, b[1], 1) - 1;
	recursive_answer += 2 * (recursivePasswordCreateWithMemoization(2, blocked_digits_four, memoized_passwords, total_digits, b[2], 1) - 1);
	recursive_answer += recursivePasswordCreateWithMemoization(6, blocked_digits_four, memoized_passwords, total_digits, b[6], 1) - 1;

	std::cout << "Recursive answer is: " << recursive_answer << std::endl;*/

	//Simple print test to make sure the above array is set up correctly. Doesn't quite work when
	//two blockers are present but should give a good idea
	/*for (int i = 1; i <= 16; i++)
	{
		for (int j = 1; j <= 16; j++)
		{
			if (blocked_digits_four[i][j] > 1)
			{
				std::cout << "The path from " << i << " to " << j << " is blocked by " << log(blocked_digits_four[i][j]) / log(2) + 1 << "\n";
			}
		}
	}*/

	/*int total_passwords = 0;
	recursivePasswordCreate(0, blocked_digits_four, &total_passwords, 16);
	std::cout << total_passwords << std::endl;*/


	/*Better Approach*/

	//Setup the data structure to hold all passwords
	std::vector<std::vector<long long> > all_passwords = { {} }; //put an empty array at index 0
	for (int i = 0; i < total_digits; i++)
	{
		std::vector<long long> passwords_ending_in_n(password_permutations, 0);
		all_passwords.push_back(passwords_ending_in_n);
	}

	long long better_recursive_answer = 0;
	better_recursive_answer += recursivePasswordCreateWithArrayMemoization(1, blocked_digits_four, all_passwords, total_digits, b[1], 1) ;
	better_recursive_answer += 2 * recursivePasswordCreateWithArrayMemoization(2, blocked_digits_four, all_passwords, total_digits, b[2], 1);
	better_recursive_answer += recursivePasswordCreateWithArrayMemoization(6, blocked_digits_four, all_passwords, total_digits, b[6], 1);
	
	//Remove the single digit passwords and then multiply by 4 to get the rotated passwords
	better_recursive_answer = ((better_recursive_answer - 4) * 4);
	//std::cout << "Better Recursive answer is: " << better_recursive_answer << std::endl;

	//Initialize all indices that are powers of two within the appropriate array
	/*int mult = 0b1;
	for (int i = 1; i <= total_digits; i++)
	{
		all_passwords[i][mult] = 1;
		mult *= 2;
	}*/

	//We can utilize the 4-way rotational symmetry of the password board. Only start
	//passwords with the numbers 1, 2, 3 and 6 and multiply the final answer by 4
	//all_passwords[1][1] = 1;
	//all_passwords[2][2] = 1;
	//all_passwords[3][4] = 1;
	//all_passwords[6][32] = 1;

	////In "total_digits" - 1 iterations, we create all possible passwords with a 
	////dynamic approach.
	//for (int i = 1; i < (password_permutations - 1); i++)
	//{
	//	for (int j = 1; j <= total_digits; j++)
	//	{
	//		//Check to see if there are actually any passwords at the current
	//		//index before moving on.
	//		if (all_passwords[j][i] == 0) continue;
	//		
	//		//in this nested loop, i represents the current password permutation
	//		//that we're looking at while j represents the final digit being looked
	//		//at for the given permutation.
	//		int shifter = 0b1;
	//		for (int k = 1; k <= total_digits; k++)
	//		{
	//			//we check all digits, 1-n, and see if they aren't part of the current
	//			//password yet. If not, we see if it's possible to add the number 
	//			//(i.e. it isn't blocked in any way).
	//			if (!(i & shifter))
	//			{
	//				//the number isn't in the password yet, check to see if there are
	//				//any blockers from the last digit (j) and the new digit (k)
	//				if ((i & blocked_digits_four[j][k]) == blocked_digits_four[j][k])
	//				{
	//					//The move is allowed, add the number at the current index to the 
	//					//final total, and then also add the number to the appropriate position
	//					//in the all_paswswords data structure.
	//					answer += all_passwords[j][i];
	//					all_passwords[k][i | b[k]] += all_passwords[j][i];
	//				}
	//			}

	//			shifter <<= 1;
	//		}
	//	}
	//}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4350069824940
	//ran in 0.0300256 seconds
}

//NOTES
/*
-- Brute Force Approach --

The way the problem is written, I'm not entirely sure if all types of movements are possible or simply movements that are a single lattice 
square away. For example, movement from the number 1->8 or 2->7 should be possible as nothing is blocked. I'm just going to write a brute
force method to solve the 3x3 case to confirm what can actually happen. A quick recursive algorithm should be able to handle this pretty easily,
I just need to pass in some kind of array to the method that will skip over numbers that are currently blocked. I think using binary numbers
to keep track of which numbers are currently in the password and which numbers block which other numbers can achieve this efficiently enough.
...
After writing the brute force recursive algorithm I got the exact same number, so this confirms that movements farther away than one lattice
square that go in between other numbers are possible. The recursive algorithm I came up with actually runs fairly quickly, I wouldn't be shocked
if I could modify it for the 4x4 case and have it run in under a minute. I should do this as it would be very easy to implement and could give
me the correct answer to shoot for while I try and come up with a better algorithm. The only major difference here is that the numbers which 
become blockers are now a little harder to pinpoint for some cases. For example, traveling from the number 1 to the number 15 in a straight line
without touching any other numbers is possible, however, the line comes extremely close to hitting the numbers 6 and 10. This line can be drawn
on a computer, however, would it be fair to assume this line could be drawn with a finger to enter in a phone password? Probably not. My algorithm 
is simple enough though that if I get the wrong answer I can easily modify the list of blockers and run it again.

There are only three different types of numbers here (corner, edge and centers) so due to rotation/symmetry they should all have the same types
of blocking numbers within each category. Also, looking at a 3x3 grid within the 4x4 grid the blockers should all be the same. With that said,
for my first attempt at the 4x4 here's what I think the list of new blockers for the 4x4 grid is for each number type:
1 Corner:
1->4 is blocked by 2 and 3 (0b1100)
1->16 is blocked by 6 and 11 (0b100001000000)
1->13 is blocked by 5 and 9 (0b1000100000)

2 Edge:
2->4 is blocked by 3 (0b1000)
2->12 is blocked by 7 (0b10000000)
2->14 is blocked by 6 and 10 (0b10001000000)

6 Center:
6->8 is blocked by 7 (0b10000000)
6->14 is blocked by 10 (0b10000000000)
6->16 is blocked by 11 (0b100000000000)

ALl of the other numbers on the grid are some rotation of these ones so no need to write them all down here.
...
I should've thought this through a little bit more before coding up the 4x4 grid, but the brute force code
I have which works will take wayyyy too long for the 4x4 case. In a rough sense, the 3x3 case had 9! possibilities
and ran in 0.0054 seconds, which translates to about 0.0000000138 seconds per password. The 4x4 case will have
roughly 16! passwords, so assuming the same amount of time to find each password we'd be looking at roughly
308766 seconds for the code to run, which is 85 hours. So clearly this will violate the 1 minute rule lol. This is
alright though, I really only coded this brute force approach to confirm which passwords could actually be selected.
I'll just need to actually think of an algorithm now.

--Algorithmic Approach--

My first big observation is that there's a lot of symmetry in the patterns that we can choose. For example, in the 3x3 version
of the problem the password 1->2->3 is the same thing as the password 9->8->7, just rotated 180 degrees about the middle. As interesting
as this fact is, I don't think it will help that much (at least with my current approach). With rotations and reflections I'm seeing
at most an 8-fold symmetry. According to my estimates, my brute force algorithm should take ~ 85 hours to solve, so with 8-fold symmetry
factored in this would still be over 10 hours of run time.

Maybe another option would be to do some form of selective combinatorics. Looking at the 3x3 case, we know that the number 2 can go 
to any other number directly with the exception of 8 (because 5 is in the way). Via combinatorics we could create all permuations
where the number 2 goes directly to the number 8 and the number 5 hasn't been used yet and deduct this number from the total number
of possibilities. The number of ways to see where the number 8 comes directly after the number 2, both of which appear before the number
5, would look something like this when looking exclusively at 9 digit codes:

28******* = 7!
[^5]28****** = 6 * 6!
[^5][^5]28***** = choose(6, 2) * 5!
[^5][^5][^5]28**** = choose(6, 3) * 4!
....
******285 = 6!

While something like this could work, it has a few major hurdles that would need to be over come. First, although the permutations created
above would indeed weed out passwords that don't work because of 2 to 8 being blocked by 5, there could potentially be other errors in there.
For example, the password 1->2->8->7->3->5->6->4->9 is incorrect because 2->8 AND 7->3 are blocked by the 5. This means that when we inevitably
go to create password exceptions having to do with going from 3->7, we will end up double counting this particular password. We could account 
for double counting like this with the Inclusion-Exclusion principle, and we may be able to get away with it in the 3x3 case, however, the 4x4
case has many more overlapping blocked numbers and trying to keep track of all conflicting ones with Inclusion-Exclusion will most likely
be a nightmare.

So if brute force with symmetry and combinatorics are both out, what about some form of dynamic algorithm? The algorithm would be carried out in
9 stages (16 for the 4x4 case) where at the end of the n-th stage we will have all the passwords of length n. This sounds fantastic on paper,
but it is it feasible to actually code up? For this to work you would need to break up passwords by their current makeup so that you would have
an idea of where they could go in the next stage, meaning, an array that holds all ordered passwords possibilities (i.e. we wouldn't care if 
the current password is 1->2->3, 2->1->3, etc., we only care that the makeup has a 1, a 2, and a 3). In the 3x3 case there are 2^10 = 1024 and
in the 4x4 case there are 2^17 = 131072 distinct digit combinations. The 3x3 case is trivial, but even in the 4x4 case this isn't that that bad,
even considering that each of these values will increase by a factor of 2 so we can include the digit 0 to make sure the array is zero indexed for
easier accessing. 

So looking at the 3x3 case first, our array would like something like this after initial setup:
{0, 1, 10, 11, 100, 101, 110, 111, 100, ..., 1000, ..., 10000, ..., 100000, ..., 1000000, ..., 10000000, ..., 100000000, ..., 1000000000, ...}
[0, 0,  1,  0,   1,   0,   0,   0,   1, ...,    1, ...,     1, ...,      1, ...,       1, ...,        1, ...,         1, ...,          1, ...]

The binary is just to help visualize what value goes at each index, the second row above is the actual array. In the "first round" of the algorithm
only indices which are powers of 2 will have a value. Each binary digit represents another number in the password and powers of 2 only have a single
binary digit. The binary 0b10 represents a password of 1 (as it's composed of 1 * 2^1 + 0 * 2^0), 0b100000 represents 5 (as it's composed of 
1 * 2^5 + 0 * (2^4 + 2^3 + 2^2 + 2^1 + 2^0), etc. The idea is that we can iterate through this array and at a glance see the makeup of the current
password, so we'll know which other nodes we can go to. In this first round each node only has a value of 0 or 1, however, in later rounds these
numbers will get much larger making things more efficient. On second though, we're also going to need to know the number currently at the end of
each password (because we need to know which numbers can potentially block us) so the above array will need to be replicated n number of times. Since
the entire array will need to be iterated over in each iteration of the main loop, and there are n copies of the array, and we try to add all n-digits to 
the current password each time, the time complexity will be in the range of O(n^2 * 2^(n+1)). Comparing this to the brute force method which was on the scale of O(n!) and we can see this is a much
better approach (even though the memory will now be on the scale of O(2^n+1)). For the 4x4 case we get O(2.092279e+13) with the brute force vs.
O(33,554,432) which is 6 orders of magnitude less. Not sure if this will translate directly into runtime, but with my earlier estimate of the brute force
method, this new method should be able to reach an answer in around 3.5 seconds. In contrast for the 3x3 grid you get O(362,880) vs. O(82,944), so the
speedup won't be nearly as pronounced.

The idea is that as we iterate through the array, the numbers in their respective indices of the array will be physically removed from where the are
(meaning the value at the current index will go to 0) and added on top of the value at a different index. This means that the cumulative sum of the 
whole array will represent the number of n-digit passwords that are possible. Since we're constantly adding digits, then numbers will only migrate
forwards in the array, so it would make sense to start at the end of the array and iterate backwards as to not double count. This would work in a single
array, however, since there are going to be n-copies of the array then we're still going to end up reading numbers for the next iteration while we're still
on the current iteration. For example, when iterating through the "ends in 2" array if we add a 5 then the value at index 100100 of the "ends in 5" array
will increase. When we then iterate through the "ends in 5" array three iterations later, the algorithm will see this non-zero value and assume that we 
need to go ahead and add another digit to it. This means that by the end of the first iteration we could actually get a full n-digit password. This isn't
necessarily a bad thing I guess, it should still take n iterations to make all the passwords.

----- Memoized Brute Force ----
After reading the forums I can see that some people were able to solve this relatively quickly using a recursive approach. All they did differently me 
was to memoize the number of passwords created from used digit and current digit pairs. Quickly altering my brute force recursive approach and sure enough
I'm able to get to the correct answer for the 4x4 case in under a second. When I first wrote this algorithm it was for the 3x3 appraoch and memoization 
didn't really occur to me because the total solution space was already low enough. If applied rotational symmetry to this approach I should be able to 
lower the runtime to maybe somewhere around 100 ms. My dynamic approach is still faster, and believe it or not takes up less memory (~24 MB for the memoized approach
vs. only 16.5MB for the dynamic appraoch). I guess the main difference in memory here is using a map instead of a standard array, so even though there are less
overall nodes, each node contains more information.
*/