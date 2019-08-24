#include <QPaintEvent>
#include <QStyleOption>
#include <QStylePainter>
#include "CustomButton.h"

CustomButton::CustomButton(const QString &title, QWidget *parent)
    : QAbstractButton(parent),
      m_title(title),
      m_color("#A5D785")
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setContentsMargins(10, 10, 10, 10);
}

QColor CustomButton::color() const
{
    return m_color;
}

void CustomButton::setColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
    }
}

QString CustomButton::title() const
{
    return m_title;
}

void CustomButton::setTitle(const QString &title)
{
    if (m_title != title)
    {
        m_title = title;
        updateGeometry();
        update();
    }
}

QSize CustomButton::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomButton::minimumSizeHint() const
{
    return QRect(QPoint(), contentSize()).marginsAdded(contentsMargins()).size();
}

void CustomButton::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionButton option;
    option.initFrom(this);
    option.features = QStyleOptionButton::None;
    if (isDown())
        option.state |= QStyle::State_Sunken;

    painter.drawControl(QStyle::CE_PushButton, option);

    paintContent(contentsRect(), painter);

    event->accept();
}

