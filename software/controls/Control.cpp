#include "Device.h"
#include "Control.h"

Control::Control(const QString &name, QObject *parent)
    : QObject(parent)
{
    setObjectName(name);
}

