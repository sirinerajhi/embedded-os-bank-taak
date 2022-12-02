#include "bank.h"

Bank::Bank() : Bank("No name", "No ID") {}

Bank::Bank(std::string name, std::string bankID) {
    this->_name = name;
    this->_bankID = bankID;
}

void Bank::addUser(User * user){
    _userList.push_back(user);
    std::string userID = this->_bankID + to_string(_userList.size());
    user->setUserID(userID);
}

bool Bank::checkBalance(User * user, int amount){
    if (user->getBalance() > amount) return true;
    else return false;
}

void Bank::checkPaymentTime(Transaction transaction) {
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

void Bank::pay(Transaction transaction){
    transaction.getBuyer()->updateBalance(-(transaction.getAmount()));
    transaction.getSeller()->updateBalance(transaction.getAmount());
}