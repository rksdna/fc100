#include "EnumControl.h"

CommonEnumControl::CommonEnumControl(const QString &name, const QMetaEnum &meta, QObject *parent)
    : Control(name, parent),
      m_meta(meta),
      m_value(meta.value(0))
{
}

QJsonValue CommonEnumControl::toJsonValue() const
{
    return m_meta.valueToKey(m_value);
}

void CommonEnumControl::setJsonValue(const QJsonValue &value)
{
    bool ok = false;
    const int tmp = m_meta.keyToValue(value.toString().toUtf8(), &ok);
    m_value = ok ? tmp : m_meta.value(0);
}

int CommonEnumControl::value1() const
{
    return m_value;
}

void CommonEnumControl::setValue1(int value)
{
    m_value = value;
}

