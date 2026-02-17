#include "ClapTrap.hpp"

/*
** CONSTRUCTOR POR DEFECTO
** Inicializa con valores por defecto y nombre vacio
*/
ClapTrap::ClapTrap(void) : _name("default"), _hitPoints(10), _energyPoints(10), _attackDamage(0)
{
	std::cout << "ClapTrap " << _name << " constructed" << std::endl;
}

/*
** CONSTRUCTOR CON NOMBRE
** Recibe el nombre como parametro y asigna los valores por defecto
*/
ClapTrap::ClapTrap(std::string name) : _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0)
{
	std::cout << "ClapTrap " << _name << " constructed" << std::endl;
}

/*
** CONSTRUCTOR DE COPIA
** Copia todos los atributos del objeto src
*/
ClapTrap::ClapTrap(const ClapTrap &src)
{
	std::cout << "ClapTrap copy constructor called" << std::endl;
	*this = src;
}

/*
** OPERADOR DE ASIGNACION
** Copia los atributos si no es auto-asignacion
*/
ClapTrap &ClapTrap::operator=(const ClapTrap &rhs)
{
	std::cout << "ClapTrap assignment operator called" << std::endl;
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
*/
ClapTrap::~ClapTrap(void)
{
	std::cout << "ClapTrap " << _name << " destroyed" << std::endl;
}

/*
** ATTACK
** Ataca al objetivo causando _attackDamage puntos de dano
** Cuesta 1 punto de energia
** No puede atacar si no tiene vida o energia
*/
void	ClapTrap::attack(const std::string &target)
{
	if (_hitPoints == 0)
	{
		std::cout << "ClapTrap " << _name << " can't attack, no hit points!" << std::endl;
		return ;
	}
	if (_energyPoints == 0)
	{
		std::cout << "ClapTrap " << _name << " can't attack, no energy points!" << std::endl;
		return ;
	}
	_energyPoints--;
	std::cout << "ClapTrap " << _name << " attacks " << target
		<< ", causing " << _attackDamage << " points of damage!" << std::endl;
}

/*
** TAKE DAMAGE
** Recibe dano y pierde puntos de vida
** Si el dano supera la vida, la vida se queda en 0
*/
void	ClapTrap::takeDamage(unsigned int amount)
{
	if (_hitPoints == 0)
	{
		std::cout << "ClapTrap " << _name << " is already dead!" << std::endl;
		return ;
	}
	if (amount >= _hitPoints)
		_hitPoints = 0;
	else
		_hitPoints -= amount;
	std::cout << "ClapTrap " << _name << " takes " << amount
		<< " points of damage! HP: " << _hitPoints << std::endl;
}

/*
** BE REPAIRED
** Se repara ganando 'amount' puntos de vida
** Cuesta 1 punto de energia
*/
void	ClapTrap::beRepaired(unsigned int amount)
{
	if (_hitPoints == 0)
	{
		std::cout << "ClapTrap " << _name << " can't repair, no hit points!" << std::endl;
		return ;
	}
	if (_energyPoints == 0)
	{
		std::cout << "ClapTrap " << _name << " can't repair, no energy points!" << std::endl;
		return ;
	}
	_energyPoints--;
	_hitPoints += amount;
	std::cout << "ClapTrap " << _name << " repairs itself, recovering "
		<< amount << " hit points! HP: " << _hitPoints << std::endl;
}
