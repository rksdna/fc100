#ifndef DEVICE_H
#define DEVICE_H

#include <QColor>
#include <QObject>

class QSettings;
class DeviceChannel;
class DeviceProcessor;
class DeviceReference;
class DeviceController;

class Device : public QObject
{
    Q_OBJECT

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

    virtual void open() = 0;
    void start();
    void stop();

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

    bool isReady() const;

signals:
    void readyChanged(bool online);

protected:
    virtual void measure() = 0;

    void setReady(bool ready);
    void complete();

private:
    void timeout();
    void clearThenRestart();    

private:
    DeviceReference * const m_reference;
    DeviceChannel * const m_channel1;
    DeviceChannel * const m_channel2;
    DeviceController * const m_controller;
    DeviceProcessor * const m_processor;
    QString m_portName;
    bool m_ready;
};

#endif // DEVICE_H
