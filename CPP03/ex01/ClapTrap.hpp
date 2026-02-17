/*
** CLAPTRAP - Clase base para herencia
**
** Los atributos son PROTECTED (no private) para que las clases
** hijas (ScavTrap, FragTrap...) puedan acceder a ellos directamente
** Protected = accesible por la clase y sus hijas, pero no desde fuera
*/
#ifndef CLAPTRAP_HPP
# define CLAPTRAP_HPP

# include <string>
# include <iostream>

class ClapTrap
{
protected:
	std::string		_name;
	unsigned int	_hitPoints;
	unsigned int	_energyPoints;
	unsigned int	_attackDamage;

public:
	ClapTrap(void);
	ClapTrap(std::string name);
	ClapTrap(const ClapTrap &src);
	ClapTrap &operator=(const ClapTrap &rhs);
	~ClapTrap(void);

	void	attack(const std::string &target);
	void	takeDamage(unsigned int amount);
	void	beRepaired(unsigned int amount);
};

#endif
