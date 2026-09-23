#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include <stdexcept>
#include <string>

class Bureaucrat;

class AForm
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

	class FormNotSignedException : public std::exception
	{
	public:
		const char* what() const throw();
	};

	AForm();
	AForm(std::string const& name, int gradeToSign, int gradeToExecute);
	AForm(AForm const& other);
	AForm& operator=(AForm const& other);
	virtual ~AForm();

	std::string const&	getName() const;
	bool				getSigned() const;
	int					getGradeToSign() const;
	int					getGradeToExecute() const;

	void			beSigned(Bureaucrat const& b);
	virtual void	execute(Bureaucrat const& executor) const = 0;

protected:
	void	checkExecution(Bureaucrat const& executor) const;

private:
	std::string const	_name;
	bool				_signed;
	int const			_gradeToSign;
	int const			_gradeToExecute;

	static const int GRADE_MAX = 1;
	static const int GRADE_MIN = 150;
};

std::ostream& operator<<(std::ostream& out, AForm const& f);

#endif
