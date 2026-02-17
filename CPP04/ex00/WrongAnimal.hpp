/*
** WRONGANIMAL - Clase SIN polimorfismo (sin virtual)
**
** Esta clase es para demostrar la diferencia:
** - Sin virtual: un WrongAnimal* que apunta a WrongCat
**   llamara a WrongAnimal::makeSound() (el del padre)
** - Con virtual (Animal): un Animal* que apunta a Cat
**   llamara a Cat::makeSound() (el del hijo)
*/
#ifndef WRONGANIMAL_HPP
# define WRONGANIMAL_HPP

# include <string>
# include <iostream>

class WrongAnimal
{
protected:
	std::string	_type;

public:
	WrongAnimal(void);
	WrongAnimal(const WrongAnimal &src);
	WrongAnimal &operator=(const WrongAnimal &rhs);
	~WrongAnimal(void);

	void		makeSound(void) const;
	std::string	getType(void) const;
};

#endif
