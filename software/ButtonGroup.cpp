#include <QToolButton>
#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(QObject *parent)
    : QButtonGroup(parent)
{
    connect(this, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled), this, &ButtonGroup::updateCurrentData);
}

QToolButton *ButtonGroup::addDataButton(const QString &text, const QVariant &data)
{
    QToolButton * const button = new QToolButton;
    button->setText(text);
    button->setCheckable(true);
    button->setProperty("data", data);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    addButton(button);

    return button;
}

QVariant ButtonGroup::currentData() const
{
    QAbstractButton * const button = checkedButton();
    return button ? button->property("data") : QVariant();
}

void ButtonGroup::setCurrentData(const QVariant &data)
{
    foreach (QAbstractButton *button, buttons())
        if (button->property("data") == data)
            button->setChecked(true);
}

void ButtonGroup::setEnabled(bool enabled)
{
    foreach (QAbstractButton *button, buttons())
        button->setEnabled(enabled);
}

void ButtonGroup::updateCurrentData(QAbstractButton *button, bool checked)
{
    if (checked)
        emit currentDataChanged(button->property("data"));
}
