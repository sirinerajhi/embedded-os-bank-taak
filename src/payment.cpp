#include "payment.h"

Payment::Payment(User * buyer, User * seller, int amount){
    _buyer = buyer;
    _seller = seller;
    _amount = amount;
}
bool Payment::Execute(void){
    this->_buyer->updateBalance(-(this->_amount));
    this->_seller->updateBalance((this->_amount));
    return true;
}

User * Payment::getSeller(void){
    return this->_seller;
}

User * Payment::getBuyer(void){
    return this->_buyer;
}

int Payment::getAmount(void){
    return this->_amount;
}