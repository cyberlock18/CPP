#include "Weapon.hpp"

// Constructor: inicializa _type con el parametro recibido
Weapon::Weapon(std::string type) : _type(type)
{
}

Weapon::~Weapon(void)
{
}

/*
** getType: devuelve referencia constante al tipo
** No crea copia del string, es mas eficiente
*/
const std::string	&Weapon::getType(void) const
{
	return (_type);
}

// setType: cambia el tipo de arma
void	Weapon::setType(std::string type)
{
	_type = type;
}
