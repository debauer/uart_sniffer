#ifndef ABSINTERFACE_H
#define ABSINTERFACE_H

#include <QString>

// Virtuelle klasse die alle Interfaces beschreibt.
// UART, ETH, CAN...

class AbsInterface{
public:
    virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual ~AbsInterface(){};
};

#endif // ABSINTERFACE_H
