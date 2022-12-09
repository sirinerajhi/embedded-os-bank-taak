#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include <vector>
#include "payment.h"

class Bank {

    public:
        Bank();
        Bank(std::string name, std::string bankID);
        void addUser(User * user);
        bool checkBalance(User * user, int amount);
        void pay(Payment transaction);
        bool checkPaymentTime(Payment payment);
        std::string getBankID(void);


    private:
        std::vector<User *> _userList;
        std::string _name = "";
        std::string _bankID = "";
    
};
