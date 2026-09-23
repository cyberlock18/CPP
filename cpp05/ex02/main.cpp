#include <cstdlib>
#include <ctime>
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
	std::srand(std::time(NULL));

	std::cout << "=== ShrubberyCreationForm ===" << std::endl;
	{
		Bureaucrat          gardener("Gardener", 137);
		ShrubberyCreationForm shrub("garden");
		gardener.signForm(shrub);
		gardener.executeForm(shrub);
	}

	std::cout << "\n=== RobotomyRequestForm ===" << std::endl;
	{
		Bureaucrat          dr("Dr.Smith", 45);
		RobotomyRequestForm robot("Bender");
		dr.signForm(robot);
		dr.executeForm(robot);
		dr.executeForm(robot);
		dr.executeForm(robot);
	}

	std::cout << "\n=== PresidentialPardonForm ===" << std::endl;
	{
		Bureaucrat             president("President", 5);
		PresidentialPardonForm pardon("Arthur Dent");
		president.signForm(pardon);
		president.executeForm(pardon);
	}

	std::cout << "\n=== Execute without signing ===" << std::endl;
	{
		Bureaucrat          boss("Boss", 1);
		ShrubberyCreationForm shrub("yard");
		boss.executeForm(shrub);
	}

	std::cout << "\n=== Grade too low to execute ===" << std::endl;
	{
		Bureaucrat             intern("Intern", 150);
		PresidentialPardonForm pardon("Nobody");
		Bureaucrat             signer("Signer", 5);
		signer.signForm(pardon);
		intern.executeForm(pardon);
	}

	return 0;
}
