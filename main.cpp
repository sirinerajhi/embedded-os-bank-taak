//#include "src/bancontact.h"
#include "bank.h"
// #include "src/terminal.h"
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

    t1.sendTransaction(&student1, 3.10);

    while(true){
        //led1 =! led1;
        //ThisThread::sleep_for(200ms);
    }
}