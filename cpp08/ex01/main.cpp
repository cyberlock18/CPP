#include "Span.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	std::cout << "=== Subject example ===" << std::endl;
	Span sp = Span(5);
	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);
	std::cout << "Shortest: " << sp.shortestSpan() << std::endl;
	std::cout << "Longest:  " << sp.longestSpan()  << std::endl;

	std::cout << "\n=== 10000 numbers ===" << std::endl;
	std::srand(std::time(NULL));
	Span big(10000);
	std::vector<int> tmp;
	for (int i = 0; i < 10000; i++) tmp.push_back(std::rand());
	big.addRange(tmp.begin(), tmp.end());
	std::cout << "Shortest: " << big.shortestSpan() << std::endl;
	std::cout << "Longest:  " << big.longestSpan()  << std::endl;

	std::cout << "\n=== Overflow exception ===" << std::endl;
	try
	{
		sp.addNumber(99);
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	std::cout << "\n=== Too few elements ===" << std::endl;
	try
	{
		Span one(1);
		one.addNumber(42);
		one.shortestSpan();
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	return 0;
}
