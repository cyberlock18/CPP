#ifndef HUMANB_HPP
# define HUMANB_HPP

# include <string>
# include <iostream>
# include "Weapon.hpp"

/*
** HumanB: PUEDE o NO tener un arma
**
** Usa PUNTERO para el arma porque:
** - Puede no tener arma (NULL)
** - El arma se asigna despues de crear el objeto
** - Puede cambiar de arma
*/
class HumanB
{
private:
	std::string	_name;
	Weapon		*_weapon;  // PUNTERO: puede ser NULL, puede cambiar

public:
	/*
	** Constructor NO recibe arma
	** El puntero se inicializa a NULL
	*/
	HumanB(std::string name);
	~HumanB(void);

	// Metodo para asignar un arma despues
	void	setWeapon(Weapon &weapon);
	void	attack(void) const;
};

#endif
