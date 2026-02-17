#ifndef WEAPON_HPP
# define WEAPON_HPP

# include <string>

class Weapon
{
private:
	std::string	_type;  // Tipo de arma (ej: "espada", "maza")

public:
	Weapon(std::string type);
	~Weapon(void);

	/*
	** getType devuelve una REFERENCIA CONSTANTE al tipo
	**
	** const std::string &getType() const;
	**        ^            ^           ^
	**        |            |           |
	**        |            |           +-- Este const dice "este metodo no modifica el objeto"
	**        |            +-- El & dice "devuelve referencia, no copia"
	**        +-- Este const dice "la referencia devuelta no puede modificarse"
	**
	** Por que referencia constante?
	** - Eficiencia: no copiamos el string, solo devolvemos una referencia
	** - Seguridad: const evita que modifiquen el _type desde fuera
	*/
	const std::string	&getType(void) const;

	// Setter: permite cambiar el tipo de arma
	void				setType(std::string type);
};

#endif
