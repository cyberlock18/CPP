#include "ScavTrap.hpp"

/*
** CONSTRUCTOR POR DEFECTO
** Primero se llama al constructor de ClapTrap (la clase padre)
** Despues se sobreescriben los valores con los de ScavTrap
*/
ScavTrap::ScavTrap(void) : ClapTrap()
{
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	std::cout << "ScavTrap " << _name << " constructed" << std::endl;
}

/*
** CONSTRUCTOR CON NOMBRE
** Llama al constructor de ClapTrap con el nombre
** y despues pone los valores propios de ScavTrap
*/
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
{
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	std::cout << "ScavTrap " << _name << " constructed" << std::endl;
}

/*
** CONSTRUCTOR DE COPIA
*/
ScavTrap::ScavTrap(const ScavTrap &src) : ClapTrap(src)
{
	std::cout << "ScavTrap copy constructor called" << std::endl;
	*this = src;
}

/*
** OPERADOR DE ASIGNACION
*/
ScavTrap &ScavTrap::operator=(const ScavTrap &rhs)
{
	std::cout << "ScavTrap assignment operator called" << std::endl;
	if (this != &rhs)
	{
		_name = rhs._name;
		_hitPoints = rhs._hitPoints;
		_energyPoints = rhs._energyPoints;
		_attackDamage = rhs._attackDamage;
	}
	return *this;
}

/*
** DESTRUCTOR
** Se llama antes que el destructor de ClapTrap
** (destruccion en orden inverso a la construccion)
*/
ScavTrap::~ScavTrap(void)
{
	std::cout << "ScavTrap " << _name << " destroyed" << std::endl;
}

/*
** ATTACK - Version propia de ScavTrap
** Mismo comportamiento pero mensaje diferente
*/
void	ScavTrap::attack(const std::string &target)
{
	if (_hitPoints == 0)
	{
		std::cout << "ScavTrap " << _name << " can't attack, no hit points!" << std::endl;
		return ;
	}
	if (_energyPoints == 0)
	{
		std::cout << "ScavTrap " << _name << " can't attack, no energy points!" << std::endl;
		return ;
	}
	_energyPoints--;
	std::cout << "ScavTrap " << _name << " attacks " << target
		<< ", causing " << _attackDamage << " points of damage!" << std::endl;
}

/*
** GUARDGATE - Habilidad especial de ScavTrap
** Activa el modo guardabarrera
*/
void	ScavTrap::guardGate(void)
{
	std::cout << "ScavTrap " << _name << " is now in Gate keeper mode" << std::endl;
}
