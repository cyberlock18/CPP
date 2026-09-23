#include "Form.hpp"
#include "Bureaucrat.hpp"

const char* Form::GradeTooHighException::what() const throw()
{
	return "form grade is too high (min is 1)";
}

const char* Form::GradeTooLowException::what() const throw()
{
	return "form grade is too low (max is 150)";
}

Form::Form() : _name("unnamed"), _signed(false), _gradeToSign(150), _gradeToExecute(150)
{
}

Form::Form(std::string const& name, int gradeToSign, int gradeToExecute)
	: _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (_gradeToSign < GRADE_MAX || _gradeToExecute < GRADE_MAX)
		throw GradeTooHighException();
	if (_gradeToSign > GRADE_MIN || _gradeToExecute > GRADE_MIN)
		throw GradeTooLowException();
}

Form::Form(Form const& other)
	: _name(other._name), _signed(other._signed),
	  _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute)
{
}

Form& Form::operator=(Form const& other)
{
	if (this != &other)
		_signed = other._signed;
	return *this;
}

Form::~Form()
{
}

std::string const& Form::getName() const
{
	return _name;
}

bool Form::getSigned() const
{
	return _signed;
}

int Form::getGradeToSign() const
{
	return _gradeToSign;
}

int Form::getGradeToExecute() const
{
	return _gradeToExecute;
}

void Form::beSigned(Bureaucrat const& b)
{
	if (b.getGrade() > _gradeToSign)
		throw GradeTooLowException();
	_signed = true;
}

std::ostream& operator<<(std::ostream& out, Form const& f)
{
	out << "Form [" << f.getName() << "]"
		<< " | signed: " << (f.getSigned() ? "yes" : "no")
		<< " | grade to sign: " << f.getGradeToSign()
		<< " | grade to execute: " << f.getGradeToExecute();
	return out;
}
