#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include <stdexcept>
#include <string>

class Bureaucrat;

class Form
{
public:

	class GradeTooHighException : public std::exception
	{
	public:
		const char* what() const throw();
	};

	class GradeTooLowException : public std::exception
	{
	public:
		const char* what() const throw();
	};

	Form();
	Form(std::string const& name, int gradeToSign, int gradeToExecute);
	Form(Form const& other);
	Form& operator=(Form const& other);
	~Form();

	std::string const&	getName() const;
	bool				getSigned() const;
	int					getGradeToSign() const;
	int					getGradeToExecute() const;

	void	beSigned(Bureaucrat const& b);

private:
	std::string const	_name;
	bool				_signed;
	int const			_gradeToSign;
	int const			_gradeToExecute;

	static const int GRADE_MAX = 1;
	static const int GRADE_MIN = 150;
};

std::ostream& operator<<(std::ostream& out, Form const& f);

#endif
