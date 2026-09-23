#include "Span.hpp"
#include <algorithm>
#include <climits>

Span::Span() : _maxSize(0) {}

Span::Span(unsigned int n) : _maxSize(n) {}

Span::Span(Span const& other) : _maxSize(other._maxSize), _numbers(other._numbers) {}

Span& Span::operator=(Span const& other)
{
	if (this != &other)
	{
		_maxSize = other._maxSize;
		_numbers = other._numbers;
	}
	return *this;
}

Span::~Span() {}

void Span::addNumber(int n)
{
	if (_numbers.size() >= _maxSize)
		throw std::overflow_error("span is full");
	_numbers.push_back(n);
}

int Span::shortestSpan() const
{
	if (_numbers.size() < 2)
		throw std::logic_error("not enough numbers to find a span");

	std::vector<int> sorted(_numbers);
	std::sort(sorted.begin(), sorted.end());

	int shortest = INT_MAX;
	for (size_t i = 1; i < sorted.size(); i++)
	{
		int diff = sorted[i] - sorted[i - 1];
		if (diff < shortest)
			shortest = diff;
	}
	return shortest;
}

int Span::longestSpan() const
{
	if (_numbers.size() < 2)
		throw std::logic_error("not enough numbers to find a span");

	int minVal = *std::min_element(_numbers.begin(), _numbers.end());
	int maxVal = *std::max_element(_numbers.begin(), _numbers.end());
	return maxVal - minVal;
}
