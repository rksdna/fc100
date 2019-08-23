#include <QPaintEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QStyleOption>
#include <QStylePainter>
#include "CommandButton.h"

CommandButton::CommandButton(const QString &text, QWidget *parent)
    : QAbstractButton(parent),
      m_margins(10, 5, 10, 5)
{
    setText(text);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize CommandButton::sizeHint() const
{
    return minimumSizeHint();
}

QSize CommandButton::minimumSizeHint() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    const int width = metrics.width(text());
    const QRect content = QRect(0, 0, width, height).marginsAdded(m_margins);

    QStyleOptionButton option;
    initStyleOption(&option);
    return style()->sizeFromContents(QStyle::CT_PushButton, &option, content.size(), this).expandedTo(QApplication::globalStrut());
}

void CommandButton::paintEvent(QPaintEvent *event)
{
    QStyleOptionButton option;
    initStyleOption(&option);

    const QColor color = palette().color(QPalette::ButtonText);

    const QRect content = style()->subElementRect(QStyle::SE_PushButtonContents, &option, this).marginsRemoved(m_margins);

    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawControl(QStyle::CE_PushButton, option);

    painter.setFont(font());
    painter.setPen(color);
    painter.drawText(content, Qt::AlignCenter, text());

    event->accept();
}

void CommandButton::initStyleOption(QStyleOptionButton *option) const
{
    option->initFrom(this);
    option->features = QStyleOptionButton::None;
    if (isDown())
        option->state |= QStyle::State_Sunken;
}
