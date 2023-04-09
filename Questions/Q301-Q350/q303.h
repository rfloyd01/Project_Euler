#pragma once

#include <Header_Files/pch.h>

long long ten_pows[18] = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17 };

struct Node
{
	Node(Node* n, int i, long long v)
	{
		nextNode = n;
		index = i;
		value = v;
	}

	Node* nextNode;
	int index;
	long long value;
};

void recursiveBuildNumbersWithDigitsLessThanThree(int maxLength, long long currentNumber, int currentLevel,
	Node* smallestMultiples, int maxMultiple, long long& answer)
{
	//Note, the currentLevel parameter must be equal to the maxLength parameter for this method to work properly
	for (int i = 0; i < 3; i++)
	{
		long long newNumber = currentNumber + i * ten_pows[currentLevel - 1];

		if (currentLevel > 1) recursiveBuildNumbersWithDigitsLessThanThree(maxLength, newNumber, currentLevel - 1, smallestMultiples, maxMultiple, answer);
		else
		{
			//iterate through the linked list
			Node* currentNode = smallestMultiples;
			while (currentNode->nextNode != nullptr)
			{
				if (currentNode->nextNode->index > newNumber) break;
				if (newNumber % currentNode->nextNode->index == 0)
				{
					answer += newNumber / currentNode->nextNode->index;
					currentNode->nextNode = currentNode->nextNode->nextNode; //set pointer to next node in the series
				}
				else currentNode = currentNode->nextNode; //iterate to the next node
			}
		}
	}
}

std::pair<std::string, long double> q303()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 1111333355557778; //The value for 9999 was calculated manually and is added here

	//Create a singly linked list to hold the numbers 1 - 10000. Whenever a node is found to be a divisor
	//of one of the numbers composed of only 0's, 1's and 2's we add its value to the answer and removed
	//it from the linked list so it doesn't get checked again.
	const int maxMultiple = 10000;
	Node* current_node = new Node(nullptr, maxMultiple, 0);

	for (int i = maxMultiple - 1; i >= 0; i--)
	{
		Node* n = new Node(current_node, i, 0);
		current_node = n;
	}

	//Recursively build numbers (in ascending order) composed of only 0's, 1's and 2's and check for divisiblilty
	//with all numbers that haven't had their minimum values found yet.
	recursiveBuildNumbersWithDigitsLessThanThree(16, 0, 16, current_node, maxMultiple, answer);

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1111981904675169
	//ran in 1.5286 seconds
}