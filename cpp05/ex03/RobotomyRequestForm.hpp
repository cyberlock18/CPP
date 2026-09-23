#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"

class RobotomyRequestForm : public AForm
{
public:
	RobotomyRequestForm();
	RobotomyRequestForm(std::string const& target);
	RobotomyRequestForm(RobotomyRequestForm const& other);
	RobotomyRequestForm& operator=(RobotomyRequestForm const& other);
	~RobotomyRequestForm();

	void execute(Bureaucrat const& executor) const;

private:
	std::string _target;
};

#endif
