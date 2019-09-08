#ifndef DEVICEPROCESSOR_H
#define DEVICEPROCESSOR_H

#include <QVector>
#include <QObject>

class QSettings;

class DeviceProcessor : public QObject
{
    Q_OBJECT

public:

public:
    explicit DeviceProcessor(QObject *parent = 0);

    void clear();
    void take(qreal value);

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

signals:
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

private:
    void updateCache(qreal absolute);

private:
    int m_size;
    bool m_sliding;

    QVector<qreal> m_samples;
};

#endif // DEVICEPROCESSOR_H
