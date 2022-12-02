#include "bancontact.h"

Bancontact::Bancontact() : Bancontact(""){}

Bancontact::Bancontact(std::string id){
    this->_id = id;
}

int Bancontact::getCount(void){
    return this->_counter;
}

bool Bancontact::addConnection(std::string terminal){
    if (this->_counter < 3){
        _terminals.push_back(terminal);
        this->_counter++;
        return true;
    } else {
        return false;
    }
}

bool Bancontact::checkUserBalance(User * buyer, int amount, Bank * bank){
    // std::string userID = buyer->getUserID();
    // std::string bankID = "";
    // for (int i = 0; i < 3 ; i++){
    //     bankID[i] = userID[i];
    // }

    return  bank->checkBalance(buyer, amount);
    
    // check via userID welke bank het is
    // roep de checkBalance functie van de bank aan
    // stuur true of false
}