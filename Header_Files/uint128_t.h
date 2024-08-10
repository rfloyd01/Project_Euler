#pragma once

#include <utility>

/*
* This class represents an unsigned 128-bit integer. The largest built-in integer type that works with 
* most compilers is only 64-bits, so this class doubles that. Unlike the int_64x class that I created,
* this one is limited at 128-bits which means most of the arithmetic operations becomes much simpler.
* 
* The main uses for this class will be in functions that are meant to not overflow a standard 64-bit
* integer, but need 128-bits for some of the intermediate steps. A good example is the ModMult method
* which returns the value of ab mod N. If a and b are both large enough then the multiplication will
* overflow 64-bits before the modulus division can take place.
*/

class uint128_t
{
public:
	//CONSTRUCTORS
	uint128_t() = default;
	uint128_t(const unsigned long long& num);
	uint128_t(const long long& num);
	uint128_t(const unsigned int& num);
	uint128_t(const int& num);
	uint128_t(const uint128_t& num);

	unsigned long long* getWords() { return &words[0]; }

	//OPERATORS
	//Addition
	uint128_t& operator+=(const uint128_t& num);
	friend uint128_t operator+(const uint128_t& num1, const uint128_t& num2);

	//Subtraction
	uint128_t& operator-=(const uint128_t& num);
	friend uint128_t operator-(const uint128_t& num1, const uint128_t& num2);

	//Multiplication
	//TODO: Implement Karatsuba multiplication
	uint128_t& operator*=(const uint128_t& num);
	friend uint128_t operator*(const uint128_t& num1, const uint128_t& num2);

	//Division
	//TODO: Can probably make this more efficient somehow
	uint128_t& operator/=(const uint128_t& num);
	friend uint128_t operator/(const uint128_t& num1, const uint128_t& num2);

	//Modular Division
	//TODO: Can probably make this more efficient somehow
	uint128_t& operator%=(const uint128_t& num);
	friend uint128_t operator%(const uint128_t& num1, const uint128_t& num2);

	//Shifting
	uint128_t& operator<<=(const unsigned int n);
	friend uint128_t operator<<(const uint128_t& num, const unsigned int n);
	uint128_t& operator>>=(const unsigned int n);
	friend uint128_t operator>>(const uint128_t& num, const unsigned int n);

	//BINARY OPERATORS
	uint128_t& operator|=(const uint128_t& num);
	friend uint128_t operator|(const uint128_t& num1, const uint128_t& num2);
	uint128_t& operator&=(const uint128_t& num);
	friend uint128_t operator&(const uint128_t& num1, const uint128_t& num2);
	uint128_t& operator^=(const uint128_t& num);
	friend uint128_t operator^(const uint128_t& num1, const uint128_t& num2);
	friend uint128_t operator~(const uint128_t& num1);

	//BOOLEAN OPERATORS
	friend bool operator==(const uint128_t& num1, const uint128_t& num2);
	friend bool operator!=(const uint128_t& num1, const uint128_t& num2);
	friend bool operator<(const uint128_t& num1, const uint128_t& num2);
	friend bool operator>(const uint128_t& num1, const uint128_t& num2);
	friend bool operator<=(const uint128_t& num1, const uint128_t& num2);
	friend bool operator>=(const uint128_t& num1, const uint128_t& num2);

	//ASSIGNMENT OPERATORS
	uint128_t& operator=(const uint128_t& num);

	//OTHER METHODS
	//std::string getNumberString();

private:
	//HELPER METHODS
	uint128_t Karatsuba64Bit(const unsigned long long& num1, const unsigned long long& num2);
	std::pair<uint128_t, uint128_t> bitwiseDivision(const uint128_t& dividend, const uint128_t& divisor);

	unsigned long long words[2] = { 0, 0 }; //holds the two 64-bit words that make up the final 128-bit number
};

//Printing Methods
//TODO: Find an efficient way to convert to decimal and print
//std::ostream& operator<<(std::ostream& os, const uint128_t& num);