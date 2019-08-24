#ifndef DEVICEPROCESSOR_H
#define DEVICEPROCESSOR_H

#include "Format.h"

class DeviceProcessor : public QObject
{
    Q_OBJECT

public:
    explicit DeviceProcessor(QObject *parent = 0);

    Format *countFormat() const;
    Format *timeFormat() const;
    Format *frequencyFormat() const;
    Format *userFormat() const;

    void clear();
    void take(Format::Type type, qreal value);

signals:

private:
    Format * const m_countFormat;
    Format * const m_timeFormat;
    Format * const m_frequencyFormat;
    Format * const m_userFormat;
    Format::Type m_type;
};

#endif // DEVICEPROCESSOR_H
