#include "bank.h"
#include "src/bancontact.h"
#include "src/bank.h"
#include "src/terminal.h"
#include "src/payment.h"
#include "user.h"
#include <iostream>
#include "mbed.h"

DigitalOut led1(LED1);

// Users
User breadPit(50.0);
User restaurant(50.0);
User starbucks(50.0);
User onthaal(50.0);
User student1(50.0);
User student2(50.0);
User student3(50.0);

// Banks
Bank KBC("KBC", "BEKBC");
Bank Belfius("Belfius", "BEBEL");

// Bancontact servers
Bancontact b1("b001");
Bancontact b2("b002");
Bancontact b3("b003");

// Terminals
Terminal t1("t001", &onthaal, &b1);
Terminal t2("t002", &onthaal, &b1);
Terminal t3("t003", &starbucks, &b1);
Terminal t4("t004", &starbucks, &b2);
Terminal t5("t005", &breadPit, &b2);
Terminal t6("t006", &breadPit, &b2);
Terminal t7("t007", &restaurant, &b3);
Terminal t8("t008", &restaurant, &b3);
Terminal t9("t009", &restaurant, &b3);

// Threads
Thread terminal1_t;
Thread terminal2_t;
Thread terminal3_t;
Thread terminal4_t;
Thread terminal5_t;
Thread terminal6_t;
Thread terminal7_t;
Thread terminal8_t;
Thread terminal9_t;
Thread paymentNow_t;
Thread paymentMidnight_t;

// Mails & Queues

Mail<Payment*, 3> sameBank_m;
Mail<Payment*, 3> otherBank_m;

void manageTransactions(Terminal * terminal, User * buyer, int amount){

    while(true){
        if (terminal->sendTransaction(buyer, amount)){
            Bancontact * bancontactServer = terminal->getBancontact();
            Bank * buyerBank = bancontactServer->getUserBank(buyer);
            Payment payment(buyer, terminal->getSeller(), amount);
            Payment *pointerToPayment = &payment;
            
            if (buyerBank->checkPaymentTime(payment)){
                pointerToPayment = &sameBank_m.try_alloc_for(Kernel::wait_for_u32_forever);
                sameBank_m.put(&pointerToPayment);
            } else {
                pointerToPayment = &otherBank_m.try_alloc_for(Kernel::wait_for_u32_forever);
                otherBank_m.put(&pointerToPayment);
            }
        }

    }
}

void managePaymentNow(void){

    while(true){
        Payment * payment = &sameBank_m.try_get_for(Kernel::wait_for_u32_forever);
        std::string bankID = payment->getBuyer()->getUserID().substr(0,payment->getBuyer()->getUserID().size()-1);
        Bank * bank = nullptr;
        if (bankID == "BEKBC"){
            bank = &KBC;
        } else if(bankID == "BEBEL") {
            bank = &Belfius;
        }
        bank->pay(*(payment));
    }
}

void managePaymentMidnight(void){

    while(true){



    }
}

int main(){

    KBC.addUser(&breadPit);
    KBC.addUser(&starbucks);
    KBC.addUser(&student1);
    KBC.addUser(&student3);
    Belfius.addUser(&restaurant);
    Belfius.addUser(&onthaal);
    Belfius.addUser(&student2);

    b1.connectToBank(&KBC);
    b2.connectToBank(&KBC);
    b3.connectToBank(&KBC);
    b1.connectToBank(&Belfius);
    b2.connectToBank(&Belfius);
    b3.connectToBank(&Belfius);

    terminal1_t.start(callback(manageTransactions, &t1, &student1, 5));

    while(true){
        //led1 =! led1;
        //ThisThread::sleep_for(200ms);
    }
}