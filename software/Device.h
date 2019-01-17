#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "Sample.h"
#include "ChannelOptions.h"
#include "ControlOptions.h"

class QTimer;
class QSerialPort;

class Device : public QObject
{
    Q_OBJECT

public:
    static quint16 vendorIdentifier();
    static quint16 productIdentifier();

public:
    explicit Device(QObject *parent = 0);

    virtual bool isDeviceConnected() const = 0;
    virtual void connectToDevice(const QString &name) = 0;
    virtual void startSampling() = 0;
    virtual void disconnectFromDevice() = 0;

    void setCh1Options(const ChannelOptions &options);
    void setCh2Options(const ChannelOptions &options);
    void setOptions(const ControlOptions &options);

signals:
    void connectionStateChanged(bool connected);
    void samplingFinished(const Sample &sample);

protected:
    ChannelOptions m_ch1Options;
    ChannelOptions m_ch2Options;
    ControlOptions m_options;
};

class HardwareDevice : public Device
{
    Q_OBJECT

public:
    explicit HardwareDevice(QObject *parent = 0);

    bool isDeviceConnected() const;
    void connectToDevice(const QString &name);
    void startSampling();
    void disconnectFromDevice();

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
    static quint8 checksum(const QByteArray &data);

private:
    void onReadyRead();
    void onTimeout();
    void read(const QByteArray &data);
    void write(const QByteArray &data);
    void finish(const Sample &sample);
    bool reset();

private:
    QSerialPort * const m_port;
    QTimer * const m_timer;
    State m_state;
};

class TestDevice : public Device
{
public:
    explicit TestDevice(QObject *parent = 0);

    virtual bool isDeviceConnected() const;
    virtual void connectToDevice(const QString &name);
    virtual void startSampling();
    virtual void disconnectFromDevice();

private:
    void onTimeout();

private:
    QTimer * const m_timer;
    bool m_connected;
};

#endif // DEVICE_H
