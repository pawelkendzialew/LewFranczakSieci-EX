#ifndef TEST_H
#define TEST_H

#include <vector>

class Testy {
private:

public:
    static void test_Sygnału_Sin();
    static void test_Sygnału_Sin2();
    static void test_Sygnału_Jednostkowego();
    static void test_Sygnału_Prostokotny();
    static void test_Sygnału_Prostokotny2();
    static void test_RegulatorPID_skokJednostkowy();
    static void test_RegulatorP_skokJednostkowy();
    static void test_RegulatorP_brakPobudzenia();

    static void test_ModelARX_brakPobudzenia();
    static void test_ModelARX_skokJednostkowy_1();
    static void test_ModelARX_skokJednostkowy_2();
    static void test_ModelARX_skokJednostkowy_3();

    static void test_UAR_1_brakPobudzenia();
    static void test_UAR_1_skokJednostkowy();
    static void test_UAR_2_skokJednostkowy();
    static void test_UAR_3_skokJednostkowy();

    static void test_UAR_ALL();
    static void test_UAR_ALL_2();
    static void test_UAR_ALL_3();
};
void raportBleduSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt);
bool porownanieSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt);

#endif // TEST_H
