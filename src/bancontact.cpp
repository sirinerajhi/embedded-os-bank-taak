#include "bancontact.h"

Bancontact::Bancontact() : Bancontact(""){

}

Bancontact::Bancontact(std::string id){
    this->_id = id;
}

int Bancontact::getCount(void){
    return this->_counter;
}

bool Bancontact::addConnection(void){
    if (this->_counter < 3){
        this->_counter++;
        return true;
    } else {
        return false;
    }
}

bool Bancontact::checkUserBalance(User * buyer, int amount){
    if(buyer->getBalance() >= amount){
        return true;
    } else {
        return false;
    }
}