#pragma once

#include <cstdint>
#include <vector>

struct MontgomeryNumber
{
	uint64_t value;
};

class MontgomerySpace
{
public:
	MontgomerySpace(uint64_t n);

	void calculateInverses(uint64_t range);

	MontgomeryNumber Transform(uint64_t a);
	uint64_t Revert(MontgomeryNumber _a);

	MontgomeryNumber Add(MontgomeryNumber _a, MontgomeryNumber _b);
	MontgomeryNumber Subtract(MontgomeryNumber _a, MontgomeryNumber _b);
	MontgomeryNumber Multiply(MontgomeryNumber _a, MontgomeryNumber _b);
	MontgomeryNumber Divide(MontgomeryNumber _a, MontgomeryNumber _b);

	MontgomeryNumber MontgomeryInverse(MontgomeryNumber _a);

private:
	uint64_t REDC(uint64_t t);
	uint64_t slowModMult(uint64_t a, uint64_t b, uint64_t m);
	void ExtendedEuclideanAlgorithm(uint64_t a, uint64_t b, uint64_t* x, uint64_t* y);

	std::vector<MontgomeryNumber> inverses;
	uint64_t n, r, n_prime, k, r2, r3;
};