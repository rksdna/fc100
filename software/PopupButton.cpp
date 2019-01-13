#include <QMenu>
#include <QStylePainter>
#include <QFontDatabase>
#include <QStyleOptionToolButton>
#include "PopupButton.h"

PopupButton::PopupButton(QWidget *parent)
    : QToolButton(parent)
{
    //setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setMenu(new QMenu(this));
    setPopupMode(InstantPopup);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void PopupButton::appendItem(const QString &text, const QVariant &data)
{
    QAction * const action = menu()->addAction(text);
    action->setData(data);
    connect(action, &QAction::triggered, this, &PopupButton::onTriggered);
}

QVariant PopupButton::data() const
{
    return m_data;
}

void PopupButton::setData(const QVariant &value)
{
    QAction *selected = menu()->actions().first();
    foreach (QAction *action, menu()->actions())
        if (action->data() == value)
        {
            selected = action;
            break;
        }

    setText(selected->text());
    m_data = value;
    emit dataChanged();
}

void PopupButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);
    option.features &= ~QStyleOptionToolButton::HasMenu;
    painter.drawComplexControl(QStyle::CC_ToolButton, option);
}

void PopupButton::onTriggered()
{
    setData(static_cast<QAction *>(sender())->data());
}
