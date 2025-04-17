#ifndef ARX_H
#define ARX_H
#include <random>
#include "App.h"
#include "BuforDanych.h"
using namespace std;
class ARX {
public:
    double generateDisturbance();
    int test=0;//usuwa zaklacia do testow
    double calaA(std::vector<BuforDanych*> data);
    double calcb(std::vector<BuforDanych*> data);
    double calcAll(std::vector<BuforDanych*> data);
    double getzasiegZak();
    void setzasiegZak(double x);
private:
    double zasiegZak=0.01;

};

#endif
