#ifndef DEVICEPROCESSOR_H
#define DEVICEPROCESSOR_H

#include <QMap>
#include <QVector>
#include <QObject>

class QJSEngine;
class QSettings;

class DeviceProcessor : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        NoType,
        DutyType,
        TimeType,
        FrequencyType,
        VoltageType,
        CustomType
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

    enum Mode
    {
        SlidingMode,
        CumulativeMode
    };

    Q_ENUM(Mode)

    enum Conversion
    {
        NoConversion,
        ExpressionConversion
    };

    Q_ENUM(Conversion)

public:
    static int minSize();
    static int maxSize();

    static QString toString(Type type, Unit unit);
    static QString toString(qreal value, Type type, Unit unit, int decimals);

public:
    explicit DeviceProcessor(QObject *parent = 0);

    Unit unit(Type type) const;
    void setUnit(Type type, Unit unit);

    int decimals(Type type) const;
    void setDecimals(Type type, int decimals);

    Conversion conversion() const;
    void setConversion(Conversion conversion);

    QString expression() const;
    void setExpression(const QString &conversion);

    int size() const;
    void setSize(int size);

    Mode mode() const;
    void setMode(Mode mode);

    void clear();
    void take(Type type, qreal value, qreal time);

    QString toString(qreal value, Type type) const;

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

signals:
    void conversionChanged(Conversion conversion);

    void timeChanged(const QString &text);
    void absoluteChanged(const QString &text);
    void relativeChanged(const QString &text);

    void minChanged(const QString &text);
    void maxChanged(const QString &text);
    void bandChanged(const QString &text);

    void meanChanged(const QString &text);
    void standartDeviationChanged(const QString &text);

    void countChanged(const QString &text);
    void trendChanged(const QVector<qreal> &trend);
    void barsChanged(const QVector<qreal> &bars);

    void cacheInvalidated();
    void cacheFilled();

private:
    static QString toString(Type type);
    static QString toString(Unit unit, bool prefix);

private:
    Type convertType(Type type) const;
    qreal convertValue(qreal value) const;
    void fixAndRefresh();
    void refresh();

private:
    struct Item
    {
        Item(Unit unit = NoUnit, int decimals = 3);

        Unit unit;
        int decimals;
    };

private:
    QJSEngine * const m_engine;
    QMap<Type, Item> m_formats;
    Conversion m_conversion;
    QString m_expression;
    Mode m_mode;
    int m_size;
    Type m_type;
    qreal m_time;
    qreal m_absolute;
    QVector<qreal> m_buffer;
};

#endif // DEVICEPROCESSOR_H
