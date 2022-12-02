#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include <vector>
#include "transaction.h"

class Bank {

    public:
        Bank();
        Bank(std::string name, std::string bankID);
        void addUser(User * user);
        bool checkBalance(User * user, int amount);
        void pay(Transaction transaction);
        void checkPaymentTime(Transaction transaction);


    private:
        std::vector<User *> _userList;
        std::string _name = "";
        std::vector<Transaction> dueTransactions;
        std::string _bankID = "";
    
};
