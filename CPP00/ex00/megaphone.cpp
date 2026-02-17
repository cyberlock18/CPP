/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:30:48 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/12/28 23:12:15 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <cctype>

int lower_to_upper(unsigned char c)
{
	return std::toupper(c);
}
int main (int arc, char **argv)
{
	if (arc == 1)
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
	else
	{
		for ( int i = 1; i < arc; i++)
		{
			std::string str = argv[i];
			for (size_t j = 0; j < str.length(); j++)
				str[j] = lower_to_upper(str[j]);
			std::cout << str;
		}
	}
	std::cout << std::endl;
	return 0;
}