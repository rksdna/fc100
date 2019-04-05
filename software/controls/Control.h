#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QJsonValue>

class Control : public QObject
{
    Q_OBJECT

public:
    explicit Control(const QString &name, QObject *parent = 0);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    virtual QJsonValue toJsonValue() const = 0;
    virtual void setJsonValue(const QJsonValue &value) = 0;

private:
    bool m_enabled;
};

#endif // CONTROL_H
