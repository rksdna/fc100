#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>

class Computer : public QObject
{
    Q_OBJECT

public:
    enum Function
    {
        ActualFunction,
        SmoothFunction,
        DeviationFunction,
        MinFunction,
        MaxFunction,
        BandFunction
    };

public:
    explicit Computer(QObject *parent = 0);

    void clear();
    void process(qreal value, qreal time);
    qreal valueOf(Function function) const;
    qreal lastTime() const;

private:
    qreal m_first;
    qreal m_min;
    qreal m_max;
    qreal m_smooth;
    qreal m_last;
    qreal m_lastTime;
};

#endif // COMPUTER_H
