#pragma once

#include <string>

class User {

    // public:
    //     User();
    //     User(int balance, std::string bank);
    //     void pay(int amount, User * receiver);
    //     int getBalance(void);
    //     void setBalance(int amount);
    //     std::string getBank(void);
    //     void setBank(std::string bank);
    //     void receiveMoney(int amount);
    //     bool checkPaymentTime(User * receiver);
    //     std::string getID();

    // private:
    //     int _balance = 0;
    //     std::string _bank = "";

    public:
        User();
        User(int balance);
        int getBalance(void);
        void setBalance(int balance);
        void updateBalance(int balance);
        void setUserID(std::string userID);
        std::string getUserID(void);

    private:
        int _balance = 0;
        std::string _accountID = "";
        
};