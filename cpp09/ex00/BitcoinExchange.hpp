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

	static void	stripCR(std::string& line);
	static bool	isValidDate(std::string const& date);
	static bool	isNumber(std::string const& val);
	static bool	isValidValue(std::string const& val, std::string const& line, double& out);
	bool		getRate(std::string const& date, double& rate) const;
};

#endif
