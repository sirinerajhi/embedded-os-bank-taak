#include "src/bancontact.h"
#include "src/bank.h"
#include "src/terminal.h"
#include "src/payment.h"
#include "src/user.h"
#include <iostream>
#include "mbed.h"
#include <random>
#include <ctime>

// Declaraties van de LED, ticker en gesimuleerde clock
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

// Lijst van kopers en verkopers die gebruikt wordt om random uit te kiezen voor de transacties
std::vector<User *> buyerList;
std::vector<User *> sellerList;

// Banken
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

// Lijst met terminals die gebruikt wordt om random uit te kiezen voor de transacties
std::vector<Terminal *> terminalList;

// Threads
Thread terminal_t;                  // Thread voor alle betalingen via alle terminals (worden random gekozen, meer uitleg in functies onderaan).
Thread paymentNow_t;                // Thread voor de betalingen die meteen uitgevoerd moeten worden.
Thread paymentMidnight_t;           // Thread voor het beheren van de betalingen die 's nachts uitgevoerd moeten worden.
Thread payDay_t;                    // Thread voor het betalen van het loon van de studenten (kopers).
Thread updateTransaction_t;         // Thread die de random transactie update met nieuwe waarden.
Thread executeMidnightPayments_t;   // Thread die de betalingen 's nachts uitvoert.
Thread timeClock_t;                 // Thread die de tijd bijhoudt.

// Queues
Queue<Payment, 50> sameBank_m;      // Queue voor de betalingen binnen dezelfde bank = om meteen uit te voeren.
Queue<Payment, 50> otherBank_m;     // Queue voor de betalingen in verschillende banken = om 's nachts uit te voeren.

//Semaphore & Mutex
Semaphore server_control(2);        // Semaphore om max. 2 terminals met de bancontact server tegelijk te laten verbinden.
Mutex mutex;                        // Mutex om gedeelde resources te beschermen.

//struct om de transactie in 1 object te kunnen meegeven aan de callback
typedef struct{ 
    Terminal * terminal;
    User * buyer;
    int amount;
} transaction_struct;

// 1 struct die de random transactie voorstelt. Deze wordt bij elke transactie geüpdated zodat elke transactie andere random waarden krijgt
transaction_struct randomTransaction;

// Declaratie van het loon van de studente
int salary = 15;

// Vector voor alle midnight transacties
std::vector<Payment *> midnightTransactionsList;

// Simpele functie om de led te toggelen en deze op het einde zeker op 'uit' te zetten.
void toggleLED() {
    for (int i = 0; i <= 5; i++){
        led =! led;
        ThisThread::sleep_for(200ms);
    }
    led = 0;
}

// Functie om de flags te zetten. Dit gebeurt tegelijk omdat deze via de ticker telkens om 'middernacht' = om de 5min geset wordt.
void flagSetter(){
    while(true) {
        paymentMidnight_t.flags_set(0x1);
        payDay_t.flags_set(0x2);
    }
}

// Functie om de randomTransaction aan te passen naar nieuwe random waarden. 
// Hierbij wordt gebruik gemaakt van een random getal binnen een range van de lengte van de vector voor de terminals en vector van kopers en een random getal tussen 1 en 5 voor het bedrag
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

/*
    Functie die de transacties beheert (nog geen betalingen).
    Via een semaphore zorgen we ervoor dat er telkens maar 2 verbingen tegelijk aan de server kunnen.
    Eerst gaat de terminal de transactie gaan opstarten. Dit wil zeggen dat er gekeken wordt als de gebruiker het bedrag kan betalen (details in de de terminal klasse).
    Dit gebeurt adhv een reeks aanroepingen van functies van de terminal naar de bancontact server en naar de bank.
    Als die goedgekeurd wordt worden de nodige gegevens uit de transactie struct gehaald en opgeslagen in de nodige objecten.
    Vervolgens wordt gecontroleerd of de bank van de koper en verkoper hetzelfde zijn of niet
    Zo ja, dan wordt de transactie op de queue van de directe betalingen geplaatst.
    Zo neen, dan wordt de transactie in een vector geplaatst die alle middernacht betalingen bijhoudt. 
*/
void manageTransactions(transaction_struct * receivedTransaction){

    while(true){  
        server_control.acquire();
        ThisThread::sleep_for(1s);
        if (receivedTransaction->terminal->sendTransaction(receivedTransaction->buyer, receivedTransaction->amount)){
            Bancontact * bancontactServer = receivedTransaction->terminal->getBancontact();
            Bank * buyerBank = bancontactServer->getUserBank(receivedTransaction->buyer);
            Payment payment(receivedTransaction->buyer, receivedTransaction->terminal->getSeller(), receivedTransaction->amount);
            if (buyerBank->checkPaymentTime(&payment)){
                sameBank_m.try_put_for(Kernel::wait_for_u32_forever, &payment);
            } 
            else {
                mutex.lock();
                midnightTransactionsList.push_back(&payment);
                mutex.unlock();
            }
        } 
        server_control.release();       
    }
}

/*
    Functie om directe betalingen uit te voeren.
    De betaling wordt eerst van de queue gehaald.
    Dan wordt bepaald welke bank de betaling hoort uit te voeren.
    Daarna wordt de betaling uitgevoerd. Verdere details over hoe de betaling uitgevoerd wordt zijn te vinden in de Bank klasse.
    Dit alles gebeurt tijdens de dag, m.a.w. in de eerst 4 minuten.
*/

void managePaymentNow(void){
    while(true){
        if(time_clock < 240){
            ThisThread::sleep_for(1s);
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

/*
    Functie om de middernacht betalingen in gang te zetten. Hier wordt nog niet betaald.
    Om middernacht gaat de ticker de flag setten en zullen de transacties uit de vector allemaal op de queue gezet worden voor wanneer de banken verschillen van elkaar.
    Die vector wordt dan ook terug leeggemaakt.

*/

void managePaymentMidnight(void){
    while(true){
        ThisThread::flags_wait_any(0x1);
        mutex.lock();
        for(int i = 0; i < midnightTransactionsList.size() ; i++){
            otherBank_m.try_put_for(Kernel::wait_for_u32_forever, midnightTransactionsList[i]);
            midnightTransactionsList[i] = nullptr;
        }
        mutex.unlock();
    }
}

/*
    Functie om de transacties uit te voeren die op de kalme momenten dienen uitgevoerd te worden = 's nachts.
    Tussen de 4e en 5e minuut worden deze transacties uitgevoerd.
    Eerst wordt de transactie van de bank gehaald, dan wordt de bank van de koper uitgelezen en deze voert dan de betaling uit.
    Het ledje zal knipperen om aan te geven dat er een middernachttransactie aan de gang is.
    Wanneer de klok aan 5 minuten komt wordt deze terug op 0 geplaatst.
*/

void executeMidnightPayments(void){
    while(true) {
        if(time_clock > 240 && time_clock < 300) {
            Payment * payment;
            otherBank_m.try_get_for(Kernel::wait_for_u32_forever, &payment);
            std::string bankID = payment->getBuyer()->getUserID().substr(0,payment->getBuyer()->getUserID().size()-1);
            Bank * bank = nullptr;
            if (bankID == "BEKBC"){
                bank = &KBC;
            } else if(bankID == "BEBEL") {
                bank = &Belfius;
            }
            ThisThread::sleep_for(1s);
            bank->pay((payment));
            toggleLED();
        } else {
            time_clock = 0;
        }
    }
}

/*
    Functie om de studenten loon te betalen zodat er transacties kunnen blijven gebeuren nadat de ze al hun geld uitgegeven hebben.
    Wanneer de ticker om middernacht de vlag hoog zet treed de betaling in werking.
    Nu krijgen ze het gespecifeerd bedrag als loon op hun rekening
*/

void payStudents(int * salary) {
    while(true) {
        ThisThread::flags_wait_any(0x2);
        student1.updateBalance(*salary);
        student2.updateBalance(*salary);
        student3.updateBalance(*salary);
        ThisThread::sleep_for(1s);
        std::cout << "Students have received their paycheck" << std::endl;
    }
}

/* 
    Simulatie voor de tijd. Elke seconde gaat de klok met 1 omhoog. 
    In de functie van de uitvoeringen van de middernacht betalingen wordt deze gereset wanneer de klok boven de 5min. gaat.
*/

void updateTime(){
    while(true) {
        time_clock++;
        ThisThread::sleep_for(1s);
    }
}

int main(){

    // De ticker wordt geactiveerd met een waarde van 5min. = 300s
    ticker.attach(&flagSetter,300);

    // Alle gebruikers worden aan een bank toegekend. Deze zijn random verdeeld over de banken.
    KBC.addUser(&breadPit);
    KBC.addUser(&starbucks);
    KBC.addUser(&student1);
    KBC.addUser(&student3);
    Belfius.addUser(&restaurant);
    Belfius.addUser(&onthaal);
    Belfius.addUser(&student2);

    // Elke bancontact server maakt verbinding met elke bank.
    b1.connectToBank(&KBC);
    b2.connectToBank(&KBC);
    b3.connectToBank(&KBC);
    b1.connectToBank(&Belfius);
    b2.connectToBank(&Belfius);
    b3.connectToBank(&Belfius);

    // Elke terminal wordt aan de vector terminalList toegevoegd. Hetzelfde voor de kopers en verkopers.
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

    // De functies van de threads worden opgestart.
    timeClock_t.start(callback(updateTime));
    updateTransaction_t.start(callback(setRandomTransaction));
    terminal_t.start(callback(manageTransactions, &randomTransaction));
    paymentNow_t.start(callback(managePaymentNow));
    paymentMidnight_t.start(callback(managePaymentMidnight));
    payDay_t.start(callback(payStudents, &salary));
    executeMidnightPayments_t.start(callback(executeMidnightPayments));
}