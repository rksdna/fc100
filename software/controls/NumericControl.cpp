#include "NumericControl.h"

NumericControl::NumericControl(const QString &name, int min, int value, int max, QObject *parent)
    : Control(name, parent)
{
}

