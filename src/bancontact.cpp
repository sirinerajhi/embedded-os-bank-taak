#include "bancontact.h"
#include <iostream>

Bancontact::Bancontact() : Bancontact(""){} // Default constructor van de bancontact klasse. Deze roept de andere constructor aan en geeft een lege string mee als ID.

Bancontact::Bancontact(std::string id){     // Constructor van Bancontact waarin een ID kan in meegegeven worden.
    this->_id = id;
}

bool Bancontact::addConnection(std::string terminal){   // Functie om een terminal met een server te verbinden.
    if (this->_terminals.size() <= 3){                  // Er wordt eerst gecheckt of er nog geen 3 terminals verbonden zijn, zo niet dan wordt de verbinding gemaakt.
        _terminals.push_back(terminal);                 // Anders wordt er geen verbinding gemaakt.
        return true;                                    // Alle terminals worden bijgehouden in een vector.
    } else {                                            // Als de verbinding gemaakt kan worden returned dez functie 'true', anders 'false'.
        return false;
    }
}

bool Bancontact::connectToBank(Bank * bank){            // Functie om de server met een bank te verbinden.
    _connectedBanks.push_back(bank);                    // Alle betrokken banken worden bijgehouden in een vector.
    return true;                                        // Als de bank toegevoegd is returned de functie 'true'.
}

Bank * Bancontact::getUserBank(User * user){                                    // Functie om aan de bank van de gebruiker te geraken.
    std::string userID = user->getUserID();                                     // Dit wordt gedaan door de userID van de gebruiker op te vragen.
                                                                                // Deze bestaat uit de bank ID en een nummer, dus de BANK ID kan hier makkelijk uitgehaald worden door het nummer te verwijderen.
    std::string userBankID = userID.substr(0,userID.size()-1);                  // Dan wordt in de vector met banken gekeken welke bank overeen komt met het ID.
    std::cout << "User ID : " << userID << std::endl;                           // De match wordt adhv een pointer gereturned.
    std::cout << "User bank ID = " << userBankID << std::endl;                  // Als de bank niet in verbinding zou staan met de server dan wordt er geen gereturned.

    Bank * userBank = nullptr;
    
    for (int i = 0; i < _connectedBanks.size() ; i++){
        if (userBankID == _connectedBanks[i]->getBankID()){
            std::cout << "Bank is " << _connectedBanks[i]->getBankID() << std::endl;
            userBank = _connectedBanks[i];
        }
    }
    if (userBank == nullptr) {
        std::cout << "Bank not found" << std::endl;
    }
    return userBank;

}

bool Bancontact::checkUserBalance(User * buyer, int amount, Bank * bank){       // Functie om via de bank na te gaan of de koper genoeg geld staan heeft.
                                                                                // Dit wordt gedaan via de bank zelf, meer uitleg over de checkBalance functie van de bank bevindt zich in de klasse van de bank.
    std::cout << "Bancontact checking balance with bank" << std::endl;          // Er wordt enkel true of false teruggegeven, dus se server weet nooit hoeveel er exact op de rekening staat.
    return bank->checkBalance(buyer, amount);                                   // Als het voldoende is dan wordt er 'true' gereturned, anders 'false' en kan de transactie niet doorgaan.
    
}