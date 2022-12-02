#include "transaction.h"

Transaction::Transaction(User * buyer, User * seller, int amount){
    this->_buyer = buyer;
    this->_seller = seller;
    this->_amount = amount;
}

bool Transaction::Execute(void){
    this->_buyer->updateBalance(-(this->_amount));
    this->_seller->updateBalance((this->_amount));
}

User * Transaction::getSeller(void){
    return this->_seller;
}

User * Transaction::getBuyer(void){
    return this->_buyer;
}

int Transaction::getAmount(void){
    return this->_amount;
}