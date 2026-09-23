#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& other);
	BitcoinExchange& operator=(BitcoinExchange const& other);
	~BitcoinExchange();

	bool	loadDatabase(std::string const& csvFile);
	void	processInput(std::string const& inputFile) const;

private:
	std::map<std::string, double> _db;

	bool		isValidDate(std::string const& date) const;
	bool		isValidValue(std::string const& val, double& out) const;
	double		getRate(std::string const& date) const;
};

#endif
