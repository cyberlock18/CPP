#include "RPN.hpp"
#include <stack>
#include <list>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <climits>

RPN::RPN() {}
RPN::RPN(RPN const&) {}
RPN& RPN::operator=(RPN const&) { return *this; }
RPN::~RPN() {}

// Applies "a op b" and throws if the result does not fit in an int.
// Every check is done BEFORE operating: signed overflow is undefined behaviour.
int RPN::apply(int a, int b, char op)
{
	if (op == '+')
	{
		if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b))
			throw std::runtime_error("Error: overflow");
		return a + b;
	}
	if (op == '-')
	{
		if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b))
			throw std::runtime_error("Error: overflow");
		return a - b;
	}
	if (op == '*')
	{
		if (a != 0 && b != 0)
		{
			if (a > 0 && b > 0 && a > INT_MAX / b)
				throw std::runtime_error("Error: overflow");
			if (a > 0 && b < 0 && b < INT_MIN / a)
				throw std::runtime_error("Error: overflow");
			if (a < 0 && b > 0 && a < INT_MIN / b)
				throw std::runtime_error("Error: overflow");
			if (a < 0 && b < 0 && a < INT_MAX / b)
				throw std::runtime_error("Error: overflow");
		}
		return a * b;
	}
	// op == '/'
	if (b == 0)
		throw std::runtime_error("Error: division by zero");
	if (a == INT_MIN && b == -1)
		throw std::runtime_error("Error: overflow");
	return a / b;
}

int RPN::evaluate(std::string const& expression) const
{
	std::stack<int, std::list<int> > st;
	std::istringstream ss(expression);
	std::string        token;

	while (ss >> token)
	{
		if (token.length() != 1)
			throw std::runtime_error("Error");

		char c = token[0];
		if (std::isdigit(static_cast<unsigned char>(c)))
		{
			st.push(c - '0');
			continue;
		}

		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			if (st.size() < 2)
				throw std::runtime_error("Error");
			int b = st.top(); st.pop();
			int a = st.top(); st.pop();
			st.push(apply(a, b, c));
			continue;
		}
		throw std::runtime_error("Error");
	}

	if (st.size() != 1)
		throw std::runtime_error("Error");

	return st.top();
}
