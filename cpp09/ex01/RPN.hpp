#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN
{
public:
	RPN();
	RPN(RPN const& other);
	RPN& operator=(RPN const& other);
	~RPN();

	int evaluate(std::string const& expression) const;
};

#endif
