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
}

void PopupButton::appendData(const QString &text, const QVariant &data)
{
    QAction * const action = menu()->addAction(text);
    action->setData(data);
    connect(action, &QAction::triggered, this, &PopupButton::onTriggered);
}

QVariant PopupButton::currentData() const
{
    return m_data;
}

void PopupButton::setCurrentData(const QVariant &data)
{
    QAction *selected = menu()->actions().first();
    foreach (QAction *action, menu()->actions())
        if (action->data() == data)
        {
            selected = action;
            break;
        }

    setText(selected->text());
    m_data = data;
    emit currentDataChanged();
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
    setCurrentData(static_cast<QAction *>(sender())->data());
}
