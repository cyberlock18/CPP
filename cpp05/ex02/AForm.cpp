#include "AForm.hpp"
#include "Bureaucrat.hpp"

const char* AForm::GradeTooHighException::what() const throw()
{
	return "form grade is too high (min is 1)";
}

const char* AForm::GradeTooLowException::what() const throw()
{
	return "form grade is too low (max is 150)";
}

const char* AForm::FormNotSignedException::what() const throw()
{
	return "form is not signed";
}

AForm::AForm()
	: _name("unnamed"), _signed(false), _gradeToSign(150), _gradeToExecute(150)
{
}

AForm::AForm(std::string const& name, int gradeToSign, int gradeToExecute)
	: _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (_gradeToSign < GRADE_MAX || _gradeToExecute < GRADE_MAX)
		throw GradeTooHighException();
	if (_gradeToSign > GRADE_MIN || _gradeToExecute > GRADE_MIN)
		throw GradeTooLowException();
}

AForm::AForm(AForm const& other)
	: _name(other._name), _signed(other._signed),
	  _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute)
{
}

AForm& AForm::operator=(AForm const& other)
{
	if (this != &other)
		_signed = other._signed;
	return *this;
}

AForm::~AForm()
{
}

std::string const& AForm::getName() const		{ return _name; }
bool               AForm::getSigned() const		{ return _signed; }
int                AForm::getGradeToSign() const	{ return _gradeToSign; }
int                AForm::getGradeToExecute() const	{ return _gradeToExecute; }

void AForm::beSigned(Bureaucrat const& b)
{
	if (b.getGrade() > _gradeToSign)
		throw GradeTooLowException();
	_signed = true;
}

void AForm::checkExecution(Bureaucrat const& executor) const
{
	if (!_signed)
		throw FormNotSignedException();
	if (executor.getGrade() > _gradeToExecute)
		throw GradeTooLowException();
}

std::ostream& operator<<(std::ostream& out, AForm const& f)
{
	out << "AForm [" << f.getName() << "]"
		<< " | signed: " << (f.getSigned() ? "yes" : "no")
		<< " | sign grade: " << f.getGradeToSign()
		<< " | exec grade: " << f.getGradeToExecute();
	return out;
}
