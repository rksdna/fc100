#include <QPainter>
#include <QPaintEvent>
#include <QFontMetrics>
#include "CustomDisplay.h"

CustomDisplay::CustomDisplay(const QString &title, QWidget *parent)
    : QWidget(parent),
      m_color("#A5D785"),
      m_title(title),
      m_text(tr("---"))
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setContentsMargins(10, 5, 10, 5);
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

QString CustomDisplay::title() const
{
    return m_title;
}

void CustomDisplay::setTitle(const QString &title)
{
    if (m_title != title)
    {
        m_title = title;
        updateGeometry();
        update();
    }
}

QString CustomDisplay::text() const
{
    return m_title;
}

void CustomDisplay::setText(const QString &text)
{
    if (m_text != text)
    {
        m_text = text;
        updateGeometry();
        update();
    }
}

QSize CustomDisplay::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomDisplay::minimumSizeHint() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    const int width = qMax(metrics.width(m_title), metrics.width(m_text));
    return QRect(0, 0, width, 2 * height).marginsAdded(contentsMargins()).size();
}

void CustomDisplay::paintEvent(QPaintEvent *event)
{
    const QColor mid = palette().color(QPalette::Mid);

    const QRect content = contentsRect();
    const int left = content.x();
    const int top = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    QPainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(mid);
    painter.drawText(QRect(left, top, width, height), Qt::AlignCenter, m_title);

    painter.setPen(isEnabled() ? color() : mid);
    painter.drawText(QRect(left, top + height, width, height), Qt::AlignCenter, m_text);

    event->accept();
}
