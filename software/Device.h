#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QJsonObject>
#include "EnumControl.h"

class Device : public QObject
{
    Q_OBJECT

public:
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    Q_ENUM(Coupling)

public:
    explicit Device(QObject *parent = 0);

    QJsonObject toJsonObject() const;
    void setJsonObject(const QJsonObject &object);

private:
    EnumControl<Coupling> * const m_ch1Coupling;
    EnumControl<Coupling> * const m_ch2Coupling;
};

#endif // DEVICE_H
