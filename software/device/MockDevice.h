#ifndef MOCKDEVICE_H
#define MOCKDEVICE_H

#include <QTime>
#include "Device.h"

class MockDevice : public Device
{
public:
    explicit MockDevice(QObject *parent = 0);

protected:
    void measure();
    void done();

private:
    QTimer * const m_timer;
    QTime m_time;
};

#endif // MOCKDEVICE_H
