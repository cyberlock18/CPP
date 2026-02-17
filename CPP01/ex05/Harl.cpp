#include "Harl.hpp"

Harl::Harl(void)
{
}

Harl::~Harl(void)
{
}

// Metodos privados: cada uno imprime un mensaje diferente
void	Harl::debug(void)
{
	std::cout << "I love having extra bacon for my 7XL-double-cheese-triple-pickle-special-ketchup burger. I really do!" << std::endl;
}

void	Harl::info(void)
{
	std::cout << "I cannot believe adding extra bacon costs more money. You didn't put enough bacon in my burger! If you did, I wouldn't be asking for more!" << std::endl;
}

void	Harl::warning(void)
{
	std::cout << "I think I deserve to have some extra bacon for free. I've been coming for years, whereas you started working here just last month." << std::endl;
}

void	Harl::error(void)
{
	std::cout << "This is unacceptable! I want to speak to the manager now." << std::endl;
}

/*
** complain: llama al metodo correspondiente usando PUNTEROS A FUNCIONES MIEMBRO
**
** SINTAXIS DE PUNTERO A FUNCION MIEMBRO:
**
** Declaracion:
**     void (Harl::*ptr)(void)
**     ^     ^     ^     ^
**     |     |     |     +-- Parametros de la funcion
**     |     |     +-- Nombre del puntero
**     |     +-- Clase a la que pertenece
**     +-- Tipo de retorno
**
** Inicializacion:
**     ptr = &Harl::debug;
**
** Llamada:
**     (this->*ptr)();
**     ^       ^
**     |       +-- Desreferenciamos el puntero
**     +-- Sobre que objeto llamamos la funcion
*/
void	Harl::complain(std::string level)
{
	/*
	** Array de punteros a funciones miembro
	** Cada elemento apunta a un metodo diferente de Harl
	*/
	void		(Harl::*functions[4])(void) = {
		&Harl::debug,
		&Harl::info,
		&Harl::warning,
		&Harl::error
	};

	// Array de strings con los nombres de los niveles (en el mismo orden)
	std::string	levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};

	// Buscamos el nivel correspondiente
	for (int i = 0; i < 4; i++)
	{
		if (levels[i] == level)
		{
			/*
			** Llamamos a la funcion usando el puntero
			**
			** (this->*functions[i])()
			**
			** this->*functions[i] = accedemos al puntero a funcion
			** () = llamamos a la funcion
			** Los parentesis externos son necesarios por precedencia
			*/
			(this->*functions[i])();
			return ;
		}
	}
	// Si no encontramos el nivel, no hacemos nada
}

/*
** POR QUE PUNTEROS A FUNCIONES Y NO IF/ELSE?
**
** El ejercicio PROHIBE usar if/else if/else para elegir la funcion.
** Los punteros a funciones son mas elegantes y escalables:
**
** MAL (prohibido):
**     if (level == "DEBUG")
**         debug();
**     else if (level == "INFO")
**         info();
**     ...
**
** BIEN (punteros a funciones):
**     - Definimos array de funciones
**     - Buscamos indice por nombre
**     - Llamamos funcion[indice]
*/
