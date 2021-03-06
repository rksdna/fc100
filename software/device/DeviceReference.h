#ifndef DEVICEREFERENCE_H
#define DEVICEREFERENCE_H

#include <QObject>

class QSettings;

class DeviceReference : public QObject
{
    Q_OBJECT

public:
    enum Source
    {
        InternalSource,
        ExternalSource
    };

    Q_ENUM(Source)

public:
    static qreal minFrequency();
    static qreal maxFrequency();

public:
    explicit DeviceReference(QObject *parent = 0);

    Source source() const;
    void setSource(Source source);

    qreal frequency() const;
    void setFrequency(qreal frequency);

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

private:
    Source m_source;
    qreal m_frequency;
};

#endif // DEVICEREFERENCE_H
