/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:32:48 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/10/23 19:35:27 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phonebook.hpp"


int main() {
	PhoneBook phoneBook;
	std::string command;

	std::cout << "Welcome to My Awesome PhoneBook!" << std::endl;
	std::cout << "Available commands: ADD, SEARCH, EXIT" << std::endl;

	while (true)
	{
		std::cout << "Enter command: ";
		std::getline(std::cin, command);

		if (command == "ADD")
		{
			phoneBook.addContact();
		} else if (command == "SEARCH")
		{
			phoneBook.searchContacts();
		} else if (command == "EXIT")
		{
			std::cout << "Exiting PhoneBook. Goodbye!" << std::endl;
			break;
		} else
		{
			std::cout << "Invalid command. Please try again." << std::endl;
		}
	}

	return 0;
}