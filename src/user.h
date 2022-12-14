#pragma once

#include <string>

class User {

    // Functies van de User klasse (uitleg in .cpp file)
    public:
        User();
        User(int balance);
        int getBalance(void);
        void setBalance(int balance);
        void updateBalance(int balance);
        void setUserID(std::string userID);
        std::string getUserID(void);
        void updateReserved(int amount);
        int getReserved(void);

    // Attributen van de User klasse
    private:
        int _balance = 0;               // Saldo van de gebruiker
        std::string _accountID = "";    // Rekeningnummer van de gebruiker (wordt verkregen via de bank)
        int _reservedBalance = 0;       // Bedrag die al gereserveerd staat maar nog niet van de rekening is gegaan. (Vb. als een transactie pas 's nachts gebeurt)
        
};