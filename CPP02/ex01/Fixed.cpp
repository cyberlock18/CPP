#include "Fixed.hpp"

// ==================== CONSTRUCTORES ====================

Fixed::Fixed(void) : _value(0)
{
	std::cout << "Default constructor called" << std::endl;
}

/*
** Constructor desde INT
**
** Para convertir int a punto fijo, desplazamos los bits a la izquierda
** n << 8 equivale a n * 256 (2^8)
**
** Ejemplo: Fixed(10)
** 10 << 8 = 10 * 256 = 2560
** Valor real = 2560 / 256 = 10.0
*/
Fixed::Fixed(const int n)
{
	std::cout << "Int constructor called" << std::endl;
	this->_value = n << _fractionalBits;  // n * 2^8
}

/*
** Constructor desde FLOAT
**
** Multiplicamos el float por 256 y redondeamos
** roundf() redondea al entero mas cercano
**
** Ejemplo: Fixed(42.42f)
** 42.42 * 256 = 10859.52
** roundf(10859.52) = 10860
** Valor almacenado = 10860
** Valor real = 10860 / 256 = 42.421875 (aproximado)
*/
Fixed::Fixed(const float f)
{
	std::cout << "Float constructor called" << std::endl;
	// (1 << _fractionalBits) = 2^8 = 256
	this->_value = roundf(f * (1 << _fractionalBits));
}

Fixed::Fixed(const Fixed &src)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = src;
}

Fixed &Fixed::operator=(const Fixed &rhs)
{
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &rhs)
		this->_value = rhs.getRawBits();
	return *this;
}

Fixed::~Fixed(void)
{
	std::cout << "Destructor called" << std::endl;
}

// ==================== GETTERS Y SETTERS ====================

int	Fixed::getRawBits(void) const
{
	// Sin mensaje en este ejercicio (a diferencia del ex00)
	return this->_value;
}

void	Fixed::setRawBits(int const raw)
{
	this->_value = raw;
}

// ==================== CONVERSIONES ====================

/*
** toFloat: convierte el valor interno a float
**
** Dividimos por 256 (2^8) para obtener el valor real
** El cast a (float) es necesario para division decimal
*/
float	Fixed::toFloat(void) const
{
	return (float)this->_value / (1 << _fractionalBits);
}

/*
** toInt: convierte a int (trunca la parte decimal)
**
** Desplazamos bits a la derecha = dividir por 256
** >> descarta los 8 bits fraccionarios
**
** Ejemplo: _value = 10860 (que representa 42.42)
** 10860 >> 8 = 42 (parte entera)
*/
int	Fixed::toInt(void) const
{
	return this->_value >> _fractionalBits;
}

// ==================== OPERADOR << ====================

/*
** Sobrecarga de operator<< para poder hacer cout << fixed
**
** NO es un metodo de la clase (no tiene Fixed::)
** Recibe el stream y el objeto, devuelve el stream
**
** Devolver el stream permite encadenar:
** cout << a << b << c;
*/
std::ostream &operator<<(std::ostream &out, const Fixed &fixed)
{
	out << fixed.toFloat();  // Imprimimos como float
	return out;
}
