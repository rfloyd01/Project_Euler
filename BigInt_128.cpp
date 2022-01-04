#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/BigInt_128.h>
#include <iostream>

BigInt_128::BigInt_128(int number)
{
	digits[0] = (long long)number;
}

BigInt_128::BigInt_128(long long number)
{
	digits[0] = number;
}

BigInt_128::BigInt_128(std::string number)
{
	//this will be the main way to create BigInt_128 types. First store digits of number in an array. Then create a different array
	//which will hold powers of two. Keep multiplying the two array by two until it's larger than the number held in the other array.
	//From here, keep dividing the power of two array by 2 until it's smaller than the number array and then subtract the two array from
	//the number array. The BigInt_128 will need to be built bit-by-bit in this fashion. For now this is the quickest way I can think of
	//to construct a binary representation of a number from a string of characters.

	if (number.length() > 40)
	{
		std::cout << "Number is too big to fit into a BigInt_128 type. Removing some digits." << std::endl;
		for (uint8_t i = number.length() - 1; i >= 40; i--) number.erase(number.back());
	}


	uint8_t base_ten[40] = { 0 }, two_power[40] = { 0 }; two_power[0] = 1; //start with 2^0 = 1
	uint8_t power = 0;
	for (uint8_t i = number.length(); i > 0; i--) base_ten[number.length() - i] = number[i - 1] - 48; //remove 48 to get from character representation of digit to actual number

	//multiply two_power by 2 until it is larger than the number now stored in base_ten
	while (!compareArraySize(two_power, base_ten, 40))
	{
		multiplyArrayByTwo(two_power, 40);
		power++;
	}

	//for (int i = 0; i < 11; i++) std::cout << (int)base_ten[i];
	//std::cout << std::endl;
	
	//as soon as two_power becomes larger than base_ten divide it by two and reduce the power count by 1
	divideArrayByTwo(two_power, 40);
	power--;

	//for (int i = 0; i < 11; i++) std::cout << (int)two_power[i];
	//std::cout << std::endl;
	//std::cout << (int)power << std::endl;

	//now subtract two_power from base_ten and set the appropriate bit in digits. Digits[1] will be covered from power = 127 to 64
	//Digits[0] will be covered from power = 63 to 0
	unsigned long long num = 0, placement;
	if (power >= 64)
	{
		placement = (unsigned long long)1 << (power - 64);
		while (power >= 64)
		{
			if (!compareArraySize(two_power, base_ten, 40)) //two_power is smaller than base_ten
			{
				//printArray(base_ten, 40);
				//printArray(two_power, 40);
				subtractArrays(base_ten, two_power, 40);
				//printArray(base_ten, 40);
				num |= placement;
			}
			divideArrayByTwo(two_power, 40);
			placement >>= 1;
			power--;
		}
	}

	digits[1] = num;
	num = 0;
	placement = (unsigned long long)1 << power;
	uint8_t compare[40] = { 0 }; //used to know when we should stop

	while (placement > 0) //keeping going until base_ten is no longer larger than 0
	{
		if (!compareArraySize(two_power, base_ten, 40)) //two_power is smaller than base_ten
		{
			subtractArrays(base_ten, two_power, 40);
			num |= placement;
		}
		divideArrayByTwo(two_power, 40);
		placement >>= 1;
		power--;
	}

	digits[0] = num; //we have now created the 128-bit number from the original string
	//std::cout << '[' << digits[0] << ", " << digits[1] << ']' << std::endl;
}

//Arithmetic Functions
void BigInt_128::Add(BigInt_128 num)
{
	//will add together 64-bit numbers together as normal, I don't think I can come up with any kind of quicker addition algorithm then what
	//already exists. After adding I check to see if the first 64 bits have gotten smaller than their original size. This is only possible if
	//there's been an overflow. In addition a maximum of 1 bit can overflow, so in this case just add 1 to the next 64 bit number and do addition
	//as normal.
	 
	long long temp = digits[0];
	digits[0] += num.digits[0];
	if (digits[0] < temp) digits[1] += 1; //this means that there was an overflow

	digits[1] += num.digits[1];
}

void BigInt_128::Multiply(long long num)
{
	//I'm sure I can think of a much better way to do this but for now, just add the BigInt_128 to itself by num amount of times
	BigInt_128 temp;
	temp.digits[0] = digits[0];
	temp.digits[1] = digits[1];

	for (long long i = 0; i < num - 1; i++) this->Add(temp);
}

//Printing Functions
void BigInt_128::print()
{
	//I can't really think of a good way to convert the current binary representation of the number to base 10 without doing a lot of addition
	//because of this it won't be super efficient to print out a number, however, all mathematics can still happen in binary form so at least that
	//will be quick

	//if there aren't any bits in the second half of number then just print the first half normally as the compiler can handle 64-bit operatiuons
	if ((digits[1] == 0))
	{
		std::cout << digits[0] << std::endl;
		return;
	}
	
	//2^128 is at its largest a 39 digit number, so create a temporary array to store each number that needs to be printed
    //also, once we get to numbers greater than 2^64, the physical digits of 2^x will also need to be saved so that we
    //know what to multiply by
	uint8_t base_ten[40] = { 0 }, two_power[40] = { 0 }, count = 0;
	unsigned long long holder = 0;
	two_power[0] = 1; //2^0 == 1
	unsigned long long location = 0x1, current_power = 1; //current_power is the same thing as two_power but in number form
	uint8_t counter = 0;
	while (counter < 64)
	{
		if (holder >= digits[0]) break; //no need to keep incrementing when we're done with the first part of digits
		if (location & digits[0])
		{
			addArrays(base_ten, two_power, 40);
			holder += current_power;
		}
		multiplyArrayByTwo(two_power, 40);
		location <<= 1;
		current_power *= 2; //since digits[0] can't be larger than 2^64 we don't have to worry about overflow
		counter++;
	}

	if (digits[1]) //only do this next loop if the BigInt_128 is 2^64 or larger
	{
		current_power = 1; //no longer matches two_power but can be used to figure out when to break out of loop still
		location = 0x1; //reset this tracking variable
		holder = 0; //reset this counting variable
		while (counter < 64) //in case two_power hasn't been incremented all the way up to 2^64 yet
		{
			multiplyArrayByTwo(two_power, 40);
			counter++;
		}
		while (holder < digits[1])
		{
			if (location & digits[1])
			{
				addArrays(base_ten, two_power, 40);
				holder += current_power;
			}
			multiplyArrayByTwo(two_power, 40);
			location <<= 1;
			current_power *= 2; //since digits[0] can't be larger than 2^64 we don't have to worry about overflow
		}
	}
	//once all of the decimal digits are set, start from the back of the number array and print out each digit, starting with the first non-zero digit
	bool zeros_broken = false;
	for (uint8_t i = 40; i > 0; i--)
	{
		if (base_ten[i - 1])
		{
			zeros_broken = true;
			std::cout << (int)base_ten[i - 1];
		}
		else
		{
			if (zeros_broken) std::cout << (int)base_ten[i - 1];
		}
	}

	//a very long winded way of printing out a number but currently not sure what other choice I have
}

void multiplyArrayByTwo(uint8_t* numbers, int elements)
{
	//takes an array of uint8_t and multiplies every element by 2, if there's any carry it gets added to the next digit
	//this function is essentially for multiplying very large numbers by 2. When multiplying by 2 the carry should never
	//get larger than 1 because 2 * 9 = 18 + 1 = 19 / 10 = 1
	uint8_t carry = 0;
	for (uint8_t i = 0; i < elements; i++)
	{
		*(numbers + i) *= 2;
		*(numbers + i) += carry;
		if (*(numbers + i) >= 10)
		{
			*(numbers + i) %= 10;
			carry = 1;
		}
		else carry = 0;
	}
}
void divideArrayByTwo(uint8_t* numbers, int elements)
{
	//takes a large number represented by an array and divides it by 2
	//The array is just a representation of each base ten digit in the number with the least significant numbers first
	//i.e. the number 12345 would be stored in the following array [5, 4, 3, 2, 1]
	//similar to integer division, if the number isn't divisible by 2 then the function will return floor(n/2)
	uint8_t carry_num = 0, current_num, start_num = elements;

	//first calculate the starting number so that any leading zeros in the array are ignored
	for (uint8_t i = elements; i > 0; i--) //after 0 i will wrap back around to 255, stop at zero and shift i down by 1 inside loop
	{
		if (*(numbers + i - 1))
		{
			start_num = i;
			break;
		}
	}

	for (uint8_t i = start_num; i > 0; i--) //after 0 i will wrap back around to 255, stop at zero and shift i down by 1 inside loop
	{
		//std::cout << (int)*(numbers + i - 1);
		current_num = *(numbers + i - 1) + carry_num;
		if (current_num < 2)
		{
			*(numbers +  i - 1) = 0;
			carry_num = 10 * current_num;
		}
		else
		{
			if (current_num & 1) //number is odd
			{
				*(numbers + i - 1) = ((current_num - 1) / 2);
				carry_num = 10;
			}
			else //number is even
			{
				*(numbers + i - 1) = (current_num / 2);
				carry_num = 0;
			}
		}
	}

	//std::cout << std::endl;
	//printArray(numbers, elements);
	//Currently appears to be working properly
}

void addArrays(uint8_t* one, uint8_t* two, int elements)
{
	//takes two arrays of the same size and adds the digits of array two to array one
	//similar to the above multiplyArrayByTwo() function, the carry can't be larger than 1 because 9 + 9 = 18 + existing_carry = 19 / 10 = 1

	//first figure out if addition needs to stop early, i.e. the length of the array is greater than the actual number of elements in the array
	uint8_t end_point = elements;
	for (uint8_t i = elements; i > 0; i--)
	{
		if (*(one + i - 1) || *(two + i - 1)) //as soon as a non-zero is found that's our starting location
		{
			end_point = i;
			break;
		}
	}

	uint8_t carry = 0;
	for (uint8_t i = 0; i <= end_point; i++)
	{
		*(one + i) += *(two + i);
		*(one + i) += carry;
		if (*(one + i) >= 10)
		{
			*(one + i) %= 10;
			carry = 1;
		}
		else carry = 0;
	}
}
void subtractArrays(uint8_t* one, uint8_t* two, int elements)
{
	//takes two arrays of the same size and subtracts the digits of array two from array one
	uint8_t ending_point = elements;

	//first check and make sure that array one is larger than array two, if not then return without doing anything
	for (uint8_t i = elements; i > 0; i--)
	{
		if (*(one + i - 1) || *(two + i - 1)) //look for first non-zero elements
		{
			//std::cout << (int)(i-1) << std::endl;
			if (*(two + i - 1) > * (one + i - 1))
			{
				std::cout << "Array two must be smaller than array one." << std::endl;
				return;
			}
			else if (*(two + i - 1) < *(one + i - 1)) break;
		}
		else ending_point--;
	}

	for (uint8_t i = 0; i <= ending_point + 1; i++)
	{
		//std::cout << (int)i << std::endl;
		if (*(two + i) > *(one + i))
		{
			*(one + i) = 10 + *(one + i) - *(two + i);
			if (*(one + i + 1) == 0)
			{
				uint8_t j = 1;
				while (*(one + i + j) == 0)
				{
					if (j + i > ending_point) break;
					*(one + i + j) = 9; //make sure to turn any 0's into 10's otherwise it will wrap around to 255
					j++;
				}
				*(one + i + j) -= 1;
			}
			else *(one + i + 1) -= 1;
		}
		else *(one + i) -= *(two + i);
		//std::cout << (int)*(one + i) << std::endl;
	}
	//Everything seems to be working ok
}

bool compareArraySize(uint8_t* one, uint8_t* two, uint8_t elements)
{
	//returns true if the number represented by one is larger than two, returns false if it's equal or less than two
	uint8_t one_length = 0, two_length = 0;

	for (uint8_t i = elements; i > 0; i--)
	{
		if (one_length && two_length) break; //stop the count when the starting digit of each number is found
		if (!one_length && *(one + i - 1)) one_length = i;
		if (!two_length && *(two + i - 1)) two_length = i;
	}

	if (one_length > two_length) return true;
	else if (two_length > one_length) return false;
	else
	{
		for (uint8_t i = one_length; i > 0; i--)
		{
			if (*(one + i - 1) > * (two + i - 1)) return true;
			else if (*(two + i - 1) > * (one + i - 1)) return false;
		}
	}
	return false; //all digits have the same value
	//Appears to be working
}

void printArray(uint8_t* arr, uint8_t elements)
{
	//prints an array in normal order without any leading zeros
	bool zeros_broken = false;
	for (uint8_t i = elements; i > 0; i--)
	{
		if (*(arr + i - 1))
		{
			std::cout << (int)*(arr + i - 1);
			zeros_broken = true;
		}
		else
		{
			if (zeros_broken) std::cout << (int)*(arr + i - 1);
		}
	}

	if (!zeros_broken) std::cout << '0'; //the value of the array was 0 so nothing printed in above loop
	std::cout << std::endl;
}