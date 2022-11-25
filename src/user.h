#pragma once

#include "mbed.h"
#include <string>

class User {

    public:
        User();
        User(int balance, std::string bank);
        void pay(int amount, User * receiver);
        int getBalance(void);
        void setBalance(int amount);
        std::string getBank(void);
        void setBank(std::string bank);
        void receiveMoney(int amount);
        bool checkPaymentTime(User * receiver);
        std::string getID();

    private:
        int _balance = 0;
        std::string _bank = "";
        
};