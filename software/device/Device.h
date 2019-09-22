#ifndef DEVICE_H
#define DEVICE_H

#include <QColor>
#include <QObject>

class QTimer;
class QSettings;
class QSerialPort;
class DeviceChannel;
class DeviceProcessor;
class DeviceReference;
class DeviceController;

class Device : public QObject
{
    Q_OBJECT

public:
    explicit Device(QObject *parent = 0);

    DeviceReference *reference() const;
    DeviceChannel *channel1() const;
    DeviceChannel *channel2() const;
    DeviceController *controller() const;
    DeviceProcessor *processor() const;

    QString portName() const;
    void setPortName(const QString &name);

    void open();
    void close();
    bool isReady() const;

    void clear();
    void start();
    void stop();

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

signals:
    void readyChanged(bool state);

private:
    enum State
    {
        IdleState,
        TriggerState,
        BusyState
    };

    enum Command
    {
        PollCommand,
        MeasureTimeCommand,
        MeasureFrequencyCommand,
        MeasureDutyCommand,
        MeasureGateFrequencyCommand
    };

    enum Status
    {
        FailStatus,
        DoneStatus,
        BusyStatus
    };

private:
    QString autoPortName() const;
    void setReady(bool state);

    void process(const QByteArray &replyData);
    void read();
    void write(const QByteArray &data);
    void get(quint8 data);
    void put(quint8 data);
    void flush();

    quint8 checksum(const QByteArray &data);

private:
    QTimer * const m_timer;
    QSerialPort * const m_port;
    DeviceReference * const m_reference;
    DeviceChannel * const m_channel1;
    DeviceChannel * const m_channel2;
    DeviceController * const m_controller;
    DeviceProcessor * const m_processor;
    QString m_portName;
    QByteArray m_inbox;
    QByteArray m_outbox;
    bool m_ready;
    bool m_escape;
    State m_state;
};

#endif // DEVICE_H
