/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contacts.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:35:39 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/12/29 17:36:45 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phonebook.hpp"

/* ------------------------------ SET CONTACT ------------------------------ */
static bool isNineDigits(const std::string& s)
{
    int i;

    if (s.length() != 9)
        return false;

    i = 0;
    while (i < 9)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
        i++;
    }
    return true;
}




void Contact::setContact()
{
	std::cout << "Enter First Name: ";
	while (first_name.empty() && !std::cin.eof())
	{
		std::getline(std::cin, first_name);
		if (first_name.empty())
			std::cout << "Field cannot be empty. Try again: ";
	}
	if (std::cin.eof())
		return ;
	std::cout << "Enter Last Name: ";
	while (last_name.empty() && !std::cin.eof())
	{
		std::getline(std::cin, last_name);
		if (last_name.empty())
			std::cout << "Field cannot be empty. Try again: ";
	}
	if (std::cin.eof())
		return ;
	std::cout << "Enter Nickname: ";
	while (nickname.empty() && !std::cin.eof())
	{
		std::getline(std::cin, nickname);
		if (nickname.empty())
			std::cout << "Field cannot be empty. Try again: ";
	}
	if (std::cin.eof())
		return ;
	std::cout << "Enter Phone Number: ";
	while (phone_number.empty() && !std::cin.eof())
	{
    std::getline(std::cin, phone_number);
    if (isNineDigits(phone_number))
        break;
    std::cout << "Invalid phone. Use exactly 9 digits (e.g., 612345678). Try again: ";
}
	if (std::cin.eof())
		return ;
	std::cout << "Enter Darkest Secret: ";
	while (darkest_secret.empty() && !std::cin.eof())
	{
		std::getline(std::cin, darkest_secret);
		if (darkest_secret.empty())
			std::cout << "Field cannot be empty. Try again: ";
	}
	if (std::cin.eof())
		return ;
	std::cout << "Contact successfully added!\n";
}

/* --------------------------- DISPLAY FULL CONTACT ------------------------ */

void Contact::displayContact() const
{
	std::cout << "First Name: " << first_name << std::endl;
	std::cout << "Last Name: " << last_name << std::endl;
	std::cout << "Nickname: " << nickname << std::endl;
	std::cout << "Phone Number: " << phone_number << std::endl;
	std::cout << "Darkest Secret: " << darkest_secret << std::endl;
}

/* --------------------------- DISPLAY TABLE SUMMARY ----------------------- */

static std::string truncateField(const std::string &field)
{
	if (field.length() > 10)
		return (field.substr(0, 9) + ".");
	return (field);
}

void Contact::displaySummary(int index) const
{
	std::cout << "|" << std::setw(10) << index;
	std::cout << "|" << std::setw(10) << truncateField(first_name);
	std::cout << "|" << std::setw(10) << truncateField(last_name);
	std::cout << "|" << std::setw(10) << truncateField(nickname) << "|" << std::endl;
}
