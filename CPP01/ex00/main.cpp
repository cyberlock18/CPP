#include "Zombie.hpp"

int	main(void)
{
	// Declaramos un PUNTERO a Zombie (no el zombie en si)
	// Un puntero guarda una direccion de memoria
	Zombie	*heapZombie;

	std::cout << "=== Zombie en heap (newZombie) ===" << std::endl;

	// newZombie crea el zombie con NEW (en el heap)
	// y nos devuelve un puntero a el
	heapZombie = newZombie("HeapZombie");

	// Usamos -> para acceder a metodos a traves de un puntero
	// heapZombie->announce() es lo mismo que (*heapZombie).announce()
	heapZombie->announce();

	// MUY IMPORTANTE: debemos liberar la memoria manualmente
	// delete llama al destructor y libera la memoria
	// Si no hacemos delete = MEMORY LEAK (fuga de memoria)
	delete heapZombie;

	std::cout << std::endl;
	std::cout << "=== Zombie en stack (randomChump) ===" << std::endl;

	// randomChump crea el zombie SIN new (en el stack)
	// El zombie se crea, anuncia, y se destruye TODO dentro de la funcion
	// No necesitamos delete, no devuelve nada
	randomChump("StackZombie");

	std::cout << std::endl;
	std::cout << "=== Fin del programa ===" << std::endl;
	return (0);
}

/*
** RESUMEN: Cuando usar HEAP vs STACK?
**
** USA STACK cuando:
** - El objeto es temporal y solo se usa en una funcion
** - No necesitas que sobreviva fuera del ambito actual
** - Ejemplo: variables locales, objetos temporales
**
** USA HEAP cuando:
** - Necesitas que el objeto sobreviva fuera de la funcion
** - No sabes el tamano hasta tiempo de ejecucion
** - El objeto es muy grande
** - Ejemplo: devolver un objeto creado en una funcion
*/
