#include <QDebug>
#include "Device.h"
#include "Control.h"

Device::Device(QObject *parent)
    : QObject(parent)
{
}

QJsonObject Device::toJsonObject() const
{
    QJsonObject object;
    foreach (Control *control, findChildren<Control *>())
        object.insert(control->objectName(), control->toJsonValue());

    return object;
}

void Device::setJsonObject(const QJsonObject &object)
{
    foreach (Control *control, findChildren<Control *>())
        control->setJsonValue(object.value(control->objectName()));
}

