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

    int decimals() const;
    void setDecimals(int decimals);

    QString unit() const;
    void setUnit(const QString &unit);

    Mode mode() const;
    void setMode(const Mode &mode);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QMargins m_margins;
    int m_decimals;
    QString m_unit;
    Mode m_mode;

    bool m_hd;
    qreal m_first;
    qreal m_last;
    qreal m_min;
    qreal m_max;
    qreal m_average;
    QMap<int, int> m_hystogram;




};

#endif // CUSTOMDISPLAY_H

