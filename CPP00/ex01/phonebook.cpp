/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:30:26 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/10/26 12:30:36 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phonebook.hpp"

PhoneBook::PhoneBook() : currentIndex(0), totalContacts(0) {}

PhoneBook::~PhoneBook() {}

/* ------------------------------ ADD CONTACT ------------------------------ */

void PhoneBook::addContact()
{
    std::cout << "\n--- ADD NEW CONTACT ---" << std::endl;
    Contact newContact;

    newContact.setContact();
    if (std::cin.eof()) // Si el usuario presiona Ctrl+D durante la entrada
    {
        std::cin.clear();
        std::cout << "\nInput interrupted.\n";
        return;
    }

    _contacts[currentIndex] = newContact;
    currentIndex = (currentIndex + 1) % 8;

    if (totalContacts < 8)
        totalContacts++;

    std::cout << "Contact added at position " << (currentIndex == 0 ? 8 : currentIndex)
              << "/" << 8 << "\n" << std::endl;
}

/* ----------------------------- DISPLAY TABLE ----------------------------- */

static void displayHeader(void)
{
    std::cout << "|" << std::setw(10) << "Index";
    std::cout << "|" << std::setw(10) << "First Name";
    std::cout << "|" << std::setw(10) << "Last Name";
    std::cout << "|" << std::setw(10) << "Nickname" << "|" << std::endl;
}

/* ------------------------------ SEARCH CONTACTS -------------------------- */

void PhoneBook::searchContacts() const
{
    if (totalContacts == 0)
    {
        std::cout << "\nPhoneBook is empty. Add some contacts first!\n" << std::endl;
        return;
    }

    std::cout << "\n--- CONTACT LIST ---" << std::endl;
    displayHeader();

    for (int i = 0; i < totalContacts; i++)
        _contacts[i].displaySummary(i);

    std::cout << "\nEnter the index of the contact to display: ";
    std::string input;
    std::getline(std::cin, input);

    if (std::cin.eof())
    {
        std::cout << "\nInput interrupted.\n";
        std::cin.clear();
        return;
    }

    if (input.empty() || input.find_first_not_of("0123456789") != std::string::npos)
    {
        std::cout << "Invalid index. Must be a number between 0 and "
                  << totalContacts - 1 << ".\n" << std::endl;
        return;
    }

    int index = 0;
    for (int i = 0; input[i]; i++)
        index = index * 10 + (input[i] - '0');
    if (index < 0 || index >= totalContacts)
    {
        std::cout << "Invalid index. Out of range.\n" << std::endl;
        return;
    }

    std::cout << "\n--- CONTACT DETAILS ---" << std::endl;
    _contacts[index].displayContact();
    std::cout << std::endl;
}
