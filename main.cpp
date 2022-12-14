#include "src/bancontact.h"
#include "src/bank.h"
#include "src/terminal.h"
#include "src/payment.h"
#include "src/user.h"
#include <iostream>
#include "mbed.h"
#include <random>
#include <ctime>

DigitalOut led(LED1);
Ticker ticker;
int time_clock = 0;

// Users (zowel kopers als verkopers/verkooppunten) - elk met een startkapitaal van €50
User breadPit(50);
User restaurant(50);
User starbucks(50);
User onthaal(50);
User student1(50);
User student2(50);
User student3(50);
std::vector<User *> buyerList;
std::vector<User *> sellerList;

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
std::vector<Terminal *> terminalList;

// Threads
Thread terminal_t;
Thread paymentNow_t;
Thread paymentMidnight_t;
Thread payDay_t;
Thread updateTransaction_t;
Thread executeMidnightPayments_t;
Thread timeClock_t;

// Mails & Queues

Queue<Payment, 1000> sameBank_m;
Queue<Payment, 1000> otherBank_m;

//Semaphore & Mutex

Semaphore server_control(2);
Mutex mutex;

// Terminal * terminal, User * buyer, int amount
// we gaan dat in een struct moeten steken, want je kan blijkbaar maar 1 object als argument meegeven met een callback.

//struct for manage transactions:
typedef struct{ 
    Terminal * terminal;
    User * buyer;
    int amount;
} transaction_struct;

transaction_struct randomTransaction;

// Declaratie van het loon van de studenten
int salary = 15;

// Vector voor alle midnight transacties
std::vector<Payment *> midnightTransactionsList;

// Simpele functie om de led te toggelen

void toggleLED() {
    for (int i = 0; i <= 5; i++){
        led =! led;
        ThisThread::sleep_for(200ms);
    }
    led = 0;
}


void flagSetter(){
    while(true) {
        paymentMidnight_t.flags_set(0x1);
        payDay_t.flags_set(0x2);
    }
}

void setRandomTransaction(void){
    while(true) {
        int randomAmount = 1 + static_cast <int> (rand()) / ( static_cast <int> (RAND_MAX/(5)));
        int randomBuyer = 0 + static_cast <int> (rand()) / ( static_cast <int> (RAND_MAX/(buyerList.size()-1)));
        int randomTerminal = 0 + static_cast <int> (rand()) / ( static_cast <int> (RAND_MAX/(terminalList.size()-1)));
        randomTransaction.amount = randomAmount;
        randomTransaction.buyer = buyerList[randomBuyer];
        randomTransaction.terminal = terminalList[randomTerminal];
    }
}

void manageTransactions(transaction_struct * receivedTransaction){

    while(true){
        
        server_control.acquire();
        ThisThread::sleep_for(1s);
        mutex.lock();
        if (receivedTransaction->terminal->sendTransaction(receivedTransaction->buyer, receivedTransaction->amount)){
            Bancontact *bancontactServer = receivedTransaction->terminal->getBancontact();
            Bank * buyerBank = bancontactServer->getUserBank(receivedTransaction->buyer);
            Payment payment(receivedTransaction->buyer, receivedTransaction->terminal->getSeller(), receivedTransaction->amount);
            
            if (buyerBank->checkPaymentTime(&payment)){
                sameBank_m.try_put_for(Kernel::wait_for_u32_forever, &payment);
                std::cout << "SAME BANK PAYMENT" << std::endl;
            } 
            else {
                mutex.lock();
                midnightTransactionsList.push_back(&payment)
                mutex.unlock();
                std::cout << "OTHER BANK PAYMENT" << std::endl;
            }
        }
        mutex.unlock();
        server_control.release();
        
    }
}

void managePaymentNow(void){
    while(true){
            if(time_clock < 240){
            std::cout << "Executing direct transactions" << std::endl;
            Payment * payment; 
            sameBank_m.try_get_for(Kernel::wait_for_u32_forever, &payment);
            std::string bankID = payment->getBuyer()->getUserID().substr(0,payment->getBuyer()->getUserID().size()-1);
            Bank * bank = nullptr;
            if (bankID == "BEKBC"){
                bank = &KBC;
            } else if(bankID == "BEBEL") {
                bank = &Belfius;
            }
            bank->pay((payment));
        }
    }
}

void managePaymentMidnight(void){
    while(true){
        ThisThread::flags_wait_any(0x1);
        mutex.lock();
        for(int i = 0; i < midnightTransactionsList.size() ; i++){
            otherBank_m.try_put_for(Kernel::wait_for_u32_forever, &midnightTransactionsList[i]);
            midnightTransactionsList[i] = nullptr;
        }
        mutex.unlock();
    }
}

void executeMidnightPayments(void){
    while(true) {
        if(time_clock > 240 && time_clock < 301) {
            Payment * payment;
            otherBank_m.try_get_for(Kernel::wait_for_u32_forever, &payment);
            std::string bankID = payment->getBuyer()->getUserID().substr(0,payment->getBuyer()->getUserID().size()-1);
            Bank * bank = nullptr;
            if (bankID == "BEKBC"){
                bank = &KBC;
            } else if(bankID == "BEBEL") {
                bank = &Belfius;
            }
            bank->pay((payment));
            toggleLED();
        }
        time_clock = 0;
    }
}

void payStudents(int * salary) {
    while(true) {
        ThisThread::flags_wait_any(0x2);
        student1.updateBalance(*salary);
        student2.updateBalance(*salary);
        student3.updateBalance(*salary);
        std::cout << "Students have received their paycheck" << std::endl;
    }
}

void updateTime(){
    while(true) {
        time_clock++;
        ThisThread::sleep_for(1s);
    }
}

int main(){

    ticker.attach(&flagSetter,300);
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

    terminalList.push_back(&t1);
    terminalList.push_back(&t2);
    terminalList.push_back(&t3);
    terminalList.push_back(&t4);
    terminalList.push_back(&t5);
    terminalList.push_back(&t7);
    terminalList.push_back(&t8);
    terminalList.push_back(&t9);

    buyerList.push_back(&student1);
    buyerList.push_back(&student2);
    buyerList.push_back(&student3);

    sellerList.push_back(&breadPit);
    sellerList.push_back(&starbucks);
    sellerList.push_back(&onthaal);
    sellerList.push_back(&restaurant);

    timeClock_t.start(callback(updateTime));
    updateTransaction_t.start(callback(setRandomTransaction));
    terminal_t.start(callback(manageTransactions, &randomTransaction));
    paymentNow_t.start(callback(managePaymentNow));
    paymentMidnight_t.start(callback(managePaymentMidnight));
    payDay_t.start(callback(payStudents, &salary));
    executeMidnightPayments_t.start(callback(executeMidnightPayments));
}