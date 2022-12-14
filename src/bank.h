#pragma once

#include "mbed.h"
#include <string>
#include "user.h"
#include <vector>
#include "payment.h"

class Bank {

    // Functies van de Bank klasse (uitleg in .cpp file)
    public:
        Bank();
        Bank(std::string name, std::string bankID);
        void addUser(User * user);
        bool checkBalance(User * user, int amount);
        void pay(Payment transaction);
        bool checkPaymentTime(Payment payment);
        std::string getBankID(void);

    // Attributen van de Bank klasse
    private:
        std::vector<User *> _userList; // Een vector waarin alle klanten van de bank zich in bevinden
        std::string _name = "";        // De naam van de bank
        std::string _bankID = "";      // Het ID van de bank (die gebruikt wordt om rekeningnr's aan te maken voor de gebruiker) 
    
};
