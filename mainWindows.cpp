#include "mainWindows.h"
#include <ui_mainWindows.h>
#include <limits>
#include <assert.h>
#include <iostream>

mainWindows *kt;
QString openDir;
QString saveDir;

mainWindows::mainWindows(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainWindows){

    QSignalMapper *signalMapper = new QSignalMapper (this);
    /* config file */

    Settings = new QSettings("config.ini", QSettings::IniFormat);
    Settings->beginGroup("loop1");
    this->comPort[0] = Settings->value("com1").toString();
    this->comPort[1] = Settings->value("com2").toString();
    this->comBaud[0] = Settings->value("baud1").toString();
    this->comBaud[1] = Settings->value("baud2").toString();
    Settings->endGroup();

    Settings->beginGroup("loop2");
    this->comPort[2] = Settings->value("com1").toString();
    this->comPort[3] = Settings->value("com2").toString();
    this->comBaud[2] = Settings->value("baud1").toString();
    this->comBaud[3] = Settings->value("baud2").toString();
    Settings->endGroup();

    Settings->beginGroup("gpib");
    this->gpibPort = Settings->value("com").toString();
    this->gpibBaud = Settings->value("baud").toString();
    Settings->endGroup();
    /* UART Verbinden */

    this->connectGpib();
    this->connectUart(0);
    this->connectUart(1);
    this->connectUart(2);
    this->connectUart(3);

    this->uart[0].setLoop(&this->uart[1]);
    this->uart[1].setLoop(&this->uart[0]);
    this->uart[2].setLoop(&this->uart[3]);
    this->uart[3].setLoop(&this->uart[2]);

    ui->setupUi(this);
    this->setWindowTitle(QString("Sniffer"));

    signalMapper -> setMapping (&uart[0], 0);
    signalMapper -> setMapping (&uart[1], 1);
    signalMapper -> setMapping (&uart[2], 2);
    signalMapper -> setMapping (&uart[3], 3);
    QObject::connect (&uart[0], SIGNAL(newTelegram()), signalMapper, SLOT(map())) ;
    QObject::connect (&uart[1], SIGNAL(newTelegram()), signalMapper, SLOT(map())) ;
    QObject::connect (&uart[2], SIGNAL(newTelegram()), signalMapper, SLOT(map())) ;
    QObject::connect (&uart[3], SIGNAL(newTelegram()), signalMapper, SLOT(map())) ;
    QObject::connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(rcvTelegramme(int)));


    QObject::connect(&gpib, SIGNAL(newTelegram()),this,SLOT(rcvGpib()));

}

mainWindows::~mainWindows()
{
    delete ui;
}

void mainWindows::rcvTelegramme(int uartId){
    QString telegram;
    while(uart[uartId].countTelegramme() > 0){
        telegram = uart[uartId].getTelegram();
        ui->log->append(QDateTime::currentDateTime().toString() + " | UART" + (uartId+0x30) + ": " + telegram);
    }
}

void mainWindows::rcvGpib(){
    QString telegram;
    while(gpib.countTelegramme() > 0){
        telegram = gpib.getTelegram();
        ui->log->append(QDateTime::currentDateTime().toString() + " | GPIB : " + telegram);
    }
}

void mainWindows::connectUart(int uartId){
    uart[uartId].setBaud(BAUD19200);
    uart[uartId].setFlow(FLOW_HARDWARE);
    uart[uartId].setPort(this->comPort[uartId]);
    uart[uartId].Open();
    if(!uart[uartId].IsOpen()){
        QMessageBox::warning(this, "Error", "Konnte Com Port (" + this->comPort[uartId] + ") nicht öffnen.",QMessageBox::Ok);
        qDebug() << "UART nicht geöffnet - falsche Config?!";
    }
}

void mainWindows::connectGpib(){
    gpib.setBaud(BAUD57600);
    gpib.setFlow(FLOW_OFF);
    gpib.setPort(this->gpibPort);
    gpib.Open();
    if(!gpib.IsOpen()){
        QMessageBox::warning(this, "Error", "Konnte GPIB Port (" + this->gpibPort + ") nicht öffnen.",QMessageBox::Ok);
        qDebug() << "GPIB nicht geöffnet - falsche Config?!";
    }
}

void mainWindows::FileSave(){
        /* Speicherort der neuen Datei bestimmen */
        QFileDialog fileDiag;
        QString NewFileName = fileDiag.getSaveFileName(this, tr("Speicherort für konvertierte Datei auswählen"), saveDir, tr("*.dat"));

        if(!NewFileName.isNull()){
            saveDir = QFileInfo(NewFileName).path(); // store path for next time
        }

        QFile fh(NewFileName);
        if(!fh.open(QIODevice::WriteOnly)){
            //ui->console->append(QString("Konnte konvertiertes Protokoll nicht speichern!\n"));
        }else{
            QTextStream Outfile(&fh);
            //Outfile << ui.
            //fh.close();
        }
}
