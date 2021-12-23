#ifndef MARRY_ME_H
#define MARRY_ME_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QSignalMapper>
#include <QDateTime>

#include "driver/uart.h"

namespace Ui {
    class mainWindows;
}

//! Konvertierungstool Hauptklasse.
/*!
Integrator Prüfprotokoll wird mit regex eingelesen und in QStrings gespeichert.
Dabei werden die werte direkt Normalisiert. zB Werte in Kilo mal 1000 gerechnet.
Das geschieht mit den "conv_" Funktionen.
*/

class mainWindows : public QMainWindow
{
    Q_OBJECT

public:
    //! Einlese Pattern Struct.
    /*! Beinhaltet den Suchstring, replace Marker, eine Beschreibung, Länge des einzulesenen Strings,  */

    //static Konversionstool kt;
    QString FileName = NULL;
    QString Outstring;
    QTimer timerTemperatur;

    QSignalMapper signalmapper;

    QSettings *Settings;
    QString comPort[4];
    QString comBaud[4];
    Uart uart[4];
    Uart gpib;
    QString gpibPort;
    QString gpibBaud;

    QString htmlSend = "<font color=\"DarkBlue\">";
    QString htmlRcv = "<font color=\"DarkSlateBlue\">";
    QString htmlErr = "<font color=\"Crimson\">";
    QString htmlEnd = "</font>";

    explicit mainWindows(QWidget *parent = 0);
    ~mainWindows();

public slots:
    void FileSave();                    //!< Konvertieres Protokoll speichern
    void connectUart(int uart);                 //!< UART öffnen
    void connectGpib();                         //!< UART öffnen
    void rcvTelegramme(int uart);               //!< Telegramme verarbeiten
    void rcvGpib();               //!< Telegramme verarbeiten
    //void sendTelegram(QString t);       //!< Befehl an EF5 senden


private:
    Ui::mainWindows *ui;


};

#endif // MARRY_ME_H
