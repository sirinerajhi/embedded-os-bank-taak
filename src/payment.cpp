#include "payment.h"

Payment::Payment() : Payment(nullptr, nullptr, 0) {}        // Default constructor van de Payment klasse. Deze roept de andere constructor aan een geeft mee dat er geen partijen en geen bedrag is.

Payment::Payment(User * buyer, User * seller, int amount){  // Constructor van de Payment klasse.
    _buyer = buyer;                                         // Deze maakt per transactie (= per payment dus) een koper, verkoper en bedrag aan.
    _seller = seller;                                       // De transactie kan dan door andere klasses uitgelezen worden en er de nodige partijen/gegevens uit halen.
    _amount = amount;
}

User * Payment::getSeller(void){    // Functie om de verkoper uit de transactie uit te lezen.
    return this->_seller;
}

User * Payment::getBuyer(void){     // Functie om de koper uit de transactie uit te lezen.
    return this->_buyer;
}

int Payment::getAmount(void){       // Functie om het bedrag uit de transactie uit te lezen.                         
    return this->_amount;   
}

void Payment::setAmount(int amount){    // Functie om het bedrag in de transactie aan te passen indien nodig.
    this->_amount = amount;
}

void Payment::setBuyer(User * buyer){   // Functie om de koper in de transactie aan te passen indien nodig.
    this->_buyer = buyer;
}

void Payment::setSeller(User * seller){     // Functie om de koper in de transactie aan te passen indien nodig.
    this->_seller = seller;
}