#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h>
#include <cmath>

//Arranged Probability
std::pair<std::string, double> q100()
{
	auto run_time = std::chrono::steady_clock::now();

	long long total_discs = 0, pell[2] = { 1, 1 };
	bool first = true;

	while (total_discs < 1e12)
	{
		pell[first] += pell[!first];
		pell[!first] += pell[first];
		
		//the values obtained above are used to give us m and n in the 
		//primative pythagorean triple equation
		total_discs = 2 * pell[first] * (pell[first] + pell[!first]) + first;

		first = !first;
	}
	long long answer = total_discs / sqrt(2) + 1;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 756872327473
	//ran in 0.0000007 seconds
}

//NOTES
/*
* I turned the problem into an equation with two variables. Blue discs = B and total discs = T so to get a probability of 50% we need B/T * (B - 1)/(T - 1) = 1/2.
* Analyzing this equation by assuming T is a known value and trying to solve for B we see that B = (1 + sqrt(1 + 2(T^2 - T))) / 2 which means that 1 + 2(T^2 - T)
* must be a perfect square. 1 + 2(T^2 - T) = X^2 can be rewritten as T^2 + (T - 1)^2 = X^2 which is of course just the Pythagorean theorem. We can take this one step 
* further even. Since T and T - 1 are coprime, and one of them must be even while the other is odd, this actually forms a primitive Pythagorean Triple (i.e. 
* a = m^2 - n^2, b = 2mn, c = m^2 + n^2). Since the equations for a and b will swap depending on whether or not T is an odd or even number we end up with the equation
* (m - n)^2 - 2n^2 = +/- 1. This is just Pell's equation where the given integer value is 2, which can be solved via the recursive relation a_{n+1} = a_n + 2b_n,
* b_{n+1} = a_n + b_n. All we need to do is use this recursive relation to build up our value of T until it's larger than 10e12 and then solve for B.
* 
* The recursive relationship for the Pell's equation can be carried out with an array of size 2. Starting with the array [a, b] we add the second element to the first 
* to get [a + b, b]. We then add the first element back into the second to get [a + b, a + 2b]. The only issue with this is that the higher and lower values get swapped,
* if we were to carry out the same operation again then the recursive relation get's messed up. Since the location of the high and low value will swap, we can get around 
* this by using a boolean variable (which can only have a value of 0 or 1 and matches the indices of the array) and inverting it after each iteration. Doing this, we'll add 
* index 1 to index 0 in one iteration of the loop, and the add index 0 to index 1 in the next iteration of the loop with the exact same code. Furthermore, the equations
* for T and T-1 in the primative pythagorean triple equations will constantly be flipping back and forth between 2mn and m^2 - n^2 as T becomes even and odd. Since these 
* values will always be one away from each other, we can just choose one of the equations to carry out each time, and then either add 1 to it or not. We can use the same 
* boolean variable that handles our index swapping in the Pell array to carry out this potential addition (because adding 0 to something will keep it at the same value).
* 
* With all of the above we can easily calculate the value for T. The final thing to do is calculating the value of B from T. The original equation I came up with says that 
* B/T * (B - 1)/(T - 1) = 1/2 so if we have T then it should just be a simple quadratic equation right? Well unfortunately, since T will be larger than 10^12 we can't multiply 
* it be (T - 1) without overflowing a 64-bit integer. Instead we simplify the equation a bit. Since T is so large, then (T - 1) / T will be extremely close to 1 (as will be 
* the case for B) so we can simplify our equation to just B^2 = T^2 / 2 --> B = T / sqrt(2). This will obviously give us a decimal value, so B-1 will be the integer part 
* of this equation and we get B by simply adding 1.
*/