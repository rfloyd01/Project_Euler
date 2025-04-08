#pragma once

#include <cstdint>
#include <vector>
#include <Header_Files/uint128_t.h>

std::vector<int> generateContinuedFractionCoefficients(double num, int limit = 50);
std::vector<int> generateContinuedFractionCoefficients(int fraction[2], int limit = 50);
int generateNextContinuedFractionCoefficient(double& num);
int generateNextContinuedFractionCoefficient(int* fraction[2]);

void generateNextFraction(double& num, int(&numerators)[2], int(&denominators)[2]);
void generateNextFraction(double& num, uint128_t(&numerators)[2], uint128_t(&denominators)[2]);