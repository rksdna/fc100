#ifndef NUMERICCONTROL_H
#define NUMERICCONTROL_H

#include "Control.h"

class NumericControl : public Control
{
    Q_OBJECT

public:
    NumericControl(const QString &name, int min, int value, int max, QObject *parent = 0);
};

#endif // NUMERICCONTROL_H
