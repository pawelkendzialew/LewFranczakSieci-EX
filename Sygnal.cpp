#include "Sygnal.h"
#include <math.h>
#include <QDebug>
Sygnal::Sygnal() {
	//konstruktor
}
double Sygnal::unit(double A) {
	
	return A;
}
double  Sygnal::sinusoidal(double A,int i, int t ) {
    double wynik = static_cast<double>(A) * sin(((i % t) /static_cast<double>(t) ) * 2.0 * 3.14);

	return wynik;
}
double Sygnal::rectangular(double A, double p, int i, int t) {

    if (i%t < static_cast<double>(p*t) ) {
		return A;
	}
    return -A;
}
double  Sygnal::generateSignal(int liczba) {
    double g;
    if (liczba == 0) {
        g=unit(A);

	}
    else if (liczba==1) {
        g=sinusoidal(A, i, t);
	}
	else {
        g= rectangular(A, p, i, t);
	}

    //qDebug()<<g;
    return g;
}
