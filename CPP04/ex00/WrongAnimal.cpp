#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal(void) : _type("WrongAnimal")
{
	std::cout << "WrongAnimal default constructor called" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal &src) : _type(src._type)
{
	std::cout << "WrongAnimal copy constructor called" << std::endl;
}

WrongAnimal &WrongAnimal::operator=(const WrongAnimal &rhs)
{
	std::cout << "WrongAnimal assignment operator called" << std::endl;
	if (this != &rhs)
		_type = rhs._type;
	return *this;
}

WrongAnimal::~WrongAnimal(void)
{
	std::cout << "WrongAnimal destructor called" << std::endl;
}

void	WrongAnimal::makeSound(void) const
{
	std::cout << "WrongAnimal makes a generic sound..." << std::endl;
}

std::string	WrongAnimal::getType(void) const
{
	return _type;
}
