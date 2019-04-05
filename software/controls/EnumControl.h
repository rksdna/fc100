#ifndef ENUMCONTROL_H
#define ENUMCONTROL_H

#include <QMetaEnum>
#include "Control.h"

class CommonEnumControl : public Control
{
    Q_OBJECT

public:
    CommonEnumControl(const QString &name, const QMetaEnum &meta, QObject *parent = 0);

    QJsonValue toJsonValue() const;
    void setJsonValue(const QJsonValue &value);

protected:
    int value1() const;
    void setValue1(int value);

private:
    const QMetaEnum m_meta;
    int m_value;
};

template<typename Type>
class EnumControl : public CommonEnumControl
{
public:
    EnumControl(const QString &name, QObject *parent = 0)
        : CommonEnumControl(name, QMetaEnum::fromType<Type>(), parent)
    {
    }

    Type value() const
    {
        return Type(value1());
    }

    void setValue(Type value)
    {
        setValue1(int(value));
    }
};

#endif // ENUMCONTROL_H
