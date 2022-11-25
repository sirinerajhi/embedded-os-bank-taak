#include "user.h"

User::User() : User(0, "No Bank"){}

User::User(int balance, std::string bank){
    this->_balance = balance;
    this->_bank = bank;
    
}

void User::pay(int amount, User * receiver){
    this->_balance -= amount;
    receiver->receiveMoney(amount);
}

void User::receiveMoney(int amount){
    this->_balance += amount;
}

bool User::checkPaymentTime(User * receiver){
    if (this->_bank == receiver->getBank()){
        return true;
    } else {
        return false;
    }
}

int User::getBalance(void){
    return this->_balance;
}

void User::setBalance(int amount){
    this->_balance = amount;
}

std::string User::getBank(void){
    return this->_bank;
}

void User::setBank(std::string bank){
    this->_bank = bank;
}