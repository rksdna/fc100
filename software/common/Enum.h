#ifndef ENUM_H
#define ENUM_H

#include <QMetaEnum>

template <typename Type>
QVariant fromEnum(Type value)
{
    return QMetaEnum::fromType<Type>().valueToKey(value);
}

template <typename Type>
Type toEnum(const QVariant &variant, Type defaultValue)
{
    bool ok = false;
    const int value = QMetaEnum::fromType<Type>().keyToValue(variant.toString().toLatin1().constData(), &ok);
    return ok ? Type(value) : defaultValue;
}

template <typename Type>
QList<Type> knownEnum()
{
    const QMetaEnum meta = QMetaEnum::fromType<Type>();
    QList<Type> result;
    for (int i = 0; i < meta.keyCount(); i++)
        result.append(Type(meta.value(i)));

    return result;
}

#endif // ENUM_H
