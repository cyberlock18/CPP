#include "easyfind.hpp"
#include <iostream>
#include <vector>
#include <list>

int main()
{
	std::vector<int> v;
	for (int i = 0; i < 10; i++) v.push_back(i * 3);

	std::cout << "Vector: ";
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	try
	{
		std::vector<int>::iterator it = easyfind(v, 15);
		std::cout << "Found 15 at position " << std::distance(v.begin(), it) << std::endl;
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	try
	{
		easyfind(v, 7);
	}
	catch (std::exception& e) { std::cerr << "7: " << e.what() << std::endl; }

	std::list<int> lst;
	for (int i = 10; i >= 1; i--) lst.push_back(i);

	try
	{
		std::list<int>::iterator it = easyfind(lst, 5);
		std::cout << "Found 5 in list: " << *it << std::endl;
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	return 0;
}
