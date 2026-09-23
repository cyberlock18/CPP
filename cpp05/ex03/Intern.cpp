#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern() {}
Intern::Intern(Intern const&) {}
Intern& Intern::operator=(Intern const&) { return *this; }
Intern::~Intern() {}

static AForm* makeShrubbery(std::string const& target)
{
	return new ShrubberyCreationForm(target);
}

static AForm* makeRobotomy(std::string const& target)
{
	return new RobotomyRequestForm(target);
}

static AForm* makePardon(std::string const& target)
{
	return new PresidentialPardonForm(target);
}

AForm* Intern::makeForm(std::string const& name, std::string const& target) const
{
	struct FormEntry
	{
		const char*	name;
		AForm*		(*create)(std::string const&);
	};

	static const FormEntry table[] = {
		{ "shrubbery creation", makeShrubbery },
		{ "robotomy request",   makeRobotomy  },
		{ "presidential pardon", makePardon   }
	};

	static const int SIZE = 3;

	for (int i = 0; i < SIZE; i++)
	{
		if (name == table[i].name)
		{
			std::cout << "Intern creates " << name << std::endl;
			return table[i].create(target);
		}
	}

	std::cerr << "Intern: unknown form '" << name << "'" << std::endl;
	return NULL;
}
