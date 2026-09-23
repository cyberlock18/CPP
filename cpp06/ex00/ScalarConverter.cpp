#include "ScalarConverter.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <cfloat>
#include <cerrno>
#include <cmath>

enum LiteralType { CHAR, INT, FLOAT, DOUBLE, SPECIAL };

static LiteralType detectType(std::string const& s)
{
	if (s == "nan" || s == "+inf" || s == "-inf" ||
		s == "nanf" || s == "+inff" || s == "-inff")
		return SPECIAL;

	if (s.length() == 1 && !std::isdigit(s[0]))
		return CHAR;

	bool hasDot = false;
	bool hasF   = false;
	size_t start = (s[0] == '-' || s[0] == '+') ? 1 : 0;

	for (size_t i = start; i < s.length(); i++)
	{
		if (s[i] == '.')      { hasDot = true; continue; }
		if (s[i] == 'f' && i == s.length() - 1) { hasF = true; continue; }
		if (!std::isdigit(s[i])) return CHAR;
	}

	if (hasDot && hasF)  return FLOAT;
	if (hasDot && !hasF) return DOUBLE;
	return INT;
}

static void printChar(double val)
{
	if (val < 0 || val > 127 || std::isnan(val) || std::isinf(val))
		std::cout << "char: impossible" << std::endl;
	else if (!std::isprint(static_cast<int>(val)))
		std::cout << "char: Non displayable" << std::endl;
	else
		std::cout << "char: '" << static_cast<char>(val) << "'" << std::endl;
}

static void printInt(double val)
{
	if (std::isnan(val) || std::isinf(val) ||
		val > static_cast<double>(INT_MAX) || val < static_cast<double>(INT_MIN))
		std::cout << "int: impossible" << std::endl;
	else
		std::cout << "int: " << static_cast<int>(val) << std::endl;
}

static void printFloat(double val)
{
	float f = static_cast<float>(val);
	if (f == static_cast<long>(f))
		std::cout << "float: " << f << ".0f" << std::endl;
	else
		std::cout << "float: " << f << "f" << std::endl;
}

static void printDouble(double val)
{
	if (val == static_cast<long>(val) && !std::isinf(val))
		std::cout << "double: " << val << ".0" << std::endl;
	else
		std::cout << "double: " << val << std::endl;
}

void ScalarConverter::convert(std::string const& literal)
{
	LiteralType type = detectType(literal);
	double val = 0.0;

	if (type == SPECIAL)
	{
		if (literal == "nanf" || literal == "nan")
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: nanf" << std::endl;
			std::cout << "double: nan" << std::endl;
		}
		else
		{
			std::string sign = (literal[0] == '-') ? "-" : "+";
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: " << sign << "inff" << std::endl;
			std::cout << "double: " << sign << "inf" << std::endl;
		}
		return;
	}

	if (type == CHAR)
	{
		val = static_cast<double>(literal[0]);
	}
	else
	{
		char* end;
		errno = 0;
		val = std::strtod(literal.c_str(), &end);
	}

	printChar(val);
	printInt(val);
	printFloat(val);
	printDouble(val);
}
