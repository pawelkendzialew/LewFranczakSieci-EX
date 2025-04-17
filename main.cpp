#include "mainwindow.h"

#include <QApplication>
#include "test.h"
#define PROGRAM
#ifdef PROGRAM
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#endif

#ifdef TEST
int main(int argc, char *argv[])
{
    Testy::test_RegulatorP_brakPobudzenia();
    Testy::test_RegulatorP_skokJednostkowy();
    Testy::test_RegulatorPID_skokJednostkowy();
    Testy::test_ModelARX_brakPobudzenia();
    Testy::test_ModelARX_skokJednostkowy_2();
    Testy::test_ModelARX_skokJednostkowy_1();
    Testy::test_ModelARX_skokJednostkowy_3();
    Testy::test_UAR_1_brakPobudzenia();
    Testy::test_UAR_1_skokJednostkowy();
    Testy::test_UAR_2_skokJednostkowy();
    Testy::test_UAR_3_skokJednostkowy();
    Testy::test_Sygnału_Sin();
    Testy::test_Sygnału_Sin2();
    Testy::test_Sygnału_Jednostkowego();
    Testy::test_Sygnału_Prostokotny();
    Testy::test_Sygnału_Prostokotny2();
    Testy::test_UAR_ALL();
    Testy::test_UAR_ALL_2();
    Testy::test_UAR_ALL_3();
}


#endif
