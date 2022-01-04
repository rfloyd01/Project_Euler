#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <vector>

void vprint(std::vector<int> numbers)
{
	std::cout << '[';
	if (numbers.size() == 0)
	{
		std::cout << ']' << std::endl;
		return;
	}
	for (int i = 0; i < numbers.size() - 1; i++) std::cout << numbers[i] << ", ";
	std::cout << numbers.back() << ']' << std::endl;
	return;
}

void vprint(std::vector<long long> numbers)
{
	std::cout << '[';
	if (numbers.size() == 0)
	{
		std::cout << ']' << std::endl;
		return;
	}
	for (int i = 0; i < numbers.size() - 1; i++) std::cout << numbers[i] << ", ";
	std::cout << numbers.back() << ']' << std::endl;
	return;
}

//template <typename T>
//void vprint(std::vector<T> numbers)
//{
//	std::cout << '[';
//	if (numbers.size() == 0)
//	{
//		std::cout << ']' << std::endl;
//		return;
//	}
//	for (int i = 0; i < numbers.size() - 1; i++) std::cout << numbers[i] << ", ";
//	std::cout << numbers.back() << ']' << std::endl;
//	return;
//}

void vfprint(std::vector<float> numbers)
{
	std::cout << '[';
	for (int i = 0; i < numbers.size() - 1; i++) std::cout << numbers[i] << ", ";
	std::cout << numbers.back() << ']' << std::endl;
	return;
}

void vecvecprint(std::vector<std::vector<int> > numbers)
{
	for (int j = 0; j < numbers.size(); j++)
	{
		std::cout << '[';
		for (int i = 0; i < numbers[j].size() - 1; i++) std::cout << numbers[j][i] << ", ";
		std::cout << numbers[j].back() << ']' << std::endl;
	}
	return;
}