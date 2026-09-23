#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <stdexcept>

class Span
{
public:
	Span();
	Span(unsigned int n);
	Span(Span const& other);
	Span& operator=(Span const& other);
	~Span();

	void		addNumber(int n);

	template <typename Iterator>
	void addRange(Iterator begin, Iterator end)
	{
		while (begin != end)
		{
			addNumber(*begin);
			++begin;
		}
	}

	int shortestSpan() const;
	int longestSpan() const;

private:
	unsigned int		_maxSize;
	std::vector<int>	_numbers;
};

#endif
