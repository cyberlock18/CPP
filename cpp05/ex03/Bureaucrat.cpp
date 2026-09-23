#include "Bureaucrat.hpp"
#include "AForm.hpp"

const char* Bureaucrat::GradeTooHighException::what() const throw()
{
	return "grade is too high (min is 1)";
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
	return "grade is too low (max is 150)";
}

Bureaucrat::Bureaucrat() : _name("unnamed"), _grade(150) {}

Bureaucrat::Bureaucrat(std::string const& name, int grade) : _name(name), _grade(grade)
{
	if (_grade < GRADE_MAX) throw GradeTooHighException();
	if (_grade > GRADE_MIN) throw GradeTooLowException();
}

Bureaucrat::Bureaucrat(Bureaucrat const& other) : _name(other._name), _grade(other._grade) {}

Bureaucrat& Bureaucrat::operator=(Bureaucrat const& other)
{
	if (this != &other)
		_grade = other._grade;
	return *this;
}

Bureaucrat::~Bureaucrat() {}

std::string const& Bureaucrat::getName() const	{ return _name; }
int                Bureaucrat::getGrade() const	{ return _grade; }

void Bureaucrat::incrementGrade()
{
	if (_grade - 1 < GRADE_MAX) throw GradeTooHighException();
	_grade--;
}

void Bureaucrat::decrementGrade()
{
	if (_grade + 1 > GRADE_MIN) throw GradeTooLowException();
	_grade++;
}

void Bureaucrat::signForm(AForm& f)
{
	try
	{
		f.beSigned(*this);
		std::cout << getName() << " signed " << f.getName() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << getName() << " couldn't sign " << f.getName()
				  << " because " << e.what() << std::endl;
	}
}

void Bureaucrat::executeForm(AForm const& f)
{
	try
	{
		f.execute(*this);
		std::cout << getName() << " executed " << f.getName() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << getName() << " couldn't execute " << f.getName()
				  << " because " << e.what() << std::endl;
	}
}

std::ostream& operator<<(std::ostream& out, Bureaucrat const& b)
{
	out << b.getName() << ", bureaucrat grade " << b.getGrade();
	return out;
}
