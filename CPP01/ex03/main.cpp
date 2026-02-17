#include "Weapon.hpp"
#include "HumanA.hpp"
#include "HumanB.hpp"

int main()
{
	/*
	** CASO 1: HumanA con REFERENCIA
	**
	** Bob SIEMPRE tiene un arma (referencia)
	** El arma se pasa en el constructor
	*/
	{
		Weapon club = Weapon("crude spiked club");

		// Bob recibe el arma en el constructor (obligatorio)
		HumanA bob("Bob", club);
		bob.attack();  // "Bob attacks with their crude spiked club"

		// Cambiamos el tipo del arma ORIGINAL
		club.setType("some other type of club");

		// Bob ve el cambio porque tiene una REFERENCIA al arma
		// No una copia!
		bob.attack();  // "Bob attacks with their some other type of club"
	}

	/*
	** CASO 2: HumanB con PUNTERO
	**
	** Jim puede NO tener arma (puntero puede ser NULL)
	** El arma se asigna despues con setWeapon()
	*/
	{
		Weapon club = Weapon("crude spiked club");

		// Jim se crea SIN arma
		HumanB jim("Jim");

		// Asignamos el arma despues
		jim.setWeapon(club);
		jim.attack();  // "Jim attacks with their crude spiked club"

		// Cambiamos el tipo del arma ORIGINAL
		club.setType("some other type of club");

		// Jim tambien ve el cambio porque guarda la DIRECCION del arma
		jim.attack();  // "Jim attacks with their some other type of club"
	}
	return 0;
}

/*
** RESUMEN: REFERENCIA vs PUNTERO para atributos
**
** USA REFERENCIA cuando:
** - El objeto SIEMPRE necesita ese atributo
** - Se inicializa en el constructor y no cambia
** - Ejemplo: HumanA siempre tiene arma
**
** USA PUNTERO cuando:
** - El atributo es OPCIONAL (puede ser NULL)
** - Se asigna despues de crear el objeto
** - Puede cambiar a lo largo de la vida del objeto
** - Ejemplo: HumanB puede no tener arma
*/
