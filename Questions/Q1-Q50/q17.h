#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Number letter counts
std::pair<std::string, double> q17()
{
	//This problem was more of a logic puzzle then a true programming problem, split it into a few smaller parts and just multipled words that get repeated many times

	auto run_time = std::chrono::steady_clock::now();

	std::vector<int> ones = { 0, 3, 3, 5, 4, 4, 3, 5, 5, 4 }; //corresponds to the number of letters in (zero), one, two, three, four, five, six, seven, eight and nine
	std::vector<int> teen = { 3, 6, 6, 8, 8, 7, 7, 9, 8, 8 }; //corresponds to the number of letters in ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen, eighteen and nineteen
	std::vector<int> tens = { 0, 0, 6, 6, 5, 5, 5, 7, 6, 6 }; //corresponds to the number of letters in (zero), (ten), twenty, thirty, forty, fifty, sixty, seventy, eighty and ninety
	std::vector<int> hund = { 7 }; //all of the hundreds are named the same but with a different prefix, so only need one value here
	std::vector<int> aand = { 3 }; //the word "and" will appear for every number in the hundreds other than the hundred itself (i.e. one hundred, one hundred AND one, two hundred, two hundred AND twenty)

	//first count all of the number-letters under 10, this value gets repeated many times between 1-1000
	int under_ten_sum = 0; for (int i = 1; i < ones.size(); i++) under_ten_sum += ones[i];

	//next count all of the number-letters from 10-19. Since the teens are named unconventionally they are counted separatley from the other two digit numbers
	int teen_sum = 0; for (int i = 0; i < teen.size(); i++) teen_sum += teen[i];

	//next count all of the two digit numbers over 19, i.e. twenty, thirty. Only worry about the numbers divisible by ten for now as the under ten sum will be added in the next step
	int tens_sum = 0; for (int i = 2; i < tens.size(); i++) tens_sum += tens[i];

	//next count the number-letters of everything under 100. First, the under_ten_sum should be used 9 times, 1-9, 21-29, 31-39, 41-49, 51-59, 61-69, 71-79, 81-89 and 91-99
	int under_onehundred_sum = (9 * under_ten_sum);
	//the teen_sum only needs to be used once
	under_onehundred_sum += teen_sum;
	//finally the tens_sum should be used 10 times because each of the tens numbers are said 10 times (i.e. twenty, twenty one, twenty two ... twenty nine ... ninety, ninety one, ... ninety nine)
	under_onehundred_sum += (10 * tens_sum);

	//next is to add up all of the hundreds. There's one hundred, two hundred ... nine hundred.
	int hundred_sum = under_ten_sum + (9 * hund[0]);
	//Each one of these phrases gets used 100 times, i.e. one hundred - one hundred and ninety nine, so multiply the hundred_sum by 100
	hundred_sum *= 100;

	//next is to add up the hundreds_sum and the under_hundred sum
	//the under one hundred pattern gets repeated everytime a new 100 is hit, so it should be used 9 times
	int under_onethousand_sum = hundred_sum + (10 * under_onehundred_sum);

	//the final step is to include the word "and". It appears in every number from 1-1000 except the first 99, 1000 and numbers divisible by 100. So "and" appears (1000 - 99 - 9 - 1) = 891 times
	int answer = 11; //"one thousand" has 11 letters
	answer += (under_onethousand_sum + (aand[0] * 891));
	//ran in 0.0000031 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 21124
	//ran in 0.0000031 seconds
}

//NOTES
//HACKERRANK UPDATE
//Here's the code I used to solve the HackerRank version of the problem:
/*
string groups[5] = {"", "Thousand", "Million", "Billion", "Trillion"};
string ones[10] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven" , "Eight", "Nine"};
string teens[10] = {"", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
string tens[10] = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

bool HundredPrint(std::vector<int> &digits, int location, bool &printed)
{
    bool this_print = false;
    //Handle the Hundred place first
    if (digits[location])
    {
        if (printed) std::cout << " ";
        std::cout << ones[digits[location]] << " Hundred";
        this_print = true;
        printed = true;
    }

    //Handle the Tens place second
    //check for teens place
    if (digits[location - 1] == 1)
    {
        if (printed || this_print) std::cout << " ";
        if (digits[location - 2]) std::cout << teens[digits[location - 2]];
        else std::cout << tens[1];
        this_print = true;
        printed = true;
        return this_print;
    }
    else if (digits[location - 1])
    {
        if (printed || this_print) std::cout << " ";
        std::cout << tens[digits[location - 1]];
        this_print = true;
        printed = true;
    }

    //Handle the Ones place last
    if (digits[location - 2])
    {
        if (printed || this_print) std::cout << " ";
        std::cout << ones[digits[location - 2]];
        this_print = true;
        printed = true;
        return this_print;
    }

    //need to add some code here, if nothing was printed inside this function
    //than printed needs to be set to false
    //if (!this_print) printed = false;

    return this_print;
}

int main()
{
    int number_trials;
    long long current_number;

    cin >> number_trials;
    for (int i = 0; i < number_trials; i++)
    {
        cin >> current_number;

        //Handle 0 separately
        if (!current_number)
        {
            std::cout << "Zero" << std::endl;
            continue;
        }

        //put each invidividual digit into a vector (it will be in reverse order)
        std::vector<int> digits;
        while (current_number > 0)
        {
            digits.push_back(current_number % 10);
            current_number /= 10;
        }

        //add zeros to the front if necessary
        int adder = digits.size() % 3;

        if (adder)
            for (int i = 0; i < (3 - adder); i++) digits.push_back(0);

        int max_group = digits.size() / 3 - 1;
        bool print_space = false;
        for (int i = max_group; i >= 0; i--)
        {
            //if (print_space) std::cout << " ";
            //print_space = false;
            if(HundredPrint(digits, i * 3 + 2, print_space))
               std::cout << " " << groups[i];
        }
        std::cout << std::endl;
    }

    return 0;
}
*/