#include <QPainter>
#include "CustomOptionButton.h"

CustomOptionButton::CustomOptionButton(const QString &title, QWidget *parent)
    : CustomButton(title, parent),
      m_value(-1)
{
    connect(this, &QAbstractButton::clicked, this, &CustomOptionButton::changeValue);
}

void CustomOptionButton::addValueOption(const QString &text, int value)
{
    m_options.insert(value, text);
    updateGeometry();
    update();
}

int CustomOptionButton::value() const
{
    return m_value;
}

void CustomOptionButton::setValue(int value)
{
    if (m_value != value)
    {
        m_value = value;
        emit valueChanged(m_value);
        update();
    }
}

void CustomOptionButton::paintContent(const QRect &content, QPainter &painter) const
{
    const QColor mid = palette().color(QPalette::Mid);
    const int left = content.x();
    const int top = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    painter.setPen(mid);
    painter.drawText(QRect(left, top, width, height), Qt::AlignCenter, title());
    painter.drawLine(left, top + height, left + width, top + height);

    painter.setPen(isEnabled() ? color() : mid);
    painter.drawText(QRect(left, top + height, width, height), Qt::AlignCenter, isEnabled() ? m_options.value(m_value, tr("---")) : tr("---"));
}

QSize CustomOptionButton::contentSize() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    int width = metrics.width(title());
    foreach (const QString &text, m_options)
        width = qMax(width, metrics.width(text));

    return QSize(width, 2 * height);
}

void CustomOptionButton::changeValue()
{
    const QList<int> values = m_options.keys();
    setValue(values.at((values.indexOf(m_value) + 1) % values.count()));
}
