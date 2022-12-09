#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include "bancontact.h"
#include "bank.h"

class Terminal {
      
    public:
        Terminal();
        // Terminal(std::string terminalID, User * seller, Bancontact * bancontact);
        Terminal(std::string terminalID, User * seller, Bancontact * bancontact);
        bool sendTransaction(User * buyer, int amount);
        User * getSeller(void);
        std::string getTerminalID();
        void setSeller(User * seller);
        void setTerminalID(std::string id);
        Bancontact * getBancontact(void);
        

    private:
        std::string _terminalID = "";
        User * _seller = nullptr;
        Bancontact * _bancontact = nullptr;
        bool _connection = false;
};