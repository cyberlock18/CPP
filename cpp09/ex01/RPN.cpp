#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>
#include <iostream>

RPN::RPN() {}
RPN::RPN(RPN const&) {}
RPN& RPN::operator=(RPN const&) { return *this; }
RPN::~RPN() {}

int RPN::evaluate(std::string const& expression) const
{
	std::stack<int>    st;
	std::istringstream ss(expression);
	std::string        token;

	while (ss >> token)
	{
		if (token.length() == 1 && std::isdigit(token[0]))
		{
			st.push(token[0] - '0');
			continue;
		}

		if (token.length() == 1 && (token[0] == '+' || token[0] == '-'
			|| token[0] == '*' || token[0] == '/'))
		{
			if (st.size() < 2)
				throw std::runtime_error("Error");
			int b = st.top(); st.pop();
			int a = st.top(); st.pop();

			if (token[0] == '+') st.push(a + b);
			else if (token[0] == '-') st.push(a - b);
			else if (token[0] == '*') st.push(a * b);
			else
			{
				if (b == 0) throw std::runtime_error("Error: division by zero");
				st.push(a / b);
			}
			continue;
		}
		throw std::runtime_error("Error");
	}

	if (st.size() != 1)
		throw std::runtime_error("Error");

	return st.top();
}
