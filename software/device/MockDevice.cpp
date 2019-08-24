#include <QtMath>
#include <QDebug>
#include <QDateTime>
#include "MockDevice.h"

MockDevice::MockDevice(QObject *parent)
    : Device(parent),
      m_timer(new QTimer(this))
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &MockDevice::done);
}

void MockDevice::measure()
{
    const int ms = qrand() % (2 * duration());
    m_timer->start(ms);
    m_time.restart();
}

void MockDevice::done()
{
    const qreal time = 0.001 * QTime::currentTime().msecsSinceStartOfDay();
    const bool v = qrand() % 100 < 90 ? true : false;
    const qreal vv = 5000 * (1 + qSin(2 * M_PI * 0.1 * time));
    complete(v ? vv : qQNaN());
}
