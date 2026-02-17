#ifndef HUMANA_HPP
# define HUMANA_HPP

# include <string>
# include <iostream>
# include "Weapon.hpp"

/*
** HumanA: SIEMPRE tiene un arma
**
** Usa REFERENCIA para el arma porque:
** - Siempre tendra un arma (no puede ser NULL)
** - Se inicializa en el constructor (obligatorio para referencias)
** - Si el arma original cambia, HumanA ve el cambio
*/
class HumanA
{
private:
	std::string	_name;
	Weapon		&_weapon;  // REFERENCIA: siempre apunta al mismo Weapon

public:
	/*
	** Constructor DEBE recibir el arma porque _weapon es referencia
	** Las referencias DEBEN inicializarse al crear el objeto
	*/
	HumanA(std::string name, Weapon &weapon);
	~HumanA(void);
	void	attack(void) const;
};

#endif
