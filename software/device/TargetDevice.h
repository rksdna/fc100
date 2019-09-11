#ifndef TARGETDEVICE_H
#define TARGETDEVICE_H

#include "Device.h"

class QTimer;
class QSerialPort;

class TargetDevice : public Device
{
    Q_OBJECT

public:
    explicit TargetDevice(QObject *parent = 0);

public:
    void open();

protected:
    void measure();

private:
    enum State
    {
        IdleState,
        TriggerState,
        WaitState
    };

    struct ShpRequest
    {
        ShpRequest();

        QByteArray serialize(bool request = false) const;

        quint8 threshold1;
        quint8 threshold2;
        quint8 coupling1;
        quint8 coupling2;
        quint16 burst;
        quint16 duration;
        quint8 counterEvent;
        quint8 timerClock;
        quint8 startEdge;
        quint8 stopEdge;
    };

    struct ShpResponse
    {
        enum
        {
            IdleState,
            ReadyState,
            TriggerState,
            Busy1State,
            Busy2State,
            Busy3State,
            Calibration1State,
            Calibration2State
        };

        ShpResponse();

        bool deserialize(const QByteArray &data);

        quint16 state;
        quint16 voltage;
        quint32 counter;
        quint32 timer;
        quint8 startDivident;
        quint8 startDivider;
        quint8 stopDivident;
        quint8 stopDivider;
    };

private:
    quint8 checksum(const QByteArray &data);

    void open1();
    void close1();

    void read(const QByteArray &data);

    void write(const QByteArray &data);

    void onReadyRead();

    void onTimeout();

private:
    QTimer * const m_timer;
    QSerialPort * const m_port;
    State m_state;
};

#endif // TARGETDEVICE_H
