#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <Header_Files/int_64.h>

long long CRT(std::vector<std::pair<long long, long long> > congruences)
{
	//the second number of each pair represents the modulo number while the first
	//represents the remainder, i.e. X congruent to congruences[i].first (mod congruences[i].second)
	long long modulus = 1, result = 0;
	for (int i = 0; i < congruences.size(); i++) modulus *= congruences[i].second;
	

	for (int i = 0; i < congruences.size(); i++)
	{
		long long withoutI = modulus / congruences[i].second;
		result += ModMult(ModMult(congruences[i].first, ModularMultiplicativeInverse(withoutI, congruences[i].second, 0), modulus), withoutI, modulus);
		std::cout << "MM^-1(" << withoutI << ", " << congruences[i].second <<
			") = " << ModularMultiplicativeInverse(withoutI, congruences[i].second, 0) << std::endl;
	}
	return result % modulus;
}

std::pair<std::string, double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0, mod = 10000000000000000;
	int max_length = 7;

	//for (long long i = 1; i < 10; i++)
	//{
	//	long long yeet = binomialMod(max_length + i - 2, i, mod);
	//	std::cout << yeet << std::endl;
	//	answer = (answer + ModMult(i, ModMult((long long)2, binomialMod(max_length + i - 2, i, mod), mod) - max_length + 1, mod)) % mod;
	//}
	////for (long long i = 1; i < 10; i++) answer += i * (2 * recursiveChoose(max_length + i - 2, i) - max_length + 1);
	//answer *= (MyPow(10, max_length) - 1) / 9;

	/*std::vector<std::pair<long long, long long> > eqs;
	eqs.push_back({ 28, 65536 }); eqs.push_back({ 56, 152587890625 });
	answer = CRT(eqs);*/

	answer = generalizedLucasTheorem(12, 7, 5, 16);
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//the answer is 2 ^ 13717420 - 1 + 256