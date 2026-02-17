#include "HumanA.hpp"

/*
** Constructor con lista de inicializacion
**
** : _name(name), _weapon(weapon)
**
** IMPORTANTE: _weapon es una referencia, DEBE inicializarse aqui
** No puedes hacer _weapon = weapon; en el cuerpo porque las referencias
** deben inicializarse en la declaracion (o lista de inicializacion)
*/
HumanA::HumanA(std::string name, Weapon &weapon) : _name(name), _weapon(weapon)
{
}

HumanA::~HumanA(void)
{
}

/*
** attack: muestra el ataque
** Usa _weapon.getType() porque _weapon es una REFERENCIA (no puntero)
** Con referencia usamos . (punto)
** Con puntero usariamos -> (flecha)
*/
void	HumanA::attack(void) const
{
	std::cout << _name << " attacks with their " << _weapon.getType() << std::endl;
}
