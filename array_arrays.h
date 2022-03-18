#pragma once

#include <Header_Files/pch.h>

//Making a struct to handle an array of arrays, where the struct can hold any number of arrays, and each of these
//arrays in turn can hold a different amount of elements than the other arrays

struct arrayOfArrays
{
	int numberOfArrays;
	int* arrayLengths;
	int** overallArray;
};

std::pair<std::string, double> array_of_arrays()
{
	//ignore these lines, I use them in other header files
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//Let's say we want to make the multi-array { {1, 99}, {1, 2, 3, 4}, {1, 39, 50}}. It contains three arrays of length
	//2, 4 and 3 respectively. This can't be created using standard multi-array [] notation as this only creates things
	//of uniform size, i.e. int yeet[3][2][2] = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}, {{9, 10}, {11, 12}}}. To accomplish the 2, 4, 3 array
	//you would need an array of pointers, where each pointer points to an array of integers (which is also represented by a pointer,
	//I think you were barking up this tree earlier). The big caveat here though is that when using pointers, the code
	//has no way to tell how many elements are included in the array that the pointer is pointing to. This is why another
	//array would need to be kept which keeps track of the individual array lengths, here's a sample struct implementing
	//how this might work.

	//first you can define the original arrays
	int a1_1[2] = { 1, 99 };
	int a1_2[4] = { 1, 2, 3, 4 };
	int a1_3[3] = { 1, 39, 50 };

	//then create another array with pointers to these individual arrays
	int* combined_a1[3] = { a1_1, a1_2, a1_3 };

	//note, this above line is the same thing as typing: int* combined_a1[3] = {&a1_1[0], &a1_2[0], &a1_3[0]},
	//passing an array as an argument to a function is the same thing as passing a reference to the first element of
	//the array.

	//finally you'd need to create an array holding the sizes of these individual arrays to help prevent weird out of 
	//index things from happening. This is because when an array is passed to a function via pointer, the function doesn't
	//know how large the array actually is. Of course this isn't necessary, and with small and few enough arrays you can 
	//avoid errors just by remembering the array lengths and making for loops that are the right size, but things can
	//get complicated quickly.
	int a1_array_lengths[3] = { 2, 4, 3 };

	//Create an instance of the struct with all of these variables, struct is outlined above
	arrayOfArrays a1(3, a1_array_lengths, combined_a1);

	//Now the array containing an unknown number of arrays, all with different lenghts has been created (a1).
	//Let's say you wanted to print the value of all the arrays contained in here in order, you could do something
	//like shown here
	for (int i = 0; i < a1.numberOfArrays; i++)
	{
		for (int j = 0; j < *(a1.arrayLengths + i); j++)
		{
			//not sure how familiar you are with this, but the *() nomenclature is called de-referencing a pointer.
			//by taking the pointer to the arrayLengths array and adding 'i' to it we get the individual pointers store in
			//the arrayLengths array. We then dereference these pointers to get the actual length
			std::cout << *(*(a1.overallArray + i) + j) << ", ";
		}
		std::cout << std::endl;
	}

	//the above loop outputs the following because I got lazy with the commas:

	/*
	1, 99,
	1, 2, 3, 4,
	1, 39, 50,
	*/

	//in all honesty what I know about arrays and pointers may just be lacking and there could be a much much
	//easier way to do this but for now this is all I got. Things start getting pretty complicated when you need
	//to de-reference a pointer, which in turn yields a pointer that also needs to be de-reference (i.e.
	//this line in the print loop: std::cout << *(*(a1.overallArray + i) + j) << ", "; but it's definitely great
	//practice!

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)
}
