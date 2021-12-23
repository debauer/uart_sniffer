#ifndef UART_H
#define UART_H

/*
 * uart class by david 'debauer' bauer
 *
 * developed for schlagertronik GmbH
 *
 * 25.06.2014
 *
 * Sniffer PROJEKT
 *
 */

#include <QTimer>
#include <QStack>
#include <QDebug>

#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "absInterface.h"

#define RCV_TELEGRAM 1
#define RCV_CHARS 1

#define MAX_TELEGRAM 50
#define MAX_CHARS 1000

struct serialStruct{
    QextSerialPort *port;
    QList<QextPortInfo> portlist;
    PortSettings settings;
    int portNr;
    bool open;
    QString portName;
};

class Uart : public QObject, public AbsInterface{
        Q_OBJECT
    private:
        serialStruct serial;
        QByteArray rxBytes;
        QStack<QString> rxTelegramme;
        QString delimiter;
         Uart *loop = 0;

        void RcvChars(QString c);
        void RcvTelegram(QString c);

    public slots:
        void Rcv();
        void sendString(QString str);
        void sendChar(char c);
        void sendTelegram(QString str); // = sendString + delimiter
    public:
        Uart ();
        ~Uart();

        QString getTelegram(); // rxStack
        QString getChar();     // rxBytes
        QString getString();   // rxBytes
        int countBytes();
        int countTelegramme();
        void setLoop(Uart *loop);

        void setTelegramende(QString s);
        void setBaud(BaudRateType baud);
        void setFlow(FlowType flow);
        void setPort(QString port);

        virtual bool Open();
        virtual bool Close();
        virtual bool IsOpen();

    signals:
        void newTelegram();
        void newChar();
};

#endif // UART_H
