#include <iostream>
#include <vector>
#include <string>

//The BigInt_128 class is my attempt at making a 128 bit integer type. Lot's of Project
//Euler problems work with large numbers so it's useful to have a way to deal with them.
//I had previously created a class called BigInt which would store large numbers as a 
//text string and then convert each character to a digit when it was time to do math, but
//that method is too slow. I realized that I can just make an array of two consecutive
//unsigned long long integers and use already established function to just manipulate those.

//This class is good for numbers up to (2^128) - 1, which includes all 39 digit numbers. If
//work is going to be done on larger numbers then the BigNum class will need to be used.

//Time Comparisons
//----------------
//Addition: 700x faster than BigInt
//Multiplication: 2,200x faster than BigInt

class BigInt_128
{
public:
	BigInt_128() = default;
	BigInt_128(int number);
	BigInt_128(long long number);
	BigInt_128(std::string number);

	//Arithmetic Functions
	void Add(BigInt_128 num); //adds num to the current value of the BigNumber
	void Multiply(long long num); //multiplies the BigInt_128 by num

	//Printing Functions
	void print(); //prints out the current number in a format that makes it look like a normal number

	//variables
	uint64_t digits[2] = { 0 }; //a 128 bit number can be expressed as two 64 bit numbers right next to eachother
private:
	
	//int current_length = 0; //keeps track of how many digits long the BigNumber is
};

//Helper Functions
void multiplyArrayByTwo(uint8_t* numbers, int elements);
void divideArrayByTwo(uint8_t* numbers, int elements);
void addArrays(uint8_t* one, uint8_t* two, int elements);
void subtractArrays(uint8_t* one, uint8_t* two, int elements);
bool compareArraySize(uint8_t* one, uint8_t* two, uint8_t elements);
void printArray(uint8_t* arr, uint8_t elements);