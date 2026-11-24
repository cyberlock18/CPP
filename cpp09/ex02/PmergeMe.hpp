#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <iostream>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(PmergeMe const& other);
	PmergeMe& operator=(PmergeMe const& other);
	~PmergeMe();

	void run(int argc, char** argv);

private:
	std::vector<int>	_vec;
	std::deque<int>		_deq;

	static int	parseNumber(std::string const& s);
	static void	validateArgs(int argc, char** argv);

	void	sortVector(int argc, char** argv);
	void	sortDeque(int argc, char** argv);

	static std::vector<size_t>	fjVec(std::vector<int> const& keys);
	static std::deque<size_t>	fjDeq(std::deque<int> const& keys);

	template <typename C>
	static void printContainer(std::string const& label, C const& c)
	{
		std::cout << label;
		for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
};

#endif
