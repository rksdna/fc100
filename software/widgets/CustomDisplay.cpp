#include <QPainter>
#include <QPaintEvent>
#include "CustomDisplay.h"

CustomDisplay::CustomDisplay(QWidget *parent)
    : QWidget(parent),
      m_color("#A5D785")
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setContentsMargins(10, 10, 10, 10);
}

QColor CustomDisplay::color() const
{
    return m_color;
}

void CustomDisplay::setColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
    }
}

QSize CustomDisplay::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomDisplay::minimumSizeHint() const
{
    return QRect(QPoint(), contentSize()).marginsAdded(contentsMargins()).size();
}

void CustomDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    paintContent(contentsRect(), painter);

    event->accept();
}
