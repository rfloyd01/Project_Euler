#pragma once

#include <iostream>
#include <vector>

//void vprint(std::vector<int> numbers);
//void vprint(std::vector<long long> numbers);
//template <typename T>
//void vprint(std::vector<T> numbers);
//void vfprint(std::vector<float> numbers);
//void vecvecprint(std::vector<std::vector<int> > numbers);

template <typename T>
void vprint(std::vector<T> v)
{
	std::cout << '[';
	if (v.size() == 0)
	{
		std::cout << ']' << std::endl;
		return;
	}
	for (int i = 0; i < v.size() - 1; i++) std::cout << v[i] << ", ";
	std::cout << v.back() << ']' << std::endl;
	return;
}

template <typename T>
void revvprint(std::vector<T> v)
{
	std::cout << '[';
	if (v.size() == 0)
	{
		std::cout << ']' << std::endl;
		return;
	}
	for (int i = v.size() - 1; i > 0; i--) std::cout << v[i] << ", ";
	std::cout << v[0] << ']' << std::endl;
	return;
}