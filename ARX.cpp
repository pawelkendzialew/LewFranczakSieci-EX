#include "ARX.h"
#include "App.h"

using namespace std;

double ARX::generateDisturbance() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1*zasiegZak, zasiegZak);
    return dis(gen);
}

double ARX::calcb(std::vector<BuforDanych*> data) {
    if (data.empty()) {
        throw std::invalid_argument("Dane są puste! Nie można wykonać obliczeń.");
    }

    double sum = 0.0;
    int n_b = data.back()->getvectorB().size();
    int k = data.back()->k;

    for (int i = 0; i < n_b; ++i) {
        int idx = data.size() - i - k-1;
        if (idx >= 0 && idx < static_cast<int>(data.size())) {
            sum += data[idx]->getU() * data.back()->getb(i);
            // std::cout << "Bcalc " << data[idx]->getU() << " U " << data.back()->getb(i) << " A " << sum << " sum\n";
        }
    }

    return sum;
}

double ARX::calaA(std::vector<BuforDanych*> data) {
    if (data.empty()) {
        throw std::invalid_argument("Dane są puste! Nie można wykonać obliczeń.");
    }

    double sum = 0.0;

    int n_b = data.back()->getvectorA().size();


    for (int i = 0; i < n_b; ++i) {
        int idx = data.size() - i  - 1;

        if (idx -i>= 0 && idx < static_cast<int>(data.size())) {
            sum += data[idx - i]->getY()* data.back()->getA(i);
            // std::cout << "Acalc " << data[idx-i]->getY() << " Y " << data.back()->getA(i) << " A " << sum << " sum\n";
        }
        else {
            sum += 0.0;
        }
    }
    return sum;
}


double ARX::calcAll(std::vector<BuforDanych*> data) {
    if(test==0){
    if (data.size() == 1) {
        return 0;
   }
    return calcb(data) - calaA(data) + data.back()->getZaklucenie();
    }
    else{
        if (data.size() == 1) {
            return 0;
        }
        return calcb(data) - calaA(data);
    }
    }
double ARX::getzasiegZak(){
    return zasiegZak;
}
void ARX::setzasiegZak(double x){
    zasiegZak=x;
}

