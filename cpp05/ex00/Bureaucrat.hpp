#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <stdexcept>
#include <string>

class Bureaucrat
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

	Bureaucrat();
	Bureaucrat(std::string const& name, int grade);
	Bureaucrat(Bureaucrat const& other);
	Bureaucrat& operator=(Bureaucrat const& other);
	~Bureaucrat();

	std::string const&	getName() const;
	int					getGrade() const;

	void	incrementGrade();
	void	decrementGrade();

private:
	std::string const	_name;
	int					_grade;

	static const int GRADE_MAX = 1;
	static const int GRADE_MIN = 150;
};

std::ostream& operator<<(std::ostream& out, Bureaucrat const& b);

#endif
