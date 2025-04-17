#ifndef PID_H
#define PID_H

class PID {
private:
    double wzmocnienieProporcjonalne; // Wzmocnienie proporcjonalne (Kp)
    double wzmocnienieCalkujace;      // Wzmocnienie calkujace (Ki)
    double wzmocnienieRowniczkujace;  // Wzmocnienie rozniczkujace (Kd)

    double odchylenieLiniowe;
    bool resetujOdchylenie;
    double poprzednia = 0;

public:
    int trybpracy=0;
    double poprzedniUchyb;
    double suma = 0;
    double Up;
    double Ui;
    double Ud;
    double uchyb=0;
    PID(double kp, double ki, double kd);
    void resetPamieci();

    void ustawWzmocnienieProporcjonalne(double wartosc);
    void ustawWzmocnienieCalkujace(double wartosc);
    void ustawWzmocnienieRowniczkujace(double wartosc);

    double oblicz(double uchyb);
    double obliczSprzezenie(double wartoscZadana, double wartoscMierzona);


    double sumator(double wartoscZadana, double wartoscMierzona);
    double getWzmocnienieProporcjonalne() const {
        return wzmocnienieProporcjonalne;
    }

    double getWzmocnienieCalkujace() const {
        return wzmocnienieCalkujace;
    }

    double getWzmocnienieRowniczkujace() const {
        return wzmocnienieRowniczkujace;
    }
};
#endif
