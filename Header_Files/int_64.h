#pragma once

#include <iostream>
#include <vector>
#include <string>

//The int_64 class is my attempt at making an integer type of arbitrary length. This
//is accomplished by stringing together nodes which each contain an unsigned long long
//as well as a pointer to another node. By stringing together multiple 64-bit numbers
//it's possible to make numbers that are 128-bit, 256-bit, etc. up to any arbitrary
//size. This class utilizes built in functions to manipulate these large numbers as
//much as possible and then employs custom algorithms to handle certain things that the
//built in arithmetic functions can't, like calculating the carry between additions. The
//goal of this class was to, first and foremost, learn about overloading operators for a
//custom class, but also to make a type for C++ that can handle large numbers ala Python.
//I've come across numerous libraries online with this goal, however, most of the one's
//that I've found either use a vector of integers to represent long numbers or a string
//to represent long numbers. The vector of integers method can be quite space intensive
//(a 500 digit number for example would need 32-bits * 500 digits = 2kB of storage) while
//the string method takes a very long time to carry out calculations as each character
//needs to be converted to an integer before it can be manipulated. This class will save
//on both space and time by keeping large numbers in a binary representation and splitting
//the number up between multiple 64-bit words (for example the number 9,223,372,036,854,775,808
//would be represented as [0x0000000000000001], [0x0000000000000000]).

//The int_64 class is my attempt at making an integer type of arbitrary length. This
//is accomplished by stringing together multiple (although not necessarily consecutive)
//unsigned long long int types together to make a single cohesive number. Instead of
//adding an entire int_64 to another int_64 type, the individual unsigned long longs
//that make up each number are manipulated individually and then are brought back 
//together to form a new int_64 type

//This class will be able to handle numbers of any size as long as it fits in the user's
//memory.

//Time Comparisons
//----------------

class int_64
{
public:
	//Digits holds the binary equivalent of our number. In order to acheive an arbitrary length a vector is utilized.
	//The digits are in Big Endian format, however, the 64-bit word order is reversed. So a 192 bit word would be ordered like so (where
	//0 is the LSB and 191 is the MSB):
	//[63, 62, 61, ... 0], [127, 126, 125 ... 64], [191, 190, 189, ... 128]
	std::vector<unsigned long long> digits;

	int_64() = default;  //digits will be vector default initialized
	//~int_64(); //just let the vector handle it's own destruction

	int_64(int number); //create a new int_64 type from a signed integer
	int_64(unsigned int number); //create a new int_64 type from an unsigned integer
	int_64(long long number); //create a new int_64 type from a signed long long integer
	int_64(unsigned long long number); //create a new int_64 type from an unsigned long long integer
	int_64(unsigned long long number, int left_shift); //create a new int_64 type by left shifting the given unsigned long long
	int_64(std::string number); //create a new int_64 type via a string of numbers, the best way for instantiating a long variable
	int_64(int zero, int number_longs); //create a new int_64 type that has "number_longs" long longs all initialized to 0
	int_64(const int_64& num); //Copy constructor

	//std::vector<unsigned long long*> getDigits();

	//Arithmetic Operators
	int_64& operator+=(const int_64& num); //TODO: Do what's necessary to change "num" to a const
	int_64& operator+=(const unsigned long long& num);
	int_64& operator-=(const int_64& num); //TODO: Do what's necessary to change "num" to a const
	int_64& operator-=(const unsigned long long& num);
	int_64& operator*=(int_64& num);
	//int_64& operator*=(const int_64& num);
	int_64& operator*=(int num);
	int_64& operator/=(int_64& num);
	//int_64& operator/=(const unsigned long long& num); //TODO: Come back to this at some point
	int_64 operator/(int_64& num);

	//Binary Operators
	//TODO: I'm not complete satisfied with the <<= operater, see if it can be reduced at all. Maybe only focus on positive numbers
	int_64& operator<<=(const unsigned int left_shift);
	int_64& operator>>=(const unsigned int left_shift);
	int_64& operator |= (int_64& num);

	int_64& operator=(const int_64& num);

	std::string getNumberString();

	void partialAddition(unsigned long long num, int word); //make this private after testing

	//Helper Functions and Operators
	bool CompareArraySize(int* one, int* two, int elements);
	void MultiplyArrayByTwo(int* numbers, int elements);
	void DivideArrayByTwo(int* numbers, int elements);
	void AddArrays(int* one, int* two, int elements);
	void SubtractArrays(int* one, int* two, int elements);
	//void PrintBinary(int_64& num);
	void PrintBinary(int_64 num);
	void twosComplement(int_64& num);
	int_64 flipBits(int_64& num);
	int fastlog2(unsigned long long value);
	int GetLeadBitLocation(int_64& num);
private:
	void zero();

	
};



//Can print int_64 types by reference or copy
std::ostream& operator<<(std::ostream& os, int_64& num);

//Addition operators allow for addition of int_64 types with unsigned or signed long long and ints
//as well as other int_64 types with implicit conversion
//TODO: It would be great if all of these could be combined into a single function, look int this at some point
//int_64 operator+(int_64& num1, int_64& num2);

//Friendly Arithmetic Operators
int_64 operator+(int_64 num1, const int_64& num2);
int_64 operator-(int_64 num1, int_64& num2);
int_64 operator-(int_64 num1, int& num2);
int_64 operator*(int_64 num1, int_64& num2);
int_64 operator*(int_64 num1, int num2);

//Friendly Binary Operators
int_64 operator<<(int_64& num, const unsigned int left_shift);
int_64 operator>>(int_64& num, const unsigned int right_shift);
int_64 operator|(int_64 num1, int_64& num2);

//Friendly Comparison Operators
bool operator==(const int_64& num1, const int_64& num2);
bool operator!=(const int_64& num1, const int_64& num2);
bool operator<(const int_64& num1, const int_64& num2);
bool operator>(const int_64& num1, const int_64& num2);
bool operator<=(const int_64& num1, const int_64& num2);
bool operator>=(const int_64& num1, const int_64& num2);

//NOTES
//Currently to create and destroy an instance from a built-int types takes 9.31e-8 seconds, creating 1,000,000 takes 0.093 seconds
//1,000,000 normal built in types can be created in 3e-7 seconds. It probably is so much longer for the int_64's because there's a
//vector which needs to be created and deleted. Vectors are 24 bytes where's the maximum built-in types is only 8 bytes so I'd expect
//it to take ~4 times longer to create and destroy the int_64 type based on number of things getting created. The 0.093 seconds feels
//like it's longer than it should be currently. Maybe in the future I wont use a vector and just link all of the 64-bit words of the
//int_64 type with pointers in a linked list fashion.

//Currently to create and destroy an instance from a string takes 1.61e-6 seconds, creating 1,000,000 takes 1.61 seconds. This is
//obviously much slower than creating from one of the built in types but that's to be expected as there's a lot more entailed in
//doing it this way. This method is really only intended for creating instances for numbers that are already really large which isn't
//possible with a built in type. I can probably make it more efficient to create int_64 types in this manner, however, the object
//is less about creating large numbers from the get-go and more about building up to large numbers through arithmetic.

//I'll need to find a more efficient way of creating and destroying instances of this class at some point