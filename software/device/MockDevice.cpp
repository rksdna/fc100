#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QDateTime>
#include "MockDevice.h"
#include "DeviceController.h"

MockDevice::MockDevice(QObject *parent)
    : Device(parent),
      m_timer(new QTimer(this))
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &MockDevice::done);
}

void MockDevice::open()
{
    setReady(false);
    setReady(true);
}

void MockDevice::measure()
{
    const int ms = qrand() % (2 * controller()->duration());
    m_timer->start(ms);
    m_time.restart();
}

void MockDevice::done()
{
    const qreal time = 0.001 * QTime::currentTime().msecsSinceStartOfDay();
    const bool v = qrand() % 100 < 90 ? true : false;
    const qreal vv = 5000 * (1 + qSin(2 * M_PI * 1 * time));
    complete(v ? vv : qQNaN());
}
