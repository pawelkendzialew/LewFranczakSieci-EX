#include "BuforDanych.h"
#include <cstring>
#include <string>
using namespace std;
int BuforDanych::licznikInstancji = -1;

BuforDanych::BuforDanych() : ID(++licznikInstancji), zaklucenie(0) {
    i=ID;
}



void BuforDanych::wypisztabele() {

    std::cout << this->getID() << " | " << this->getZaklucenie();

    //formacie liczba;liczba;...
    std::cout << " ";
    for (size_t i = 0; i < b.size(); ++i) {
        std::cout << b[i];
        if (i < b.size() - 1) {
            std::cout << ";"; 
        }
    }
    std::cout << "|";
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i];
        if (i < b.size() - 1) {
            std::cout << ";";
        }
    }
    std::cout << " | " << this->getY() << " | " << this->getU();
    std::cout << "\n";
}
// Setery
void BuforDanych::setZaklucenie(double data) {
    zaklucenie = data;
}

void BuforDanych::setID(int id) {
    ID = id;
}
void BuforDanych::setB(const string& data1) {
    if (data1.empty()) { 
        return;
    }

    string data(data1);

    size_t pos = 0;
    while ((pos = data.find(";")) != string::npos) { 
        string token = data.substr(0, pos); 
        try {
            b.push_back(std::stod(token)); 
        }
        catch (const std::invalid_argument&) {
            cerr << "B³¹d " << token << std::endl;
        }
        data.erase(0, pos + 1); 
    }

   
    try {
        if (!data.empty()) {
            b.push_back(std::stod(data));
        }
    }
    catch (const std::invalid_argument&) {
      cerr << "Blad " << data << std::endl;
    }
}
void BuforDanych::setA(const string& data1) {
    if (data1.empty()) {
        return;
    }

    string data(data1);

    size_t pos = 0;
    while ((pos = data.find(";")) != string::npos) {

        string token = data.substr(0, pos);

        try {
            a.push_back(std::stod(token));
        }
        catch (const std::invalid_argument&) {
            cerr << "Blad " << token << std::endl;
        }
        data.erase(0, pos + 1);
    }


    try {
        if (!data.empty()) {

            a.push_back(std::stod(data));
        }
    }
    catch (const std::invalid_argument&) {
        cerr << "Blad " << data << std::endl;
    }
}


// Getery
double BuforDanych::getZaklucenie() const {
    return zaklucenie;
}

int BuforDanych::getID() const {
    return ID;
}
int BuforDanych::getI()const {
    return i;
}
double BuforDanych::getA(int i) {
    if (i < 0 || i >= a.size()) {

        return 0;
    }
    return a[i];
}

double BuforDanych::getb(int i) {
    if (i < 0 || i >= b.size()) {

        return 0;
    }
    return b[i];
}

double BuforDanych::getY() {
    return y;
}
double BuforDanych::getUd(){
    return Ud;
}
double BuforDanych::getUi(){
    return Ui;
}
double BuforDanych::getUp(){
    return Up;
}
void BuforDanych::setUd(double im){
    Ud=im;
}
void BuforDanych::setUp(double im){
    Up=im;
}
void BuforDanych::setUi(double im){
    Ui=im;
}
double BuforDanych::getUchyb(){
    return Uchyb;
}
void BuforDanych::setUchyb(double im){
    Uchyb=im;
}






