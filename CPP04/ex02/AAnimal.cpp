#include "AAnimal.hpp"

AAnimal::AAnimal(void) : _type("AAnimal")
{
	std::cout << "AAnimal default constructor called" << std::endl;
}

AAnimal::AAnimal(const AAnimal &src) : _type(src._type)
{
	std::cout << "AAnimal copy constructor called" << std::endl;
}

AAnimal &AAnimal::operator=(const AAnimal &rhs)
{
	std::cout << "AAnimal assignment operator called" << std::endl;
	if (this != &rhs)
		_type = rhs._type;
	return *this;
}

AAnimal::~AAnimal(void)
{
	std::cout << "AAnimal destructor called" << std::endl;
}

std::string	AAnimal::getType(void) const
{
	return _type;
}
