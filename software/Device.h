#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QJsonObject>

#include "EnumControl.h"

class Device : public QObject
{
    Q_OBJECT

public:
    explicit Device(QObject *parent = 0);

    QJsonObject toJsonObject() const;
    void setJsonObject(const QJsonObject &object);

private:

};

#endif // DEVICE_H
