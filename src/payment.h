#pragma once
#include "user.h"

class Payment {

    // Functies van de Payment klasse (uitleg in .cpp file)
    public:
        Payment();
        Payment(User * buyer, User * seller, int amount);
        User * getSeller(void);
        User * getBuyer(void);
        int getAmount(void);
        void setAmount(int amount);
        void setBuyer(User * buyer);
        void setSeller(User * seller);

    // Attributen van de Payment klasse
    private:
        User * _buyer = nullptr;    // De koper in de transactie
        User * _seller = nullptr;   // De verkoper in de transactie
        int _amount = 0;            // Het bedrag van de transactie

};