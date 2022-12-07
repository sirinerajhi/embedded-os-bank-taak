#pragma once
#include "user.h"

class Payment {

    public:
        Payment(User * buyer, User * seller, int amount);
        bool Execute(void);
        User * getSeller(void);
        User * getBuyer(void);
        int getAmount(void);

    private:
        User * _buyer = nullptr;
        User * _seller = nullptr;
        int _amount = 0;

};