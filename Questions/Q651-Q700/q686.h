#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>

//Inverse Digit Sum
long long s(long long n, long long mod)
{
	return ((n % 9 + 1) * ModPow((long long)10, n / 9, mod, 1)) % mod - 1;
}

long long S(long long n, long long mod)
{
	long long multiple9 = n / 9 * 9; //the first division gets floored so multiple 9 won't necessarily equal n
	long long S_value = ((6 * ModPow((long long)10, multiple9 / 9, mod, 1)) % mod - (multiple9 % mod + 6 % mod) % mod) % mod;

	for (int i = 1; i <= (n % 9); i++) S_value = (S_value + s(multiple9 + i, mod)) % mod;
	return S_value;
}

std::pair<std::string, double> q684()
{
	auto run_time = std::chrono::steady_clock::now();

	//answer starts at 2 because that's the sum of f1 and f2
	long long answer = 0, fibbonaci[2] = { 0, 1 }, mod = 1000000007;

	for (int i = 2; i <= 90; i++)
	{
		long long temp = fibbonaci[1];
		fibbonaci[1] += fibbonaci[0];
		fibbonaci[0] = temp;

		answer = (answer + S(fibbonaci[1], mod)) % mod;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 922058210
	//ran in 0.0017131 seconds
}

//NOTES
/*
My Project Euler numbers have stagnated recently so I've decided to go on a low hanging fruit hunt. This problem is only a 5% difficulty level, and involves
the first 90 fibonacci numbers. After checking with the int_64x class, the 90th fibonacci number just fits into a long long type. No matter what the number
is, the way to minimze the length of the digits that sum to it is by mazimizing the number of 9's we have. For example, with the number 20, we could sum to it
with all 1's if we wanted, i.e. 11111111111111111111, but even better would be to compress the 1's 9 at a time into a 9, i.e. 1199. There are two left over 1's
in this case so they should be compressed into a 2, as less digits will always yield a lower number, despite the physicaly digit being larger. So 1199 goes to 
299 which is indeed the lowest number whose digits sum to 20. Following this theory, I think the minimal number whose digits sum to a different number will 
always follow the formula:

digit 1 = N % 9
next N/9 amount of digits = 9

If we did this for the first 20 numbers would get: [1, 2, 3, 4, 5, 6, 7, 8, 9, 19, 29, 39, 49, 59, 69, 79, 89, 99, 199, 299]. Adding all of these values together
yields 1,074 and matches the problem statement, confirming my theory. So figuring out the appropriate lowest number is a simple task, the only difficult thing
is figuring out the appropriate value after modulus dividing it by 1000000007. It's easy enough to figure out the smallest number whose digits sum to 10,000 
is 199999999... (1111 9's), but how to I get the modulus of this large number? Well, since the number will always end with a bunch of 9's this means that it
will always be of the form (N % 9 + 1) * 10^(N / 9) - 1. Looking back to our example of 20, it will have the form (20 % 9 + 1) * 10^(20/9) - 1 = 3 * 10^2 - 1 = 299.
If we can break down our large number into an equation like this then taking the modulus should be simple. With all of this knowledge, our answer should just be
((fib_i % 9 + 1) * ModPow(10, fib_i / 9, 1000000007)) % 1000000007 - 1.

I didn't get the answer right at first, and couldn't figure out why. After re-reading the problem I saw the issue, at first I thought they were asking for the sum
of small s, s(fi) but it turns out that they want the sum of big S, S(fi) for the 2nd through 90th Fibonacci numbers. This definitely adds a layer of complexity
to the problem. This means that we're going to be calculating all values of s(n) for n = 1 through n = F90 which is a 19 digit number. Calculating this all one
at a time isn't feasible so instead we'll need to calculate the sum over a given range each time. Although this adds a layer of complexity, the problem still isn't
that difficult. Looking at our range for s(1) - s(20) we see that a pattern forms. Every 9 numbers that gets added will have the numbers (1-9) one place further 
than the 9 previous numbers and a 9 in the ones place 9 times in a row. Basically you multiply the sume of 1-9 (45) by a repunit and it to 9*9 (81) multiplied by
the repunit that's one less. In equation form this would look something like:

S(n) where n % 9 == 0 --> 45 * ((10 ^ (n / 9) - 1) / 9) + 81 * 
sum these consecutive values together it would give us our value of big S, S(n) for every 9th number -> [45, 576, 5967, 59958]. The equations for these S(n) values
are

45 * 1 + 81 * 0 = 45
45 * 11 + 81 * (1) = 576
45 * 111 + 81 * (10 + 2) = 5,967
45 * 1111 + 81 * (100 + 20 + 3) = 59,958

So in each iteration we multiply 45 by an n-digit repunit and add that to 81 times the sum of all repunits that are less than n digits. A little off screen math 
and I found that the sum of consecutive repunits can be characterized by the following equation.

Sum Repunits 1 - N = (10^(N+1) - 9N - 10) / 81

Putting everything together and we get that the value for every 9th S(n) is:
S(n) where n % 9 == 0 --> 5 * (10 ^ (n / 9) - 1) + 10^(n / 9) - 9(n / 9 - 1) - 10
S(n) = 6 * 10^(n/9) - n - 6

trying this formula out we get
S(9) = 6 * 10^1 - 15 = 45
S(18) = 6 * 10^2 - 24 = 576
S(27) = 6 * 10^3 - 33 = 5,967
S(36) = 6 * 10^4 - 42 = 59,958

All of these values match so the formula appears to be correct. With this the problem should now be pretty easy. Divide each fibonocci number by 9 to get the value of
S(floor(fib_i / 9)). Then calculate the values of little s, s(i) for i = floor(fib_i / 9) * 9 + 1 through fib_i which will be a maximum of only 8 calculations.
*/