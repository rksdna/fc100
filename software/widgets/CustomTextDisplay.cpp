#include <QPainter>
#include <QFontMetrics>
#include "CustomTextDisplay.h"

CustomTextDisplay::CustomTextDisplay(const QString &title, QWidget *parent)
    : CustomDisplay(parent),
      m_title(title),
      m_text(tr("---"))
{
}

QString CustomTextDisplay::title() const
{
    return m_title;
}

void CustomTextDisplay::setTitle(const QString &title)
{
    if (m_title != title)
    {
        m_title = title;
        updateGeometry();
        update();
    }
}

QString CustomTextDisplay::text() const
{
    return m_title;
}

void CustomTextDisplay::setText(const QString &text)
{
    if (m_text != text)
    {
        m_text = text;
        updateGeometry();
        update();
    }
}

void CustomTextDisplay::paintContent(const QRect &content, QPainter &painter) const
{
    const QColor mid = palette().color(QPalette::Mid);

    const int left = content.x();
    const int top = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    painter.setPen(mid);
    painter.drawText(QRect(left, top, width, height), Qt::AlignCenter, m_title);

    painter.setPen(isEnabled() ? color() : mid);
    painter.drawText(QRect(left, top + height, width, height), Qt::AlignCenter, m_text);

}

QSize CustomTextDisplay::contentSize() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    const int width = qMax(metrics.width(m_title), metrics.width(m_text));
    return QSize(width, 2 * height);
}
