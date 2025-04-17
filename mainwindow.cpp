#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
#include "NetworkManager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , app(new App())
    , timer(new QTimer(this))


{
    ARX* arx = new ARX();
    Sygnal* syg = new Sygnal();
    PID* pid = new PID(0.5, 5.0, 0.2);

    app->setSyg(syg);
    app->setARX(arx);
    app->setPID(pid);
    app->syg->setA(1);
    app->syg->sett(10);
    app->syg->setp(0.5);
    ui->setupUi(this);
    app->sygnal=0;
    app->a="-0.4";
    app->b="0,6";

    ui->setupUi(this);
    networkManager = new NetworkManager(this);

    connect(networkManager, &NetworkManager::commandReceived, this, [=](const QString &cmd) {
        if (cmd == "START") {
            on_pushButton_clicked(); // albo ui->START->click();
        } else if (cmd == "STOP") {
            on_STOP_clicked(); // lub przypisana funkcja
        }
    });

    connect(ui->pushButton_startServer, &QPushButton::clicked, this, [=]() {
        networkManager->startServer();
    });

    connect(ui->pushButton_connectClient, &QPushButton::clicked, this, [=]() {
        networkManager->connectToServer();
    });
    connect(ui->pushButton_sendCommand, &QPushButton::clicked, this, [=]() {
        QString msg = ui->lineEdit_command->text();
        if (networkManager && !msg.isEmpty()) {
            networkManager->sendMessage(msg);
        }
    });



    ui->comboBox->addItem("Jednostkowy", QVariant(0));
    ui->comboBox->addItem("Sinusoidalny", QVariant(1));
    ui->comboBox->addItem("Prostokontny", QVariant(2));

    ui->comboBox_2->addItem("stałą przed sumą",QVariant(0));
    ui->comboBox_2->addItem("stałą pod sumą",QVariant(1));

    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    app->setk(1);
    app->a =   "-0.4";
    app->b =   "0.6";
    app->Pid->trybpracy=ui->comboBox_2->currentData().toInt();
    /*
     * QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
        app->a="0";
        return;
    }

    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
    input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
        app->b="0";
        return;
    }

    app->b = input.toStdString();

    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    */
    timer->start(app->getinterwal());
    //char 3
    chart3 = new QChart();
    chart3->setBackgroundVisible(true);

    // Uchyb
    seriesUchyb = new QLineSeries();
    seriesUchyb->setName("Uchyb");
    QPen pende(Qt::black);
    pende.setWidth(2);
    seriesUchyb->setPen(pende);
    chart3->addSeries( seriesUchyb);

    // Oś X
    axisX3 = new QValueAxis();
    axisX3->setRange(1, 100);
    axisX3->setTitleText("Czas");
    chart3->addAxis(axisX3, Qt::AlignBottom);

    // Oś Y
    axisY3 = new QValueAxis();
    axisY3->setRange(-0.4,0.5);
    axisY3->setTitleText("Wartość");
    chart3->addAxis(axisY3, Qt::AlignLeft);


    // Tworzenie widoku wykresu
    chartView3 = new QChartView(chart3, this);
    chartView3->setRenderHint(QPainter::Antialiasing);
    chartView3->setStyleSheet("border: none;");
    // Dodanie wykresu do kontenera w interfejsie użytkownika
    ui->chartContainer_3->setLayout(new QVBoxLayout());
    ui->chartContainer_3->layout()->addWidget(chartView3);

    // Tworzenie wykresu dla chart1
    chart1 = new QChart();
    chart1->setBackgroundVisible(true);



    // Up
    seriesUp = new QLineSeries();
    seriesUp->setName("Up");
    chart1->addSeries(seriesUp);

    // Ui
    seriesUi = new QLineSeries();
    seriesUi->setName("Ui");
    QPen peni(Qt::yellow);
    peni.setWidth(2);
    seriesUi->setPen(peni);
    chart1->addSeries(seriesUi);

    // Ud
    seriesUd = new QLineSeries();
    seriesUd->setName("Ud");
    QPen pend(Qt::green);
    pend.setWidth(2);
    seriesUd->setPen(pend);
    chart1->addSeries(seriesUd);



    // Oś X
    axisX1 = new QValueAxis();
    axisX1->setRange(1, 100);
    axisX1->setTitleText("Czas");
    chart1->addAxis(axisX1, Qt::AlignBottom);

    // Oś Y
    axisY1 = new QValueAxis();
    axisY1->setRange(-0.1, 0.1);
    axisY1->setTitleText("Wartość");
    chart1->addAxis(axisY1, Qt::AlignLeft);



    // Podłączenie serii do osi
    seriesUp->attachAxis(axisX1);
    seriesUp->attachAxis(axisY1);

    seriesUi->attachAxis(axisX1);
    seriesUi->attachAxis(axisY1);

    seriesUd->attachAxis(axisX1);
    seriesUd->attachAxis(axisY1);

    seriesUchyb->attachAxis(axisX3);
    seriesUchyb->attachAxis(axisY3);

    // Tworzenie widoku wykresu
    chartView1 = new QChartView(chart1, this);
    chartView1->setRenderHint(QPainter::Antialiasing);
    chartView1->setStyleSheet("border: none;");
    // Dodanie wykresu do kontenera w interfejsie użytkownika
    ui->chartContainer_2->setLayout(new QVBoxLayout());
    ui->chartContainer_2->layout()->addWidget(chartView1);
    //wykres
    chart = new QChart();


    chart->setBackgroundVisible(true);








    //Y
    seriesY = new QLineSeries();
    seriesY->setName("Y");
    chart->addSeries(seriesY);



    //U
    seriesU = new QLineSeries();
    seriesU->setName("U");
    QPen pen(Qt::yellow);
    pen.setWidth(2);
    seriesU->setPen(pen);
    chart->addSeries(seriesU);
    //zalucenie


    //
    seriesW = new QLineSeries();
    seriesW->setName("Wejscie");
    QPen pen3(Qt::red);
    pen3.setWidth(2);
    seriesW->setPen(pen3);
    chart->addSeries(seriesW);

    //os X
    axisX = new QValueAxis();
    axisX->setRange(1, 100);
    axisX->setTitleText("Czas");
    chart->addAxis(axisX, Qt::AlignBottom);
    seriesY->attachAxis(axisX);
    seriesU->attachAxis(axisX);
    seriesW->attachAxis(axisX);


    //os y
    axisY = new QValueAxis();
    axisY->setRange(-0.1, 0.1);
    axisY->setTitleText("Wartość");
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->setFont(QFont("Arial", 8));
    seriesY->attachAxis(axisY);
    seriesU->attachAxis(axisY);

    seriesW->attachAxis(axisY);
    //tworzenie widoku wykrersu
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("border: none;");
    //dodanie wykresu do widoku cartcontiner
    ui->chartContainer->setLayout(new QVBoxLayout());
    ui->chartContainer->layout()->addWidget(chartView);

    //conect(wysyjacys sygnal,sygnal emitowany pzez,odbiorca,lambda [this]
    //ma dstep do danych w mainwindow
    connect(timer, &QTimer::timeout, this, [this]() {
        app->symulacjaStep();
        updateChart();
    });
}

MainWindow::~MainWindow()
{   delete app;
    delete ui;
}



void MainWindow::setUIFromApp()
{
    // Ustawienia SpinBoxów i ComboBoxa
    ui->SPINBOXAMPLITUDA->setValue(app->syg->axx());
    ui->SPINBOXOKRES->setValue(app->syg->getT());
    ui->SPINBOXWYPELNIENIE->setValue(app->syg->getP());

    ui->SPINBOXKD->setValue(app->Pid->getWzmocnienieRowniczkujace());
    ui->SPINBOXKI->setValue(app->Pid->getWzmocnienieCalkujace());
    ui->SPINBOXKP->setValue(app->Pid->getWzmocnienieProporcjonalne());

    ui->SPINBOXINTERWAL->setValue(app->getinterwal());
    ui->comboBox->setCurrentIndex(app->sygnal);
    ui->comboBox_2->setCurrentIndex(app->Pid->trybpracy);
    // Jeśli kiedyś będziesz używać INPUTA i INPUTB
    // ui->INPUTA->setPlainText(QString::fromStdString(app->a));
    // ui->INPUTB->setPlainText(QString::fromStdString(app->b));

    // Analogicznie, jeśli masz jeszcze inne elementy, ustaw je tutaj.
}



void MainWindow::on_START_clicked() {
     if (app) {
    app->startSymulacja();
       //  qDebug()<<app->getinterwal();
    timer->start(app->getinterwal());      // timer co 100 ms
    //qDebug() << "Symulacja uruchomiona.";
    ui->START->setEnabled(false);
    ui->STOP->setEnabled(true);


    //ui->SAVE->setDisabled(true);
     }
    else {
         qDebug() << "Błąd: Obiekt `App` nie został zainicjalizowany.";
    }
}

void MainWindow::on_STOP_clicked() {
    if (app) {
    app->stopSymulacja();  // Zatrzymaj symulację
    timer->stop();         // Zatrzymaj timer
   // qDebug() << "Symulacja zatrzymana.";
    ui->STOP->setEnabled(false);
    ui->START->setEnabled(true);

   // ui->SAVE->setDisabled(false);

    }
    else {
         qDebug() << "Błąd: `app->data` to nullptr";
     }
}

void MainWindow::on_pushButton_clicked()
{
    if (app) { //app?
        app->zapisBazydanychdopliku();


    }
    else {
        qDebug() << "`app->data` to nullptr";
    }
}

void MainWindow::on_ODCZYT_clicked()
{
    if (app) { //app?
        clearAllSeries();


        //restart X Y
       resetAxes();
        for (auto& elem : app->data) {
            delete elem; // usun dynamicznie  obiekty
        }
        app->data.clear();

        //
        BuforDanych::licznikInstancji=-1;
        app->odczytajDaneZPliku();

        if (app->data.empty()) {
            qDebug() << "`app->data` to nullptr";
            return;
        }
        for (auto& ostatniElement : app->data) {
            if (ostatniElement) {
                seriesY->append(ostatniElement->getI(), ostatniElement->getY());
                seriesU->append(ostatniElement->getI(), ostatniElement->getU());

                seriesW->append(ostatniElement->getI(), ostatniElement->getW());
                seriesUp->append(ostatniElement->getI(), ostatniElement->getUp());
                seriesUi->append(ostatniElement->getI(), ostatniElement->getUi());
                seriesUd->append(ostatniElement->getI(), ostatniElement->getUd());
                seriesUchyb->append(ostatniElement->getI(), ostatniElement->getUchyb());
                //Rozszerzenie zakresu osi X
                if (ostatniElement->getI() > axisX->max()*0.80 ) {
                    axisX->setRange(axisX->min() + 1, axisX->max() + 1);
                    axisX1->setRange(axisX1->min() + 1, axisX1->max() + 1);
                }
                if (ostatniElement->getY() < axisY->min()*0.8 ||
                    ostatniElement->getW()<  axisY->min()*0.8 ||
                    ostatniElement->getU()<axisY->min()*0.8
                    ) {
                    axisY->setRange(axisY->min() *1.8, axisY->max() );
                }
                if (ostatniElement->getY() > axisY->max()*0.8 ||
                    ostatniElement->getW()>  axisY->max()*0.8 ||
                    ostatniElement->getU()> axisY->max()*0.8) {
                    axisY->setRange(axisY->min(), axisY->max()*1.8 );
                }
                if (ostatniElement->getUp() < axisY1->min()*0.8 ||

                    ostatniElement->getUi()<axisY1->min()*0.8 ||
                    ostatniElement->getUd()<axisY1->min()*0.8) {
                    axisY1->setRange(axisY1->min()*1.8, axisY1->max() );
                }
                if (ostatniElement->getUp() > axisY1->max()*0.8 ||

                    ostatniElement->getUi()>axisY1->max()*0.8 ||
                    ostatniElement->getUd()>axisY1->max()*0.8
                    ) {
                    axisY1->setRange(axisY1->min(), axisY1->max()*1.8 );
                }
                if(ostatniElement->getUchyb()< axisY3->min()*0.8 ){
                axisY3->setRange(axisY3->min()*1.8, axisY3->max() );
                }
                if(ostatniElement->getUchyb()> axisY1->max()*0.8  ){
                     axisY1->setRange(axisY1->min(), axisY1->max()*1.8 );
                }

            } else {
                qDebug() <<"`app->data` to nullptr";
            }
        }





        //app->wypiszDane();
        setUIFromApp();

        app->setinterwal(ui->SPINBOXINTERWAL->value());
        app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
        app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
        app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
        app->sygnal=ui->comboBox->currentData().toInt();
        app->syg->sett(ui->SPINBOXOKRES->value());
        app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
        app->syg->setA(ui->SPINBOXAMPLITUDA->value());
        /*
         *
         *
        ui->INPUTA->setText(QString::fromStdString(app->a));
        ui->INPUTB->setText(QString::fromStdString(app->b));
 ui->SPINBOXK->setValue(app->getk());



        app->setk(ui->SPINBOXK->value());
        //
    QString input = ui->INPUTA->toPlainText();
        if (input.isEmpty()) {
            app->a="0";
            return;
        }
        app->a = input.toStdString();
        // Wyświetl komunikat o sukcesie
        //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
        input = ui->INPUTB->toPlainText();
        if (input.isEmpty()) {
            app->b="0";
            return;
        }

        app->b = input.toStdString();
        */ //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
        timer->start(app->getinterwal());

    }
    else {
        qDebug() << "Blad: App niezainicjalizwane";
    }
}
#include <QRandomGenerator> // dodaj na górze pliku


void MainWindow::autoAdjustGroupedAxisY(const QList<QLineSeries*>& seriesList, QValueAxis* axisY) {

        if (seriesList.isEmpty()) return;

        qreal minY = 0;
        qreal maxY = 0;
        bool first = true;

        for (QLineSeries* series : seriesList) {
            int count = series->count();
            if (count == 0) continue;

            int endIndex = count - 1;
            int startIndex = qMax(0, endIndex - 99);
            int rangeSize = endIndex - startIndex + 1;

            int samples = qMin(25, rangeSize); // liczba próbek do analizy

            for (int s = 0; s < samples; ++s) {
                int randomIndex = startIndex + QRandomGenerator::global()->bounded(rangeSize);
                qreal y = series->at(randomIndex).y();

                if (first) {
                    minY = maxY = y;
                    first = false;
                } else {
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }

        if (first) return; // brak danych

        qreal margin = (maxY - minY) * 0.15;
        if (margin == 0) margin = 0.01;

        axisY->setRange(minY - margin, maxY + margin);}


void MainWindow::updateChart()
{
    if (app->data.empty()) {
       // qDebug() << "Brak danych w `app->data`";
        return;
    }

    BuforDanych* ostatniElement = app->data.back();
    if (ostatniElement) {
        seriesY->append(ostatniElement->getI(), ostatniElement->getY());
        seriesU->append(ostatniElement->getI(), ostatniElement->getU());

        seriesW->append(ostatniElement->getI(), ostatniElement->getW());
        seriesUp->append(ostatniElement->getI(),app->Pid->Up);
        seriesUi->append(ostatniElement->getI(),app->Pid->Ui);
        seriesUd->append(ostatniElement->getI(),app->Pid->Ud);
        seriesUchyb->append(ostatniElement->getI(), ostatniElement->getUchyb());

        //zwiekszanie wykresy
        if (ostatniElement->getI() > axisX->max()*0.80 ) {
            axisX->setRange(axisX->min() + 1, axisX->max() + 1);
            axisX1->setRange(axisX1->min() + 1, axisX1->max() + 1);
            axisX3->setRange(axisX3->min() + 1, axisX3->max() + 1);
        }
         if (ostatniElement->getY() < axisY->min()*0.8 ||
            ostatniElement->getW()<  axisY->min()*0.8 ||
            ostatniElement->getU()<axisY->min()*0.8
            ) {
            axisY->setRange(axisY->min() *1.8, axisY->max() );
        }
        if (ostatniElement->getY() > axisY->max()*0.8 ||
            ostatniElement->getW()>  axisY->max()*0.8 ||
            ostatniElement->getU()> axisY->max()*0.8) {
            axisY->setRange(axisY->min(), axisY->max()*1.8 );
        }
        if (ostatniElement->getUp() < axisY1->min()*0.8 ||

            ostatniElement->getUi()<axisY1->min()*0.8 ||
            ostatniElement->getUd()<axisY1->min()*0.8) {
            axisY1->setRange(axisY1->min() *1.8, axisY1->max() );
        }
        if (ostatniElement->getUp() > axisY1->max()*0.8 ||

            ostatniElement->getUi()>axisY1->max()*0.8 ||
            ostatniElement->getUd()>axisY1->max()*0.8
            ) {
            axisY1->setRange(axisY1->min(), axisY1->max()*1.8 );
        }
        if( ostatniElement->getUchyb()> axisY3->max()*0.8 ){
             axisY3->setRange(axisY3->min(), axisY3->max()*1.8 );
        }
        if(   ostatniElement->getUchyb()< axisY3->min()*0.8  ){
             axisY3->setRange(axisY3->min() *1.8, axisY3->max() );
        }




    } else {
        //qDebug() << "`app->data` to nullptr";
    }
    updateCounter++;

    if (updateCounter % 75 ==0) {
        autoAdjustGroupedAxisY({seriesY, seriesU, seriesW}, axisY);
        autoAdjustGroupedAxisY({seriesUp, seriesUi, seriesUd}, axisY1);
        autoAdjustGroupedAxisY({seriesUchyb}, axisY3);
    }
}

void MainWindow::on_RESET_clicked()
{
    clearAllSeries();

    resetAxes();
    //czyszczenie bazy
    for (auto& elem : app->data) {
        delete elem; // usun dynamicznie  obiekty
    }
    app->data.clear();

    BuforDanych::licznikInstancji=-1;
    app->Pid->resetPamieci();

    app->Pid->trybpracy=0;



    ui->SPINBOXAMPLITUDA->setValue(1);
    ui->SPINBOXOKRES->setValue(10);
    ui->SPINBOXWYPELNIENIE->setValue(0.5);
    //
    ui->SPINBOXKD->setValue(0.5);
    ui->SPINBOXKI->setValue(5);
    ui->SPINBOXKP->setValue(0.2);
    //
    ui->SPINBOXINTERWAL->setValue(100);

    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    app->Pid->trybpracy=0;
    app->Pid->poprzedniUchyb=0;
    app->Pid->suma=0;
    app->Pid->uchyb=0;
    app->oldY=0;

    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    timer->start(app->getinterwal());
}
void MainWindow::on_POMOC_clicked()
{//wzmocnienieProporcjonalne(kp), wzmocnienieCalkujace(ki), wzmocnienieRowniczkujace(kd),
    QString instrukcja = "Instrukcja obsługi aplikacji:\n\n"
                         "1. Wprowadź wartości do pól 'USTAW A' i 'USTAW B' w odpowiednim formacie (np. 0.4;-0.2).\n"
                         "2. Ustaw współczynniki Kp, Ki, Kd za pomocą pól na dole interfejsu.\n"
                         "3. Kliknij przycisk 'START', aby rozpocząć symulację.\n"
                         "4. Użyj przycisku 'RESET', aby zresetować ustawienia. Reset usuwa niezapisane dane\n"
                         "5. Możesz zapisać dane za pomocą przycisku 'ZAPISZ' lub wczytać dane z pliku za pomocą 'ODCZYTAJ Z PLIKU'.\n\n"
                         "Rozwiniecie Skrótów\n\n"
                         "A - Wspolczynik wielomianu A \n"
                         "A - Wspolczynik wielomianu B \n"
                         "Kp - Wzmocnienie Proporcjonalne \n"
                         "Ti - Wzmocnienie Calkujace\n"
                         "Td - Wzmocnienie Rowniczkujace\n";
     QMessageBox::information(this, "POMOC",instrukcja);






}

void MainWindow::on_BTNUSTAWALL_clicked()
{

    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    app->Pid->trybpracy=ui->comboBox_2->currentData().toInt();
   /* QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
       app->a="0";
        return;
    }
    app->a = input.toStdString();
    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
     input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
         app->b="0";
        return;
    }

    app->b = input.toStdString();
    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
 */
  timer->start(app->getinterwal());
}


void MainWindow::on_BTNZAPISZCONF_clicked()
{
    app->zapisconf();
}


void MainWindow::on_pushButton_2_clicked()
{

    setUIFromApp();



    //app->setk(ui->SPINBOXK->value());
    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    /*QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
        app->a="0";
        return;
    }
    app->a = input.toStdString();
    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
    input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
        app->b="0";
        return;
    }

    app->b = input.toStdString();
    */ //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    timer->start(app->getinterwal());


}


void MainWindow::on_pushButton_3_clicked()
{
    arx_window = new ARXwindow(nullptr,app);
    arx_window->show();


}
void MainWindow::clearAllSeries()
{
    if (seriesY)  seriesY->clear();
    if (seriesU)  seriesU->clear();
    if (seriesW)  seriesW->clear();
    if (seriesUp) seriesUp->clear();
    if (seriesUd) seriesUd->clear();
    if (seriesUi) seriesUi->clear();
    if (seriesUchyb) seriesUchyb->clear();
}



void MainWindow::resetAxes()
{
    if (axisX)  axisX->setRange(0, 100);
    if (axisY)  axisY->setRange(-0.1, 0.1);
    if (axisX1) axisX1->setRange(0, 100);
    if (axisY1) axisY1->setRange(-0.1, 0.1);
    if (axisX3) axisX3->setRange(0, 100);
    if (axisY3) axisY3->setRange(-0.1, 0.1);
}

void MainWindow::on_ResetCalk_clicked()
{
    app->Pid->suma=0;
}


void MainWindow::on_pushButton_5_clicked()
{
    app->Pid->poprzedniUchyb=0;
}


void MainWindow::on_SPINBOXAMPLITUDA_editingFinished()
{
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
}
/*
 *
    //


    ;




    app->Pid->trybpracy=ui->comboBox_2->currentData().toInt();
   /* QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
       app->a="0";
        return;
    }
    app->a = input.toStdString();
    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
     input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
         app->b="0";
        return;
    }

    app->b = input.toStdString();
    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);


}
*/

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    app->sygnal=index;
}


void MainWindow::on_SPINBOXOKRES_editingFinished()
{
    app->syg->sett(ui->SPINBOXOKRES->value());
}


void MainWindow::on_SPINBOXWYPELNIENIE_editingFinished()
{
     app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
}


void MainWindow::on_SPINBOXINTERWAL_editingFinished()
{   app->setinterwal(ui->SPINBOXINTERWAL->value());
    timer->start(app->getinterwal());
}


void MainWindow::on_SPINBOXKD_editingFinished()
{
     app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
}


void MainWindow::on_SPINBOXKI_editingFinished()
{
     app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
}


void MainWindow::on_SPINBOXKP_editingFinished()
{
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
}


void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    app->Pid->trybpracy=index;

}

