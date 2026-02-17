#include <iostream>
#include <string>

int	main(void)
{
	/*
	** Variable normal: guarda el valor directamente
	*/
	std::string		str = "HI THIS IS BRAIN";

	/*
	** PUNTERO: variable que guarda una DIRECCION de memoria
	**
	** std::string *stringPTR  = declaramos un puntero a string
	** &str                    = obtenemos la direccion de str
	**
	** El * en la declaracion indica "esto es un puntero"
	** El & delante de una variable significa "dame su direccion"
	*/
	std::string		*stringPTR = &str;

	/*
	** REFERENCIA: es un ALIAS (otro nombre) para una variable existente
	**
	** std::string &stringREF = str;
	**
	** El & en la declaracion indica "esto es una referencia"
	** stringREF ahora es OTRO NOMBRE para str
	** Son la misma variable, no una copia
	**
	** DIFERENCIA CON PUNTERO:
	** - Puntero: guarda direccion, puede ser NULL, puede cambiar
	** - Referencia: es un alias, NO puede ser NULL, NO puede cambiar
	*/
	std::string		&stringREF = str;

	std::cout << "=== Direcciones de memoria ===" << std::endl;

	// &str = direccion de la variable str
	std::cout << "Direccion de str:       " << &str << std::endl;

	// stringPTR YA ES una direccion (guarda la direccion de str)
	std::cout << "Direccion en stringPTR: " << stringPTR << std::endl;

	// &stringREF = direccion de stringREF, que ES la misma que str
	std::cout << "Direccion en stringREF: " << &stringREF << std::endl;

	// Las tres direcciones son IDENTICAS!

	std::cout << std::endl;
	std::cout << "=== Valores ===" << std::endl;

	// str = el valor directamente
	std::cout << "Valor de str:       " << str << std::endl;

	// *stringPTR = "dame el valor en la direccion que guarda stringPTR"
	// El * aqui significa "desreferenciar" (ir a la direccion y coger el valor)
	std::cout << "Valor de stringPTR: " << *stringPTR << std::endl;

	// stringREF = como es un alias, lo usamos directamente sin *
	std::cout << "Valor de stringREF: " << stringREF << std::endl;

	// Los tres valores son IDENTICOS!

	return (0);
}

/*
** RESUMEN:
**
** VARIABLE: int x = 42;
**   - x contiene el valor 42
**
** PUNTERO: int *ptr = &x;
**   - ptr contiene la DIRECCION de x
**   - *ptr nos da el VALOR (42)
**   - ptr puede ser NULL
**   - ptr puede apuntar a otra variable despues
**
** REFERENCIA: int &ref = x;
**   - ref es OTRO NOMBRE para x
**   - ref nos da el valor directamente (42)
**   - ref NO puede ser NULL
**   - ref NO puede cambiar a referenciar otra variable
*/
