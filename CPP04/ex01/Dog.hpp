/*
** DOG con Brain*
** Ahora Dog tiene un puntero a Brain que se crea con new
** y se destruye con delete en el destructor
** La copia debe ser DEEP COPY (copiar el contenido del Brain, no el puntero)
*/
#ifndef DOG_HPP
# define DOG_HPP

# include "Animal.hpp"
# include "Brain.hpp"

class Dog : public Animal
{
private:
	Brain*	_brain;

public:
	Dog(void);
	Dog(const Dog &src);
	Dog &operator=(const Dog &rhs);
	~Dog(void);

	void	makeSound(void) const;
	Brain*	getBrain(void) const;
};

#endif
