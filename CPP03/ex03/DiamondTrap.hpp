/*
** DIAMONDTRAP - Herencia multiple (el problema del diamante)
**
** DiamondTrap hereda de FragTrap Y ScavTrap a la vez
** Gracias a la herencia virtual, solo hay UNA copia de ClapTrap
**
** Atributos:
** - _name: nombre propio de DiamondTrap (diferente de ClapTrap::_name)
** - ClapTrap::_name = nombre + "_clap_name"
** - HP: de FragTrap (100)
** - EP: de ScavTrap (50)
** - AD: de FragTrap (30)
** - attack(): de ScavTrap
**
** El "truco" es que DiamondTrap tiene su propio _name
** que "oculta" (shadow) el _name de ClapTrap
** ClapTrap::_name guarda el nombre con sufijo "_clap_name"
*/
#ifndef DIAMONDTRAP_HPP
# define DIAMONDTRAP_HPP

# include "ScavTrap.hpp"
# include "FragTrap.hpp"

class DiamondTrap : public FragTrap, public ScavTrap
{
private:
	std::string	_name;

public:
	DiamondTrap(void);
	DiamondTrap(std::string name);
	DiamondTrap(const DiamondTrap &src);
	DiamondTrap &operator=(const DiamondTrap &rhs);
	~DiamondTrap(void);

	// Usa el attack de ScavTrap
	using ScavTrap::attack;

	void	whoAmI(void);
};

#endif
