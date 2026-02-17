/*
** AANIMAL - Clase ABSTRACTA
**
** CLASE ABSTRACTA: una clase que NO se puede instanciar directamente
** Se consigue poniendo "= 0" en al menos una funcion virtual
** Esto se llama funcion virtual pura (pure virtual function)
**
** makeSound() = 0 significa:
** - "No tengo implementacion aqui, las clases hijas DEBEN implementarla"
** - No puedes hacer: AAnimal a; // ERROR de compilacion
** - Si puedes hacer: AAnimal* a = new Dog(); // OK, Dog implementa makeSound
**
** El prefijo 'A' indica que es una clase abstracta (convencion de 42)
*/
#ifndef AANIMAL_HPP
# define AANIMAL_HPP

# include <string>
# include <iostream>

class AAnimal
{
protected:
	std::string	_type;

public:
	AAnimal(void);
	AAnimal(const AAnimal &src);
	AAnimal &operator=(const AAnimal &rhs);
	virtual ~AAnimal(void);

	virtual void		makeSound(void) const = 0;
	std::string			getType(void) const;
};

#endif
