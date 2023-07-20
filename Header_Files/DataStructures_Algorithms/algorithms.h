#pragma once

#include <vector>
#include <queue>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include "graph.h"

//This header file includes implementations of useful algorithms (such as bubble sort, merge sort, binary serach, etc...). I'm using it 
//as a combination for practicing these algorithms (never know when a coding interview might be coming up) but also to use in Euler 
//problems. The number of times I've had to manually write a binary search function is too high. These functions are all templates so 
//there isn't an associated .cpp file for this header.

//SORTING FUNCTIONS
template <typename T>
std::pair<int, int> BubbleSort(std::vector<T>& list, bool des = false)
{
	//The bubble sort works by starting at the front of the vector and comparing two consecutive elements. Which ever element is 
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

	//There's a boolean argument which defaults to false. If this is set to true then the sort will happen in descending order.
	
	//This method returns a pair of integers, one representing the total number of comparisons made and one representing the total 
	//number of swaps made. This is just for the purpose of comparing against other sorting algorithms.

	T temp;
	int last_swap = list.size(), comparisons = 0, swaps = 0;
	int flip = des; //convert bool to int to avoid multiple conversions

	while (last_swap != 1)
	{
		int end = last_swap;
		last_swap = 1;
		for (int i = 1; i < end; i++)
		{
			comparisons++;
			if (list[i - 1 + flip] > list[i - flip])
			{
				swaps++;
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;
				last_swap = i;
			}
		}
	}

	return { comparisons, swaps };
}

template <typename T>
std::pair<int, int> InsertionSort(std::vector<T>& list, bool des = false)
{
	//Like the bubble sort method, pairs of numbers are compared, although in this method the smaller numbers work towards the beginning
	//instead of the larger numbers working towards the end of the vector. This is achieved with two nested loops, the outer loop works
	//from the beginning of the vector to the end while the inner loop increments from the current location in the outer loop back towards 
	//the beginning. Any time the outer loop variable is less than the inner loop variable the switch places.

	//Each element will be compared to all of the elements less then it, so the total number of comparisons will be n*(n+1)/2 for a time 
	//complexity of O(n^2). This sorting technique is in-place and so has a memory complexity of O(1).

	//Like the bubble sort, we can terminate the sort early if no swaps occur in a given iteration. Any time no swap occurs in the inner loop 
	//we break out of it. Since the elements at the front are sorted, as soon as we find an element smaller than the one being tested we can 
	//guarantee that earlier elements will also be smaller. If the vector was to start sorted, there would still be n comparisons made.

	//There's a boolean argument which defaults to false. If this is set to true then the sort will happen in descending order.

	//This method returns a pair of integers, one representing the total number of comparisons made and one representing the total 
	//number of swaps made. This is just for the purpose of comparing against other sorting algorithms.

	T temp;
	int comparisons = 0, swaps = 0;
	int flip = des;

	for (int i = 1; i < list.size(); i++)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			comparisons++;
			if (list[j + 1 - flip] < list[j + flip])
			{
				swaps++;
				temp = list[j + 1];
				list[j + 1] = list[j];
				list[j] = temp;
			}
			else break;
		}
	}

	return { comparisons, swaps };
}

template <typename T>
std::pair<int, int> SelectionSort(std::vector<T>& list, bool des = false)
{
	//This is an interesting sort method. It involves iterating through the entire vector to find the largest value and then moving it 
	//to the back. You then iterate through all elements again (except the final one) and move the largest element to one spot away from 
	//the back. You keep going this way until the array is completely sorted. What's really interesting about this algorithm is that regardless 
	//of how in or out of order the vector starts at, it will require the exact same amount of comparisons and swaps to fully sort it. This 
	//means that a fully sorted array would take just as long to "sort" as a completely unsorted array.

	//This begs the question, what benefit even is there to this algorithm? The most important thing is that there will only be n swaps needed 
	//to completely sort the array (unlike insertion sort which may have n swaps on a single iteration [which occurs when the smallest element is 
	//located at the back of the vector]). If you had a vector full of really large objects then you would want to minimize the amount of swaps 
	//necessary as swapping requires copying an element into a temporary variable.

	//Another interesting aspect of this algorithm is that it's considered "unstable". That is, elements that have the same value can potentially 
	//switch relative positions due to the long blind swaps to the back of the array. Runtime for this algorithm is O(n^2) in all cases.

	//There's a boolean argument which defaults to false. If this is set to true then the sort will happen in descending order.

	//This method returns a pair of integers, one representing the total number of comparisons made and one representing the total 
	//number of swaps made. This is just for the purpose of comparing against other sorting algorithms.

	T temp;
	int comparisons = 0, swaps = 0;
	int flip = des;

	for (int i = list.size() - 1; i > 0; i--)
	{
		int largest_element_index = i;// , largest_element_value = list[i];
		for (int j = 0; j < i; j++)
		{
			comparisons++;
			if (list[j + flip * (largest_element_index - j)] > list[largest_element_index + flip * (j - largest_element_index)])
			{
				//largest_element_value = list[j];
				largest_element_index = j;
			}
		}

		//swap the largest index to the appropriate location in the vector
		swaps++;
		temp = list[i];
		list[i] = list[largest_element_index];
		list[largest_element_index] = temp;
	}

	return { comparisons, swaps };
}

template <typename T>
std::pair<int, int> CocktailShakerSort(std::vector<T>& list, bool des = false)
{
	//This algorithm is a variant of bubble sort. In each iteration of the loop there are two passes of the array, one that bubbles the largest
	//element to the back and one that bubbles the smallest element to the front. The runtimes for cocktail shaker are the same as bubble sort in all
	//instances, so what are the actual benefits? As an example, look at the array [2, 3, 4, 5, 6, 7, 8, 9, 10, 1]. If we used bubble sort on this
	//array, it would take 9 full iterations to get the 1 to bubble all the way to the front with a total of 55 comparisons being made along the way.
	//With cocktail shaker sort, we would only need a single iteration to bubble the 1 to the front with a total of only 17 comparisons (another half
	//iteration would take place to confirm that the array is sorted so really there would be 25 total comparisons). So, although the time complexities 
	//are the same as bubble sort, it can be seen that there are instances where Cocktail Shaker sort is more efficient.

	//Whereas bubble sort used a single variable to keep track of the position of the last swapped element, this algorithm has two variables that can be 
	//used to shrink the sort space.

	//There's a boolean argument which defaults to false. If this is set to true then the sort will happen in descending order.

	//This method returns a pair of integers, one representing the total number of comparisons made and one representing the total 
	//number of swaps made. This is just for the purpose of comparing against other sorting algorithms.

	T temp;
	int last_swap_low = 0, last_swap_high = list.size(), comparisons = 0, swaps = 0;
	int flip = des; //convert bool to int to avoid multiple conversions

	while (true)
	{
		//first bubble the largest element to the front of the vector
		int end = last_swap_high;
		bool swaps_made = false;
		for (int i = last_swap_low + 1; i < end; i++)
		{
			comparisons++;
			if (list[i - 1 + flip] > list[i - flip])
			{
				swaps++;
				swaps_made = true;
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;
				last_swap_high = i;
			}
		}

		//if no swaps took place, break out of the main loop
		if (!swaps_made) break;
		swaps_made = false;

		//now bubble to smallest element to the front of the array
		int start = last_swap_low;
		for (int i = last_swap_high - 1; i > start; i--)
		{
			comparisons++;
			if (list[i - flip] < list[i - 1 + flip])
			{
				swaps++;
				swaps_made = true;
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;
				last_swap_low = i;
			}
		}

		if (!swaps_made) break;
	}

	return { comparisons, swaps };
}

template <typename T>
void MergeSortHelper(std::vector<T>& list, int& comparisons, int& swaps, bool des = false)
{
	//This is a recursive sorting technique that uses a "divide and conquer" methodology.
	//Basically, the vector is cut in half and each half is sorted individiually. Once the two halves are 
	//sorted they then get merged back together. This cutting in half of the vector happens at each level 
	//of the recursion, until the sub-vector getting sorted either has a length of 0 or 1 which signifies 
	//the base of the recursion.

	//This technique allows for sorting of the overall vector with much less comparisons than the iterative 
	//techniques in the above functions. This does come at the cost of memory overhead though. Every time we 
	//cut the vector in half we need to make a copy of the sub-vectors to pass to the next level of the recursion.

	//Since this isn't an in-place sort there technically isn't any swapping that takes place, only overwritting 
	//of the existing elements. Because of this only the number of comparisons are kept track of.

	if (list.size() <= 1) return; //the base of the recursion, the list is guaranteed to be sorted here

	int split_index = list.size() / 2;
	std::vector<T> left(list.begin(), list.begin() + split_index);
	std::vector<T> right(list.begin() + split_index, list.end());

	//sort the two halves of the vector
	MergeSortHelper(left, comparisons, swaps, des);
	MergeSortHelper(right, comparisons, swaps, des);

	//recombine into the original vector
	int i = 0, j = 0;
	while (i < left.size() && j < right.size())
	{
		comparisons++;
		if (((left[i] < right[j]) && !des) || ((left[i] >= right[j]) && des))
		{
			swaps++;
			list[i + j] = left[i];
			i++;
		}
		else
		{
			swaps++;
			list[i + j] = right[j];
			j++;
		}
	}

	//one of the sub-vectors will get emptied first, when this happens we just tak on the rest of the 
	//other vector to the end of list. Only ony of the below while loops will actually execute.
	while (i < left.size())
	{
		swaps++;
		list[i + j] = left[i];
		i++;
	}
	while (j < right.size())
	{
		swaps++;
		list[i + j] = right[j];
		j++;
	}

	return;
}

template <typename T>
std::pair<int, int> MergeSort(std::vector<T>& list, bool des = false)
{
	//The jump in point for the below MergeSortHelper function. This function is just for setting up the variables used 
	//to keep track of the total number of comparisons and swaps made and then calls the recursive portion of the sort.
	int comparisons = 0, swaps = 0;
	MergeSortHelper(list, comparisons, swaps, des);
	return { comparisons, swaps };
}

template <typename T>
void QuickSortHelper(std::vector<T>& list, int i, int j, int& comparisons, int& swaps, bool des = false)
{
	//This is another recursive sorting method. Similarly to the Merge Sort method it uses a divide and conquer recursive approach, however, there are two main differences.
	//First, when the vector is split into two parts it isn't done so evenly. A random pivot value is chosen at some location in the vector
	//and all values less then this pivot value are placed to the left and all values greater are placed to the right of it. In other words,
	//this pivot value gets sorted into its correct location before the recursion starts. The second main difference is that this algorithm 
	//happens in place which greatly reduces the memory requirements when compared to Merge Sort. This is accomplished by using two index 
	//variables to keep track of the begining and ending of the current sub-array being looked at.

	//So for the actual algorithm, at each step of the recursion a random value is chosen to get sorted into the correct location while all 
	//values less are placed to the left and all values greater than are placed to the right. The same step happens on each of these sub-arrays
	//respectively. There are two different bases to the recursion. If the passed in vector has a size of 1 or 0 it's already considered to be 
	//sorted and nothing happens. If the passed in vector has a size of 2, swap the variables (if necessary) before returning from the 
	//recursion.

	//The performance of this algorithm is closely tied with how "well" the pivot point is chosen. Since the pivot is chosen randomly at each 
	//step of the recursion we can actually expect a different performance even when the same vector is passed in. The closer to the median 
	//the pivot is, the better. For example given an array with the numbers 1-10, the best pivot we could pick for the first step is 5 as it 
	//will split the search space in half. The best pivots for the next two steps we could pick would be 2 and 7, and so on. With good pivot 
	//selection the runtime has a complexity of O(n log(n)) which matches the marge sort. When the pivot is picked poorly though, the time 
	//complexity deteriorates to that of the iterative sorts above, O(n^2). For large arrays the odds of this happening should be relatively small 
	//though (which is why the pivot is selected randomly) and the ammortized runtime for this algorithm works out to be better than the iterative sorts.
	//Because of this, it's recommended to use quick sort on large arrays to avoid the memeory hit that merge sort provides, and to use merge sort
	//on smaller arrays where the memory hit is minimal and we get a guaranteed logarithmic run time.

	//base case 1, array is already sorted
	if (j - i < 1) return;
	else if (j - i == 1)
	{
		//base case 2, swap varaibles if necessary before returning
		comparisons++;
		if (list[j + des * (i - j)] < list[i + des * (j - i)])
		{
			swaps++;
			T temp = list[j];
			list[j] = list[i];
			list[i] = temp;
		}
		return;
	}

	//pick a random index between i and j (inclusive) to become the pivot number.
	int pivot_index = rand() % (j - i + 1) + i;

	//iterate i forwards until a value greater than the pivot value is found and j backwards until a value less than the 
	//pivot value is found. If j reaches the pivot index before i does, we swap the pivot value towards the front of the 
	//vector and update it's location. As soon as the j and i indices cross each other we stop the swap loop.
	int a = i, b = j;
	while (true)
	{
		//increment 'a' until a value greater than the pivot is found
		for (; a <= j; a++)
		{
			comparisons++;
			if (list[a + des * (pivot_index - a)] > list[pivot_index + des * (a - pivot_index)]) break;
		}

		//decrement 'b' until a value less than or equal to the pivot is found
		for (; b >= i; b--)
		{
			comparisons++;
			if (list[b + des * (pivot_index - b)] <= list[pivot_index + des * (b - pivot_index)]) break;
		}

		//if b and a have crossed over each other then we break out of this while loop
		if (b < a) break;

		//swap the elements at indices 'a' and 'b', if the pivot value is getting moved then update the pivot index
		swaps++;
		T temp = list[a];
		list[a] = list[b];
		list[b] = temp;
		if (b == pivot_index) pivot_index = a;
	}

	//once the initial sort is complete, swap the pivot value with the element at index b
	swaps++;
	T temp = list[pivot_index];
	list[pivot_index] = list[b];
	list[b] = temp;
	pivot_index = b;

	//Recursively call this method on the two sub-arrays divided by the pivot value
	QuickSortHelper(list, i, pivot_index - 1, comparisons, swaps, des);
	QuickSortHelper(list, pivot_index + 1, j, comparisons, swaps, des);
}

template <typename T>
std::pair<int, int> QuickSort(std::vector<T>& list, bool des = false)
{
	//The jump in point for the below MergeSortHelper function. This function is just for setting up the variables used 
	//to keep track of the total number of comparisons and swaps made and then calls the recursive portion of the sort.
	int comparisons = 0, swaps = 0;
	srand(time(0)); //To make sure this algorithm is truly randomized set the random number generated seed based on the current time.
	QuickSortHelper(list, 0, list.size() - 1, comparisons, swaps, des);
	return { comparisons, swaps };
}

template <typename T>
void SortComparison(std::vector<T> list, bool des = false)
{
	//This method is just used to compare the total number of comparisons and swaps for the various sorting algorithms on this page.
	//It's good to compare and see how some sorts are better suited for certain situations than others.

	
	std::vector<T> list_copy = list;
	auto run_time = std::chrono::steady_clock::now();
	long double seconds = 0;
	std::pair<int, int> comps = BubbleSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Bubble Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Bubble Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	comps = InsertionSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Insertion Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Insertion Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	comps = SelectionSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Selection Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Selection Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	comps = CocktailShakerSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Cocktail Shaker Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Cocktail Shaker Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	comps = MergeSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Merge Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Merge Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	comps = QuickSort(list_copy, des);
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "Quick Sort required " << comps.first << " comparisons and " << comps.second << " swaps to sort the vector." << std::endl;
	std::cout << "Quick Sort took " << seconds << " seconds.\n" << std::endl;

	list_copy = list;
	run_time = std::chrono::steady_clock::now();
	if (des) std::sort(list_copy.begin(), list_copy.end(), std::greater <>());
	else std::sort(list_copy.begin(), list_copy.end());
	seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0;
	std::cout << "For reference, the C++ built-in sort took " << seconds << " seconds." << std::endl;

	std::cout << std::endl;
}

void SortComparisonRandom(int preorder = 0)
{
	//Generates a vector of random integers of a randomized length (from 1 to 10000) and compares the time to sort this vector between the 
	//various sorting methods in this file.

	//pick a seed for random number generation based on the current time
	srand(time(0));
	int length = rand() % 10000 + 1;
	std::vector<int> random_numbers;

	

	for (int i = 0; i < length; i++) random_numbers.push_back(rand());

	if (preorder == 1)
	{
		//sort the array in ascending order first to see the best case performance of the sorting algorithms
		QuickSort(random_numbers);
	}
	else if (preorder > 1)
	{
		//sort the array in descending order first to see the worst case performance of the sorting algorithms
		QuickSort(random_numbers, 1);
	}

	std::cout << "Time and memory for sorting a random array of length: " << length << ": " << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;
	SortComparison(random_numbers);
}

//GRAPH ALGORITHMS
template <typename T>
void DepthFirstSearch(Graph<T>& graph, const GraphVertex<T>& start)
{
	//This is a method for a depth first traversal of a graph. Basically what this means is that vertices will be visited 
	//in a straight line until we can't go any more, we then go backwards until a different path can be selected. Given the 
	//following graph:
	/*
							A
						  /   \
						B       C
					  /  \    /   \
					D     E  F     G
	*/
	//starting the search at Location A will lead to a traversal order of: A, B, D, E, C, F, G. A recursive helper method is
	//used to accomplish this traversal.

	//TODO:
	//At present, all this array does is traverse the graph and print out each vertex in the order they get visited. Should
	//consider adding a lambda expression parameter that will allow something to actually happen during the traversal (a printing 
	//method could be set as the default).

	std::cout << "DFS Order of Vertex Visitation:\n";
	std::vector<const GraphVertex<T>*> visited_set; //let's us know which vertices have already been visited
	DFSHelper(graph, start, visited_set);
}

template <typename T>
void DFSHelper(Graph<T>& graph, const GraphVertex<T>& current_vertex, std::vector<const GraphVertex<T>*>& visited_set)
{
	std::cout << current_vertex << std::endl;
	auto vertex_edges = graph.getAdjacencyList()->find(&current_vertex)->second;
	visited_set.push_back(&current_vertex);
	for (int i = 0; i < vertex_edges.size(); i++)
	{
		if (std::find(visited_set.begin(), visited_set.end(), vertex_edges[i]->getV()) == visited_set.end())
		{
			DFSHelper(graph, *(vertex_edges[i]->getV()), visited_set);
		}
	}
}

template <typename T>
void BreadthFirstSearch(Graph<T>& graph, const GraphVertex<T>& start)
{
	//This is a method for a breadth first search of a graph. Basically what this means is that vertices will be visited 
	//in the order of how far they are from the starting vertex. For example, given the following graph:
	/*
	                        A
						  /   \
						B       C
					  /  \    /   \
					D     E  F     G
	*/
	//starting the search at Location A will lead to a traversal order of: A, B, C, D, E, F, G. This is acheived by using 
	//a queue to mark the order the vertices get visited in, and using a visited set to prevent going to the same vertex
	//twice.

	const GraphVertex<T>* current_vertex = &start;
	std::vector<const GraphVertex<T>*> visited_set; //initialize the visted set;
	std::queue<const GraphVertex<T>*> next; //initialize the queue
	std::cout << "BFS Order of Vertex Visitation:\n";

	next.push(current_vertex);
	while (!next.empty())
	{
		current_vertex = next.front();
		next.pop();

		if (std::find(visited_set.begin(), visited_set.end(), current_vertex) != visited_set.end()) continue; //already visited this vertex
		std::cout << *(current_vertex) << std::endl;
		visited_set.push_back(current_vertex);
		auto vertex_edges = graph.getAdjacencyList()->find(current_vertex)->second;

		for (int i = 0; i < vertex_edges.size(); i++)
		{
			if (std::find(visited_set.begin(), visited_set.end(), vertex_edges[i]->getV()) == visited_set.end())
			{
				next.push(vertex_edges[i]->getV());
			}
		}
	}
}

template <typename T>
int DijkstrasAlgorithm(Graph<T>& graph, const GraphVertex<T>& start, const GraphVertex<T>& end)
{
	//This is a very interesting and useful algorithm. It's used for finding the shortest path in a weighted graph that goes form 
	//the start vertex to the end vertex. A priority queue is used which tells us the shortest distance between the start node 
	//and the node in the queue. Any time a shorter distance is found to a node which hasn't been visited yet, this distance 
	//gets moved up inside the priority queue. The main driving principle behind the algorithm is that by the time we visit a 
	//vertex we MUST have discovered the shortest path, if not, then the vertex would be farther back in the priority queue.
	//Similar to the depth and breadth first searches, a visited set is used to prevent us from going to the same vertices
	//multiple times.

	//For the priority queue, we use pairs of the form {edge weight, edge pointer}. These pairs are sorted first by which 
	//edge weights are less, followed by the pointer values. For example, given the following pairs {5, 0x0001}, {5, 0x0002},
	//{1, 0x0002} and {3, 0x0003} they would have the following order in the priority queue: {1, 0x0002}, {3, 0x0003}, {5, 0x0001}, {5, 0x0002}.

	//priority is on the minimum edge weight
	std::priority_queue<std::pair<int, const GraphVertex<T>*>, std::vector<std::pair<int, const GraphVertex<T>*>>, std::greater<std::pair<int, const GraphVertex<T>*>>> pq;
	std::vector<const GraphVertex<T>*> visited_set; //initialize the visted set;

	//The first thing that goes into the priority queue is the starting vertex with a distance of 0
	pq.push({ 0, &start });

	while (!pq.empty())
	{
		//remove whatever is at the front of the priority queue and check whether or not it's been visited already. If it has just 
		//continue to the next iteration
		auto next = pq.top();
		pq.pop();

		if (std::find(visited_set.begin(), visited_set.end(), next.second) != visited_set.end()) continue; 

		//We've found the shortest path to a new vertex, check to see if it's the goal vertex. If it is then we terminate 
		//the algorithm early
		if (next.second == &end) return next.first;

		//add the new vertex to the visited set
		visited_set.push_back(next.second);

		//add the cumulative weight of all edges from the current vertex to the priority queue
		auto edge_list = graph.getAdjacencyList()->find(next.second)->second;
		for (int i = 0; i < edge_list.size(); i++)
		{
			//only look at edges connecting to unvisited vertices
			if (std::find(visited_set.begin(), visited_set.end(), edge_list[i]->getV()) != visited_set.end()) continue;
			pq.push({next.first + edge_list[i]->getWeight(), edge_list[i]->getV() });
		}
	}
	
	//In an unconnected graph it's possible that there's no route from the start vertex to the end vertex. In this instance
	//return a negative value indicating that no path is possible.
	return -1;

}