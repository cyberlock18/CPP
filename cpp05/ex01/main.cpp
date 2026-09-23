#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	std::cout << "--- Test 1: Formulario normal ---" << std::endl;
	try
	{
		Form        taxForm("Tax Return", 50, 25);
		Bureaucrat  manager("Carlos", 30);

		std::cout << taxForm << std::endl;
		std::cout << manager << std::endl;
		manager.signForm(taxForm);
		std::cout << taxForm << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 2: Grado insuficiente para firmar ---" << std::endl;
	try
	{
		Form        secretForm("Top Secret", 10, 5);
		Bureaucrat  junior("Bob", 80);

		std::cout << secretForm << std::endl;
		junior.signForm(secretForm);
		std::cout << secretForm << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cout << "\n--- Test 3: Formulario con grado invalido ---" << std::endl;
	try
	{
		Form badForm("Illegal", 0, 50);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	return 0;
}
