#ifndef TARGETDEVICE_H
#define TARGETDEVICE_H

#include <QTime>
#include "Device.h"
#include "DeviceController.h"
#include "Regs.h"

class QTimer;
class QSerialPort;

class TargetDevice : public Device
{
    Q_OBJECT

public:
    explicit TargetDevice(QObject *parent = 0);

    void reconnect();

    void restart();
    bool isStarted() const;

private:
    enum State
    {
        IdleState,
        CalibrateFullScaleState,
        CalibrateZeroScaleState,
        RunState,


        Ps2State,
        Ps3State,
        Ps4State,
        Burst2State,
        Burst3State,
        Burst4State,
        DropState,

    };

private:
    State proc(State state, int elapsed);
    void write(const QByteArray &data);
    void read();
    quint8 checksum(const QByteArray &data);

private:
    QTimer * const m_timer;
    QSerialPort * const m_port;
    State m_state;
    QTime m_time;
    Regs m_regs;
    bool m_measure;
    int m_tac_start;
    int m_tac_stop;
    int m_duration;
    qreal m_ti;
    //DeviceProcessor::Type m_type;
    DeviceController::Mode m_mode;
};

#endif // TARGETDEVICE_H
