#pragma once

#include <vector>

//This header file includes implementations of useful algorithms (such as bubble sort, merge sort, binary serach, etc...). I'm using it 
//as a combination for practicing these algorithms (never know when a coding interview might be coming up) but also to use in Euler 
//problems. The number of times I've had to manually write a binary search function is too high. These functions are all templates so 
//there isn't an associated .cpp file for this header.

//SORTING FUNCTIONS
template <typename T>
void BubbleSort(std::vector<T>& list, bool asc = true)
{
	//THe bubble sort works by starting at the front of the vector and comparing two consecutive elements. Which ever element is 
	//larger will get shifted towards the back of the array. By swapping elements one at a time like this, the largest element 
	//will eventually "bubble" to the end of the vector. After one pass through the vector the largest element is guaranteed to be 
	//moved to the end. After a second pass the second largest element is guaranteed to be in the correct location, etc. etc.

	//In the worst case scenario, a vector of length n will have n * (n+1) / 2 comparisons and swaps making the runtime O(n^2).
	//The vector is sorted in place though, needing only a single temp variable to help swap elements, so the additionaly memory
	//requirement of this algorithm is O(1).

	//There is a speedup we can implement to make the algorithm more efficient (although it won't change the runtime in the 
	//worst case scenarios). We keep track of the index where the most recent swapping of variables occured. Since the largest elements 
	//bubble to the back of the array, if no swaps occur past a certain location we can be sure that everything beyond that location 
	//is already in order, so there's no reason to iterate beyond that point any longer. In a similar vein, we can break out of the 
	//algorithm entirely if the last swapped index is 0, which signifies that the vector is already sorted.

	int last_swap = list.size();
	T temp;

	while (last_swap != 0)
	{
		for (int i = 1; i < last_swap; i++)
		{
			if (list[i - 1] > list[i])
			{
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;
				last_swap = i;
			}
		}
	}
	
}