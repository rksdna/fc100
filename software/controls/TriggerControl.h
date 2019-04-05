#ifndef TRIGGERCONTROL_H
#define TRIGGERCONTROL_H

#include "Control.h"

class TriggerControl : public Control
{
    Q_OBJECT

public:
    explicit TriggerControl(const QString &name, QObject *parent = 0);
};

#endif // TRIGGERCONTROL_H
