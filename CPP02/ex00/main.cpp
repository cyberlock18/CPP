#include <iostream>
#include "Fixed.hpp"

int main(void)
{
	// Constructor por defecto
	Fixed a;           // "Default constructor called"

	// Constructor de copia (creamos b copiando a)
	Fixed b(a);        // "Copy constructor called"

	// Constructor por defecto
	Fixed c;           // "Default constructor called"

	// Operador de asignacion (c ya existe, le asignamos b)
	c = b;             // "Copy assignment operator called"

	// Mostramos los valores (todos son 0)
	std::cout << a.getRawBits() << std::endl;  // "getRawBits..." + 0
	std::cout << b.getRawBits() << std::endl;  // "getRawBits..." + 0
	std::cout << c.getRawBits() << std::endl;  // "getRawBits..." + 0

	return 0;
	// Aqui se destruyen c, b, a (en orden inverso)
	// "Destructor called" x3
}

/*
** FLUJO DE EJECUCION:
**
** Fixed a;         --> Default constructor
** Fixed b(a);      --> Copy constructor
**                      --> Copy assignment (dentro del copy constructor)
**                      --> getRawBits (para copiar el valor)
** Fixed c;         --> Default constructor
** c = b;           --> Copy assignment
**                      --> getRawBits
** a.getRawBits()   --> getRawBits, imprime 0
** b.getRawBits()   --> getRawBits, imprime 0
** c.getRawBits()   --> getRawBits, imprime 0
** }                --> Destructor x3
*/
