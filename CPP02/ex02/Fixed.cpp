#include "Fixed.hpp"

Fixed::Fixed(void) : _value(0)
{
}

Fixed::Fixed(const int n)
{
	this->_value = n << _fractionalBits;
}

Fixed::Fixed(const float f)
{
	this->_value = roundf(f * (1 << _fractionalBits));
}

Fixed::Fixed(const Fixed &src)
{
	*this = src;
}

Fixed &Fixed::operator=(const Fixed &rhs)
{
	if (this != &rhs)
		this->_value = rhs.getRawBits();
	return *this;
}

Fixed::~Fixed(void)
{
}

int	Fixed::getRawBits(void) const
{
	return this->_value;
}

void	Fixed::setRawBits(int const raw)
{
	this->_value = raw;
}

float	Fixed::toFloat(void) const
{
	return (float)this->_value / (1 << _fractionalBits);
}

int	Fixed::toInt(void) const
{
	return this->_value >> _fractionalBits;
}

bool	Fixed::operator>(const Fixed &rhs) const
{
	return this->_value > rhs._value;
}

bool	Fixed::operator<(const Fixed &rhs) const
{
	return this->_value < rhs._value;
}

bool	Fixed::operator>=(const Fixed &rhs) const
{
	return this->_value >= rhs._value;
}

bool	Fixed::operator<=(const Fixed &rhs) const
{
	return this->_value <= rhs._value;
}

bool	Fixed::operator==(const Fixed &rhs) const
{
	return this->_value == rhs._value;
}

bool	Fixed::operator!=(const Fixed &rhs) const
{
	return this->_value != rhs._value;
}

Fixed	Fixed::operator+(const Fixed &rhs) const
{
	return Fixed(this->toFloat() + rhs.toFloat());
}

Fixed	Fixed::operator-(const Fixed &rhs) const
{
	return Fixed(this->toFloat() - rhs.toFloat());
}

Fixed	Fixed::operator*(const Fixed &rhs) const
{
	return Fixed(this->toFloat() * rhs.toFloat());
}

Fixed	Fixed::operator/(const Fixed &rhs) const
{
	return Fixed(this->toFloat() / rhs.toFloat());
}

Fixed	&Fixed::operator++(void)
{
	this->_value++;
	return *this;
}

Fixed	Fixed::operator++(int)
{
	Fixed	tmp(*this);
	this->_value++;
	return tmp;
}

Fixed	&Fixed::operator--(void)
{
	this->_value--;
	return *this;
}

Fixed	Fixed::operator--(int)
{
	Fixed	tmp(*this);
	this->_value--;
	return tmp;
}

Fixed	&Fixed::min(Fixed &a, Fixed &b)
{
	if (a < b)
		return a;
	return b;
}

const Fixed	&Fixed::min(const Fixed &a, const Fixed &b)
{
	if (a < b)
		return a;
	return b;
}

Fixed	&Fixed::max(Fixed &a, Fixed &b)
{
	if (a > b)
		return a;
	return b;
}

const Fixed	&Fixed::max(const Fixed &a, const Fixed &b)
{
	if (a > b)
		return a;
	return b;
}

std::ostream &operator<<(std::ostream &out, const Fixed &fixed)
{
	out << fixed.toFloat();
	return out;
}
