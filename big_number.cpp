#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/big_number.h>
#include <iostream>

BigNumber::BigNumber(int number)
{
	uint8_t counter = 0; //keeps track of whether we're currently on the first or second half of a byte
	uint8_t digit_holder; //will hold individual digits from number

	while (number > 0)
	{
		if (counter & 1)
		{
			digit_holder = (digit_holder | ((number % 10) << 4));
			digits[counter / 2] = digit_holder;
		}
		else digit_holder = (number % 10);

		counter++;
		current_length++;
		number /= 10;
	}

	if (counter & 1) digits[counter / 2] = digit_holder; //make sure to add final digit of number of original number has odd number of digits
}

BigNumber::BigNumber(long long number)
{
	uint8_t counter = 0; //keeps track of whether we're currently on the first or second half of a byte
	uint8_t digit_holder; //will hold individual digits from number

	while (number > 0)
	{
		if (counter & 1)
		{
			digit_holder = (digit_holder | ((number % 10) << 4));
			digits[counter / 2] = digit_holder;
		}
		else digit_holder = (number % 10);

		counter++;
		current_length++;
		number /= 10;
	}

	if (counter & 1) digits[counter / 2] = digit_holder; //make sure to add final digit of number of original number has odd number of digits
}

BigNumber::BigNumber(std::string number)
{
	uint8_t counter = 0; //keeps track of whether we're currently on the first or second half of a byte
	uint8_t digit_holder = 0; //will hold individual digits from number

	int end = 0;
	for (int i = 0; i < number.length(); i++) //ignore any leading zeros fed into number string
		if (number[i] == '0') end++;

	for (int inc = number.length() - 1; inc >= end; inc--)
	{
		uint8_t digit = number[inc] - 48; //value of number characters is 48 more than their physical digit counter parts
		//std::cout << digit;
		//std::cout << (int)digit << std::endl;
		if (counter & 1)
		{
			digit_holder = (digit_holder | (digit << 4));
			digits[counter / 2] = digit_holder;
			//std::cout << (int)digit_holder << std::endl;
		}
		else digit_holder = digit;

		counter++;
		current_length++;
	}

	//std::cout << std::endl;
	if (counter & 1) digits[counter / 2] = (digit_holder | (number[0] << 4)); //make sure to add final digit of number of original number has odd number of digits
}
//Arithmetic Functions
/*
void BigNumber::Add(BigNumber num)
{
	std::vector<int> new_number = num.GetVec();

	if (new_number.size() > number.size())
	{
		int yo = new_number.size() - number.size();
		for (int i = 0; i < yo; i++) number.push_back(0);
	}
	else if (new_number.size() < number.size())
	{
		int yo = number.size() - new_number.size();
		for (int i = 0; i < yo; i++) new_number.push_back(0);
	}

	int carry = 0;

	for (int i = 0; i < number.size(); i++)
	{
		int digit = number[i] + new_number[i] + carry;
		if (digit >= 10)
		{
			carry = digit / 10;
			number[i] = digit % 10;
		}
		else
		{
			carry = 0;
			number[i] = digit;
		}
	}

	if (carry) number.push_back(carry);
	digits = number.size();
	UpdateNumberString();
}
*/

//Printing Functions
void BigNumber::print()
{
	uint8_t digits_to_print = current_length; //helps keep track of where in the number we are

	while (digits_to_print > 0)
	{
		if (digits_to_print & 1) std::cout << (digits[(digits_to_print - 1) / 2] & 0xF);
		else std::cout << ((digits[(digits_to_print - 1) / 2] & 0xF0) >> 4);
		digits_to_print--;
	}
	std::cout << std::endl;
}