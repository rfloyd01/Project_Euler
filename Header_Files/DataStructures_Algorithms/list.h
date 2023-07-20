#pragma once

#include "node.h"

//This header file includes implementations for singly linked lists and double linked lists

template <typename T>
class SinglyLinkedList
{
	SLL_Node<T>* head;
	SLL_Node<T>* tail;
	int length;

public:
	SinglyLinkedList()
	{
		//default constructor
		this->head = new SLL_Node<T>;
		this->tail = new SLL_Node<T>;
		this->length = 0;
	}

	~SinglyLinkedList()
	{
		//the destructor is responsible for freeing up all of the nodes
		SLL_Node<T>* current_node = this->head, *previous_node;

		while (current_node->next_node != nullptr)
		{
			previous_node = current_node;
			current_node = previous_node->next_node;
			delete previous_node;
		}

		//free up memory of the current node and tail pointer
		delete current_node;
		delete this->tail;
	}

	void add(T t, int index = 0)
	{
		//Takes the value 't', and creates a new node with it which then gets added at the given index 
		//of the list. Since the location of the front of the list is saved in the head variable
		//this add can happen in O(1) time. If the given index is out of range for the current
		//list then the new node will simply be added to the front.

		//First create a new node and have it point to the node currently being pointed at by the
		//head pointer. Then have the head pointer point at the new node.
		SLL_Node<T>* new_node = new SLL_Node<T>;
		new_node->data = t;
		new_node->next_node = this->head->next_node;
		this->head->next_node = new_node;

		//If the current size of the list is 0, then the new node also needs to be pointed at by
		//the tail pointer
		if (this->length == 0) this->tail->next_node = new_node;
		this->length++;
	}
	
	void push_back(T t)
	{
		//Takes the value 't', and creates a new node with it which then gets added to the back
		//of the list. Since the location of the back of the list is saved in the tail variable
		//this add can happen in O(1) time.

		//First create a new node and have it point to a null node, which signifies the end of
		//the list.
		SLL_Node<T>* new_node = new SLL_Node<T>;
		new_node->data = t;
		new_node->next_node = nullptr;

		//If there is already data in the list then the tail is currently pointing at a non-null node.
		//This node must now also point to the newly created node. If there isn't any data in the list 
		//yet, then the head pointer must point at the newly created node.
		if (this->length > 0) this->tail->next_node->next_node = new_node;
		else this->head->next_node = new_node;

		//Update the tail pointer to point at the new node and increment the list size by 1
		this->tail->next_node = new_node;
		this->length++;
	}

	//add at index
	//find

	int size() { return this->length; }

	void print()
	{
		//prints every node in the list
		SLL_Node<T>* current_node = this->head->next_node;
		if (current_node == nullptr) return;
		std::cout << current_node->data;

		while (current_node->next_node != nullptr)
		{
			std::cout << " --> " << current_node->next_node->data;
			current_node = current_node->next_node;
		}
		std::cout << std::endl;
	}

	SLL_Node<T>* getHead() { return this->head; }
	SLL_Node<T>* getTail() { return this->tail; }
};