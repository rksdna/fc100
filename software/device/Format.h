#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>

class QSettings;

class Format : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        CountType,
        TimeType,
        FrequencyType,
        UserType,
        VoltageType
    };

    Q_ENUM(Type)

    enum Unit
    {
        NanoUnit = -9,
        MicroUnit = -6,
        MilliUnit = -3,
        OneUnit = 0,
        KiloUnit = 3,
        MegaUnit = 6,
        GigaUnit = 9
    };

    Q_ENUM(Unit)

public:
    static QString formatType(Type type);
    static QString formatUnit(Unit unit);
    static QString suffix(Unit unit, Type type);
    static QString format(qreal value, int decimals, Unit unit, Type type);

    static int minDecimals();
    static int maxDecimals();

public:
    explicit Format(Type type, Unit unit, int decimals, QObject *parent = 0);

    Type type() const;

    Unit unit() const;
    void setUnit(Unit unit);

    int decimals() const;
    void setDecimals(int decimals);

    QString suffix() const;
    QString suffix(Unit unit) const;
    QString format(qreal value) const;

    void saveToSettings(QSettings &settings);
    void restoreFromSettings(QSettings &settings);

signals:
    void unitChanged(Unit unit);
    void decimalsChanged(int decimals);

private:
    const Type m_type;
    Unit m_unit;
    int m_decimals;
};

#endif // FORMAT_H
