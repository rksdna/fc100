#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "DeviceMode.h"
#include "DeviceSample.h"
#include "DeviceDisplay.h"
#include "DeviceChannel.h"

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
    ~Device();

    bool isDeviceConnected() const;
    void connectToDevice(const QString &name);
    void disconnectFromDevice();

    void startSampling();
    void setChannel1(const DeviceChannel &channel);
    void setChannel2(const DeviceChannel &channel);

    DeviceDisplay *display();
    DeviceMode *mode();

signals:
    void connectionStateChanged(bool connected);
    void samplingFinished(const DeviceSample &sample);

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
        quint8 counterEdge;
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
    bool reset();
    void finish(const DeviceSample &sample);

private:
    QSerialPort * const m_port;
    QTimer * const m_timer;
    State m_state;
    DeviceChannel m_channel1;
    DeviceChannel m_channel2;

    DeviceDisplay m_display;
    DeviceMode m_mode;
};

#endif // DEVICE_H
