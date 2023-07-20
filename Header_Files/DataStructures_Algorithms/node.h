#pragma once

//Generic node classes/structs that are used in various data structures

template <typename T>
struct Node
{
	T data;
};

template <typename T>
struct SLL_Node
{
	T data;
	SLL_Node* next_node = nullptr;
};

template <typename T>
struct DLL_Node
{
	T data;
	DLL_Node* next_node;
	DLL_Node* previous_node;
};