#include "Dial.h"

Dial::Dial(QWidget *parent)
    : QDial(parent)
{
    //setMinimumSize(60, 60);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}
