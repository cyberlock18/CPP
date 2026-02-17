/*
** FRAGTRAP con HERENCIA VIRTUAL
** Igual que ScavTrap, usa "virtual public ClapTrap"
** para que DiamondTrap solo tenga una copia de ClapTrap
*/
#ifndef FRAGTRAP_HPP
# define FRAGTRAP_HPP

# include "ClapTrap.hpp"

class FragTrap : virtual public ClapTrap
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
