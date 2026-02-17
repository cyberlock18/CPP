#include "Zombie.hpp"

/*
** Constructor por defecto
** No hace nada, pero es OBLIGATORIO para poder usar new Zombie[N]
** Sin este constructor, el compilador no sabria como crear cada zombie del array
*/
Zombie::Zombie(void)
{
	// Vacio - el nombre se asignara despues con setName
}

Zombie::~Zombie(void)
{
	std::cout << _name << " is dead" << std::endl;
}

void	Zombie::announce(void)
{
	std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

/*
** Setter para el nombre
** Un "setter" es un metodo que permite modificar un atributo privado
** desde fuera de la clase
*/
void	Zombie::setName(std::string name)
{
	_name = name;
}
