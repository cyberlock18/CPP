#include "Zombie.hpp"

/*
** CONSTRUCTOR con lista de inicializacion
** Zombie::Zombie significa "el constructor Zombie de la clase Zombie"
**
** : _name(name) es una LISTA DE INICIALIZACION
** Es la forma correcta de inicializar atributos en C++
** Equivale a: this->_name = name; pero es mas eficiente
*/
Zombie::Zombie(std::string name) : _name(name)
{
	// El cuerpo puede estar vacio si solo inicializamos atributos
}

/*
** DESTRUCTOR
** Se llama automaticamente cuando:
** - Un objeto en STACK sale de su ambito (scope)
** - Un objeto en HEAP es eliminado con delete
**
** Aqui imprimimos un mensaje para ver cuando se destruye
*/
Zombie::~Zombie(void)
{
	std::cout << _name << " is dead" << std::endl;
}

/*
** METODO announce
** void = no devuelve nada
** Zombie:: indica que pertenece a la clase Zombie
*/
void	Zombie::announce(void)
{
	// _name es accesible porque estamos dentro de la clase
	std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
