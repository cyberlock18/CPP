/*
** INCLUDE GUARDS - Evitan que el archivo se incluya mas de una vez
** Si ZOMBIE_HPP no esta definido, lo define y procesa el contenido
** Si ya esta definido, ignora todo el contenido (evita errores de redefinicion)
*/
#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>   // Para usar std::string
# include <iostream> // Para usar std::cout

/*
** CLASE ZOMBIE
** Una clase es como un "molde" para crear objetos
** Tiene atributos (variables) y metodos (funciones)
*/
class Zombie
{
/*
** PRIVATE: Solo accesible desde dentro de la clase
** Nadie de fuera puede tocar _name directamente
*/
private:
	std::string	_name;  // Atributo privado: nombre del zombie
	                    // El guion bajo _ es convencion de 42 para atributos privados

/*
** PUBLIC: Accesible desde cualquier parte del codigo
*/
public:
	// Constructor: se llama automaticamente al crear un objeto
	// Recibe el nombre como parametro
	Zombie(std::string name);

	// Destructor: se llama automaticamente cuando el objeto se destruye
	// El ~ indica que es un destructor
	~Zombie(void);

	// Metodo para que el zombie se anuncie
	void	announce(void);
};

/*
** FUNCIONES EXTERNAS (no son metodos de la clase)
** Estas funciones crean zombies de diferentes maneras
*/

// Crea un zombie en el HEAP (memoria dinamica) y devuelve un puntero
Zombie	*newZombie(std::string name);

// Crea un zombie en el STACK (memoria automatica) - temporal
void	randomChump(std::string name);

#endif
