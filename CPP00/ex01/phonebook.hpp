/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:14:29 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/12/29 17:39:12 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

# include <iostream>
# include <string>
# include <iomanip>

class Contact {
public:
     void setContact();               
    void displayContact() const;    
    void displaySummary(int index) const;

private:
    std::string first_name;
    std::string last_name;
    std::string nickname;
    std::string phone_number;
    std::string darkest_secret;
};

class PhoneBook {
public:
    PhoneBook();
    ~PhoneBook();

    void addContact();
    void searchContacts() const;

private:
    Contact _contacts[8];
    int currentIndex;
    int totalContacts;
};

#endif
