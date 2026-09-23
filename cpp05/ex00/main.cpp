#include "Bureaucrat.hpp"

int main()
{
	std::cout << "--- Test 1: Bureaucrat normal ---" << std::endl;
	try
	{
		Bureaucrat alice("Alice", 42);
		std::cout << alice << std::endl;
		alice.incrementGrade();
		std::cout << "After increment: " << alice << std::endl;
		alice.decrementGrade();
		alice.decrementGrade();
		std::cout << "After 2 decrements: " << alice << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 2: Grade too high ---" << std::endl;
	try
	{
		Bureaucrat god("Zeus", 0);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 3: Grade too low ---" << std::endl;
	try
	{
		Bureaucrat intern("Bob", 151);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 4: Increment past max ---" << std::endl;
	try
	{
		Bureaucrat top("Top", 1);
		std::cout << top << std::endl;
		top.incrementGrade();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 5: Decrement past min ---" << std::endl;
	try
	{
		Bureaucrat bottom("Bottom", 150);
		std::cout << bottom << std::endl;
		bottom.decrementGrade();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	return 0;
}
