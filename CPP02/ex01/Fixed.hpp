#ifndef FIXED_HPP
# define FIXED_HPP

# include <iostream>
# include <cmath>      // Para roundf()

class Fixed
{
private:
	int					_value;
	static const int	_fractionalBits = 8;

public:
	// ==================== ORTHODOX CANONICAL FORM ====================
	Fixed(void);                        // Constructor por defecto
	Fixed(const Fixed &src);            // Constructor de copia
	Fixed &operator=(const Fixed &rhs); // Operador de asignacion
	~Fixed(void);                       // Destructor

	// ==================== CONSTRUCTORES ADICIONALES ====================
	/*
	** Constructor desde INT
	** Convierte un entero a punto fijo
	** Ejemplo: Fixed(10) --> _value = 10 * 256 = 2560
	*/
	Fixed(const int n);

	/*
	** Constructor desde FLOAT
	** Convierte un float a punto fijo
	** Ejemplo: Fixed(10.5f) --> _value = roundf(10.5 * 256) = 2688
	*/
	Fixed(const float f);

	// ==================== GETTERS Y SETTERS ====================
	int		getRawBits(void) const;
	void	setRawBits(int const raw);

	// ==================== CONVERSIONES ====================
	float	toFloat(void) const;  // Convierte a float
	int		toInt(void) const;    // Convierte a int (trunca decimales)
};

/*
** SOBRECARGA DE operator<<
** Permite hacer: std::cout << miFixed;
** Es una funcion EXTERNA (no metodo de la clase)
*/
std::ostream &operator<<(std::ostream &out, const Fixed &fixed);

#endif
