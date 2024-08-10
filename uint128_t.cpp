#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/uint128_t.h>
#include <iostream>
#include <bitset>
#include <cmath>

uint128_t::uint128_t(const unsigned long long& num)
{
	this->words[0] = num;
}

uint128_t::uint128_t(const long long& num)
{
	this->words[0] = num;
	if (num < 0) this->words[1] = -1;
}

uint128_t::uint128_t(const unsigned int& num)
{
	this->words[0] = num;
}

uint128_t::uint128_t(const int& num)
{
	this->words[0] = num;
	if (num < 0) this->words[1] = -1;
}

uint128_t::uint128_t(const uint128_t& num)
{
	*this = num;
}

uint128_t& uint128_t::operator+=(const uint128_t& num)
{
	//Simply add the first word of each number together and see if they overflow.
	//Add the second words together and the overflow from the first addition if necessary.
	//Overflow of the second word isn't handled by design.
	this->words[0] += num.words[0];
	this->words[1] += num.words[1];
	this->words[1] += (this->words[0] < num.words[0]); //if overflow has occured than words[0] will go down instead of up

	return *this;
}

uint128_t operator+(const uint128_t& num1, const uint128_t& num2)
{
	uint128_t num1_copy = num1;
	num1_copy += num2;
	return num1_copy;
}

uint128_t& uint128_t::operator-=(const uint128_t& num)
{
	//Similar to addition, but the opposite.
	this->words[1] -= (this->words[0] < num.words[0]); //if overflow has occured than words[0] will go down instead of up
	this->words[0] -= num.words[0];
	this->words[1] -= num.words[1];

	return *this;
}

uint128_t operator-(const uint128_t& num1, const uint128_t& num2)
{
	uint128_t num1_copy = num1;
	num1_copy -= num2;
	return num1_copy;
}

uint128_t& uint128_t::operator*=(const uint128_t& num)
{

	//First handle any trivial cases
	if (*this == 0 || num == 0) *this = 0;
	else if (*this == 1) *this = num;
	else if (num != 1)
	{
		//To multiply the two numbers together they get broken up into their four separate 64-bit
		//words (A, B, C and D). If the current number is represented as AB and "num" is represented as CD,
		//then their multiplication would equal (AC << 128) + ((AD + BC) << 64) + BD. The multiplication
		//of AC can be excluded as it will be shifted outside of the 128-bit number. Likewise,
		//the most significant 64-bits from the multiplications AD and BC will also get shifted outside
		//of the 128-bit number while the least significant bits will occupy slots 64-127 of the 128-bit
		//number. This means that these multiplications can be carried out as standard 64-bit
		//multiplications as we don't care about the overflow from them. The only multiplication where all
		//bits must be preserved is BD. To ensure that none of the bits are lost we break B and D up
		//into smaller 32-bit words and carry out a manual multiplication.
		
		//First we multiply AD and BC and store the results in the most significant word of the uin128_t
		this->words[1] = (this->words[0] * num.words[1]) + (this->words[1] * num.words[0]);

		//Next we split up the multiplication BD into 32-bit words ab and cd. We need to add
		//(ac << 64) + ((ad + bc) << 32) + bd to our current answer.
		unsigned long long a = this->words[0] >> 32, b = this->words[0] & 0xFFFFFFFF;
		unsigned long long c = num.words[0] >> 32, d = num.words[0] & 0xFFFFFFFF;

		//First add ac and bd because they can be added completely to the 128-bit answer without
		//the need for any shifting.
		this->words[0] = b * d;
		this->words[1] += a * c;

		//The multiplications of ad and bc need to get shifted by 32-bits. This means that the
		//most significant 32-bits of the multiplication can be added to the most
		//significant word of the 128-bit number, while the least significant 32-bit of the 
		//multiplication will form the basis for the least significant word of the 128-bit
		//number
		unsigned long long ad = a * d, bc = b * c;
		this->words[1] += (ad >> 32); //This addition may overflow this->words[1] but that's ok
		this->words[1] += (bc >> 32); //This addition may overflow this->words[1] but that's ok

		//Adding the less significant 32-bits of ad and bc to the more significant 32-bits of
		//this->words[0] can cause overflow, so the uint128_t += operator must be used for these
		//additions
		*this += (ad << 32);
		*this += (bc << 32);
	}

	return *this;
}

uint128_t operator*(const uint128_t& num1, const uint128_t& num2)
{
	uint128_t num1_copy = num1;
	num1_copy *= num2;
	return num1_copy;
}

uint128_t& uint128_t::operator/=(const uint128_t& num)
{
	//First check for trivial divisions
	if (num > *this || *this == 0) *this = 0;
	else if (num == *this) *this = 1;
	else if (this->words[1] == 0 && num.words[1] == 0) this->words[0] /= num.words[0];
	else
	{
		*this = bitwiseDivision(*this, num).first;
	}

	return *this;
}

uint128_t operator/(const uint128_t& num1, const uint128_t& num2)
{
	uint128_t num1_copy = num1;
	num1_copy /= num2;
	return num1_copy;
}

uint128_t& uint128_t::operator%=(const uint128_t& num)
{
	//First check for trivial divisions
	if (num > *this)
	{
		//intentionally blank, mod division by a larger number
		//simply yields the current number
	}
	else if (num == *this) *this = 0;
	else if (this->words[1] == 0 && num.words[1] == 0) this->words[0] %= num.words[0];
	else
	{
		*this = bitwiseDivision(*this, num).second;
	}

	return *this;
}

uint128_t operator%(const uint128_t& num1, const uint128_t& num2)
{
	uint128_t num1_copy = num1;
	num1_copy %= num2;
	return num1_copy;
}

uint128_t& uint128_t::operator<<=(const unsigned int n)
{
	//Before performing any kind of shifting, modulus divide
	//the given modulus by 128 as we can't shift by numbers
	//>= 128 or less than 0. Since 128 is a power of 2 the
	//modulus is simple to carry out.
	unsigned int shift = n & 127;

	if (shift >= 64)
	{
		//In this case the most significant word is replaced by a shifted version
		//of the least significant word and the least significant word becomes 0.
		this->words[1] = (this->words[0] << (shift - 64));
		this->words[0] = 0;
	}
	else
	{
		//Left shift the most significant word first
		this->words[1] <<= shift;

		//Take the 'shift' most significant bits of the lesser word and
		//add them to the back of the greater word
		this->words[1] |= (this->words[0] >> (64 - shift));

		//Finally, shift the least significant word by 'shift'
		this->words[0] <<= shift;
	}

	return *this;
}

uint128_t operator<<(const uint128_t& num, const unsigned int n)
{
	//Define the left shift operator in terms of the already defined <<= operator
	uint128_t num_copy = num;
	num_copy <<= n;
	return num_copy;
}

uint128_t& uint128_t::operator>>=(const unsigned int n)
{
	//Before performing any kind of shifting, modulus divide
	//the given modulus by 128 as we can't shift by numbers
	//>= 128 or less than 0. Since 128 is a power of 2 the
	//modulus is simple to carry out.
	unsigned int shift = n & 127;

	if (shift >= 64)
	{
		//In this case the least significant word is replaced by a shifted version
		//of the most significant word and the most significant word becomes 0.
		this->words[0] = (this->words[1] >> (shift - 64));
		this->words[1] = 0;
	}
	else
	{
		//right shift the leaast significant word first
		this->words[0] >>= shift;

		//Take the 'shift' least significant bits of the greater word and
		//add them to the front of the lesser word
		this->words[0] |= (this->words[1] << (64 - shift));

		//Finally, shift the most significant word by 'shift'
		this->words[1] >>= shift;
	}

	return *this;
}

uint128_t operator>>(const uint128_t& num, const unsigned int n)
{
	//Define the right shift operator in terms of the already defined >>= operator
	uint128_t num_copy = num;
	num_copy >>= n;
	return num_copy;
}

uint128_t& uint128_t::operator|=(const uint128_t& num)
{
	//Simply carry out the |= operation on each of the words of the 128-bit number
	this->words[0] |= num.words[0];
	this->words[1] |= num.words[1];

	return *this;
}

uint128_t operator|(const uint128_t& num1, const uint128_t& num2)
{
	//Define the OR operator in terms of the already defined >>= operator
	uint128_t num_copy = num1;
	num_copy |= num2;
	return num_copy;
}

uint128_t& uint128_t::operator&=(const uint128_t& num)
{
	//Simply carry out the &= operation on each of the words of the 128-bit number
	this->words[0] &= num.words[0];
	this->words[1] &= num.words[1];

	return *this;
}

uint128_t operator&(const uint128_t& num1, const uint128_t& num2)
{
	//Define the AND operator in terms of the already defined >>= operator
	uint128_t num_copy = num1;
	num_copy &= num2;
	return num_copy;
}

uint128_t& uint128_t::operator^=(const uint128_t& num)
{
	//Simply carry out the ^= operation on each of the words of the 128-bit number
	this->words[0] ^= num.words[0];
	this->words[1] ^= num.words[1];

	return *this;
}

uint128_t operator^(const uint128_t& num1, const uint128_t& num2)
{
	//Define the XOR operator in terms of the already defined >>= operator
	uint128_t num_copy = num1;
	num_copy ^= num2;
	return num_copy;
}

uint128_t operator~(const uint128_t& num1)
{
	//Create a copy of this number and call the ~ operator on each word
	uint128_t num_copy = num1;
	num_copy.getWords()[0] = ~num1.words[0];
	num_copy.getWords()[1] = ~num1.words[1];
	return num_copy;
}

bool operator==(const uint128_t& num1, const uint128_t& num2)
{
	return (num1.words[0] == num2.words[0]) && (num1.words[1] == num2.words[1]);
}

bool operator!=(const uint128_t& num1, const uint128_t& num2)
{
	return !operator==(num1, num2);
}

bool operator<(const uint128_t& num1, const uint128_t& num2)
{
	//This operator will return true if the most significant word of the first number
	//is less than the most significant word of the second number. If the most signifiant
	//words are the same, then this operator will return true if the least significant word
	//of the first number is less than the least significant word of the second number. If 
	//neither of these conditions are met the operator returns false.
	return (num1.words[1] < num2.words[1]) ||
		((num1.words[1] == num2.words[1]) && (num1.words[0] < num2.words[0]));
}

bool operator>(const uint128_t& num1, const uint128_t& num2)
{
	//This operator will return true if the most significant word of the first number
	//is greater than the most significant word of the second number. If the most signifiant
	//words are the same, then this operator will return true if the least significant word
	//of the first number is greater than the least significant word of the second number. If 
	//neither of these conditions are met the operator returns false.
	return (num1.words[1] > num2.words[1]) ||
		((num1.words[1] == num2.words[1]) && (num1.words[0] > num2.words[0]));
}

bool operator<=(const uint128_t& num1, const uint128_t& num2)
{
	//Simply return the opposite of the > operator
	return !operator>(num1, num2);
}

bool operator>=(const uint128_t& num1, const uint128_t& num2)
{
	//Simply return the opposite of the < operator
	return !operator<(num1, num2);
}

uint128_t& uint128_t::operator=(const uint128_t& num)
{
	//check for mistaken self assignment
	if (this == &num) return *this;

	//Set both words to be equal to the passed in number
	this->words[0] = num.words[0];
	this->words[1] = num.words[1];
	return *this;
}

uint128_t uint128_t::Karatsuba64Bit(const unsigned long long& num1, const unsigned long long& num2)
{
	//This method multiplies two unsigned 64-bit numbers together to get an unsigned 128-bit number.
	//Multiplying two 64-bit numbers together would normally result in overflow of some kind, but
	//using this method prevents it by turning a single multiplication into three smaller multiplications,
	//four additions, two subtractions and one bit shift.

	//First separate the input numbers into two 32-bit numbers each and store them in signed
	//64-bit containers
	long long a = num1 >> 32, b = num1 & 0xFFFFFFFF, c = num2 >> 32, d = num2 & 0xFFFFFFFF;

	//Multiply A*C and B*D and cast the results into unsigned integers to get
	//the correct positive values for Z2 and Z0. The reason that we get the correct positive
	//values is that the binary representations for A, B, C and D don't change when placed into
	//signed or unsigned integers, only the meaning changes (i.e. 0b1111 means 15 when unsigned
	//and -1 when signed).
	unsigned long long Z2 = a * c, Z0 = b * d;

	//Create Z1 as an unsigned 128-bit integer by multiplying (B - A) and (C - D). If this 
	//multiplication results in a negative 64-bit number than any necessary leading 1 bits
	//are automatically added to Z1 due to how signed integers are casted into uint128_t.
	//This is the reason why A,B,C and D were created as signed integers as opposed to unsigned ones.
	long long ba = b - a, cd = c - d;
	long long bacd = ba * cd;

	uint128_t Z1;
	if ((ba & 0x8000000000000000) == (cd & 0x8000000000000000) && (bacd & 0x8000000000000000))
	{
		Z1 = unsigned long long((b - a) * (c - d));
	}
	else if ((ba & 0x8000000000000000) != (cd & 0x8000000000000000) && !(bacd & 0x8000000000000000))
	{
		Z1 = (b - a) * (c - d);
		if (bacd != 0) Z1.getWords()[1] |= 0xFFFFFFFFFFFFFFFF;
	}
	else
	{
		Z1 = (b - a) * (c - d);
	}

	//Adding Z2 and Z0 together could cause overflow since they are both positive 64-bit numbers.
	//Add them to Z1 individually as the uint128_t class can handle 64-bit additions without overflow.
	//Once Z2 and Z0 are added, Z1 will be guaranteed to be a positive number instead of a positive
	//representation of a negative number (i.e. it's lead bit will be a 0).
	Z1 += Z2;
	Z1 += Z0;

	//Right shift Z1 by 32 and then add Z0
	Z1 <<= 32;
	Z1 += Z0;

	//At this point we have Z1 = ((B-A)*(C-D) + Z2 + Z0) << 32 + Z0. The only thing missing to 
	//complete the Karatsuba multiplication is adding Z2 << 64. Since Z2 is a maximum of 64-bits
	//we simply add it to the most significant words of the 128-bit Z1 to get our final answer.
	Z1.getWords()[1] += Z2;

	return Z1;
}

std::pair<uint128_t, uint128_t> uint128_t::bitwiseDivision(const uint128_t& dividend, const uint128_t& divisor)
{
	if (divisor == 0) {
		throw std::invalid_argument("Divisor cannot be zero.");
	}

	uint128_t quotient = 0;
	uint128_t remainder = 0;
	uint128_t quotientOR = 1;

	// Determine the number of bits needed to represent the dividend
	int bitLength = 0;
	uint128_t temp = dividend;
	while (temp > 0) {
		temp >>= 1;
		++bitLength;
	}

	// Perform the division
	for (int i = bitLength - 1; i >= 0; --i) {
		// Shift remainder to the left and bring down the next bit from dividend
		remainder = (remainder << 1) | ((dividend >> i) & 1);

		if (remainder >= divisor) {
			remainder -= divisor;
			quotient |= (quotientOR << i);
		}
	}

	return { quotient, remainder };
}

//std::string uint128_t::getNumberString()
//{
//	/*if (this->words[1] == 0)
//	{
//		return std::to_string(this->words[1]);
//	}*/
//
//	unsigned long long base = 10000000000000000000ULL; // 10^19, fits within 64-bit
//	unsigned long long parts[4] = { this->words[0] % base, this->words[0] / base, this->words[1] % base, this->words[1] / base };
//	unsigned long long carry = 0;
//
//	/*for (int i = 1; i <= 2; i++) {
//		parts[i] += parts[i + 1] % base * base;
//		parts[i + 1] /= base;
//	}*/
//
//	for (int i = 1; i <= 2; ++i) {
//		parts[i] += carry;
//		if (parts[i] >= base) {
//			carry = parts[i] / base;
//			parts[i] %= base;
//		}
//		else {
//			carry = 0;
//		}
//	}
//	parts[3] += carry;
//
//	std::string result;
//	for (int i = 3; i >= 0; --i) {
//		if (!result.empty() || parts[i] > 0) {
//			std::string partStr = std::to_string(parts[i]);
//			if (!result.empty()) {
//				result.append(19 - partStr.length(), '0');
//			}
//			result.append(partStr);
//		}
//	}
//
//	return result.empty() ? "0" : result;
//}
//
//std::ostream& operator<<(std::ostream& os, const uint128_t& num)
//{
//	uint128_t num_copy = num;
//	std::string number_string = num_copy.getNumberString();
//	return os << number_string;
//}