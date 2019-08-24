#include <QtMath>
#include <QPaintEvent>
#include <QFontMetrics>
#include <QStyleOption>
#include <QStylePainter>
#include "CustomDial.h"

CustomDial::CustomDial(const QString &title, QWidget *parent)
    : QAbstractSlider(parent),
      m_color("#A5D785"),
      m_clearance(10),
      m_thickness(4),
      m_minAngle(-M_PI * 0.75),
      m_maxAngle(M_PI * 0.75),
      m_title(title),
      m_text(tr("---"))
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setContentsMargins(10, 10, 10, 10);
}

QColor CustomDial::color() const
{
    return m_color;
}

void CustomDial::setColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
    }
}

int CustomDial::clearance() const
{
    return m_clearance;
}

void CustomDial::setClearance(int clearance)
{
    if (m_clearance != clearance)
    {
        m_clearance = clearance;
        updateGeometry();
        update();
    }
}

int CustomDial::thickness() const
{
    return m_thickness;
}

void CustomDial::setThickness(int thickness)
{
    if (m_thickness != thickness)
    {
        m_thickness = thickness;
        update();
    }
}
qreal CustomDial::minAngle() const
{
    return m_minAngle;
}

qreal CustomDial::maxAngle() const
{
    return m_maxAngle;
}

void CustomDial::setAngleRange(qreal min, qreal max)
{
    if (m_minAngle != min || m_maxAngle != max)
    {
        m_minAngle = min;
        m_maxAngle = max;
        update();
    }
}

QString CustomDial::title() const
{
    return m_title;
}

void CustomDial::setTitle(const QString &title)
{
    if (m_title != title)
    {
        m_title = title;
        updateGeometry();
        update();
    }
}

QString CustomDial::text() const
{
    return m_text;
}

void CustomDial::setText(const QString &text)
{
    if (m_text != text)
    {
        m_text = text;
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
    const int width = qMax(metrics.width(m_title), metrics.width(m_text));
    const int size = M_SQRT2 * qMax(height, width) + 2 * m_clearance;

    return QRect(0, 0, size, size).marginsAdded(contentsMargins()).size();
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
    const QColor mid = palette().color(QPalette::Mid);
    const QColor dark = palette().color(QPalette::Dark);

    const QRect rect = contentsRect();
    const int size = qMin(rect.width(), rect.height());

    QRect groove(0, 0, size, size);
    groove.moveCenter(rect.center());

    const int begin = 16 * (90 - 180 * valueToAngle(minimum()) / M_PI);
    const int position = 16 * (90 - 180 * valueToAngle(value()) / M_PI);
    const int end = 16 * (90 - 180 * valueToAngle(maximum()) / M_PI);

    const QRect content = groove.adjusted(m_clearance, m_clearance, -m_clearance, -m_clearance);
    const int left = content.x();
    const int top = content.y();
    const int height = content.height() / 2;
    const int width = content.width();

    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionButton option;
    option.initFrom(this);
    option.features = QStyleOptionButton::None;

    painter.drawControl(QStyle::CE_PushButton, option);

    painter.setPen(QPen(dark, m_thickness, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(groove, begin, end - begin);

    painter.setPen(mid);
    painter.drawText(QRect(left, top, width, height), Qt::AlignHCenter | Qt::AlignBottom, m_title);

    painter.setPen(QPen(isEnabled() ? color() : mid, m_thickness, Qt::SolidLine, Qt::RoundCap));
    painter.drawText(QRect(left, top + height, width, height), Qt::AlignHCenter | Qt::AlignTop, m_text);
    painter.drawArc(groove, begin, position - begin);

    event->accept();
}

qreal CustomDial::pointToAngle(const QPoint &point) const
{
    return qAtan2(QPoint::dotProduct(point, QPoint(1, 0)), QPoint::dotProduct(point, QPoint(0, -1)));
}

int CustomDial::pointToValue(const QPoint &pos) const
{
    return minimum() + (maximum() - minimum()) * (pointToAngle(pos) - m_minAngle) / (m_maxAngle - m_minAngle);
}

qreal CustomDial::valueToAngle(int value) const
{
    return m_minAngle + (m_maxAngle - m_minAngle) * (value - minimum()) / (maximum() - minimum());
}

QPoint CustomDial::valueToPoint(int value, int radius) const
{
    const qreal angle = valueToAngle(value);
    return QPoint(radius * qSin(angle), -radius * qCos(angle));
}







