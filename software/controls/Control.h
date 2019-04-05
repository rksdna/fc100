#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QJsonValue>

class Control : public QObject
{
    Q_OBJECT

public:
    explicit Control(const QString &name, QObject *parent = 0);

    virtual QJsonValue toJsonValue() const = 0;
    virtual void setJsonValue(const QJsonValue &value) = 0;
};

#endif // CONTROL_H
