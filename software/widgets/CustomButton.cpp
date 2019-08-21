#include <QMenu>
#include <QDebug>
#include <QPixmap>
#include <QFontDatabase>
#include <QStylePainter>
#include <QMetaProperty>
#include <QStyleOptionToolButton>
#include "CustomButton.h"

static QPixmap pixmap(const QSize &size, const QString &u, const QString &d)
{
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    QPen pen1(Qt::gray);
    QPen pen2("#A5D785");

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(pen1);
    painter.setFont(font);

    painter.drawText(QRect(0, 0, size.width(), size.height() / 2), Qt::AlignCenter, u);
    painter.drawLine(0, size.height() / 2, size.width(), size.height() / 2);

    painter.setPen(pen2);
    painter.drawText(QRect(0, size.height() / 2, size.width(), size.height() / 2), Qt::AlignCenter, d);

    return pixmap;
}

CustomButton::CustomButton(const QString &text, QObject *object, const char *property, QWidget *parent)
    : QToolButton(parent),
      m_object(object),
      m_property(property)
{
    setText(text);
    setMenu(new QMenu(this));
    setPopupMode(InstantPopup);
    setIconSize(QSize(48, 32));
    setToolButtonStyle(Qt::ToolButtonIconOnly);

    const QMetaObject * const meta = m_object->metaObject();
    const QMetaProperty objectProperty = meta->property(meta->indexOfProperty(m_property));
    if (objectProperty.hasNotifySignal())
    {
        const QMetaMethod notifySlot = staticMetaObject.method(staticMetaObject.indexOfMethod("refresh()"));
        connect(m_object, objectProperty.notifySignal(), this, notifySlot);
    }
}

void CustomButton::addItem(const QString &text, const QVariant &data)
{
    QAction * const action = menu()->addAction(text);
    action->setData(data);
    connect(action, &QAction::triggered, this, &CustomButton::changeData);
}

QVariant CustomButton::data() const
{
    return m_object->property(m_property);
}

void CustomButton::setData(const QVariant &data)
{
    m_object->setProperty(m_property, data);
}

void CustomButton::refresh()
{
    foreach (QAction *action, menu()->actions())
    {
        if (action->data() == data())
            menu()->setActiveAction(action);
    }

    setIcon(pixmap(iconSize(), text(), menu()->activeAction() ? menu()->activeAction()->text() : tr("???")));
}

void CustomButton::changeData()
{
    setData(static_cast<QAction *>(sender())->data());
}

void CustomButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);
    option.features &= ~QStyleOptionToolButton::HasMenu;
    painter.drawComplexControl(QStyle::CC_ToolButton, option);
}
