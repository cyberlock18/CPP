#include "Fixed.hpp"

/*
** CONSTRUCTOR POR DEFECTO
** Inicializa _value a 0 usando lista de inicializacion
*/
Fixed::Fixed(void) : _value(0)
{
	std::cout << "Default constructor called" << std::endl;
}

/*
** CONSTRUCTOR DE COPIA
** Se llama cuando creamos un objeto a partir de otro:
**     Fixed b(a);    // Copia a en b
**     Fixed b = a;   // Tambien llama al constructor de copia
**
** Recibe una REFERENCIA CONSTANTE al objeto original
** - Referencia: para no copiar (eficiencia)
** - Constante: para no modificar el original
*/
Fixed::Fixed(const Fixed &src)
{
	std::cout << "Copy constructor called" << std::endl;
	// Reutilizamos el operador de asignacion
	*this = src;
}

/*
** OPERADOR DE ASIGNACION (operator=)
** Se llama cuando asignamos un objeto a otro YA EXISTENTE:
**     Fixed c;
**     c = b;    // c ya existe, se llama operator=
**
** Diferencia con constructor de copia:
** - Constructor copia: el objeto NO existe todavia
** - Operador =: el objeto YA existe
**
** rhs = "right hand side" (lado derecho de la asignacion)
**
** Devolvemos *this para permitir asignaciones encadenadas:
**     a = b = c;  // Funciona porque b = c devuelve b
*/
Fixed &Fixed::operator=(const Fixed &rhs)
{
	std::cout << "Copy assignment operator called" << std::endl;

	// Proteccion contra auto-asignacion: a = a
	// Si son el mismo objeto, no hacemos nada
	if (this != &rhs)
		this->_value = rhs.getRawBits();

	// Devolvemos referencia a nosotros mismos
	return *this;
}

/*
** DESTRUCTOR
** Se llama cuando el objeto se destruye
*/
Fixed::~Fixed(void)
{
	std::cout << "Destructor called" << std::endl;
}

/*
** getRawBits: devuelve el valor interno
** const al final = este metodo NO modifica el objeto
*/
int	Fixed::getRawBits(void) const
{
	std::cout << "getRawBits member function called" << std::endl;
	return this->_value;
}

/*
** setRawBits: establece el valor interno
** int const raw = el parametro no se puede modificar dentro de la funcion
*/
void	Fixed::setRawBits(int const raw)
{
	this->_value = raw;
}
