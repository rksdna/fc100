#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent)
    : QToolButton(parent)
{
    //setMinimumSize(80, 40);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}
