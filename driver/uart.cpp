#include "uart.h"

Uart::Uart(){
    serial.settings.BaudRate = BAUD9600;
    serial.settings.DataBits = DATA_8;
    serial.settings.Parity = PAR_NONE;
    serial.settings.StopBits = STOP_1;
    serial.settings.FlowControl = FLOW_OFF;
    serial.settings.Timeout_Millisec = 0;
    delimiter = QString(0x0D);
}

Uart::~Uart(){

}

void Uart::setFlow(FlowType flow){
    serial.settings.FlowControl = flow;
}

void Uart::setBaud(BaudRateType baud){
    serial.settings.BaudRate = baud;
}

void Uart::setPort(QString port){
    serial.portName = port;
}

void Uart::setLoop(Uart *loop){
    this->loop = loop;
}

bool Uart::Open(){
    // Port öffnem
    serial.port = new QextSerialPort(serial.portName,serial.settings);

    // Prüfen ob geöffnet
    if(serial.port->open(QextSerialPort::ReadWrite)){
        QObject::connect(serial.port, SIGNAL(readyRead()),this,SLOT(Rcv()));
        serial.open = true;
        return true;
    }else{
        return false;
    }
}

bool Uart::Close(){
    if(serial.open == true){
        QObject::disconnect(serial.port, SIGNAL(readyRead()),this,SLOT(RcvData()));
        serial.port->close();
        serial.portNr = 0;
        serial.open = false;
        return true;
    }else{
        return false;
    }
}

void Uart::Rcv(){
        static QString telegram;
        char data;
        char *dataPtr;
        dataPtr = &data;
        int read;  // Anzahl gelesener Bytes
        while(serial.port->bytesAvailable()){
            read = serial.port->read(dataPtr,1);
            if(read > 0){
                RcvChars(QString(*dataPtr));
                RcvTelegram(QString(*dataPtr));
                if(this->loop != 0){
                    if(this->loop->IsOpen()){
                        this->loop->sendChar(*dataPtr);
                    }
                }
            }
        }
}

void Uart::RcvChars(QString c){
        if(rxBytes.count() < MAX_CHARS)
            rxBytes.append(c);
        emit newChar();
}

void Uart::RcvTelegram(QString c){
        static QString telegram;
        if(c == QString(0x0D) || c == QString(0x0A)){ // <CR> empfangen
            if( rxTelegramme.count() < MAX_TELEGRAM)
                rxTelegramme.push(telegram);
            telegram.clear();
            emit newTelegram();
        }else{
            telegram.append(c);
        }
}

QString Uart::getChar(){
    char c;
    c = rxBytes.at(0);
    rxBytes.remove(0,1);
    return QString(c);
}

QString Uart::getTelegram(){
    return rxTelegramme.pop();
}

int Uart::countBytes(){
    return rxBytes.count();
}

int Uart::countTelegramme(){
    return rxTelegramme.count();
}

bool Uart::IsOpen(){
    return serial.open;
}

void Uart::sendString(QString str){
    if(this->IsOpen()){
        serial.port->write(str.toLatin1());
    }else{
        qDebug() << "Interface ist geschlossen!!";
    }
}

void Uart::sendTelegram(QString str){
    sendString(str.append(delimiter));
}

void Uart::sendChar(char c){
    sendString(QString(c));
}
