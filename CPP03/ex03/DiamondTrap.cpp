#include "DiamondTrap.hpp"

/*
** CONSTRUCTOR POR DEFECTO
**
** Con herencia virtual, DiamondTrap es responsable de llamar
** al constructor de ClapTrap directamente (no lo hacen FragTrap/ScavTrap)
**
** Orden de construccion:
** 1. ClapTrap (la base virtual, una sola vez)
** 2. FragTrap (primer padre en la declaracion)
** 3. ScavTrap (segundo padre)
** 4. DiamondTrap (la clase final)
*/
DiamondTrap::DiamondTrap(void) : ClapTrap("default_clap_name"), FragTrap(), ScavTrap()
{
	_name = "default";
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 30;
	std::cout << "DiamondTrap " << _name << " constructed" << std::endl;
}

/*
** CONSTRUCTOR CON NOMBRE
** ClapTrap recibe el nombre + "_clap_name"
** DiamondTrap guarda el nombre original en su propio _name
*/
DiamondTrap::DiamondTrap(std::string name) : ClapTrap(name + "_clap_name"), FragTrap(name), ScavTrap(name)
{
	_name = name;
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 30;
	std::cout << "DiamondTrap " << _name << " constructed" << std::endl;
}

DiamondTrap::DiamondTrap(const DiamondTrap &src) : ClapTrap(src), FragTrap(src), ScavTrap(src)
{
	std::cout << "DiamondTrap copy constructor called" << std::endl;
	*this = src;
}

DiamondTrap &DiamondTrap::operator=(const DiamondTrap &rhs)
{
	std::cout << "DiamondTrap assignment operator called" << std::endl;
	if (this != &rhs)
	{
		ClapTrap::_name = rhs.ClapTrap::_name;
		_name = rhs._name;
		_hitPoints = rhs._hitPoints;
		_energyPoints = rhs._energyPoints;
		_attackDamage = rhs._attackDamage;
	}
	return *this;
}

/*
** DESTRUCTOR
** Orden de destruccion inverso a la construccion:
** DiamondTrap -> ScavTrap -> FragTrap -> ClapTrap
*/
DiamondTrap::~DiamondTrap(void)
{
	std::cout << "DiamondTrap " << _name << " destroyed" << std::endl;
}

/*
** WHOAMI - Habilidad especial de DiamondTrap
** Muestra su nombre propio y el nombre de ClapTrap
** Para acceder al _name de ClapTrap usamos ClapTrap::_name
** porque DiamondTrap tiene su propio _name que lo "oculta"
*/
void	DiamondTrap::whoAmI(void)
{
	std::cout << "I am " << _name << " and my ClapTrap name is "
		<< ClapTrap::_name << std::endl;
}
