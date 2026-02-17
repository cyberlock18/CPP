/*
** SCAVTRAP - Clase derivada de ClapTrap
**
** HERENCIA: ScavTrap hereda de ClapTrap usando "public"
** Esto significa que los miembros public de ClapTrap siguen siendo public
** y los protected siguen siendo protected en ScavTrap
**
** ScavTrap tiene stats diferentes:
** - HP: 100 (en vez de 10)
** - EP: 50 (en vez de 10)
** - AD: 20 (en vez de 0)
**
** Tiene su propia funcion attack() y una habilidad especial: guardGate()
*/
#ifndef SCAVTRAP_HPP
# define SCAVTRAP_HPP

# include "ClapTrap.hpp"

class ScavTrap : public ClapTrap
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
