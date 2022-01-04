#include <iostream>
#include <vector>
#include <string>

//The BigNumber class is an attempt at making larger numbers by storing numbers in an array
//Every digit 0-9 can be represented with 4 bits 0000 - 1001 so the array will be of uint8_t
//type where each element in the array represents two individual digits of the number. The first
//digit in the array will represent the one's place while the last number in the array is the
//most significant.

//Other big integer type classes work by converting long numbers to strings of characters. This
//isn't efficient as converting each digit from a number into a character, and then back into 
//a number when arithmetic needs to happen on it can get quite time consuming. This class attempts
//to speed things up by skipping any conversion to a string while minimizing the amount of storage
//required.

class BigNumber
{
public:
	BigNumber(int number);
	BigNumber(long long number);
	BigNumber(std::string number);

	//Arithmetic Functions
	void Add(BigNumber num); //adds num to the current value of the BigNumber

	//Printing Functions
	void print(); //prints out the current number in a format that makes it look like a normal number

private:
	uint8_t digits[100] = { 0 }; //for now maxing out at 100 digit numbers. Can update this down the line if needed
	int current_length = 0; //keeps track of how many digits long the BigNumber is
};