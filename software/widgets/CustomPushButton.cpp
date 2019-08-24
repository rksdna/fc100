#include <QPainter>
#include "CustomPushButton.h"

CustomPushButton::CustomPushButton(const QString &title, QWidget *parent)
    : CustomButton(title, parent)
{
}

void CustomPushButton::paintContent(const QRect &content, QPainter &painter) const
{
    const QColor mid = palette().color(QPalette::Mid);
    painter.setPen(isEnabled() ? color() : mid);
    painter.drawText(content, Qt::AlignCenter, title());
}

QSize CustomPushButton::contentSize() const
{
    const QFontMetrics metrics(font());
    return QSize(metrics.width(text()), metrics.height());
}
