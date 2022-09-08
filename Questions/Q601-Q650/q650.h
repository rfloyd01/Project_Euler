#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes ModMult()
#include <Header_Files/print.h>

//Divisors of Binomial Product
long long triangle(long long n)
{
	return n * (n + 1) / 2;
}

std::pair<std::string, double> q650()
{
	auto run_time = std::chrono::steady_clock::now();

	long long answer = 1, mod = 1000000007; //skip the first row and start with answer at 1
	int max_k = 10000;

	std::vector<int> prims = primes(max_k);

	for (int k = 2; k <= max_k; k++)
	{
		long long sum = 1;
		for (int i = 0; i < prims.size(); i++)
		{
			if (prims[i] > k) break;

			long long number_of_current_prime = 0, prime_power = prims[i], rolling_denominator_sum = 0;
			
			while (prime_power <= k)
			{
				long long divisor = k / prime_power;
				long long final_exponent = 2 * (k - divisor * prime_power) + 2;
				number_of_current_prime += divisor;
				rolling_denominator_sum += 2 * prime_power * triangle(divisor) - (divisor * (2 * prime_power - final_exponent));
				prime_power *= prims[i];
			}
			
			number_of_current_prime = number_of_current_prime * (k + 1) - rolling_denominator_sum;
			
			if (number_of_current_prime)
			{
				long long power_sum = (ModPow((long long)prims[i], number_of_current_prime + 1, mod, 1) - 1) * ModularMultiplicativeInverse((long long)prims[i] - 1, mod);
				sum = ModMult(sum, power_sum, mod);
			}
		}

		answer = (answer + sum) % mod;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 538319652
	//ran in 161.606 seconds
}

//NOTES
/*
Upon first glance I know that going through all of the binomial coefficients for a given n in order will gives us the n + 1 row of Pascal's triangle.
It should be a simple enough formula to figure out what the value is when multiplying all of these values for any given row. Using the example of 5 
we can see that:

Choose(5, 0) = 5!/(0!5!) = (5 * 4 * 3 * 2 * 1) / (1 * 5 * 4 * 3 * 2 * 1) = 1
Choose(5, 1) = 5!/(1!4!) = (5 * 4 * 3 * 2 * 1) / (1 * 4 * 3 * 2 * 1) = 5
Choose(5, 2) = 5!/(2!3!) = (5 * 4 * 3 * 2 * 1) / (2* 1 * 3 * 2 * 1) = 10
Choose(5, 3) = 5!/(3!2!) = (5 * 4 * 3 * 2 * 1) / (3 * 2 * 1 * 2 * 1) = 10
Choose(5, 4) = 5!/(4!1!) = (5 * 4 * 3 * 2 * 1) / (4 * 3 * 2 * 1 * 1) = 5
Choose(5, 5) = 5!/(5!0!) = (5 * 4 * 3 * 2 * 1) / (5 * 4 * 3 * 2 * 1 * 1) = 1

Multiplying the above equations together gives the single equation:

5^6 * 4^6 * 3^6 * 2^6 * 1^6
---------------------------
5^2 * 4^4 * 3^6 * 2^8 * 1^12

If we convert all of the numbers into their prim factorizations it will allow us to cancel out all the 
numbers in the denominator

5^6 * (2*2)^6 * 3^6 * 2^6 * 1^6
---------------------------
5^2 * (2*2)^4 * 3^6 * 2^8 * 1^12

5^6 * 3^6 * 2^18 * 1^6
---------------------------
5^2 * 3^6 * 2^16 * 1^12

5^4 * 2^2 = 2,500

We can see that the product of any row of pascal's triangle can be figured out with the equation:

product(pascal_n) = product(i = n-1 --> 2) {i^n} / product(i = n-1 --> 2) {i^2(n - i)}

Knowing this, we can put the product for any row of pascal's triangle into it's prime factorization form, which will then allow us to 
calculate the sum of all the factors via the equation:

sum(d|n) d = prod(i=1, k) \frac{p_i^{m_i+1} - 1}{p_i - 1}

The most time consuming thing here should be figuring out the prime factorization for the product of each row, however, since we're only
doing the first 20,000 rows it shouldn't be that much of a problem. We can either do this manually, or by trying to come up with some kind 
of an equation. It would be much easier to just go through and do each row manually, however, I'm sure that I can come up with some kind 
of an equation to do this as well. I'm not under any kind of time constraint to solve this problem so I'll take some time and try to come 
up with an equation.

-----
Coming up with an "easy" equation for the prime factorization of the product of rows of pascal's triangle
-----

We know that the product of each row of Pascal's triangle can be expressed as a numerator and denominator with the above equation.
Figuring out the amount of each prime in this numerator is easy. If we're in the nth row of Pascal's triangle then the amount of each prime
in the numerator (before cancelling from the denominator) will be n * (((2^{floor(log(n-1)/log2} - 1) * (n-1)) / 2^{floor(log(n-1)/log2}).
Looking at our example of 5, which is the 6th row of Pascal's triangle this would mean that in the prime factorization of the numerator 
there should be 6 * floor((((2^{floor(log5/log2} - 1) * 5) / 2^{floor(log5/log2})) = 6 * floor((((2^2 - 1) * 5) / 2^2)) = 6 * floor(15 / 4) = 18.
We can confirm this is true by looking at the above equation where we have the numerator as 5^2 * 3^6 * 2^18. This also means that we can calculate 
the amount of each following numerator by building off of the current ones.

Calculating the number of each prime in the denominator is a little trickier unfortunately because the exponents are different for each item.
In the case of row 11 of the triangle, for the denominator we have 10^2 * 9^4 * 8^6 * 7^8 * 6^10 * 5^12 * 4^14 * 3^16 * 2^18. If we only focus on items that are divisible
by two then we get 2^18 * 4^14 * 6^10 * 8^6 * 10^2. With a little bit of refactorint this can be rewritten as:

(2^4) * (2^4 * 4^4) * (2^4 * 4^4 * 6^4) * (2^4 * 4^4 * 6^4 * 8^4) * (2^4 * 4^4 * 6^4 * 8^4 * 10^4) * (2^-2 * 4^-2 * 6^-2 * 8^-2 * 10^-2)

using the equation from above, the number of 2's in the prime factorization for all of these items in parentheses could be found with 

4 * Sum(i = 1, 5){floor((((2^{floor(log2i/log2} - 1) * 2i) / 2^{floor(log2i/log2}))} - 2 * floor((((2^{floor(log10/log2} - 1) * 10) / 2^{floor(log10/log2)
4 * (1 + 3 + 4 + 7 + 8) - 2 * 8
4 * 23 - 16 = 76

If we were in the 11th row of the triangle then the number of 2's in the numerator would be 11 * floor((((2^{floor(log10/log2} - 1) * 10) / 2^{floor(log10/log2) = 88
so the final prime factorization of the 11th row should have a total of 88 - 76 = 12 2's in it. Let's see if this is correct;

(Choose(10, 0) * Choose(10, 1) * Choose(10, 2) * Choose(10, 3) * Choose(10, 4))^2 * Choose(10, 5) = 32,406,091,200,000,000
prime_factors(32,406,091,200,000,000) = 2^12 * 3^10 * 5^8 * 7^3.

Ok cool so that works. With this we don't have to actually hold the prime factorizations for any numbers, we can just work our way  through all the primes from 
2 to n-1 to figure out the sum of the factors for the nth row of Pascal's triangle. I can't say that I'm 100% satisfied with this equation though, it uses a lot 
of logarithms and would end up repeating a decent amount of things. I feel like I'm maybe missing one more piece of the puzzle that would give a much easier and
general equation so I'm going to keep digging just a little bit more.

-----
Coming up with a slightly more general equation
-----

I don't have much hope here, but just out of curiosity I want to see what happens when lining up the prime factorizations for the products of the first 
11 rows of Pascal's triangle to see if there's some kind of a pattern.

1 = 1
2 = 2^1
3 = 3^2
4 = 2^5 * 3^1
5 = 5^4 * 2^2
6 = 5^3 * 3^4 * 2^4
7 = 7^6 * 5^2 * 3^2
8 = 7^5 * 5^1 * 2^17
9 = 7^4 * 3^12 * 2^10
10 = 7^3 * 5^8 * 3^10 * 2*12

Try as I might I can't seem to find any helpful pattern. The only thing I notice is that when the row is a prime number, there will be n-1 of that prime and then 
the next n-1 rows will have one less of that prime. For example, the first time we see a 5 in the prime factorization is in the 5th row and there are 4 of them. 
In the next 4 rows the amount of 5's are 3, 2, 1 and 0. Outside of this it looks almost completely random, I'm sure it's not, but I'm finding it pretty hard to 
notice anything else.

------
After first solve: run time ~2.5 minutes
------
Currently the part of the algorithm that takes the longest is the block that calculates the sum of the prime powers and multiplies them (i.e. sum of 2^0 - 2^10 = (2^11 - 1)).
I currently get the answer for 10,000 in 40 seconds, and 39 of those seconds are dedicated to the above function. I'm not sure if memoization will help because I'm 
getting powers that go into the hundred thousands, so saving these values for all the different primes would be pretty memory intensive. When going all the way up to the 
answer the highest power of 2 encountered is 219,138 for reference.

If I want a faster time then I'll probably need to find a way to calculate multiple rows at a time as opposed to doing each of them one at a time.

Maybe what I can do is keep an array of length 20,000 where all values are initialzed to 1. Then, instead of iterating through all 20,000 rows and going through each
prime for each row, I can just iterate over all primes and plug their values into the appropriate row. We know for example that starting in the second row, 2 will
appear in the denominator and by raised to the second power. In the third row it'll be raised to the 4th power, in the 4th row it'll be raised to the 6th power and 
so on and so on. Since the exponents are always shifting by 2, we should always know how many exponents will be added from a given number by looking at the row before.
*/