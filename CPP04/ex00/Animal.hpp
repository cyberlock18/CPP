/*
** CLASE ANIMAL - Clase base con POLIMORFISMO
**
** POLIMORFISMO: permite que un puntero de tipo Animal*
** pueda apuntar a un Dog o Cat, y al llamar makeSound()
** se ejecute la funcion correcta (la del hijo, no la del padre)
**
** Para que funcione el polimorfismo necesitamos:
** 1. La palabra "virtual" en las funciones que queremos sobreescribir
** 2. Un destructor virtual (para que se destruya correctamente)
**
** Sin virtual: siempre se llama a la funcion del padre (enlace estatico)
** Con virtual: se llama a la funcion del hijo real (enlace dinamico)
*/
#ifndef ANIMAL_HPP
# define ANIMAL_HPP

# include <string>
# include <iostream>

class Animal
{
protected:
	std::string	_type;

public:
	Animal(void);
	Animal(const Animal &src);
	Animal &operator=(const Animal &rhs);
	virtual ~Animal(void);

	virtual void		makeSound(void) const;
	std::string			getType(void) const;
};

#endif
