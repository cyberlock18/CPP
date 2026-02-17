/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Account.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:01:22 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/10/27 19:19:51 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Account.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

// Variables estáticas inicializadas a cero
int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

// Constructor
Account::Account(int initial_deposit) : _accountIndex(_nbAccounts), _amount(initial_deposit), _nbDeposits(0), _nbWithdrawals(0) {
    _nbAccounts++;                  // Aumenta el número total de cuentas
    _totalAmount += initial_deposit; // Suma el depósito inicial al total global
    _displayTimestamp();             // Muestra el timestamp
    std::cout << "Account " << _accountIndex << ": amount " << _amount << "; created\n"; // Mensaje de creación
}

// Destructor
Account::~Account() {
    _displayTimestamp(); // Muestra el timestamp
    std::cout << "Account " << _accountIndex << ": amount " << _amount << "; closed\n"; // Mensaje de cierre
}

// Método estático para mostrar el timestamp
void Account::_displayTimestamp() {
    std::time_t time = std::time(NULL); // Obtiene el tiempo actual
    std::tm* now = std::localtime(&time); // Convierte a formato local
    std::cout << "[" << (now->tm_year + 1900) // Año
              << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) // Mes
              << std::setw(2) << std::setfill('0') << now->tm_mday // Día
              << "_"
              << std::setw(2) << std::setfill('0') << now->tm_hour // Hora
              << std::setw(2) << std::setfill('0') << now->tm_min // Minuto
              << std::setw(2) << std::setfill('0') << now->tm_sec // Segundo
              << "] ";
}

// Método para realizar un depósito
void Account::makeDeposit(int deposit) {
    _amount += deposit;               // Aumenta el saldo de la cuenta
    _nbDeposits++;                    // Aumenta el número de depósitos de esta cuenta
    _totalAmount += deposit;          // Aumenta el total global
    _totalNbDeposits++;               // Aumenta el total global de depósitos
    _displayTimestamp();               // Muestra el timestamp
    std::cout << "Deposit: " << deposit << "; New balance: " << _amount << "; Total deposits: " << _nbDeposits << "\n"; // Mensaje de depósito
}

// Método para realizar un retiro
bool Account::makeWithdrawal(int withdrawal) {
    if (_amount < withdrawal) {       // Si no hay suficiente saldo
        _displayTimestamp();           // Muestra el timestamp
        std::cout << "Withdrawal: refused\n"; // Mensaje de rechazo
        return false;                  // Retorna false para indicar que el retiro fue rechazado
    }
    _amount -= withdrawal;            // Resta el retiro del saldo
    _nbWithdrawals++;                 // Aumenta el número de retiros
    _totalAmount -= withdrawal;       // Resta el retiro del total global
    _totalNbWithdrawals++;            // Aumenta el total global de retiros
    _displayTimestamp();              // Muestra el timestamp
    std::cout << "Withdrawal: " << withdrawal << "; New balance: " << _amount << "; Total withdrawals: " << _nbWithdrawals << "\n"; // Mensaje de retiro
    return true;                      // Retorna true para indicar que el retiro fue exitoso
}

// Método para mostrar el estado de una cuenta
void Account::displayStatus() const {
    _displayTimestamp();               // Muestra el timestamp
    std::cout << "Account " << _accountIndex << "; balance " << _amount
              << "; deposits: " << _nbDeposits << "; withdrawals: " << _nbWithdrawals << "\n"; // Muestra el estado de la cuenta
}

// Método estático para mostrar la información global
void Account::displayAccountsInfos() {
    _displayTimestamp(); // Muestra el timestamp
    std::cout << "Total accounts: " << _nbAccounts << "; Total balance: " << _totalAmount
              << "; Total deposits: " << _totalNbDeposits << "; Total withdrawals: " << _totalNbWithdrawals << "\n"; // Muestra la información global
}
