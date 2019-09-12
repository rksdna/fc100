#include <QDebug>
#include <QTimer>
#include <QSettings>
#include "Device.h"
#include "MockDevice.h"
#include "TargetDevice.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"
#include "DeviceProcessor.h"
#include "DeviceController.h"

Device *Device::createDevice(const QString &type, QObject *parent)
{
    if (type == "mock")
        return new MockDevice(parent);

    return new TargetDevice(parent);
}

Device::Device(QObject *parent)
    : QObject(parent),
      m_timer(new QTimer(this)),
      m_reference(new DeviceReference(this)),
      m_channel1(new DeviceChannel(this)),
      m_channel2(new DeviceChannel(this)),
      m_controller(new DeviceController(this)),
      m_processor(new DeviceProcessor(this)),
      m_ready(false),
      m_measure(false),
      m_delay(false)
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Device::timeout);

    connect(m_controller, &DeviceController::controlInvalidated, this, &Device::clearThenRestart);

    connect(m_processor, &DeviceProcessor::cacheInvalidated, this, &Device::clearThenRestart);
    connect(m_processor, &DeviceProcessor::cacheFilled, this, &Device::stop);
}

DeviceReference *Device::reference() const
{
    return m_reference;
}

DeviceChannel *Device::channel1() const
{
    return m_channel1;
}

DeviceChannel *Device::channel2() const
{
    return m_channel2;
}

DeviceController *Device::controller() const
{
    return m_controller;
}

DeviceProcessor *Device::processor() const
{
    return m_processor;
}

QString Device::portName() const
{
    return m_portName;
}

void Device::setPortName(const QString &name)
{
    if (m_portName != name)
    {
        m_portName = name;

        open();
    }
}

void Device::start()
{
    m_measure = true;
    m_delay = true;
    m_timer->start(m_controller->duration());
    measure();
}

void Device::stop()
{
    m_controller->setTrigger(DeviceController::ManualTrigger);
}

void Device::saveToSettings(QSettings &settings) const
{
    settings.setValue("PortName", portName());

    settings.beginGroup("Reference");
    m_reference->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel1");
    m_channel1->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Controller");
    m_controller->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Processor");
    m_processor->saveToSettings(settings);
    settings.endGroup();
}

void Device::restoreFromSettings(QSettings &settings)
{
    setPortName(settings.value("PortName").toString());

    settings.beginGroup("Reference");
    m_reference->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel1");
    m_channel1->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Controller");
    m_controller->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Processor");
    m_processor->restoreFromSettings(settings);
    settings.endGroup();
}

void Device::complete()
{
    m_measure = false;
    if (m_controller->trigger() == DeviceController::AutoTrigger && !m_delay)
        start();
}

void Device::timeout()
{
    m_delay = false;
    if (m_controller->trigger() == DeviceController::AutoTrigger && !m_measure)
        start();
}

void Device::clearThenRestart()
{
    m_processor->clear();

    if (m_measure)
        start();
}

bool Device::isReady() const
{
    return m_ready;
}

void Device::setReady(bool ready)
{
    if (m_ready != ready)
    {
        m_ready = ready;
        emit readyChanged(m_ready);
    }
}


