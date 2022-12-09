#include "terminal.h"
#include <iostream>

Terminal::Terminal() : Terminal("No ID", nullptr, nullptr){}

Terminal::Terminal(std::string terminalID, User * seller, Bancontact * bancontact){
    this->_terminalID = terminalID;
    this->_seller = seller;
    if(bancontact->addConnection(this->getTerminalID())){
        this->_bancontact = bancontact;
        this->_connection = true;
    } else {
        this->_connection = false;
    }
}

bool Terminal::sendTransaction(User * buyer, int amount){
    std::cout << "Terminal processing transaction..." << std::endl;
    
    Bank * bank = (_bancontact->getUserBank(buyer));
    if(this->_bancontact->checkUserBalance(buyer, amount, bank)){
        std::cout << "Transaction accepted" << std::endl;
        return true;
    } else {
        std::cout << "Transaction refused" << std::endl;
        return false;
    }
    
}

std::string Terminal::getTerminalID(){
    return this->_terminalID;
}

void Terminal::setSeller(User * seller){
    this->_seller = seller;
}

User * Terminal::getSeller(void){
    return this->_seller;
}

void Terminal::setTerminalID(std::string id){
    this->_terminalID = id;
}

Bancontact * Terminal::getBancontact(void){
    return this->_bancontact;
}