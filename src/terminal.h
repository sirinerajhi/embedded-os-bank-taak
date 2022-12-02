#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include "bancontact.h"
#include "bank.h"

class Terminal {
      
    public:
        Terminal();
        Terminal(std::string terminalID, User * seller, Bancontact * bancontact);
        std::string sendTransaction(User * buyer, int amount, Bank * bank);
        User getSeller();
        std::string getTerminalID();
        void setSeller(User * seller);
        void setTerminalID(std::string id);

    private:
        std::string _terminalID = "";
        User * _seller = nullptr;
        Bancontact * _bancontact = nullptr;
        bool _connection = false;

};