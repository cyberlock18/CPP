#include "iter.hpp"
#include <iostream>
#include <string>

template <typename T>
void printElement(T const& x)
{
	std::cout << x << " ";
}

template <typename T>
void doubleIt(T& x)
{
	x *= 2;
}

int main()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	std::cout << "int array: ";
	iter(arr, 5, printElement<int>);
	std::cout << std::endl;

	iter(arr, 5, doubleIt<int>);
	std::cout << "after doubleIt: ";
	iter(arr, 5, printElement<int>);
	std::cout << std::endl;

	std::string words[] = { "hello", "world", "42" };
	std::cout << "string array: ";
	iter(words, 3, printElement<std::string>);
	std::cout << std::endl;

	return 0;
}
