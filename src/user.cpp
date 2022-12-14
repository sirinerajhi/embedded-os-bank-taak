#include "user.h"

User::User() : User(0){}    // Default constructor van de gebruiker. Deze roept de andere constructor aan en geeft aan dat er geen geld op de rekening staat.

User::User(int balance){        // Constructor van de gebruiker. Hierin wordt aangegeven hoeveel geld er op de rekening staat.
    this->_balance = balance;
}

int User::getBalance(void){     // Functie om op te vragen hoeveel geld er op de rekening staat.
    return this->_balance;
}

void User::setBalance(int balance){        // Functie om het saldo volledig opnieuw in te stellen (dus geen betaling uitvoeren of ontvangen).
    this->_balance = balance;
}

void User::updateBalance(int balance){      // Functie om het salde te updaten na een betaling.
    this->_balance += balance;
}

void User::setUserID(std::string userID) {  // Functie die gebruikt wordt om het rekeningnummer in te geven. Dit wordt door de bank aangeroepen om dan via de bank het nummer te verkrijgen.
    this->_accountID = userID;
}

std::string User::getUserID(void) {     // Functie om het rekeningnummer van de gebruiker op te vragen.
    return this->_accountID;
}

