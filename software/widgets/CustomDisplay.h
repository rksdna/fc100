#ifndef CUSTOMDISPLAY_H
#define CUSTOMDISPLAY_H

#include <QMap>
#include <QFrame>

class CustomDisplay : public QFrame
{
    Q_OBJECT

public:

    enum Mode
    {
        NormalMode,
        MinMode,
        MaxMode,
        BandMode,
        DeviationMode,
        AverageMode
    };

public:
    explicit CustomDisplay(QWidget *parent = 0);

    void display(const QList<qreal> &data);

    void display1(qreal val);
    void clear();

    int decimals() const;
    void setDecimals(int decimals);

    QString unit() const;
    void setUnit(const QString &unit);

    Mode mode() const;
    void setMode(const Mode &mode);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    QString format(qreal value) const;

    void paintEvent(QPaintEvent *event);

private:
    QMargins m_margins;
    int m_decimals;
    QString m_unit;
    Mode m_mode;

    qreal m_sample;
    qreal m_origin;
    qreal m_min;
    qreal m_max;
    QList<qreal> m_samples;




};

#endif // CUSTOMDISPLAY_H

