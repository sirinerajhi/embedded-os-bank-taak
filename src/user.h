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

    // Attributen van de User klasse
    private:
        int _balance = 0;               // Saldo van de gebruiker
        std::string _accountID = "";    // Rekeningnummer van de gebruiker (wordt verkregen via de bank)
        
};