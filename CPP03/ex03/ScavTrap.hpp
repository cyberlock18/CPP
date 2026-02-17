/*
** SCAVTRAP con HERENCIA VIRTUAL
**
** "virtual public ClapTrap" = herencia virtual
** Esto es necesario para el problema del diamante (DiamondTrap)
**
** Sin virtual: DiamondTrap tendria DOS copias de ClapTrap
**        ClapTrap    ClapTrap
**           |            |
**        ScavTrap    FragTrap
**            \        /
**           DiamondTrap    <- DOS ClapTrap! Ambiguo!
**
** Con virtual: DiamondTrap tiene UNA SOLA copia de ClapTrap
**           ClapTrap
**          /        \
**      ScavTrap   FragTrap
**          \        /
**         DiamondTrap      <- UN solo ClapTrap!
*/
#ifndef SCAVTRAP_HPP
# define SCAVTRAP_HPP

# include "ClapTrap.hpp"

class ScavTrap : virtual public ClapTrap
{
public:
	ScavTrap(void);
	ScavTrap(std::string name);
	ScavTrap(const ScavTrap &src);
	ScavTrap &operator=(const ScavTrap &rhs);
	~ScavTrap(void);

	void	attack(const std::string &target);
	void	guardGate(void);
};

#endif
