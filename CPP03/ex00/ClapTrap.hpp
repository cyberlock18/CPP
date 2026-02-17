/*
** INCLUDE GUARDS - Evitan doble inclusion del header
*/
#ifndef CLAPTRAP_HPP
# define CLAPTRAP_HPP

# include <string>
# include <iostream>

/*
** CLASE CLAPTRAP - Robot basico del juego Borderlands
**
** Atributos privados:
** - _name: nombre del robot
** - _hitPoints: vida (10)
** - _energyPoints: energia para atacar o repararse (10)
** - _attackDamage: dano que hace al atacar (0)
**
** ORTHODOX CANONICAL FORM (OCF):
** 1. Constructor por defecto
** 2. Constructor de copia
** 3. Operador de asignacion
** 4. Destructor
*/
class ClapTrap
{
private:
	std::string		_name;
	unsigned int	_hitPoints;
	unsigned int	_energyPoints;
	unsigned int	_attackDamage;

public:
	// ==================== OCF ====================
	ClapTrap(void);
	ClapTrap(std::string name);
	ClapTrap(const ClapTrap &src);
	ClapTrap &operator=(const ClapTrap &rhs);
	~ClapTrap(void);

	// ==================== ACCIONES ====================
	void	attack(const std::string &target);
	void	takeDamage(unsigned int amount);
	void	beRepaired(unsigned int amount);
};

#endif
