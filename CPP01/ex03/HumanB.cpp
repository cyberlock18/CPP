#include "HumanB.hpp"

/*
** Constructor: inicializa sin arma (NULL)
** _weapon(NULL) significa que no tiene arma al principio
*/
HumanB::HumanB(std::string name) : _name(name), _weapon(NULL)
{
}

HumanB::~HumanB(void)
{
}

/*
** setWeapon: asigna un arma al humano
**
** Recibe una REFERENCIA pero guardamos la DIRECCION
** &weapon nos da la direccion de la referencia
**
** Esto permite que si el weapon original cambia,
** HumanB vea los cambios (porque apunta al mismo objeto)
*/
void	HumanB::setWeapon(Weapon &weapon)
{
	_weapon = &weapon;  // Guardamos la direccion
}

/*
** attack: muestra el ataque
**
** IMPORTANTE: debemos comprobar si _weapon es NULL
** porque HumanB puede no tener arma
**
** Usamos -> porque _weapon es un PUNTERO
*/
void	HumanB::attack(void) const
{
	if (_weapon)  // Si tiene arma (no es NULL)
		std::cout << _name << " attacks with their " << _weapon->getType() << std::endl;
	else
		std::cout << _name << " has no weapon to attack" << std::endl;
}
