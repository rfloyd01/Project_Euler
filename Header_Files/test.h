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
	int max_length = 2020;

	for (long long i = 1; i < 10; i++)
	{
		/*long long yeet = binomialMod(max_length + i - 2, i, mod);
		std::cout << yeet << std::endl;*/
		//long long yo = ModMult((long long)2, binomialMod(max_length + i - 2, i, mod), mod);
		//yo = (yo -)
		answer = (answer + ModMult(i, (((ModMult((long long)2, binomialMod(max_length + i - 2, i, mod), mod)) - (max_length - 1)) + mod) % mod, mod)) % mod;
	}
	//for (long long i = 1; i < 10; i++) answer += i * (2 * recursiveChoose(max_length + i - 2, i) - max_length + 1);
	answer = ModMult(answer, ModMult(((ModPow((long long)10, (long long)max_length, mod, 1) - 1) + mod) % mod, ModularMultiplicativeInverse((long long)9, mod, 0), mod), mod);
	//answer = ModMult(answer, ModMult(1111111111111111, ModularMultiplicativeInverse((long long)9, mod, 0), mod), mod);

	//answer = ((ModPow((long long)10, (long long)max_length, mod, 1) - 1) + mod) % mod;
	//answer = ModularMultiplicativeInverse((long long)9, mod, 0);
	//answer = ModMult((ModPow((long long)10, (long long)max_length, mod, 1) - 1), ModularMultiplicativeInverse((long long)9, mod), mod);
	//ModMult((ModPow((long long)10, (long long)max_length, mod, 1) - 1), ModularMultiplicativeInverse((long long)9, mod), mod)
	/*std::vector<std::pair<long long, long long> > eqs;
	eqs.push_back({ 28, 65536 }); eqs.push_back({ 56, 152587890625 });
	answer = CRT(eqs);*/

	//answer = generalizedLucasTheorem(12, 7, 5, 16);

	/*int_64x yeet = (BigPow(10, 2020) - 1) / 9, yote("28513230804693658426662405");
	int_64x ans = yeet * yote;
	std::string ansString = ans.getNumberString();
	std::cout << ansString.substr(ansString.length() - 16) << std::endl;*/
	int_64x ans = 0;
	for (long long i = 1; i < 10; i++)
	{
		ans += i * (2 * BigChoose(max_length + i - 2, i) - max_length + 1);
		//answer = (answer + ModMult(i, (((ModMult((long long)2, binomialMod(max_length + i - 2, i, mod), mod)) - (max_length - 1)) + mod) % mod, mod)) % mod;
	}

	int_64x mult = (BigPow(10, max_length) - 1) / 9;
	std::cout << (mult * ans) % mod << std::endl;
	
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//the answer is 2 ^ 13717420 - 1 + 256