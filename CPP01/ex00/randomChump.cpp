#include "Zombie.hpp"

/*
** randomChump - Crea un zombie en el STACK (memoria automatica)
**
** El zombie se crea, hace su announce, y se DESTRUYE AUTOMATICAMENTE
** cuando la funcion termina (sale del ambito {})
**
** No necesitamos delete porque el STACK se limpia solo
** No devolvemos nada porque el zombie no sobrevive fuera de la funcion
*/
void	randomChump(std::string name)
{
	// Sin "new" = se crea en el STACK
	// El zombie existe solo dentro de estas llaves {}
	Zombie	zombie(name);

	// El zombie hace su anuncio
	zombie.announce();

	// Aqui termina la funcion
	// El destructor de zombie se llama AUTOMATICAMENTE
	// Veras "zombie is dead" en la consola
}
