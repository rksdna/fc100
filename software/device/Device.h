#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class QTimer;
class QSettings;

class DeviceChannel;
class DeviceProcessor;
class DeviceReference;
class DeviceController;

class Device : public QObject
{
    Q_OBJECT

public:



public:
    static Device *createDevice(const QString &type, QObject *parent = 0);

public:
    explicit Device(QObject *parent = 0);

    DeviceReference *reference() const;
    DeviceChannel *channel1() const;
    DeviceChannel *channel2() const;
    DeviceController *controller() const;
    DeviceProcessor *processor() const;

    QString portName() const;
    void setPortName(const QString &name);

    void start();
    void stop();

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

protected:
    virtual void measure() = 0;
    void complete(qreal sample);

private:
    void timeout();
    void clearThenRestart();

private:
    QTimer * const m_timer;
    DeviceReference * const m_reference;
    DeviceChannel * const m_channel1;
    DeviceChannel * const m_channel2;
    DeviceController * const m_controller;
    DeviceProcessor * const m_processor;
    QString m_portName;
    bool m_measure;
    bool m_delay;
};

#endif // DEVICE_H
