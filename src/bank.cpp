#include "bank.h"
#include <iostream>

Bank::Bank() : Bank("No name", "No ID") {}

Bank::Bank(std::string name, std::string bankID) {
    this->_name = name;
    this->_bankID = bankID;
}

void Bank::addUser(User * user){
    _userList.push_back(user);
    std::string userID = this->_bankID + to_string(_userList.size());
    user->setUserID(userID);
    std::cout << "User " << user->getUserID() << " added." << std::endl;
}

bool Bank::checkBalance(User * user, int amount){
    std::cout << "Bank checking user balance" << std::endl;
    //ThisThread::sleep_for(20ms);
    if (user->getBalance() > amount){
        return true;
    } 
    else 
    {
        return false;
    } 
}

bool Bank::checkPaymentTime(Payment payment) {
    User * buyer = payment.getBuyer();
    User * seller = payment.getSeller();
    std::string buyerBankID = buyer->getUserID().substr(0,buyer->getUserID().size()-1);
    std::string sellerBankID = seller->getUserID().substr(0,seller->getUserID().size()-1);
    
    if(buyerBankID == sellerBankID) return true;
}

void Bank::pay(Payment transaction){
    transaction.getBuyer()->updateBalance(-(transaction.getAmount()));
    transaction.getSeller()->updateBalance(transaction.getAmount());

    std::cout << "Payment from " << transaction.getBuyer()->getUserID() << " to " << transaction.getSeller()->getUserID() << " of " << transaction.getAmount() << " has been paid." << std:endl;
}

 std::string Bank::getBankID(void) {
    return this->_bankID;
 }