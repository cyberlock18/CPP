#include "ScalarConverter.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <climits>
#include <cfloat>
#include <cerrno>
#include <cmath>

enum LiteralType { CHAR, INT, FLOAT, DOUBLE, SPECIAL, INVALID };

static LiteralType detectType(std::string const& s)
{
	if (s.empty())
		return INVALID;

	if (s == "nan" || s == "+inf" || s == "-inf" ||
		s == "nanf" || s == "+inff" || s == "-inff")
		return SPECIAL;

	if (s.length() == 3 && s[0] == '\'' && s[2] == '\'')
		return CHAR;
	if (s.length() == 1 && !std::isdigit(s[0]))
		return CHAR;

	int    digits = 0;
	int    dots   = 0;
	bool   hasF   = false;
	size_t start  = (s[0] == '-' || s[0] == '+') ? 1 : 0;

	for (size_t i = start; i < s.length(); i++)
	{
		if (std::isdigit(s[i]))                        digits++;
		else if (s[i] == '.')                          dots++;
		else if (s[i] == 'f' && i == s.length() - 1)   hasF = true;
		else                                           return INVALID;
	}

	if (digits == 0 || dots > 1 || (hasF && dots == 0))
		return INVALID;
	if (hasF)  return FLOAT;
	if (dots)  return DOUBLE;
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

static void printNumber(double n)
{
	if (n == std::floor(n))
	{
		std::cout << std::fixed << std::setprecision(1) << n;
		std::cout.unsetf(std::ios::fixed);
		std::cout << std::setprecision(6);
	}
	else
		std::cout << n;
}

static void printFloat(double val)
{
	if (std::fabs(val) > FLT_MAX)
	{
		std::cout << "float: impossible" << std::endl;
		return;
	}
	float f = static_cast<float>(val);
	std::cout << "float: ";
	printNumber(f);
	std::cout << "f" << std::endl;
}

static void printDouble(double val)
{
	std::cout << "double: ";
	printNumber(val);
	std::cout << std::endl;
}

void ScalarConverter::convert(std::string const& literal)
{
	LiteralType type = detectType(literal);
	double val = 0.0;

	if (type == INVALID)
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}

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
		char c = (literal.length() == 3) ? literal[1] : literal[0];
		val = static_cast<double>(c);
	}
	else
	{
		char* end;
		errno = 0;
		val = std::strtod(literal.c_str(), &end);
		if (errno == ERANGE && std::isinf(val))
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
	}

	printChar(val);
	printInt(val);
	printFloat(val);
	printDouble(val);
}
