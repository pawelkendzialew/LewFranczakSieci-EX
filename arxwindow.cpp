#include "arxwindow.h"
#include "ui_arxwindow.h"
#include "app.h"  // jeśli jeszcze nie jest dołączony

ARXwindow::ARXwindow(QWidget *parent, App* appPtr)
    : QDialog(parent)
    , ui(new Ui::ARXwindow)
    , app(appPtr)
{
    ui->setupUi(this);
    ui->INPUTA->setPlainText(QString::fromStdString(app->a));
    ui->INPUTB->setPlainText(QString::fromStdString(app->b));
    ui->SpinBoxZaklucenia->setValue(app->arx->getzasiegZak());
    ui->SPINBOXK->setValue(app->getk());
}

ARXwindow::~ARXwindow()
{
    delete ui;
}

void ARXwindow::setData(){
    double zaklocenia = ui->SpinBoxZaklucenia->value();
    if (app) {
        // korzystamy z metod obiektu App
        app->arx->setzasiegZak(zaklocenia);
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
        //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
        app->setk(ui->SPINBOXK->value());
    }
}
void ARXwindow::on_btnUstaw_clicked()
{
    setData();
}

void ARXwindow::on_btnRestart_clicked()
{
    ui->SPINBOXK->setValue(1);
    ui->INPUTA->setText(QString::fromStdString("-0.4"));
    ui->INPUTB->setText(QString::fromStdString("0.6"));
    ui->SpinBoxZaklucenia->setValue(0.01);
    app->setk(ui->SPINBOXK->value());

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
    app->arx->setzasiegZak(ui->SpinBoxZaklucenia->value());
    setData();
}


void ARXwindow::on_buttonBox_accepted()
{
    setData();
}


void ARXwindow::on_SpinBoxZaklucenia_editingFinished()
{
        app->arx->setzasiegZak(ui->SpinBoxZaklucenia->value());
}


void ARXwindow::on_SPINBOXK_editingFinished()
{
    app->setk(ui->SPINBOXK->value());
}

