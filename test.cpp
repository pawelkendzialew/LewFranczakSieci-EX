
#include "BuforDanych.h"
#include "Sygnal.h"
#include "App.h"
#include "PID.h"
#include "ARX.h"
#include "test.h"
#include <iostream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace std;

void raportBleduSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt) {
    constexpr size_t PREC = 3;
    std::cerr << std::fixed << std::setprecision(PREC);
    std::cerr << "  Spodziewany:\t";
    for (auto& el : spodz)
        std::cerr << el << ", ";
    std::cerr << "\n  Faktyczny:\t";
    for (auto& el : fakt)
        std::cerr << el << ", ";
    std::cerr << std::endl << std::endl;
}

bool porownanieSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt) {
    constexpr double TOL = 1e-3;
    if (fakt.size() != spodz.size()) return false;
    for (size_t i = 0; i < fakt.size(); i++) {
        if (std::fabs(fakt[i] - spodz[i]) >= TOL) {
            return false;
        }
    }
    return true;
}

void Testy::test_RegulatorPID_skokJednostkowy()
{
    //Sygnatura testu:
    std::cerr << "RegPID (k = 0.5, TI = 10.0, TD = 0.2) -> test skoku jednostkowego: ";

    try
    {
        // Przygotowanie danych:
        PID instancjaTestowa(0.5, 10.0, 0.2);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
        {
            sygWe[i] = !!i;

        }
        spodzSygWy = { 0, 0.8, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

        // Symulacja modelu:
        for (size_t  i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.oblicz(sygWe[i]);

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_RegulatorP_skokJednostkowy()
{
    //Sygnatura testu:
    std::cerr << "RegP (k = 0.5) -> test skoku jednostkowego: ";

    try
    {
        // Przygotowanie danych:
        PID instancjaTestowa(0.5, 0, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };

        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.oblicz(sygWe[i]);

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_RegulatorP_brakPobudzenia()
{
    //Sygnatura testu:
    std::cerr << "RegP (k = 0.5) -> test zerowego pobudzenia: ";
    try
    {
        // Przygotowanie danych:
        PID instancjaTestowa(0.5, 0, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja modelu:

        for (size_t  i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.oblicz(sygWe[i]);

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}





void Testy::test_ModelARX_brakPobudzenia()
{
    //Sygnatura testu:
    std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia: ";
    try
    {
        // Przygotowanie danych:
        App app;
        ARX ARX;
        //app.arx = &ARX;
        app.setARX(&ARX);

        //BuforDanych data({ -0.4 }, { 0.6 }, 0);
        //app.data.push_back(&data);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja modelu:

        for (size_t  i = 0; i < LICZ_ITER; i++) {
            BuforDanych* nowaBaza = new BuforDanych();
            nowaBaza->setB("0.6");
            nowaBaza->setA("-0.4");
            //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i]);
            nowaBaza->setU(sygWe[i]);
            app.data.push_back(nowaBaza);
            //app.data[app.data.size() - 1]->y = faktSygWy[i-1];

            faktSygWy[i] = ARX.calcAll(app.data);

        }

        //faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);


        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}

void Testy::test_ModelARX_skokJednostkowy_1()
{
    //Sygnatura testu:
    std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1: ";

    try
    {
        App app;
        ARX ARX;
        //app.arx = &ARX;
        app.setARX(&ARX);
        constexpr size_t LICZ_ITER = 30;

        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        // Symulacja modelu:
        for (size_t  i = 0; i < LICZ_ITER; i++) {

            if (i >= 1) {
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i], faktSygWy[i - 1]);


                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = app.data[i - 1]->k;
                nowaBaza->setB("0.6");
                nowaBaza->setA("-0.4");
                nowaBaza->setZaklucenie(0);
                nowaBaza->setU(sygWe[i]);
                //cout << "\n" << app.data.back()->getY()<< "sssffaddddd\n";
                //nowaBaza->setY(faktSygWy[i - 1]);
                nowaBaza->setY(app.getoldY());
                app.data.push_back(nowaBaza);


                faktSygWy[i] = ARX.calcAll(app.data);
                app.data.back()->setY(faktSygWy[i]);
                app.setoldY(faktSygWy[i]);

                /*cout << "\n" << ARX.calcAll(app.data) << "saddddd\n";
                cout << "\n" << faktSygWy[i] << "saddddd\n";
                cout << "\n" << faktSygWy[i-1] << "saddddd\n";
                cout << "\n" << app.data[i - 1]->getY() << "saddddd\n";*/





            }
            else {
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i]);

                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = 1;
                nowaBaza->setB("0.6");
                nowaBaza->setA("-0.4");
                nowaBaza->setZaklucenie(0);
                nowaBaza->setY(app.getoldY());
                nowaBaza->setU(sygWe[i]);
                app.data.push_back(nowaBaza);


                faktSygWy[i] = ARX.calcAll(app.data);
                // cout << "\n" << app.arxsym() << "asda\n";

                app.data.back()->setY(faktSygWy[i]);
                app.setoldY(faktSygWy[i]);
            }

            //app.data.back()->y = faktSygWy[i];
            // cout << "KONIEC\n";
        }
        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_ModelARX_skokJednostkowy_2()
{
    //Sygnatura testu:
    std::cerr << "ModelARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2: ";

    try
    {
        // Przygotowanie danych:
        App app;
        ARX ARX;
        //app.arx = &ARX;
        app.setARX(&ARX);
        //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i], faktSygWy[i - 1]);
        //app.arx = &ARX;

        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu,
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        // Symulacja modelu:


        for (size_t  i = 0; i < LICZ_ITER; i++) {

            if (i >= 1) {
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i], faktSygWy[i - 1]);


                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = app.data[i - 1]->k;
                nowaBaza->setB("0.6");
                nowaBaza->setA("-0.4");
                nowaBaza->setZaklucenie(0);
                nowaBaza->setU(sygWe[i]);
                nowaBaza->setY(app.getoldY());
                //cout << "\n" << app.data.back()->getY()<< "sssffaddddd\n";
                //nowaBaza->setY(faktSygWy[i - 1]);

                app.data.push_back(nowaBaza);


                faktSygWy[i] = ARX.calcAll(app.data);
                app.data.back()->setY(faktSygWy[i]);
                app.setoldY(faktSygWy[i]);
                /*cout << "\n" << ARX.calcAll(app.data) << "saddddd\n";
                    cout << "\n" << faktSygWy[i] << "saddddd\n";
                    cout << "\n" << faktSygWy[i-1] << "saddddd\n";
                    cout << "\n" << app.data[i - 1]->getY() << "saddddd\n";*/





            }
            else {
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i]);

                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = 2;
                nowaBaza->setB("0.6");
                nowaBaza->setA("-0.4");
                nowaBaza->setZaklucenie(0);

                nowaBaza->setU(sygWe[i]);
                app.data.push_back(nowaBaza);
                nowaBaza->setY(app.getoldY());

                faktSygWy[i] = ARX.calcAll(app.data);
                // cout << "\n" << app.arxsym() << "asda\n";
                app.setoldY(faktSygWy[i]);
                app.data.back()->setY(faktSygWy[i]);
            }

            //app.data.back()->y = faktSygWy[i];
            // cout << "KONIEC\n";
        }



        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}void Testy::test_ModelARX_skokJednostkowy_3()
{
    //Sygnatura testu:
    std::cerr << "ModelARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3: ";
    try
    {
        // Przygotowanie danych:
        //ModelARX instancjaTestowa({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
        App app;
        ARX ARX;
        //app.arx = &ARX;
        app.setARX(&ARX);
        constexpr size_t LICZ_ITER = 30;

        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu,
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

        // Symulacja modelu:
        for (size_t  i = 0; i < LICZ_ITER; i++) {

            if (i >= 1) {
                // app.wypiszDane();
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i], faktSygWy[i - 1]);
                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = app.data[i - 1]->k;
                nowaBaza->setB("0.6;0.3");
                nowaBaza->setA("-0.4;0.2");
                nowaBaza->setZaklucenie(0);
                nowaBaza->setU(sygWe[i]);
                //cout << "\n" << app.data.back()->getY()<< "sssffaddddd\n";
                //nowaBaza->setY(faktSygWy[i - 1]);
                nowaBaza->setY(app.oldY);
                app.data.push_back(nowaBaza);
                //cout << nowaBaza->getA(0) << nowaBaza->getA(1) << "XDXDXD\n";

                faktSygWy[i] = ARX.calcAll(app.data);
                app.data.back()->setY(faktSygWy[i]);
                app.oldY = faktSygWy[i];
                /*cout << "\n" << ARX.calcAll(app.data) << "saddddd\n";
                cout << "\n" << faktSygWy[i] << "saddddd\n";
                cout << "\n" << faktSygWy[i-1] << "saddddd\n";
                cout << "\n" << app.data[i - 1]->getY() << "saddddd\n";*/
                //app.wypiszDane();
            }
            else {
                //BuforDanych* newdata = new BuforDanych({ -0.4 }, { 0.6 }, sygWe[i]);
                // app.wypiszDane();
                BuforDanych* nowaBaza = new BuforDanych();
                nowaBaza->k = 2;
                nowaBaza->setB("0.6;0.3");
                nowaBaza->setA("-0.4;0.2");
                nowaBaza->setZaklucenie(0);
                nowaBaza->setY(app.oldY);
                nowaBaza->setU(sygWe[i]);
                app.data.push_back(nowaBaza);
                // cout << nowaBaza->getA(0) << nowaBaza->getA(1)<<"XDXDXD\n";

                faktSygWy[i] =  app.arx->calcAll(app.data);
                // cout << "\n" << app.arxsym() << "asda\n";
                app.oldY = faktSygWy[i];
                app.data.back()->setY(faktSygWy[i]);
                // app.wypiszDane();
            }

            //app.data.back()->y = faktSygWy[i];
            // cout << "KONIEC\n";
        }


        // Weryfikacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (out_of_range )
    {
        std::cerr << "INTERsadsadUPTED! (niespodziwany wyjatek)\n";
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}

//
//
void Testy::test_UAR_1_brakPobudzenia()
{
    //Sygnatura testu:
    std::cerr << "UAR_1 -> test zerowego pobudzenia: ";
    try
    {
        // Przygotowanie danych:
        PID testPID(0.5, 5.0, 0.2);
        //odelARX testARX({ -0.4 }, { 0.6 });
        App app;
        ARX ARX;
        Sygnal syg;
        app.setSyg(&syg);
        //app.arx = &ARX;
        app.setARX(&ARX);
        app.setPID(&testPID);
        //ProstyUAR instancjaTestowa(testARX, testPID);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja UAR:

        for (size_t  i = 0; i < LICZ_ITER; i++)
        //faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);
        {
            app.amplituda = sygWe[i];
            app.a = "-0.4";
            app.b = "0.6";
            ARX.test=1;
            app.symulacjaStep();
            faktSygWy[i] = app.getoldY();
        }
        // Walidacja poprawności i raport:
        // myAssert(spodzSygWy, faktSygWy);
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}


void Testy::test_UAR_1_skokJednostkowy()
{
    //Sygnatura testu:
    std::cerr << "UAR_1 -> test skoku jednostkowego: ";
    try
    {
        PID testPID(0.5, 5.0, 0.2);
        //odelARX testARX({ -0.4 }, { 0.6 });
        App app;
        ARX ARX;
        Sygnal syg;
        app.setSyg(&syg);
        //app.arx = &ARX;
        app.setARX(&ARX);
        app.setPID(&testPID);
        app.startSymulacja();
        //RegulatorPID testPID(0.5, 5.0, 0.2);
        //ModelARX testARX({ -0.4 }, { 0.6 });
        //ProstyUAR instancjaTestowa(testARX, testPID);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0.0, 0.0, 0.54, 0.756, 0.6708, 0.64008, 0.729, 0.810437, 0.834499,
            0.843338, 0.8664, 0.8936, 0.911886, 0.923312, 0.93404, 0.944929,
            0.954065, 0.961042, 0.966815, 0.971965, 0.97642, 0.980096, 0.983143,
            0.985741, 0.987964, 0.989839, 0.991411, 0.992739, 0.993865, 0.994818
        };
        // Symulacja UAR:
        //app.clikrun();

        for (size_t  i = 0; i < LICZ_ITER; i++) {

            app.a = "-0.4";
            app.b = "0.6";
           // app.amplituda = sygWe[i];
            app.syg->setA(sygWe[i]);
            app.setk(1);
            ARX.test=1;
            app.symulacjaStep();

            faktSygWy[i] = app.oldY;
        }

        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}


void Testy::test_UAR_2_skokJednostkowy()
{
    //Sygnatura testu:
    std::cerr << "UAR_2 (k = 2) -> test skoku jednostkowego: ";
    try
    {
        // Przygotowanie danych:
        /* RegulatorPID testPID(0.5, 5.0, 0.2);
        ModelARX testARX({ -0.4 }, { 0.6 }, 2);
        ProstyUAR instancjaTestowa(testARX, testPID);*/
        PID testPID(0.5, 5.0, 0.2);
        //odelARX testARX({ -0.4 }, { 0.6 });
        App app;
        ARX ARX;
        Sygnal syg;
        app.setSyg(&syg);
        //app.arx = &ARX;
        app.setARX(&ARX);
        app.setPID(&testPID);
        app.startSymulacja();
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = { 0.0, 0.0, 0.0, 0.54, 0.756, 0.9624, 0.87336, 0.841104, 0.771946, 0.821644,
            0.863453, 0.93272, 0.952656, 0.965421, 0.954525, 0.955787, 0.957472,
            0.969711, 0.978075, 0.985968, 0.987821, 0.989149, 0.989053, 0.990645,
            0.992248, 0.994403, 0.995707, 0.996677, 0.997024, 0.997388
        };
        // Symulacja UAR:

        for (size_t  i = 0; i < LICZ_ITER; i++) {

            app.a = "-0.4";
            app.b = "0.6";
            app.syg->setA(sygWe[i]);
            app.setk(2);
            ARX.test=1;
            app.symulacjaStep();

            faktSygWy[i] = app.getoldY();
        }  
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}

void Testy::test_UAR_3_skokJednostkowy()
{
    //Sygnatura testu:
    std::cerr << "UAR_3 (kP=1.0,Ti=2.0) -> test skoku jednostkowego: ";
    try
    {
        // Przygotowanie danych:
        //RegulatorPID testPID(1.0, 2.0, 0.2);
        //ModelARX testARX({ -0.4 }, { 0.6 }, 1);
        //ProstyUAR instancjaTestowa(testARX, testPID);
        PID testPID(1, 2, 0.2);
        //odelARX testARX({ -0.4 }, { 0.6 });
        App app;
        ARX ARX;
        Sygnal syg;
        app.setSyg(&syg);
        //app.arx = &ARX;
        app.setARX(&ARX);
        app.setPID(&testPID);
        //app.clikrun();// Przygotowanie danych:
        app.startSymulacja();
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[i] = !!i;
        spodzSygWy = spodzSygWy = { 0.0, 0.0, 1.02, 1.608, 1.1028, 0.41736, 0.546648, 1.20605, 1.43047,
            0.999176, 0.615056, 0.799121, 1.21304, 1.26025, 0.939289, 0.748507,
            0.927166, 1.17292, 1.14155, 0.921616, 0.843258, 0.990018, 1.12577,
            1.068, 0.927024, 0.908125, 1.01702, 1.08484, 1.02618, 0.941508
        };
        // Symulacja UAR:


        for (size_t  i = 0; i < LICZ_ITER; i++) {

            app.a = "-0.4";
            app.b= "0.6";
            app.syg->setA(sygWe[i]);
            ARX.test=1;
            app.setk(1);
            //cerr<< app.syg->generateSignal(0)<<"\n";
            app.symulacjaStep();

            faktSygWy[i] = app.getoldY();
            //cerr<<app.getoldY()<<"\n";
        }

        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}


//
/*
 *
 *fajnie by było miec test kazdego sygnału i moze jeden z przejsciami
 *moze jakies testy sygnał +PID
 *i całosc test 1 lub 2
 *jak pokombinujes to moze test przycisków jakos to sie da zrobic
 *
*/
void Testy::test_Sygnału_Sin()
{
    //Sygnatura testu:
    std::cerr << "SygSin (Amplituda = 1,okres=10) -> test Sygnaly Sin: ";

    try
    {
        // Przygotowanie danych:
        Sygnal syg;
        syg.setA(2);
        syg.sett(10);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[1] = 2;
        spodzSygWy = { 0.000, 1.175, 1.902, 1.903, 1.178, 0.003, -1.172, -1.901, -1.904, -1.180, 0.000, 1.175, 1.902, 1.903, 1.178, 0.003, -1.172, -1.901, -1.904, -1.180, 0.000, 1.175, 1.902, 1.903, 1.178, 0.003, -1.172, -1.901, -1.904, -1.180};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            syg.seti(i);
            faktSygWy[i]=syg.generateSignal(1);
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_Sygnału_Sin2()
{
    //Sygnatura testu:
    std::cerr << "SygSin (Amplituda = 0,okres=10) -> test Sygnaly Sin: ";

    try
    {
        // Przygotowanie danych:
        Sygnal syg;
        syg.setA(0);
        syg.sett(10);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        for (size_t  i = 0; i < LICZ_ITER; i++)
            sygWe[1] = 2;
         spodzSygWy = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};// Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            syg.seti(i);
            faktSygWy[i]=syg.generateSignal(1);
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_Sygnału_Jednostkowego()
{
    //Sygnatura testu:
    std::cerr << "SygJednostkowy (Amplituda = 1) -> test Sygnaly Jednostkowego: ";

    try
    {
        // Przygotowanie danych:
        Sygnal syg;
        syg.setA(1);
       // syg.sett(10);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;
        spodzSygWy = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1,1,1, 1,1,1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            syg.seti(i);
            faktSygWy[i]=syg.generateSignal(0);
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_Sygnału_Prostokotny()
{
    //Sygnatura testu:
    std::cerr << "SygJednostkowy (Amplituda = 1,Okres =10,Wypelnienie =0.5) -> test Sygnaly Prostokatny: ";

    try
    {
        // Przygotowanie danych:
        Sygnal syg;
        syg.setA(1);
        syg.sett(10);
        syg.setp(0.5);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;
        spodzSygWy = { 1.000, 1.000, 1.000, 1.000, 1.000, -1.000, -1.000, -1.000, -1.000, -1.000, 1.000, 1.000, 1.000, 1.000, 1.000, -1.000, -1.000, -1.000, -1.000, -1.000, 1.000, 1.000, 1.000, 1.000, 1.000, -1.000, -1.000, -1.000, -1.000, -1.000};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            syg.seti(i);
            faktSygWy[i]=syg.generateSignal(2);
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_Sygnału_Prostokotny2()
{
    //Sygnatura testu:
    std::cerr << "SygJednostkowy (Amplituda = 0,Okres =10,Wypelnienie =0.5) -> test Sygnaly Prostokatny: ";

    try
    {
        // Przygotowanie danych:
        Sygnal syg;
        syg.setA(0);
        syg.sett(10);
        syg.setp(0.5);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;

        spodzSygWy = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
       // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            syg.seti(i);
            faktSygWy[i]=syg.generateSignal(2);
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
void Testy::test_UAR_ALL()
{
    //Sygnatura testu:
    std::cerr << "test_UAR_ALL_1 (A = -0.4 B=0.6,PID(1,5,0.2),k=1, sygnal Jednostkowy, amplituda=1) -> test Sygnaly UARALL: ";

    try
    {
        // Przygotowanie danych:
        App* app = new App();
        ARX* arx = new ARX();
        Sygnal* syg = new Sygnal();
        PID* pid = new PID(1, 5, 0.2);
        app->setSyg(syg);
        app->setARX(arx);
        app->setPID(pid);
        app->a="-0.4";
        app->b="0.6";
        app->syg->setA(1);
        app->syg->sett(10);
        app->syg->setp(0.5);
        app->arx->test=1;
        app->startSymulacja();
        app->sygnal=0;
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;

        spodzSygWy = {0.000, 0.840, 1.176, 0.725, 0.382, 0.643, 1.014, 0.977, 0.724, 0.697, 0.897, 1.002, 0.910, 0.820, 0.875, 0.972, 0.975, 0.917, 0.906, 0.954, 0.985, 0.967, 0.945, 0.957, 0.982, 0.986, 0.973, 0.970, 0.981, 0.990};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            app->symulacjaStep();
            faktSygWy[i]=app->getoldY();
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}

void Testy::test_UAR_ALL_2()
{
    //Sygnatura testu:
    std::cerr << "test_UAR_ALL_2 (A = -0.4 B=0.6,PID(1,5,0.2),k=1, sygnal SIN, amplituda=1,okres=10) -> test Sygnaly UARALL: ";

    try
    {
        // Przygotowanie danych:
        App* app = new App();
        ARX* arx = new ARX();
        Sygnal* syg = new Sygnal();
        PID* pid = new PID(1, 5, 0.2);
        app->setSyg(syg);
        app->setARX(arx);
        app->setPID(pid);
        app->a="-0.4";
        app->b="0.6";
        app->syg->setA(1);
        app->syg->sett(10);
        app->syg->setp(0.5);
        app->arx->test=1;
        app->startSymulacja();
        app->sygnal=1;
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;

        spodzSygWy = {0.000, 0.000, 0.494, 0.996, 0.854, 0.184, -0.403, -0.617, -0.619, -0.532, -0.259, 0.234, 0.698, 0.839, 0.607, 0.178, -0.262, -0.601, -0.746, -0.612, -0.215, 0.284, 0.663, 0.774, 0.596, 0.208, -0.257, -0.633, -0.770, -0.605};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            app->symulacjaStep();
            faktSygWy[i]=app->getoldY();
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}

void Testy::test_UAR_ALL_3()
{
    //Sygnatura testu:
    std::cerr << "test_UAR_ALL_3 (A = -0.4 B=0.6,PID(1,5,0.2),k=1, sygnal Postokątny, amplituda=1,okres=10,Wypelnienie=0,5) -> test Sygnaly UAR_ALL: ";

    try
    {
        // Przygotowanie danych:
        App* app = new App();
        ARX* arx = new ARX();
        Sygnal* syg = new Sygnal();
        PID* pid = new PID(1, 5, 0.2);
        app->setSyg(syg);
        app->setARX(arx);
        app->setPID(pid);
        app->a="-0.4";
        app->b="0.6";
        app->syg->setA(1);
        app->syg->sett(10);
        app->syg->setp(0.5);
        app->arx->test=1;
        app->startSymulacja();
        app->sygnal=2;
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu
        std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
        std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

        // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
        //for (size_t  i = 0; i < LICZ_ITER; i++)
        //    sygWe[1] = 2;

        spodzSygWy = {0.000, 0.840, 1.176, 0.725, 0.382, 0.643, -0.666, -1.375, -0.726, -0.067, -0.389, 0.654, 1.308, 0.822, 0.245, 0.463, -0.681, -1.301, -0.737, -0.166, -0.450, 0.673, 1.329, 0.788, 0.195, 0.451, -0.667, -1.305, -0.764, -0.186};
        // Symulacja modelu:
        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            app->symulacjaStep();
            faktSygWy[i]=app->getoldY();
        }

        // Walidacja poprawności i raport:
        if (porownanieSekwencji(spodzSygWy, faktSygWy))
            std::cerr << "OK!\n";
        else
        {
            std::cerr << "FAIL!\n";
            raportBleduSekwencji(spodzSygWy, faktSygWy);
        }
    }
    catch (...)
    {
        std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
    }
}
