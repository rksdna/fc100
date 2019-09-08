#ifndef SAMPLE_H
#define SAMPLE_H

#include <QApplication>

class QSettings;

class Sample
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(Sample)

public:
    enum Type
    {
        NoType,
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
        NoUnit = 0,
        KiloUnit = 3,
        MegaUnit = 6,
        GigaUnit = 9
    };

    Q_ENUM(Unit)

public:
    class Format
    {
    public:
        Unit unit(Type type) const;
        void setUnit(Type type, Unit unit);

        int decimals(Type type) const;
        void setDecimals(Type type, int decimals);

    public:
        void saveToSettings(QSettings &settings) const;
        void restoreFromSettings(QSettings &settings);

    private:
        struct Item
        {
            Item(Unit unit = NoUnit, int decimals = 3);

            void saveToSettings(QSettings &settings) const;
            void restoreFromSettings(QSettings &settings);

            Unit unit;
            int decimals;
        };

    private:
        QMap<Type, Item> m_items;
    };

public:
    static QString toString(Type type);
    static QString toString(Unit unit);
    static QString toString(Type type, Unit unit);
    static QString toString(qreal value, Type type, Unit unit, int decimals);
    static QString toString(qreal value, Type type);

public:
    static Format *format();

public:
    explicit Sample(Type type, qreal value = 0);

    Type type() const;
    void setType(Type type);

    qreal value() const;
    void setValue(qreal value);

    QString toString() const;
    QString toString(Unit unit, int decimals) const;

private:
    Type m_type;
    qreal m_value;
};

#endif // SAMPLE_H
