#include <QtMath>
#include <QPaintEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QStyleOption>
#include <QStylePainter>
#include "CustomDial.h"

CustomDial::CustomDial(const QString &text, QWidget *parent)
    : QAbstractSlider(parent),
      m_text(text),
      m_description(tr("---")),
      m_margins(10, 10, 10, 0),
      m_clearance(10),
      m_thickness(4),
      m_min(-M_PI * 0.8),
      m_max(M_PI * 0.8)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QString CustomDial::description() const
{
    return m_description;
}

void CustomDial::setDescription(const QString &description)
{
    if (description != m_description)
    {
        m_description = description;
        updateGeometry();
        update();
    }
}

QSize CustomDial::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomDial::minimumSizeHint() const
{
    const QFontMetrics metrics(font());
    const int height = metrics.height();
    const int width = qMax(metrics.width(m_text), metrics.width(m_description));
    const int size = M_SQRT2 * qMax(height, width) + 2 * m_clearance;

    const QRect content = QRect(0, 0, size, size).marginsAdded(m_margins);

    QStyleOptionButton option;
    initStyleOption(&option);
    return style()->sizeFromContents(QStyle::CT_PushButton, &option, content.size(), this).expandedTo(QApplication::globalStrut());
}

void CustomDial::mousePressEvent(QMouseEvent *event)
{
    QAbstractSlider::mousePressEvent(event);

    setSliderDown(true);
    setSliderPosition(pointToValue(event->pos() - rect().center()));
}

void CustomDial::mouseMoveEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseMoveEvent(event);

    if (isSliderDown())
        setSliderPosition(pointToValue(event->pos() - rect().center()));
}

void CustomDial::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseReleaseEvent(event);

    setSliderPosition(pointToValue(event->pos() - rect().center()));
    setSliderDown(false);
}

void CustomDial::enterEvent(QEvent *event)
{
    QAbstractSlider::enterEvent(event);
    update();
}

void CustomDial::leaveEvent(QEvent *event)
{
    QAbstractSlider::leaveEvent(event);
    update();
}

void CustomDial::paintEvent(QPaintEvent *event)
{
    QStyleOptionButton option;
    initStyleOption(&option);

    const QColor textColor = palette().color(QPalette::Mid);
    const QColor valueColor = QColor("#A5D785");
    const QColor frameColor = palette().color(QPalette::Dark);

    const QRect rect = style()->subElementRect(QStyle::SE_PushButtonContents, &option, this).marginsRemoved(m_margins);
    const int size = qMin(rect.width(), rect.height());

    QRect groove(0, 0, size, size);
    groove.moveCenter(rect.center());

    const int begin = 16 * (90 - 180 * valueToAngle(minimum()) / M_PI);
    const int position = 16 * (90 - 180 * valueToAngle(value()) / M_PI);
    const int end = 16 * (90 - 180 * valueToAngle(maximum()) / M_PI);

    const QRect content = groove.adjusted(m_clearance, m_clearance, -m_clearance, -m_clearance);
    const int x = content.x();
    const int y = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    QStylePainter painter(this);

    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawControl(QStyle::CE_PushButton, option);

    painter.setFont(font());
    painter.setPen(QPen(frameColor, m_thickness, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(groove, begin, end - begin);

    painter.setPen(textColor);
    painter.drawText(QRect(x, y, width, height), Qt::AlignHCenter | Qt::AlignBottom, m_text);
    painter.drawLine(x, y + height, x + width, y + height);

    painter.setPen(QPen(valueColor, m_thickness, Qt::SolidLine, Qt::RoundCap));
    painter.drawText(QRect(x, y + height, width, height), Qt::AlignHCenter | Qt::AlignTop, m_description);
    painter.drawArc(groove, begin, position - begin);

    event->accept();
}

void CustomDial::initStyleOption(QStyleOptionButton *option) const
{
    option->initFrom(this);
    option->features = QStyleOptionButton::None;
}

qreal CustomDial::pointToAngle(const QPoint &point) const
{
    return qAtan2(QPoint::dotProduct(point, QPoint(1, 0)), QPoint::dotProduct(point, QPoint(0, -1)));
}

int CustomDial::pointToValue(const QPoint &pos) const
{
    return minimum() + (maximum() - minimum()) * (pointToAngle(pos) - m_min) / (m_max - m_min);
}

qreal CustomDial::valueToAngle(int value) const
{
    return m_min + (m_max - m_min) * (value - minimum()) / (maximum() - minimum());
}

QPoint CustomDial::valueToPoint(int value, int radius) const
{
    const qreal angle = valueToAngle(value);
    return QPoint(radius * qSin(angle), -radius * qCos(angle));
}
