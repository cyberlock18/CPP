#include "ShrubberyCreationForm.hpp"
#include "Bureaucrat.hpp"

ShrubberyCreationForm::ShrubberyCreationForm()
	: AForm("ShrubberyCreation", 145, 137), _target("default") {}

ShrubberyCreationForm::ShrubberyCreationForm(std::string const& target)
	: AForm("ShrubberyCreation", 145, 137), _target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(ShrubberyCreationForm const& other)
	: AForm(other), _target(other._target) {}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(ShrubberyCreationForm const& other)
{
	if (this != &other)
	{
		AForm::operator=(other);
		_target = other._target;
	}
	return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

void ShrubberyCreationForm::execute(Bureaucrat const& executor) const
{
	checkExecution(executor);

	std::ofstream file((_target + "_shrubbery").c_str());
	if (!file.is_open())
		throw std::runtime_error("cannot open file");

	file << "       *       " << std::endl;
	file << "      ***      " << std::endl;
	file << "     *****     " << std::endl;
	file << "    *******    " << std::endl;
	file << "   *********   " << std::endl;
	file << "  ***********  " << std::endl;
	file << "       |       " << std::endl;
	file << "       |       " << std::endl;
}
