/*
** CLASE BRAIN - Cerebro con 100 ideas
**
** Es un recurso que Dog y Cat tendran como puntero (Brain*)
** Se crea con new y se destruye con delete
**
** La copia debe ser PROFUNDA (deep copy):
** - No copiar el puntero (eso seria shallow copy, ambos apuntan al mismo Brain)
** - Copiar el contenido (crear un Brain nuevo con las mismas ideas)
*/
#ifndef BRAIN_HPP
# define BRAIN_HPP

# include <string>
# include <iostream>

class Brain
{
public:
	std::string	ideas[100];

	Brain(void);
	Brain(const Brain &src);
	Brain &operator=(const Brain &rhs);
	~Brain(void);
};

#endif
