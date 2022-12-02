#pragma once

#include "mbed.h"
#include "user.h"
#include <string>
#include <vector>
#include "terminal.h"
#include "bank.h"

class Bancontact {

    public:
        Bancontact();
        Bancontact(std::string id);
        int getCount(void);
        bool addConnection(std::string terminal);
        bool checkUserBalance(User * buyer, int amount, Bank * bank);

    private:
        std::string _id = "";
        int _counter = 0;
        std::vector<std::string> _terminals;

};