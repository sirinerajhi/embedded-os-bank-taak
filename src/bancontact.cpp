#include "bancontact.h"
#include <iostream>

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

bool Bancontact::connectToBank(Bank * bank){
    _connectedBanks.push_back(bank);
    return true;
}

Bank * Bancontact::getUserBank(User * user){
    std::string userID = user->getUserID();

    std::string userBankID = userID.substr(0,userID.size()-1);
    // for (int i = 0; i < 5; i++){
    //     userBankID[i] += userID[i];
        
    // }
    std::cout << "User ID : " << userID << std::endl;
    std::cout << "User bank ID = " << userBankID << std::endl;

    for (int i = 0; i < _connectedBanks.size() ; i++){
        if (userBankID == _connectedBanks[i]->getBankID()){
            std::cout << "Bank is " << _connectedBanks[i]->getBankID() << std::endl;
            return (_connectedBanks[i]);
        }
        else {
            std::cout << "Bank not found" << std::endl;
            return nullptr;
        }
    }

}

bool Bancontact::checkUserBalance(User * buyer, int amount, Bank * bank){

    std::cout << "Bancontact checking balance with bank" << std::endl;
    return bank->checkBalance(buyer, amount);
    
}