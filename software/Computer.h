#ifndef COMPUTER_H
#define COMPUTER_H

#include <QtGlobal>

class Computer
{
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
    Computer();

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
