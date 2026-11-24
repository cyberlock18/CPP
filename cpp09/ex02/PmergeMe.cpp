#include "PmergeMe.hpp"
#include <stdexcept>
#include <ctime>
#include <climits>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(PmergeMe const& other) : _vec(other._vec), _deq(other._deq) {}
PmergeMe& PmergeMe::operator=(PmergeMe const& other)
{
	if (this != &other) { _vec = other._vec; _deq = other._deq; }
	return *this;
}
PmergeMe::~PmergeMe() {}

// ─── Jacobsthal insertion order ─────────────────────────────────────────────
//
// m = number of "b" elements (b0 .. b(m-1)); b0 is already in the chain.
// Knuth (1-based): insert b3 b2, then b5 b4, then b11 .. b6, then b21 .. b12 ...
// Group limits are the Jacobsthal numbers 1, 3, 5, 11, 21, 43 ...
// Here indices are 0-based, so the result is: 2 1, 4 3, 10 .. 5, 20 .. 11 ...

static std::vector<size_t> buildInsertOrder(size_t m)
{
	std::vector<size_t> order;
	size_t prev = 1;	// J(k-1)
	size_t curr = 3;	// J(k)
	while (prev < m)
	{
		size_t hi = (curr < m) ? curr : m;
		for (size_t i = hi; i > prev; i--)
			order.push_back(i - 1);
		size_t next = curr + 2 * prev;	// J(k+1) = J(k) + 2 * J(k-1)
		prev = curr;
		curr = next;
	}
	return order;
}

// ─── Ford-Johnson for std::vector ───────────────────────────────────────────
//
// Sorts POSITIONS instead of values: returns the positions of `keys` in sorted
// order. Working with positions lets each larger element remember its partner
// in O(1), even with duplicate values.

std::vector<size_t> PmergeMe::fjVec(std::vector<int> const& keys)
{
	size_t n = keys.size();
	std::vector<size_t> result;
	if (n < 2)
	{
		if (n == 1) result.push_back(0);
		return result;
	}

	// 1. Pairs: one comparison per pair; remember where the small and large are
	size_t k = n / 2;
	std::vector<size_t> smallPos, largePos;
	std::vector<int>    largeKeys;
	for (size_t i = 0; i < k; i++)
	{
		size_t x = 2 * i, y = 2 * i + 1;
		if (keys[x] > keys[y]) { size_t t = x; x = y; y = t; }
		smallPos.push_back(x);
		largePos.push_back(y);
		largeKeys.push_back(keys[y]);
	}

	// 2. Recursively sort the larger elements (we get the order of the pairs)
	std::vector<size_t> pairOrder = fjVec(largeKeys);

	// 3. a[i] = i-th smallest larger, b[i] = its partner; straggler = b[k]
	std::vector<size_t> a, b;
	for (size_t i = 0; i < k; i++)
	{
		a.push_back(largePos[pairOrder[i]]);
		b.push_back(smallPos[pairOrder[i]]);
	}
	if (n % 2 == 1)
		b.push_back(n - 1);

	// 4. Main chain: b0 a0 a1 ... a(k-1); posA[i] = where a[i] is in the chain
	std::vector<size_t> chain;
	chain.push_back(b[0]);
	for (size_t i = 0; i < k; i++)
		chain.push_back(a[i]);
	std::vector<size_t> posA;
	for (size_t i = 0; i < k; i++)
		posA.push_back(i + 1);

	// 5. Insert b1 .. b(m-1) in Jacobsthal order with binary search
	std::vector<size_t> order = buildInsertOrder(b.size());
	for (size_t t = 0; t < order.size(); t++)
	{
		size_t i = order[t];
		int    value = keys[b[i]];
		// b[i] <= a[i], so search only in [0, posA[i]); the straggler has no partner
		size_t lo = 0;
		size_t hi = (i < k) ? posA[i] : chain.size();
		while (lo < hi)
		{
			size_t mid = lo + (hi - lo) / 2;
			if (keys[chain[mid]] < value)
				lo = mid + 1;
			else
				hi = mid;
		}
		chain.insert(chain.begin() + lo, b[i]);
		for (size_t j = 0; j < k; j++)
			if (posA[j] >= lo)
				posA[j]++;
	}
	return chain;
}

// ─── Ford-Johnson for std::deque ────────────────────────────────────────────

std::deque<size_t> PmergeMe::fjDeq(std::deque<int> const& keys)
{
	size_t n = keys.size();
	std::deque<size_t> result;
	if (n < 2)
	{
		if (n == 1) result.push_back(0);
		return result;
	}

	size_t k = n / 2;
	std::deque<size_t> smallPos, largePos;
	std::deque<int>    largeKeys;
	for (size_t i = 0; i < k; i++)
	{
		size_t x = 2 * i, y = 2 * i + 1;
		if (keys[x] > keys[y]) { size_t t = x; x = y; y = t; }
		smallPos.push_back(x);
		largePos.push_back(y);
		largeKeys.push_back(keys[y]);
	}

	std::deque<size_t> pairOrder = fjDeq(largeKeys);

	std::deque<size_t> a, b;
	for (size_t i = 0; i < k; i++)
	{
		a.push_back(largePos[pairOrder[i]]);
		b.push_back(smallPos[pairOrder[i]]);
	}
	if (n % 2 == 1)
		b.push_back(n - 1);

	std::deque<size_t> chain;
	chain.push_back(b[0]);
	for (size_t i = 0; i < k; i++)
		chain.push_back(a[i]);
	std::deque<size_t> posA;
	for (size_t i = 0; i < k; i++)
		posA.push_back(i + 1);

	std::vector<size_t> order = buildInsertOrder(b.size());
	for (size_t t = 0; t < order.size(); t++)
	{
		size_t i = order[t];
		int    value = keys[b[i]];
		size_t lo = 0;
		size_t hi = (i < k) ? posA[i] : chain.size();
		while (lo < hi)
		{
			size_t mid = lo + (hi - lo) / 2;
			if (keys[chain[mid]] < value)
				lo = mid + 1;
			else
				hi = mid;
		}
		chain.insert(chain.begin() + lo, b[i]);
		for (size_t j = 0; j < k; j++)
			if (posA[j] >= lo)
				posA[j]++;
	}
	return chain;
}

// ─── Input ──────────────────────────────────────────────────────────────────

// Strict positive integer: only digits (optional '+'), 1 .. INT_MAX
int PmergeMe::parseNumber(std::string const& s)
{
	size_t i = 0;
	if (i < s.length() && s[i] == '+')
		i++;
	if (i == s.length())
		throw std::runtime_error("Error");
	long val = 0;
	for (; i < s.length(); i++)
	{
		if (s[i] < '0' || s[i] > '9')
			throw std::runtime_error("Error");
		val = val * 10 + (s[i] - '0');
		if (val > INT_MAX)
			throw std::runtime_error("Error");
	}
	if (val == 0)
		throw std::runtime_error("Error");
	return static_cast<int>(val);
}

void PmergeMe::validateArgs(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
		parseNumber(argv[i]);
}

// Each sort* function does ALL the work for its container (load + sort),
// so the measured time includes the data management part.

void PmergeMe::sortVector(int argc, char** argv)
{
	std::vector<int> keys;
	for (int i = 1; i < argc; i++)
		keys.push_back(parseNumber(argv[i]));
	std::vector<size_t> order = fjVec(keys);
	_vec.clear();
	for (size_t i = 0; i < order.size(); i++)
		_vec.push_back(keys[order[i]]);
}

void PmergeMe::sortDeque(int argc, char** argv)
{
	std::deque<int> keys;
	for (int i = 1; i < argc; i++)
		keys.push_back(parseNumber(argv[i]));
	std::deque<size_t> order = fjDeq(keys);
	_deq.clear();
	for (size_t i = 0; i < order.size(); i++)
		_deq.push_back(keys[order[i]]);
}

void PmergeMe::run(int argc, char** argv)
{
	if (argc < 2)
		throw std::runtime_error("Error");

	validateArgs(argc, argv);	// any error is reported before printing anything

	std::cout << "Before: ";
	for (int i = 1; i < argc; i++)
		std::cout << parseNumber(argv[i]) << " ";
	std::cout << std::endl;

	clock_t startV = clock();
	sortVector(argc, argv);
	clock_t endV = clock();

	clock_t startD = clock();
	sortDeque(argc, argv);
	clock_t endD = clock();

	printContainer("After:  ", _vec);

	double timeV = static_cast<double>(endV - startV) / CLOCKS_PER_SEC * 1000000.0;
	double timeD = static_cast<double>(endD - startD) / CLOCKS_PER_SEC * 1000000.0;

	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : " << timeV << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
			  << " elements with std::deque  : " << timeD << " us" << std::endl;
}
