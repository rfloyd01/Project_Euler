#include <iostream>
#include <vector>
#include <string>

//My implementation of a Singly Linked List

template <class T>
struct SinglyLinkedListNode
{
	T value;
	SinglyLinkedListNode* next;

	SinglyLinkedListNode(T val)
	{ 
		value = val;
		next = nullptr;
	}
};

template <class T>
class SinglyLinkedList
{
public:
	SinglyLinkedListNode<T>* head;
	SinglyLinkedListNode<T>* tail;
	int size;

	SinglyLinkedList()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	void addToFront(T data)
	{
		SinglyLinkedListNode<T>* new_node = new SinglyLinkedListNode<T>(data);
		if (size == 0) tail = new_node;
		else new_node->next = head;
		head = new_node;

		size++;
	}

	void addToBack(T data)
	{
		SinglyLinkedListNode<T>* new_node = new SinglyLinkedListNode<T>(data);
		if (size == 0) head = new_node;
		else tail->next = new_node;
		tail = new_node;

		size++;
	}

	void addAtIndex(unsigned int index, T data)
	{
		SinglyLinkedListNode<T>* new_node = new SinglyLinkedListNode<T>(data);
		if (index > size)
		{
			std::cout << "List not big enough to add here, adding new data to back of list." << std::endl;
			addToBack(data);
		}
		else if (index == size) addToBack(data);
		else if (index == 0) addToFront(data);
		else if (size == 0) addToFront(data);
		else
		{
			//iterate to the node before the desired index
			SinglyLinkedListNode<T>* current = head;
			for (int i = 0; i < index - 1; i++) current = current->next;
			new_node->next = current->next;
			current->next = new_node;
			size++;
		}
		
	}

	void addNext(SinglyLinkedListNode<T>* current_node, T data)
	{
		//given a pointer to a node in the list, this function adds a new node directly after the current one
		if (current_node == nullptr)
		{
			std::cout << "Node must be in a list. Data not added." << std::endl;
			return;
		}

		//as of right now, a pointer to a node not in the list can be passed which will mess things up. Only pass a pointer
		//to a node in the list.
		SinglyLinkedListNode<T>* new_node = new SinglyLinkedListNode<T>(data);
		new_node->next = current_node->next;
		current_node->next = new_node;
		size++;
	}

	void printList()
	{
		std::cout << '[';
		SinglyLinkedListNode<T>* current = head;
		if (size > 0)
		{
			for (int i = 0; i < size - 1; i++)
			{
				std::cout << current->value << ", ";
				current = current->next;
			}
			std::cout << current->value;
		} 
		std::cout << ']' << std::endl;
	}
};