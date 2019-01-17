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
        MinFunction,
        MaxFunction,
        BandFunction,
        DeviationFunction,
        AverageFunction
    };

public:
    explicit Computer(QObject *parent = 0);

    void clear();
    void process(qreal value);
    qreal toValue(Function function) const;

private:
    qreal m_first;
    qreal m_min;
    qreal m_max;
    qreal m_average;
    qreal m_last;
};

#endif // COMPUTER_H
