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

void Bank::checkPaymentTime(Payment transaction) {
    int counter = 0;
    for(auto x : _userList){
        if (x == transaction.getBuyer()){
            pay(transaction);
            counter++;
        }
    }
    if (counter == 0){
        dueTransactions.push_back(transaction);
    }
}

void Bank::pay(Payment transaction){
    transaction.getBuyer()->updateBalance(-(transaction.getAmount()));
    transaction.getSeller()->updateBalance(transaction.getAmount());
}

 std::string Bank::getBankID(void) {
    return this->_bankID;
 }