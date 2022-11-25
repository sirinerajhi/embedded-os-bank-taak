#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include "bancontact.h"

class Terminal {

    private:
        std::string _terminalID = "";
        User * _seller = nullptr;
        Bancontact * _bancontact = nullptr;
        bool _connection = false;
        
    
    public:
        Terminal();
        Terminal(std::string terminalID, User * seller, Bancontact * bancontact);
        std::string sendTransaction(User * buyer, int amount);
        User getSeller();
        void setSeller(User * seller);
        std::string getTerminalID();
        void setTerminalID(std::string id);

};