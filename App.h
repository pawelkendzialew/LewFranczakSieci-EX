#ifndef APP_H
#define APP_H

#include <fstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QCoreApplication>
#include "BuforDanych.h"
#include "Sygnal.h"
#include "PID.h"

#include <qDebug>
#include "ARX.h"
using namespace std;
class ARX;

class App {
private:
    bool run = false; // start stop

    int k=1;
    int interwal=100;

public:
    ARX* arx;
    double oldY = 0;
    double getoldY();
    void setoldY(double y);

    void setk(int i);
    int getk();

    void setinterwal(int i);
    int getinterwal();
    Sygnal* syg;
    PID* Pid;
    std::vector<BuforDanych*> data;

    void symulacjaStep();
    void symulacja(bool start); //g³ówna fukcja programu
    void wypiszDane();
    void setARX(ARX* A) { this->arx = A; }
    void setSyg(Sygnal* S) { this->syg = S; }
    void setPID(PID* p) { this->Pid = p; }
    void seta(std::string i);

    std::string a;
    std::string b;
    double u;
    double p;
    double t;
    double i;
    double amplituda=1;
    int sygnal=0;

    void startSymulacja();  // rozpocznij symulacj?
    void stopSymulacja();   // zatrzymaj symulacj?
    void zapisBazydanychdopliku();
    App();
    ~App();
    void odczytajDaneZPliku() {
        const std::string nazwaPliku = "example.txt"; // Plik na sztywno
        std::ifstream file(nazwaPliku);
        if (!file.is_open()) {
            std::cerr << "Nie mo¿na otworzyæ pliku: " << nazwaPliku << std::endl;
            return;
        }

        std::string linia;
        string a;
        string b;

        while (std::getline(file, linia)) {
            std::istringstream iss(linia);//obiekt strumienia wejscia

            int id;
            double zaklucenie;
            std::string str1, str2;
            double val, val1, val2,val3, val4,val5,val6, val7,val8,val9,val13;
            int val10,val11,val12;

           // int, double, string, string, double, double
            if (iss >> id >> zaklucenie >> str1 >> str2 >>val>> val1 >>  val2>>val3 >>  val4 >> val5 >>val6>>val7>>val8>>val9>>val10>>val11>>val12>>val13) {

                BuforDanych* bufor = new BuforDanych();
                bufor->setID(id);
                bufor->setZaklucenie(zaklucenie);
                bufor->setB(str1);

                bufor->setA(str2);
                bufor->setW(val);
                bufor->setU(val2);
                bufor->setY(val1);
                Pid->ustawWzmocnienieCalkujace(val3);
                Pid->ustawWzmocnienieProporcjonalne(val4);
                Pid->ustawWzmocnienieRowniczkujace(val5);
                bufor->setUp(val6);
                bufor->setUi(val7);
                bufor->setUd(val8);
                bufor->setUchyb(val9);
                setk(val10);
                setinterwal(val11);
                a=str2;
                b=str1;
                data.push_back(bufor);
                sygnal=val12;
                Pid->suma=val13;
            }
            else {
             std::cerr << "B³¹d odczytu wiersza danych: " << linia << std::endl;
            }
             Pid->poprzedniUchyb=data[data.size()-1]->getUchyb();
        }

        file.close();
    }
    void zapisconf();
    void odczytajConf() {
    const std::string nazwaPliku = "conf.txt"; // Plik na sztywno
    std::ifstream file(nazwaPliku);
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << nazwaPliku << std::endl;
        return;
    }

    std::string linia;
    string a;
    string b;

    while (std::getline(file, linia)) {
        std::istringstream iss(linia);//obiekt strumienia wejscia


        std::string str1, str2;
        double val16,val6, val7,val8,val9,val13, val10,val11,val12;
        //int val10,val11,val12;

        // int, double, string, string, double, double
        if (iss>>str1 >> str2 >> val6>>val7>>val8>>val9>>val13>>val10>>val16>>val11>>val12) {

            b=str2;
            a=str1;
            Pid->ustawWzmocnienieCalkujace(val6);
            Pid->ustawWzmocnienieProporcjonalne(val7);
            Pid->ustawWzmocnienieRowniczkujace(val8);
            syg->setA(val9);
            syg->sett(val13);
            syg->setp(val16);
            setk(val10);
            setinterwal(val12);
             sygnal=val11;

        }
        else {
            std::cerr << "B³¹d odczytu wiersza danych: " << linia << std::endl;
        }
    }

    file.close();
}
};


#endif
