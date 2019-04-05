#include "Device.h"
#include "Control.h"

Control::Control(const QString &name, QObject *parent)
    : QObject(parent),
      m_enabled(false)
{
    setObjectName(name);
}

bool Control::isEnabled() const
{
    return m_enabled;
}

void Control::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

