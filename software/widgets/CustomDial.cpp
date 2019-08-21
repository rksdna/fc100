#include <QMetaProperty>
#include "CustomDial.h"
#include <QStyleOptionSlider>

CustomDial::CustomDial(const QString &text, QObject *object, const char *property, QWidget *parent)
    : QDial(parent),
      m_text(text),
      m_object(object),
      m_property(property)
{
    setNotchesVisible(true);
    setNotchTarget(5);

    const QMetaObject * const meta = m_object->metaObject();
    const QMetaProperty objectProperty = meta->property(meta->indexOfProperty(m_property));
    if (objectProperty.hasNotifySignal())
    {
        const QMetaMethod notifySlot = staticMetaObject.method(staticMetaObject.indexOfMethod("refresh()"));
        connect(m_object, objectProperty.notifySignal(), this, notifySlot);
    }

    connect(this, &QDial::valueChanged, this, &CustomDial::changeData);
}

QVariant CustomDial::data() const
{
    return m_object->property(m_property);
}

void CustomDial::setData(const QVariant &data)
{
    m_object->setProperty(m_property, data);
}

void CustomDial::refresh()
{
    setValue(data().toInt());
}

void CustomDial::changeData()
{
    setData(value());
}

void CustomDial::initStyleOption(QStyleOptionSlider *option) const
{
    QDial::initStyleOption(option);
}

#include <QStylePainter>
#include <QDebug>
#include <QtMath>

static QPointF calcRadialPos(const QStyleOptionSlider *dial, qreal offset)
{
#define Q_PI 3.1415926

    const int width = dial->rect.width();
    const int height = dial->rect.height();
    const int r = qMin(width, height) / 2;
    const int currentSliderPosition = dial->upsideDown ? dial->sliderPosition : (dial->maximum - dial->sliderPosition);
    qreal a = 0;
    if (dial->maximum == dial->minimum)
        a = Q_PI / 2;
    else if (dial->dialWrapping)
        a = Q_PI * 3 / 2 - (currentSliderPosition - dial->minimum) * 2 * Q_PI
            / (dial->maximum - dial->minimum);
    else
        a = (Q_PI * 8 - (currentSliderPosition - dial->minimum) * 10 * Q_PI
            / (dial->maximum - dial->minimum)) / 6;
    qreal xc = width / 2.0;
    qreal yc = height / 2.0;
    qreal len = r - 4 - 3;
    qreal back = offset * len;
    QPointF pos(QPointF(xc + back * qCos(a), yc - back * qSin(a)));
    return pos;
}


void CustomDial::paintEvent(QPaintEvent *event)
{
  /*  Q_UNUSED(event)

    QStylePainter painter(this);
    QStyleOptionSlider option;
    initStyleOption(&option);

    option.state &= ~QStyle::State_HasFocus ;
    option.state &= ~QStyle::State_Enabled;

    QPalette p;
    option.palette.setColor(QPalette::Button, qRgb(64, 0, 0));
    option.palette.setColor(QPalette::Dark, Qt::yellow);

    painter.drawComplexControl(QStyle::CC_Dial, option);*/

    QPainter painter(this);

    QPainter *p = &painter;
    QStyleOptionSlider opt;
    QStyleOptionSlider *option = &opt;
    initStyleOption(option);


    QPalette pal = option->palette;
        QColor buttonColor = pal.button().color();
        const int width = option->rect.width();
        const int height = option->rect.height();
        const bool enabled = option->state & QStyle::State_Enabled;
        qreal r = qMin(width, height) / 2;
        r -= r/50;
        const qreal penSize = r/20.0;

        p->save();
        p->setRenderHint(QPainter::Antialiasing);

        // Draw notches
        if (option->subControls & QStyle::SC_DialTickmarks) {
            p->setPen(option->palette.dark().color().darker(120));
            //p->drawLines(QStyleHelper::calcLines(option));
        }

        // Cache dial background
      //  BEGIN_STYLE_PIXMAPCACHE(QString::fromLatin1("qdial"));
        p->setRenderHint(QPainter::Antialiasing);

        const qreal d_ = r / 6;
        const qreal dx = option->rect.x() + d_ + (width - 2 * r) / 2 + 1;
        const qreal dy = option->rect.y() + d_ + (height - 2 * r) / 2 + 1;

        QRectF br = QRectF(dx + 0.5, dy + 0.5,
                           int(r * 2 - 2 * d_ - 2),
                           int(r * 2 - 2 * d_ - 2));
        buttonColor.setHsv(buttonColor .hue(),
                           qMin(140, buttonColor .saturation()),
                           qMax(180, buttonColor.value()));
        QColor shadowColor(0, 0, 0, 20);

        if (enabled) {
            // Drop shadow
            qreal shadowSize = qMax(1.0, penSize/2.0);
            QRectF shadowRect= br.adjusted(-2*shadowSize, -2*shadowSize,
                                           2*shadowSize, 2*shadowSize);
            QRadialGradient shadowGradient(shadowRect.center().x(),
                                           shadowRect.center().y(), shadowRect.width()/2.0,
                                           shadowRect.center().x(), shadowRect.center().y());
            shadowGradient.setColorAt(qreal(0.91), QColor(0, 0, 0, 40));
            shadowGradient.setColorAt(qreal(1.0), Qt::transparent);
            p->setBrush(shadowGradient);
            p->setPen(Qt::NoPen);
            p->translate(shadowSize, shadowSize);
            p->drawEllipse(shadowRect);
            p->translate(-shadowSize, -shadowSize);

            // Main gradient
            QRadialGradient gradient(br.center().x() - br.width()/3, dy,
                                     br.width()*1.3, br.center().x(),
                                     br.center().y() - br.height()/2);
            gradient.setColorAt(0, buttonColor.lighter(110));
            gradient.setColorAt(qreal(0.5), buttonColor);
            gradient.setColorAt(qreal(0.501), buttonColor.darker(102));
            gradient.setColorAt(1, buttonColor.darker(115));
            p->setBrush(gradient);
        } else {
            p->setBrush(Qt::NoBrush);
        }

        p->setPen(QPen(buttonColor.darker(280)));
        p->drawEllipse(br);
        p->setBrush(Qt::NoBrush);
        p->setPen(buttonColor.lighter(110));
        p->drawEllipse(br.adjusted(1, 1, -1, -1));

        if (option->state & QStyle::State_HasFocus) {
            QColor highlight = pal.highlight().color();
            highlight.setHsv(highlight.hue(),
                             qMin(160, highlight.saturation()),
                             qMax(230, highlight.value()));
            highlight.setAlpha(127);
            p->setPen(QPen(highlight, 2.0));
            p->setBrush(Qt::NoBrush);
            p->drawEllipse(br.adjusted(-1, -1, 1, 1));
        }

        //END_STYLE_PIXMAPCACHE

        QPointF dp = calcRadialPos(option, qreal(0.70));
        buttonColor = buttonColor.lighter(104);
        buttonColor.setAlphaF(qreal(0.8));
        const qreal ds = r/qreal(7.0);
        QRectF dialRect(dp.x() - ds, dp.y() - ds, 2*ds, 2*ds);
        QRadialGradient dialGradient(dialRect.center().x() + dialRect.width()/2,
                                     dialRect.center().y() + dialRect.width(),
                                     dialRect.width()*2,
                                     dialRect.center().x(), dialRect.center().y());
        dialGradient.setColorAt(1, buttonColor.darker(140));
        dialGradient.setColorAt(qreal(0.4), buttonColor.darker(120));
        dialGradient.setColorAt(0, buttonColor.darker(110));
        if (penSize > 3.0) {
            p->setPen(QPen(QColor(0, 0, 0, 25), penSize));
            p->drawLine(calcRadialPos(option, qreal(0.90)), calcRadialPos(option, qreal(0.96)));
        }

        p->setBrush(dialGradient);
        p->setPen(QColor(255, 255, 255, 150));
        p->drawEllipse(dialRect.adjusted(-1, -1, 1, 1));
        p->setPen(QColor(0, 0, 0, 80));
        p->drawEllipse(dialRect);
        p->restore();


}
