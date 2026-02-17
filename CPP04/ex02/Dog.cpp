#include "Dog.hpp"

Dog::Dog(void)
{
	_type = "Dog";
	_brain = new Brain();
	std::cout << "Dog default constructor called" << std::endl;
}

Dog::Dog(const Dog &src) : AAnimal(src)
{
	_brain = new Brain(*src._brain);
	std::cout << "Dog copy constructor called" << std::endl;
}

Dog &Dog::operator=(const Dog &rhs)
{
	std::cout << "Dog assignment operator called" << std::endl;
	if (this != &rhs)
	{
		AAnimal::operator=(rhs);
		delete _brain;
		_brain = new Brain(*rhs._brain);
	}
	return *this;
}

Dog::~Dog(void)
{
	delete _brain;
	std::cout << "Dog destructor called" << std::endl;
}

void	Dog::makeSound(void) const
{
	std::cout << "Woof! Woof!" << std::endl;
}

Brain*	Dog::getBrain(void) const
{
	return _brain;
}
