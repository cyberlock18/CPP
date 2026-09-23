#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(BitcoinExchange const& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const& other)
{
	if (this != &other) _db = other._db;
	return *this;
}
BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::loadDatabase(std::string const& csvFile)
{
	std::ifstream file(csvFile.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database." << std::endl;
		return false;
	}

	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line.empty()) continue;
		size_t comma = line.find(',');
		if (comma == std::string::npos) continue;
		std::string date = line.substr(0, comma);
		double rate = std::atof(line.substr(comma + 1).c_str());
		_db[date] = rate;
	}
	return true;
}

bool BitcoinExchange::isValidDate(std::string const& date) const
{
	if (date.length() != 10) return false;
	if (date[4] != '-' || date[7] != '-') return false;
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7) continue;
		if (date[i] < '0' || date[i] > '9') return false;
	}
	int month = std::atoi(date.substr(5, 2).c_str());
	int day   = std::atoi(date.substr(8, 2).c_str());
	return (month >= 1 && month <= 12 && day >= 1 && day <= 31);
}

bool BitcoinExchange::isValidValue(std::string const& val, double& out) const
{
	char* end;
	out = std::strtod(val.c_str(), &end);
	if (*end != '\0') return false;
	if (out < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (out > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

double BitcoinExchange::getRate(std::string const& date) const
{
	std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
	if (it == _db.end() || it->first != date)
	{
		if (it == _db.begin()) return 0.0;
		--it;
	}
	return it->second;
}

void BitcoinExchange::processInput(std::string const& inputFile) const
{
	std::ifstream file(inputFile.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		size_t sep = line.find(" | ");
		if (sep == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = line.substr(0, sep);
		std::string val  = line.substr(sep + 3);

		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		double amount;
		if (!isValidValue(val, amount))
			continue;

		double rate = getRate(date);
		std::cout << date << " => " << amount << " = " << amount * rate << std::endl;
	}
}
