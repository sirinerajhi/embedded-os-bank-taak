#include "terminal.h"

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

std::string Terminal::sendTransaction(User * buyer, int amount, Bank * bank){
   if(this->_bancontact->checkUserBalance(buyer, amount, bank)){
        return "Transaction accepted";
   } else {
        return "Transaction refused";
   }
}

std::string Terminal::getTerminalID(){
    return this->_terminalID;
}

void Terminal::setSeller(User * seller){
    this->_seller = seller;
}

void Terminal::setTerminalID(std::string id){
    this->_terminalID = id;
}