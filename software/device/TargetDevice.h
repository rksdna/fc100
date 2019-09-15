#ifndef TARGETDEVICE_H
#define TARGETDEVICE_H

#include <QTime>
#include "Device.h"
#include "DeviceProcessor.h"
#include "Regs.h"

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
        CalibrateFullScaleState,
        CalibrateZeroScaleState,

        Ps1State,
        Ps2State,
        Ps3State,
        Ps4State,

        Single1State,
        Burst0State,
        Burst1State,

        Burst2State,
        Burst3State,
        Burst4State
    };

private:
    QString preparePortName(const QString &name) const;

    State fail() const;
    State done() const;

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
    DeviceProcessor::Type m_type;
};

#endif // TARGETDEVICE_H
