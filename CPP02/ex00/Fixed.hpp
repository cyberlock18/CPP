#ifndef FIXED_HPP
# define FIXED_HPP

# include <iostream>

/*
** CLASE FIXED - Numero de punto fijo
**
** QUE ES PUNTO FIJO?
** Es una forma de representar decimales usando enteros.
** Dividimos el entero en dos partes:
** - Parte entera (bits altos)
** - Parte fraccionaria (bits bajos)
**
** Con 8 bits fraccionarios:
** El valor real = _value / 256 (2^8 = 256)
**
** ORTHODOX CANONICAL FORM (OCF)
** Toda clase debe tener estos 4 elementos:
** 1. Constructor por defecto
** 2. Constructor de copia
** 3. Operador de asignacion
** 4. Destructor
*/
class Fixed
{
private:
	int					_value;         // Valor interno (entero que representa el punto fijo)

	/*
	** static const = constante compartida por TODAS las instancias
	** No ocupa memoria por cada objeto, solo existe una vez
	** 8 bits fraccionarios = precision de 1/256 = 0.00390625
	*/
	static const int	_fractionalBits = 8;

public:
	// ==================== ORTHODOX CANONICAL FORM ====================

	Fixed(void);                        // 1. Constructor por defecto
	Fixed(const Fixed &src);            // 2. Constructor de copia
	Fixed &operator=(const Fixed &rhs); // 3. Operador de asignacion
	~Fixed(void);                       // 4. Destructor

	// ==================== GETTERS Y SETTERS ====================

	int		getRawBits(void) const;     // Devuelve el valor crudo
	void	setRawBits(int const raw);  // Establece el valor crudo
};

#endif
