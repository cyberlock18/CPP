#ifndef HARL_HPP
# define HARL_HPP

# include <string>
# include <iostream>

class Harl
{
/*
** METODOS PRIVADOS: solo accesibles desde dentro de la clase
** Cada uno imprime un tipo de queja diferente
*/
private:
	void	debug(void);
	void	info(void);
	void	warning(void);
	void	error(void);

public:
	Harl(void);
	~Harl(void);

	/*
	** complain: llama al metodo correspondiente segun el nivel
	** Usa PUNTEROS A FUNCIONES MIEMBRO (concepto clave del ejercicio)
	*/
	void	complain(std::string level);
};

#endif
