#pragma once

#include <Header_Files/pch.h>
#include <cmath>
#include <map>
#include <vector>

//Odd Period Square Roots
std::pair<std::string, double> q64()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int* square_root_integers = new int[10001];
	square_root_integers[0] = 0; square_root_integers[10000] = 100;

	for (int i = 1; i < sqrt(10000); i++)
		for (int j = i * i; j < ((i + 1) * (i + 1)); j++) square_root_integers[j] = i;

	for (int n = 2; n < 10001; n++)
	{
		long long period_count = 0, n_root = square_root_integers[n];
		if (n == (n_root * n_root)) continue; //skip numbers that are perfect squares

		std::map<std::vector<long long>, bool> used_triplets;
		std::vector<long long> triple;

		triple.push_back(n_root);
		triple.push_back(0);
		triple.push_back(1);

		used_triplets[triple] = true;

		while (true)
		{
			triple[1] = triple[0] * triple[2] - triple[1];
			triple[2] = (n - (triple[1] * triple[1])) / triple[2];
			triple[0] = (n_root + triple[1]) / triple[2];

			if (used_triplets[triple] == true)
			{
				if (period_count % 2 == 1) answer++;
				break;
			}
			used_triplets[triple] = true;
			period_count++;
		}
	}

	delete[] square_root_integers;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1322
	//ran in 0.0776815 seconds
}

//NOTES
//Admittedly it took me awhile of just looking at the problem to figure out how they were calculating a0, a1, a2... It seemed a little arbitrary to me, because all
//of the fractions can theoretically have any a(n) added to it and still work. For example, in the problem text they simplify (sqrt(23) + 4) / 7 to 1 + ((sqrt(23) - 3) / 7),
//but instead of making a(n) 1, it could be made into 3 in which case (sqrt(23) + 4) / 7 would change to 3 + ((sqrt(23) - 17) / 7) which is the same value as
//1 + ((sqrt(23) - 3) / 7). I realized eventually though that 1 was selected because (sqrt(23) + 4) / 7 = 1.2565... It's because the value when written in decimal form
//begins with a 1 and not a 3. With this realization out of the way the problem no longer seems nearly as random to me. The next issue to figure out is how to figure out once
//you've reached the end of a pattern and not just an a(n) that happens to be the same as one already seen. This actually doesn't seem to bad when looking at the problem text as
//there are a two examples that lead to an a(n) of one. It can be seen in these instances that the number after sqrt(23) are different. And although they're the same, theoretically
//the denominator should be able to be different as well. Therefore it goes to reason that the period isn't complete until you run into an iteration where the a(n) value, value
//subtracted from the sqrt() and value dividing the sqrt() have all been seen at the same time before. In other words, if those three values were put into a triple then sqrt(23) would yield
//a string of triples that looked like this: (4, 4, 1), (1, 3, 7), (3, 3, 2), (1, 4, 7), (8, 4, 1), (1, 3, 7). The triple (1, 3, 7) has already appeared before so that means
//the period is found by taking the first digit of each triple starting with the second triple and ending at the second to last, i.e. [1, 3, 1, 8]. I'm not currently sure at this
//point if the middle element of each triple can be positive or if it's always negative. Converting from one triple to the next shouldn't be overly difficult, the third element can
//be found by the equation (n - (element2[n-1] * element2[n-1])) / element3[n-1], element1[n] = (int)(sqrt(n) + element2[n-1]) / element3[n] and element2[n] = element1[n] * element3[n]
// - element2[n-1]. Do this sequence until the first repeat triple is found for each number and that should be it. All of the examples given in the problem text are shown to be 
//repeating from the very first digit, i.e. [1;(2)], [2;(2, 4)], [3;(1, 1, 1, 1, 6}] but I'm not sure at this time if every root will follow suit or if it's possible for something
//like this to happen [4;1, 2, (3, 2, 3)] where not every digit repeats. If every digit is guaranteed to repeat then there's no need to keep track of any triple other than the first one,
//otherwise, each triple will need to be tracked. Instead of saving all triples, they can just be concatenated into a single number and stored in a map (i.e. (3, 3, 2) becomes 332,
//(1, 3, 7) becomes 137, etc.)

//I think the logic here is sound, however, I keep arriving at the wrong answer. After a little investigating I can see that there's some precision error going on with the square root
//calculations. One of the numbers I calculated by hand that a term should be 48.0102, but my code was calculating it to be 47.9989. Since I'm dropping the decimals off of the square
//roots this calculation is off by 1 instead of by just .0122 which is a pretty huge difference. I may need to rethink the algorithm here and try to avoid using square roots if possible.
//Instead of calculating out exact square roots (because having the decimal portion isn't necessary) I'll just sieve the integer portions. Just take all numbers from 1^2 - sqrt(10000)^2
//to initialize the sieve and fill in the gaps. So if we were looking at the first 20 numbers the sieve would start like this [0, 1, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0]
//and then be filled in like this [0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4]. This way the integer portion of all square roots is found without actually taking a single
//square root.

//After changing the algorithm so that it doesn't need square roots anymore I'm still arriving at the same wrong answer. A lot more thinking and debugging has now led me to believe that
//it's not a floating point issue at all, but an error with how I'm storing the seen triples as an integer. In a hypotheitcal situation, if I came across the triples (1, 23, 4) and
//(12, 3, 4) my program would save both of them as 1234 and would assume that they're the same thing but in reality they're different. I was hoping to avoid using a map to track these
//triples in vector form from a speed perspective but the amount of headache this issue has potentially caused it probably would've been better to just use one from the beginning. Who cares
//about shaving a few milliseconds off of the code if it causes an extra few hours of debugging time? After changing the map to hold a vector instead of a concatenated long long the 
//code spit out the answer on the first try and runs in only 77 milliseconds. This question was annoying because I had the right logic after only 20 minutes or so of thinking, but the
//implementation was just very sloppy.