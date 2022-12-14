#pragma once

#include "mbed.h"
#include "user.h"
#include <string>
#include <vector>
#include "bank.h"

class Bancontact {

    // Functies van de Bancontact klasse (uitleg in .cpp file)
    public:                    
        Bancontact();
        Bancontact(std::string id);
        int getCount(void);
        bool addConnection(std::string terminal);
        bool checkUserBalance(User * buyer, int amount, Bank * bank);
        bool connectToBank(Bank * bank);
        Bank * getUserBank(User * user);

    // Attributen van de Bancontact klasse
    private:
        std::string _id = "";                   // ID van de bancontact server
        std::vector<std::string> _terminals;    // vector die alle verbonden terminals bijhoudt waarmee de server in verbinding staat
        std::vector<Bank *> _connectedBanks;    // vector die alle banken bijhoudt waarmee de server in verbinding staat

};