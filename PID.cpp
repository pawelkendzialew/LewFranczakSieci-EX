#include "PID.h"
#include <QDebug>
PID::PID(double kp, double ki, double kd)
    : wzmocnienieProporcjonalne(kp), wzmocnienieCalkujace(ki), wzmocnienieRowniczkujace(kd),
    odchylenieLiniowe(0),  resetujOdchylenie(false),poprzedniUchyb(0) {
}
void PID::resetPamieci() {
    odchylenieLiniowe = 0.0;
    poprzedniUchyb = 0;
    suma=0;
}

void PID::ustawWzmocnienieProporcjonalne(double wartosc) {
    wzmocnienieProporcjonalne = wartosc;
}

void PID::ustawWzmocnienieCalkujace(double wartosc) {
    wzmocnienieCalkujace = wartosc;
}

void PID::ustawWzmocnienieRowniczkujace(double wartosc) {
    wzmocnienieRowniczkujace = wartosc;
}
double PID::oblicz(double uchyb) {
 double calkujaca = 0;

    if(trybpracy == 0) {
        // Tryb "stała przed sumą":
        if (wzmocnienieCalkujace != 0) {
            suma += uchyb;
            calkujaca = (1 / wzmocnienieCalkujace) * suma;

        }
        Ui=calkujaca;
    }
    else if(trybpracy == 1) {
        // Tryb "stała pod sumą":

        suma += uchyb/wzmocnienieCalkujace;
        //calkujaca = (wzmocnienieCalkujace * suma);
        Ui=calkujaca=suma;
    }
    double proporcjonalna = wzmocnienieProporcjonalne * uchyb;
    Up=proporcjonalna;

    /*if (wzmocnienieCalkujace != 0) {
        suma += uchyb;
        calkujaca = (1 / wzmocnienieCalkujace) * suma;

    }
    Ui=calkujaca;*/

    double rozniczkujaca = wzmocnienieRowniczkujace * (uchyb - poprzedniUchyb);

    poprzedniUchyb = uchyb;
    Ud=rozniczkujaca;
   // qDebug()<<proporcjonalna << calkujaca << rozniczkujaca;
    return proporcjonalna + calkujaca + rozniczkujaca;

}

double PID::sumator(double wartoscZadana, double wartoscMierzona) {
    return wartoscZadana - wartoscMierzona;
}

double PID::obliczSprzezenie(double wartoscZadana, double wartoscMierzona) {

    uchyb = sumator(wartoscZadana, wartoscMierzona);
   // qDebug()<<uchyb;
    double sygnalSterujacy = this->oblicz(uchyb);

    return sygnalSterujacy;
}
