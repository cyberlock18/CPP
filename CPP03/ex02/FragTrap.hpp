/*
** FRAGTRAP - Otra clase derivada de ClapTrap
**
** Muy similar a ScavTrap pero con stats diferentes:
** - HP: 100
** - EP: 100
** - AD: 30
**
** Habilidad especial: highFivesGuys()
*/
#ifndef FRAGTRAP_HPP
# define FRAGTRAP_HPP

# include "ClapTrap.hpp"

class FragTrap : public ClapTrap
{
public:
	FragTrap(void);
	FragTrap(std::string name);
	FragTrap(const FragTrap &src);
	FragTrap &operator=(const FragTrap &rhs);
	~FragTrap(void);

	void	highFivesGuys(void);
};

#endif
