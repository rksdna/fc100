#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}
