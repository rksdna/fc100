#ifndef TARGETDEVICE_H
#define TARGETDEVICE_H

#include <QTime>
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
        T1State,
        T1TState,
        T2State,
        T2TState,
        T3State,
        T4State,
        T5State,
        T6State,
        T7State,
    };


    struct Regs
    {
        Regs();

        QByteArray serialize() const;
        void deserialize(const QByteArray &data);

        quint8 dac1;
        quint8 dac2;
        quint8 mode;
        quint8 ctrl;
        quint8 id;
        quint8 ack;
        quint8 tac_strt;
        quint8 tac_stop;
        quint32 cnt;
        quint32 tmr;
    };

private:
    State proc(State state);
    void hop();
    void write(const QByteArray &data);
    void read();
    quint8 checksum(const QByteArray &data);

private:
    QTimer * const m_timer;
    QSerialPort * const m_port;
    State m_state;
    QTime m_tm;
    Regs m_regs;

};

#endif // TARGETDEVICE_H
