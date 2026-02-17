#include "Zombie.hpp"

/*
** newZombie - Crea un zombie en el HEAP (memoria dinamica)
**
** HEAP vs STACK:
** - HEAP: memoria que TU controlas con new/delete
**   El objeto existe hasta que TU lo destruyas con delete
**   Devuelve un PUNTERO al objeto
**
** - STACK: memoria automatica gestionada por el programa
**   El objeto se destruye automaticamente al salir del ambito {}
**
** Usamos HEAP cuando queremos que el objeto sobreviva
** fuera de la funcion donde se creo
*/
Zombie	*newZombie(std::string name)
{
	// new = reserva memoria en el HEAP y llama al constructor
	// Devuelve un puntero a la memoria reservada
	Zombie	*zombie = new Zombie(name);

	// Devolvemos el puntero - el zombie sigue vivo fuera de esta funcion
	// IMPORTANTE: quien llame a esta funcion debe hacer delete despues!
	return (zombie);
}
