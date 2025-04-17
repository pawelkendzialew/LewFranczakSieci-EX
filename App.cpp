#include "App.h"
#include <iostream>
#include <QDebug>
using namespace std;

App::App()
    : run(false),
    data(0),
    arx(nullptr) {
    a="-0.4";
    b="0.6";
    sygnal=0;
};

App::~App() {
 
}



void App::startSymulacja() {
    run = true;
}

void App::stopSymulacja() {
    run = false;
}
void App::symulacjaStep() {
    if (!run) {
        return;
    }
    BuforDanych* nowaBaza = new BuforDanych();
    //syg->setA(amplituda);
    nowaBaza->setB(b);
    //cerr<<syg->axx();
    nowaBaza->setA(a);
    nowaBaza->setZaklucenie(arx->generateDisturbance());
    nowaBaza->k = getk();
    nowaBaza->setY(getoldY());
    syg->seti(nowaBaza->getID());
    nowaBaza->setW(syg->generateSignal(sygnal)); 
    nowaBaza->setU(Pid->obliczSprzezenie(nowaBaza->getW(), oldY));
    nowaBaza->setUp(Pid->Up);
    nowaBaza->setUi(Pid->Ui);
    nowaBaza->setUd(Pid->Ud);
    nowaBaza->setUchyb(Pid->uchyb);
   // qDebug()<<Pid->uchyb;
    data.push_back(nowaBaza);
    if (arx) {
        double wynikCalcAll = arx->calcAll(data);
        data.back()->setY(wynikCalcAll);

    }
    oldY=data.back()->getY();
        // Debug: Informacje o dodanych danych
  //  qDebug() << "Dodano punkt do symulacji: ID =" << data.back()->getI()
   //          << ", Y =" << data.back()->getY() << ", U =" << data.back()->getU();
}


void App::symulacja(bool start) {
    run = start; // Ustawia flagê run
    if (start) {
        std::cout << "Symulacja rozpoczêta.\n";
    } else {
        std::cout << "Symulacja zatrzymana.\n";
    }
}



void App::wypiszDane() {

    std::cout << "Rozmiar danych: " << data.size() << "\n";
    std::cout << "ID\tZaklucenie\tB\tA\tY\tU\n";
    std::cout << "------------------------\n";

    for (const auto& elem : data) {
        if (elem) {
            elem->wypisztabele();
        }
        else {
            std::cout << "Blad: pusty wskaznik w danych.\n";
        }
    }
}

void App::zapisBazydanychdopliku() {


    ofstream file("example.txt");
    if (file.is_open()) {
        for (const auto& elem : data) {
          
            if (elem) {
                file << elem->getID() << "\t" << elem->getZaklucenie() << "\t";
               
               for (const auto data : elem->getvectorB()) {
                   file << data << ";";
                }
                file << "\t";
               for (const auto data : elem->getvectorA()) {
                  file << data << ";";
               }
               file << "\t"<<elem->getW()<<"\t";
                file << elem->getY() << "\t" << elem->getU() << "\t";
                file<< Pid->getWzmocnienieCalkujace()<<"\t"<<Pid->getWzmocnienieProporcjonalne()<<"\t"<<Pid->getWzmocnienieRowniczkujace()<<"\t";
                file<<elem->getUp()<<"\t" <<elem->getUi() << "\t"<< elem->getUd()<< "\t" ;
                file<<elem->getUchyb()<<"\t"<<getk()<<"\t"<<getinterwal()<<"\t";
                file<<sygnal<<"\t"<<Pid->suma<<"\n";

            }
            else {
                std::cout << "Blad: pusty wskaznik w danych.\n";
            }
        }
    }
    file.close();
}




void App::zapisconf() {


    ofstream file("conf.txt");
    if (file.is_open()) {



            file << a ;

        file << "\t";

            file << b ;
        file<<"\t"<< Pid->getWzmocnienieCalkujace()<<"\t"<<Pid->getWzmocnienieProporcjonalne()<<"\t"<<Pid->getWzmocnienieRowniczkujace()<<"\t";
        file<<syg->axx()<<"\t"<<syg->getT()<<"\t"<<syg->getP()<<"\t";
        file<<getk()<<"\t"<<sygnal<<"\t"<<getinterwal()<<"\n";

        }


    file.close();
}







//seter
void App::setoldY(double y){
    oldY=y;
}
void App::setk(int i){
    if(i>0){
        k=i;
    }
    else{
        k=1;
    }
}
void App::setinterwal(int i){
    interwal=i;
}
//geter
double App::getoldY(){
    return oldY;
}
int App::getk(){
    return k;
}
int App::getinterwal(){

    return interwal;
}

