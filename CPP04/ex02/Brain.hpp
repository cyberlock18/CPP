#ifndef BRAIN_HPP
# define BRAIN_HPP

# include <string>
# include <iostream>

class Brain
{
public:
	std::string	ideas[100];

	Brain(void);
	Brain(const Brain &src);
	Brain &operator=(const Brain &rhs);
	~Brain(void);
};

#endif
