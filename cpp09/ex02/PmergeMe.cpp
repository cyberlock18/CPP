#include "PmergeMe.hpp"
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <climits>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(PmergeMe const& other) : _vec(other._vec), _deq(other._deq) {}
PmergeMe& PmergeMe::operator=(PmergeMe const& other)
{
	if (this != &other) { _vec = other._vec; _deq = other._deq; }
	return *this;
}
PmergeMe::~PmergeMe() {}

// ─── Jacobsthal-based insertion order ───────────────────────────────────────

static std::vector<size_t> buildInsertOrder(size_t n)
{
	std::vector<size_t> jac;
	jac.push_back(0);
	jac.push_back(1);
	while (jac.back() < n)
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

	std::vector<size_t> order;
	for (size_t k = 1; k < jac.size(); k++)
	{
		size_t hi = jac[k];
		if (hi > n) hi = n;
		size_t lo = jac[k - 1];
		for (size_t i = hi; i > lo; i--)
			order.push_back(i - 1);
	}
	return order;
}

// ─── Ford-Johnson for std::vector ───────────────────────────────────────────

void PmergeMe::fjVec(std::vector<int>& v)
{
	size_t n = v.size();
	if (n < 2) return;

	typedef std::pair<int, int> pii;

	bool hasStraggler = (n % 2 == 1);
	int  straggler    = hasStraggler ? v[n - 1] : 0;

	std::vector<pii> pairs;
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int a = v[i], b = v[i + 1];
		if (a > b) std::swap(a, b);
		pairs.push_back(pii(a, b));
	}

	std::vector<int> largers;
	for (size_t i = 0; i < pairs.size(); i++)
		largers.push_back(pairs[i].second);

	fjVec(largers);

	// match sorted largers back to their smalls (handle duplicates with used[])
	std::vector<bool> used(pairs.size(), false);
	std::vector<pii>  sorted;
	for (size_t i = 0; i < largers.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].second == largers[i])
			{
				sorted.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}

	// build main chain: [a0, b0, b1, ..., bk]
	std::vector<int> chain;
	chain.push_back(sorted[0].first);
	for (size_t i = 0; i < sorted.size(); i++)
		chain.push_back(sorted[i].second);

	// pend: remaining smalls
	std::vector<int> pend;
	for (size_t i = 1; i < sorted.size(); i++)
		pend.push_back(sorted[i].first);

	// insert pend in Jacobsthal order
	std::vector<size_t> order = buildInsertOrder(pend.size());
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t idx = order[k];
		if (idx >= pend.size()) continue;
		// search up to (and including) the position of its paired larger
		std::vector<int>::iterator limit =
			std::upper_bound(chain.begin(), chain.end(), sorted[idx + 1].second);
		chain.insert(std::lower_bound(chain.begin(), limit, pend[idx]), pend[idx]);
	}

	if (hasStraggler)
		chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler), straggler);

	v = chain;
}

// ─── Ford-Johnson for std::deque ────────────────────────────────────────────

void PmergeMe::fjDeq(std::deque<int>& d)
{
	size_t n = d.size();
	if (n < 2) return;

	typedef std::pair<int, int> pii;

	bool hasStraggler = (n % 2 == 1);
	int  straggler    = hasStraggler ? d[n - 1] : 0;

	std::deque<pii> pairs;
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int a = d[i], b = d[i + 1];
		if (a > b) std::swap(a, b);
		pairs.push_back(pii(a, b));
	}

	std::deque<int> largers;
	for (size_t i = 0; i < pairs.size(); i++)
		largers.push_back(pairs[i].second);

	fjDeq(largers);

	std::vector<bool> used(pairs.size(), false);
	std::deque<pii>   sorted;
	for (size_t i = 0; i < largers.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].second == largers[i])
			{
				sorted.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}

	std::deque<int> chain;
	chain.push_back(sorted[0].first);
	for (size_t i = 0; i < sorted.size(); i++)
		chain.push_back(sorted[i].second);

	std::deque<int> pend;
	for (size_t i = 1; i < sorted.size(); i++)
		pend.push_back(sorted[i].first);

	std::vector<size_t> order = buildInsertOrder(pend.size());
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t idx = order[k];
		if (idx >= pend.size()) continue;
		std::deque<int>::iterator limit =
			std::upper_bound(chain.begin(), chain.end(), sorted[idx + 1].second);
		chain.insert(std::lower_bound(chain.begin(), limit, pend[idx]), pend[idx]);
	}

	if (hasStraggler)
		chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler), straggler);

	d = chain;
}

// ─── Public interface ────────────────────────────────────────────────────────

void PmergeMe::parseArgs(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::istringstream ss(argv[i]);
		long val;
		if (!(ss >> val) || val < 0 || val > INT_MAX)
			throw std::runtime_error("Error");
		std::string rest;
		if (ss >> rest)
			throw std::runtime_error("Error");
		_vec.push_back(static_cast<int>(val));
		_deq.push_back(static_cast<int>(val));
	}
}

void PmergeMe::sortVector()
{
	fjVec(_vec);
}

void PmergeMe::sortDeque()
{
	fjDeq(_deq);
}

void PmergeMe::run(int argc, char** argv)
{
	if (argc < 2)
		throw std::runtime_error("Error: no input");

	parseArgs(argc, argv);

	printContainer("Before: ", _vec);

	// time vector sort
	clock_t startV = clock();
	sortVector();
	clock_t endV = clock();

	// time deque sort
	std::deque<int> tmpDeq = _deq;
	clock_t startD = clock();
	sortDeque();
	clock_t endD = clock();
	(void)tmpDeq;

	printContainer("After:  ", _vec);

	double timeV = static_cast<double>(endV - startV) / CLOCKS_PER_SEC * 1000000.0;
	double timeD = static_cast<double>(endD - startD) / CLOCKS_PER_SEC * 1000000.0;

	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : " << timeV << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
			  << " elements with std::deque  : " << timeD << " us" << std::endl;
}
