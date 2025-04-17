#ifndef BUFORDANYCH_H
#define BUFORDANYCH_H
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

class BuforDanych {
private:
    double zaklucenie;
    int ID;

    vector<double> b;
    vector<double>a;
    double ster;
    double y=0;
    int i;
    double u=0;
    double w=0;
    double Up;
    double Ui;
    double Ud;
    double Uchyb;
    int interwal;
public:

    void setUp(double im);
    double getUp();
    void setUchyb(double im);
    double getUchyb();
    void setUi(double im);
    double getUi();
    void setUd(double im);
    double getUd();
    int k=1;
    static int licznikInstancji; //licznik ile jest danych
    BuforDanych();//konstruktor jak tworzy sie pierwszy to nullptr
    //setery
    void setZaklucenie(double data);
    void setID(int id);
    void setB(const std::string& data);
    void setA(const std::string& data);

    void setU(double data) {
        u = data;
    }
    void setY(double data) {
        y = data;
    }
    void setI(int im){
        i=im;
    }
    void setW(double im){
        w=im;
    }
    //Getter
    double getU() {
        return u;
    }
    double getW() {
        return w;
    }
    double getZaklucenie() const;
    int getID() const;
    int getI()const;
    double getA(int e);
    double getb(int e);
    double getY();
    vector<double> getvectorB() {
        return b;
    }
    vector<double> getvectorA() {
        return a;
    }
    void wypisztabele(); //wypisuje na ekran pojedyncza tablice
    
};
#endif
