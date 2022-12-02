// #include "bancontact.h"
// #include "bank.h"
// #include "terminal.h"
// #include "transaction.h"
//#include "user.h"
#include <iostream>
#include "mbed.h"

DigitalOut led1(LED1);

int main(){

    while(true){
        led1 =! led1;
        ThisThread::sleep_for(200ms);
    }
}