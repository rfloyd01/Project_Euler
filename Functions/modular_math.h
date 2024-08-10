#pragma once

long long BetterModMult(long long a, long long b, long long m);

//Montgomery Methods
long long MontgomeryMultiplication(unsigned long long a, unsigned long long b, unsigned long long n);
long long MontgomeryMultiplication32Bit(unsigned long long a, unsigned long long b, unsigned int n);
long long MontgomeryMultiplication64Bit(unsigned long long a, unsigned long long b, unsigned long long n);

long long REDC(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t);

long long EfficientREDC(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t, int k);
long long EfficientREDC32Bit(unsigned long long r, unsigned long long n, unsigned long long n_prime, unsigned long long t, int k);