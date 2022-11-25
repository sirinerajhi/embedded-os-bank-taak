#pragma once

#include "mbed.h"
#include "user.h"
#include <string>

class Bancontact {
    public:
        Bancontact();
        Bancontact(std::string id);
        int getCount(void);
        bool addConnection(void);
        bool checkUserBalance(User * buyer, int amount);

    private:
        std::string _id = "";
        int _counter = 0;
};