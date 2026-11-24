#include "BitcoinExchange.hpp"
#include <fstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(BitcoinExchange const& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const& other)
{
	if (this != &other) _db = other._db;
	return *this;
}
BitcoinExchange::~BitcoinExchange() {}

// Remove a trailing '\r' (files saved with Windows line endings)
void BitcoinExchange::stripCR(std::string& line)
{
	if (!line.empty() && line[line.length() - 1] == '\r')
		line.erase(line.length() - 1);
}

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
		stripCR(line);
		if (line.empty()) continue;
		size_t comma = line.find(',');
		if (comma == std::string::npos) continue;
		std::string date = line.substr(0, comma);
		double rate = std::atof(line.substr(comma + 1).c_str());
		_db[date] = rate;
	}
	if (_db.empty())
	{
		std::cerr << "Error: database is empty." << std::endl;
		return false;
	}
	return true;
}

bool BitcoinExchange::isValidDate(std::string const& date)
{
	if (date.length() != 10) return false;
	if (date[4] != '-' || date[7] != '-') return false;
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7) continue;
		if (date[i] < '0' || date[i] > '9') return false;
	}
	int year  = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day   = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12) return false;

	static const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int maxDay = daysInMonth[month - 1];
	bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (month == 2 && leap)
		maxDay = 29;
	return (day >= 1 && day <= maxDay);
}

// Accepts: optional sign, digits, at most one '.', at least one digit.
// Rejects: "", "abc", "1e3", "1.2.3", " 5", "inf", "nan"
bool BitcoinExchange::isNumber(std::string const& val)
{
	size_t i = 0;
	if (i < val.length() && (val[i] == '-' || val[i] == '+'))
		i++;
	bool digits = false;
	bool dot = false;
	for (; i < val.length(); i++)
	{
		if (val[i] >= '0' && val[i] <= '9')
			digits = true;
		else if (val[i] == '.' && !dot)
			dot = true;
		else
			return false;
	}
	return digits;
}

bool BitcoinExchange::isValidValue(std::string const& val, std::string const& line, double& out)
{
	if (!isNumber(val))
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
	out = std::strtod(val.c_str(), NULL);
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

// Exact date, or the closest LOWER date. False if the date is before the whole DB.
bool BitcoinExchange::getRate(std::string const& date, double& rate) const
{
	std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
	if (it == _db.end() || it->first != date)
	{
		if (it == _db.begin()) return false;
		--it;
	}
	rate = it->second;
	return true;
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
		stripCR(line);
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
		if (!isValidValue(val, line, amount))
			continue;

		double rate;
		if (!getRate(date, rate))
		{
			std::cerr << "Error: no data for this date => " << date << std::endl;
			continue;
		}
		std::cout << date << " => " << amount << " = " << amount * rate << std::endl;
	}
}
