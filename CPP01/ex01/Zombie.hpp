#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>
# include <iostream>

class Zombie
{
private:
	std::string	_name;

public:
	/*
	** Constructor POR DEFECTO (sin parametros)
	** Es necesario para poder crear arrays de objetos con new Zombie[N]
	** porque new[] llama al constructor por defecto para cada elemento
	*/
	Zombie(void);

	~Zombie(void);
	void	announce(void);

	/*
	** setName: como el constructor por defecto no recibe nombre,
	** necesitamos este metodo para asignar el nombre despues
	*/
	void	setName(std::string name);
};

/*
** Crea un ARRAY de N zombies en el heap
** Devuelve un puntero al primer zombie del array
*/
Zombie	*zombieHorde(int N, std::string name);

#endif
