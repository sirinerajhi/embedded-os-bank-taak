#include "terminal.h"
#include <iostream>

Terminal::Terminal() : Terminal("No ID", nullptr, nullptr){}    // Default constructor van de Terminal klasse. Deze roept de andere constructor aan en geeft aan dat er geen ID, verkoper of server aanwezig is.

Terminal::Terminal(std::string terminalID, User * seller, Bancontact * bancontact){     // Constructor van de Terminal klasse.
    this->_terminalID = terminalID;                                                     // Hierin wordt de verkoper en ID opgeslagen + verbinding opgezet met de server
    this->_seller = seller;                                                             // Als de server stuurt dat de verbinding goed opgezet is wordt het 'connection' attribuut op 'true' gezet.    
    if(bancontact->addConnection(this->getTerminalID())){                               // Lukt dit niet dan is de 'connection' false.
        this->_bancontact = bancontact;
        this->_connection = true;
    } else {
        this->_connection = false;
    }
}

bool Terminal::sendTransaction(User * buyer, int amount){                                           // Functie om een transactie op te starten.
    std::cout << "Terminal " << this->_terminalID << " processing transaction..." << std::endl;     // Eerst wordt er via de server de bank van de gebruiker opgevraagd
                                                                                                    // Dan wordt via de server gekeken of het saldo van de gebruiker voldoende is. (Hiervoor gaat de server dit navragen bij de bank; zie functies in bancontact.cpp en bank.cpp)
    Bank * bank = (_bancontact->getUserBank(buyer));                                                // Als alle partijen (server en bank) doorlopen zijn en geen van hen geeft een foutmelden wordt er 'true' gereturned om aan te geven dat dit een transactie is die door kan gaan.      
    if(this->_bancontact->checkUserBalance(buyer, amount, bank)){                                   // Wordt er ergens een foutmelding gegeven dan kan de transactie niet doorgaan ('false'). 
        std::cout << "Transaction accepted" << std::endl;
        return true;
    } else {
        std::cout << "Transaction refused" << std::endl;
        return false;
    }
    
}

std::string Terminal::getTerminalID(){          // Functie om het ID van de terminal te verkrijgen
    return this->_terminalID;
}

void Terminal::setSeller(User * seller){    // Functie om de verkoper/verkooppunt waar de terminal staat te wijzingen.
    this->_seller = seller;
}

User * Terminal::getSeller(void){   // Functie om de verkoper/verkooppunt waar de terminal staat te verkrijgen.           
    return this->_seller;
}

void Terminal::setTerminalID(std::string id){   // Functie om het ID te verkrijgen.
    this->_terminalID = id;
}

Bancontact * Terminal::getBancontact(void){     // Functie om het de server waarmee de terminal in verbinding staat te verkrijgen.
    return this->_bancontact;
}