#include "Zombie.hpp"

int	main(void)
{
	int		hordeSize = 5;
	Zombie	*horde;  // Puntero que apuntara al array de zombies

	std::cout << "=== Creando horda de " << hordeSize << " zombies ===" << std::endl;

	// zombieHorde crea 5 zombies en una sola asignacion
	// y devuelve un puntero al primero
	horde = zombieHorde(hordeSize, "ZombieHorda");

	// Comprobamos que la creacion fue exitosa
	if (horde == NULL)
	{
		std::cout << "Error al crear la horda" << std::endl;
		return (1);
	}

	std::cout << std::endl << "=== Zombies anunciandose ===" << std::endl;

	// Recorremos el array usando indices
	// horde[i] accede al zombie en la posicion i
	for (int i = 0; i < hordeSize; i++)
		horde[i].announce();

	std::cout << std::endl << "=== Eliminando horda ===" << std::endl;

	/*
	** delete[] (con corchetes) para arrays
	**
	** IMPORTANTE: Si usamos new[] debemos usar delete[]
	**
	** delete[] hace:
	** 1. Llama al destructor de CADA zombie (5 veces)
	** 2. Libera TODA la memoria del array
	**
	** Si usaramos delete (sin []) solo destruiria el primero
	** y tendriamos memory leak de los otros 4
	*/
	delete[] horde;

	return (0);
}

/*
** DIFERENCIA ENTRE new/delete Y new[]/delete[]
**
** UN objeto:
**     Zombie *z = new Zombie("uno");
**     delete z;
**
** ARRAY de objetos:
**     Zombie *arr = new Zombie[5];
**     delete[] arr;
**
** NUNCA mezcles! new con delete, new[] con delete[]
*/
