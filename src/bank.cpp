#include "bank.h"
#include <iostream>

Bank::Bank() : Bank("No name", "No ID") {}          // Default constructor van Bank. Deze roept de andere constructor aan en geeft aan dat er geen ID of naam is.

Bank::Bank(std::string name, std::string bankID) {  // Constructor van Bank. Hierin worden de naam en ID gespecifieerd.
    this->_name = name;
    this->_bankID = bankID;
}

void Bank::addUser(User * user){                                            // Functie om een klant toe te voegen aan de bank.
    _userList.push_back(user);                                              // De klant wordt eerst (adhv een pointer) aan de vector toegevoegd.
    std::string userID = this->_bankID + to_string(_userList.size());       // Dan wordt er een uniek rekeningnr (aka userID) aangemaakt voor de klant.
    user->setUserID(userID);                                                // Het rekeningnr wordt dan ook ingesteld bij de klant zelf.
    std::cout << "User " << user->getUserID() << " added." << std::endl;
}

bool Bank::checkBalance(User * user, int amount){                           // Functie om het saldo van de gebruiker te checken.
    std::cout << "Bank checking user balance" << std::endl;                 // De functie van meegegeven gebruiker om het saldo te checken wordt aangeroepen.
    if (user->getBalance() > amount){                                       // Als deze groter is dan het gespecifieerd bedrag wordt er 'true' gereturned, anders 'false'.
        return true;
    } 
    else 
    {
        return false;
    } 
}

bool Bank::checkPaymentTime(Payment payment) {                                                  // Functie om te zien als de betaling nu of om middernacht moet uitgevoerd worden.
    User * buyer = payment.getBuyer();                                                          // Eerst worden zowel de koper als verkoper uit de transactie gehaald.
    User * seller = payment.getSeller();                                                        // Dan wordt uit beide partijen het rekeningnr uitgelezen en hieruit de bank afgeleid.
    std::string buyerBankID = buyer->getUserID().substr(0,buyer->getUserID().size()-1);         // Als het bankID van beide gebruikers overeenkomt (= ze zitten bij dezelfde bank) dan wordt 'true' gereturned om aan te geven dat de transactie meteen door mag gaan.
    std::string sellerBankID = seller->getUserID().substr(0,seller->getUserID().size()-1);      // Als dit niet overeen komt dan returnt de functie 'false' en moet er gewacht worden tot 's nachts.
    
    if(buyerBankID == sellerBankID) return true;
}

void Bank::pay(Payment transaction){                                        // Functie om een betaling uit te voeren.
    transaction.getBuyer()->updateBalance(-(transaction.getAmount()));      // De koper en verkoper worden uit de transactie gehaald. 
    transaction.getSeller()->updateBalance(transaction.getAmount());        // Beide hun saldo's worden aangepast, positief bedrag voor de verkoper, negatief bedrag voor de koper

    std::cout << "Payment from " << transaction.getBuyer()->getUserID() << " to " << transaction.getSeller()->getUserID() << " of " << transaction.getAmount() << " has been paid." << std::endl;
}

 std::string Bank::getBankID(void) {        // Functie om de bankID van de bank terug te geven
    return this->_bankID;                   // Deze ID wordt gebruikt om de unieke rekeningnr's van de klanten aan te maken
 }