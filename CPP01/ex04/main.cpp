#include <iostream>  // Para cout y cerr
#include <fstream>   // Para ifstream (leer) y ofstream (escribir) archivos
#include <string>    // Para std::string

/*
** replaceAll: reemplaza todas las ocurrencias de s1 por s2 en content
**
** NO usamos std::string::replace (prohibido en el ejercicio)
** En su lugar usamos: find(), substr(), length() y concatenacion
**
** ALGORITMO:
** 1. Buscamos s1 en content
** 2. Si encontramos, copiamos todo lo anterior + s2
** 3. Avanzamos la posicion y repetimos
** 4. Al final, copiamos lo que quede
*/
std::string	replaceAll(std::string content, std::string s1, std::string s2)
{
	std::string	result;      // String donde construimos el resultado
	size_t		pos = 0;     // Posicion actual en content
	size_t		found;       // Posicion donde encontramos s1

	// Si s1 esta vacio, no hay nada que reemplazar
	if (s1.empty())
		return (content);

	/*
	** content.find(s1, pos) busca s1 empezando desde la posicion pos
	** Devuelve la posicion donde encuentra s1
	** Si no lo encuentra, devuelve std::string::npos (un valor especial)
	*/
	while ((found = content.find(s1, pos)) != std::string::npos)
	{
		// Copiamos desde pos hasta donde encontramos s1
		// substr(inicio, longitud) extrae una subcadena
		result += content.substr(pos, found - pos);

		// Anadimos s2 (el reemplazo) en lugar de s1
		result += s2;

		// Avanzamos pos para continuar despues de s1
		pos = found + s1.length();
	}

	// Copiamos el resto del string (despues de la ultima ocurrencia)
	result += content.substr(pos);

	return (result);
}

int	main(int argc, char **argv)
{
	// Verificamos que tenemos exactamente 3 argumentos
	// argv[0] = nombre del programa
	// argv[1] = nombre del archivo
	// argv[2] = string a buscar (s1)
	// argv[3] = string de reemplazo (s2)
	if (argc != 4)
	{
		std::cerr << "Usage: ./sed <filename> <s1> <s2>" << std::endl;
		return (1);
	}

	std::string		filename = argv[1];
	std::string		s1 = argv[2];
	std::string		s2 = argv[3];

	/*
	** ifstream = input file stream (para LEER archivos)
	**
	** .c_str() convierte std::string a const char*
	** porque ifstream en C++98 no acepta std::string directamente
	*/
	std::ifstream	infile(filename.c_str());

	// Verificamos que el archivo se abrio correctamente
	if (!infile.is_open())
	{
		std::cerr << "Error: cannot open file " << filename << std::endl;
		return (1);
	}

	// Leemos todo el contenido del archivo
	std::string	content;
	std::string	line;

	/*
	** std::getline(stream, string) lee una linea del stream
	** Devuelve el stream, que se evalua como false cuando llega al final
	*/
	while (std::getline(infile, line))
	{
		content += line;
		// Anadimos salto de linea excepto al final del archivo
		if (!infile.eof())
			content += "\n";
	}
	infile.close();  // Cerramos el archivo de entrada

	// Creamos el nombre del archivo de salida
	std::string		outfilename = filename + ".replace";

	/*
	** ofstream = output file stream (para ESCRIBIR archivos)
	** Si el archivo no existe, lo crea
	** Si existe, lo sobreescribe
	*/
	std::ofstream	outfile(outfilename.c_str());

	if (!outfile.is_open())
	{
		std::cerr << "Error: cannot create file " << outfilename << std::endl;
		return (1);
	}

	// Escribimos el contenido con los reemplazos
	outfile << replaceAll(content, s1, s2);
	outfile.close();  // Cerramos el archivo de salida

	return (0);
}
