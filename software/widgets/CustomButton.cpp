#include <QPaintEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QStyleOption>
#include <QStylePainter>
#include "CustomButton.h"

CustomButton::CustomButton(const QString &text, QWidget *parent)
    : QAbstractButton(parent),
      m_margins(10, 5, 10, 5),
      m_value(-1)
{
    setText(text);
    setFocusPolicy(Qt::StrongFocus);
    connect(this, &QAbstractButton::clicked, this, &CustomButton::changeValue);
}

int CustomButton::value() const
{
    return m_value;
}

void CustomButton::setValue(int value)
{
    if (m_value != value)
    {
        m_value = value;
        update();

        emit valueChanged(m_value);
    }
}

void CustomButton::addValue(const QString &text, int value)
{
    m_items.insert(value, text);
}

QSize CustomButton::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomButton::minimumSizeHint() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    int width = metrics.width(text());
    foreach (const QString &text, m_items)
        width = qMax(width, metrics.width(text));

    const QRect content = QRect(0, 0, width, 2 * height).marginsAdded(m_margins);

    QStyleOptionButton option;
    initStyleOption(&option);
    return style()->sizeFromContents(QStyle::CT_PushButton, &option, content.size(), this).expandedTo(QApplication::globalStrut());
}

void CustomButton::paintEvent(QPaintEvent *event)
{
    QStyleOptionButton option;
    initStyleOption(&option);

    const QColor textColor = palette().color(QPalette::Mid);
    const QColor valueColor = QColor("#A5D785");

    const QRect content = style()->subElementRect(QStyle::SE_PushButtonContents, &option, this).marginsRemoved(m_margins);
    const int x = content.x();
    const int y = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawControl(QStyle::CE_PushButton, option);

    painter.setFont(font());

    painter.setPen(textColor);
    painter.drawText(QRect(x, y, width, height), Qt::AlignCenter, text());
    painter.drawLine(x, y + height, x + width, y + height);

    painter.setPen(valueColor);
    painter.drawText(QRect(x, y + height, width, height), Qt::AlignCenter, m_items.value(m_value, tr("???")));

    event->accept();
}

void CustomButton::initStyleOption(QStyleOptionButton *option) const
{
    option->initFrom(this);
    option->features = QStyleOptionButton::None;
    option->state |= isDown() ? QStyle::State_Sunken : QStyle::State_None;
}

void CustomButton::changeValue()
{
    const QList<int> values = m_items.keys();
    setValue(values.at((values.indexOf(m_value) + 1) % values.count()));
}

