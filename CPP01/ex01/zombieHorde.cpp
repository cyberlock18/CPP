#include "Zombie.hpp"

/*
** zombieHorde - Crea N zombies en UNA SOLA asignacion de memoria
**
** new Zombie[N] reserva espacio para N objetos Zombie CONTIGUOS en memoria
** Es como un array pero en el heap
**
** IMPORTANTE:
** - new Zombie[N] llama al constructor POR DEFECTO de cada zombie
** - Por eso necesitamos Zombie() sin parametros
** - Luego usamos setName() para darles nombre
**
** LIBERACION:
** - Como usamos new[], debemos usar delete[] (con corchetes)
** - delete[] llama al destructor de CADA elemento del array
*/
Zombie	*zombieHorde(int N, std::string name)
{
	// Proteccion contra valores invalidos
	if (N <= 0)
		return (NULL);

	// new Zombie[N] = crea array de N zombies en el heap
	// horde apunta al PRIMER zombie del array
	Zombie	*horde = new Zombie[N];

	// Asignamos el nombre a cada zombie
	// horde[i] accede al zombie en la posicion i
	for (int i = 0; i < N; i++)
		horde[i].setName(name);

	// Devolvemos puntero al primer elemento
	return (horde);
}

/*
** MEMORIA:
**
** Despues de new Zombie[5], la memoria se ve asi:
**
** horde ---> [Zombie0][Zombie1][Zombie2][Zombie3][Zombie4]
**            ^
**            | horde apunta aqui (al primero)
**
** horde[0] = primer zombie
** horde[1] = segundo zombie
** etc.
*/
