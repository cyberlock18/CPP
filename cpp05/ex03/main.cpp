#include <cstdlib>
#include <ctime>
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include "AForm.hpp"

int main()
{
	std::srand(std::time(NULL));

	Intern someRandomIntern;
	AForm* rrf;

	std::cout << "=== Robotomy Request ===" << std::endl;
	rrf = someRandomIntern.makeForm("robotomy request", "Bender");
	if (rrf)
	{
		Bureaucrat boss("Boss", 45);
		boss.signForm(*rrf);
		boss.executeForm(*rrf);
		delete rrf;
	}

	std::cout << "\n=== Shrubbery Creation ===" << std::endl;
	AForm* shrub = someRandomIntern.makeForm("shrubbery creation", "home");
	if (shrub)
	{
		Bureaucrat gardener("Gardener", 137);
		gardener.signForm(*shrub);
		gardener.executeForm(*shrub);
		delete shrub;
	}

	std::cout << "\n=== Presidential Pardon ===" << std::endl;
	AForm* pardon = someRandomIntern.makeForm("presidential pardon", "Ford Prefect");
	if (pardon)
	{
		Bureaucrat president("President", 5);
		president.signForm(*pardon);
		president.executeForm(*pardon);
		delete pardon;
	}

	std::cout << "\n=== Unknown form ===" << std::endl;
	AForm* unknown = someRandomIntern.makeForm("coffee form", "mug");
	if (!unknown)
		std::cout << "No form created (expected)." << std::endl;

	return 0;
}
