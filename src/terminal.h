#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include "bancontact.h"
#include "bank.h"

class Terminal {

    // Functies van de Terminal klasse (uitleg in .cpp file)  
    public:
        Terminal();
        Terminal(std::string terminalID, User * seller, Bancontact * bancontact);
        bool sendTransaction(User * buyer, int amount);
        User * getSeller(void);
        std::string getTerminalID();
        void setSeller(User * seller);
        void setTerminalID(std::string id);
        Bancontact * getBancontact(void);
        
    // Attributen van de Terminal klasse
    private:
        std::string _terminalID = "";       // ID van de terminal
        User * _seller = nullptr;           // Verwijzing naar de verkoper of het verkooppunt waar de terminal zich bevindt
        Bancontact * _bancontact = nullptr; // Verwijzing naar de Bancontactserver waarmee de terminal verbonden is
        bool _connection = false;           // Een check om te zien of er verbinding is met de server
};