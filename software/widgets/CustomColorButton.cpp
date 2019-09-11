#include <QPainter>
#include <QColorDialog>
#include "CustomColorButton.h"

CustomColorButton::CustomColorButton(QWidget *parent)
    : CustomButton(QString(), parent)
{
    setContentsMargins(5, 5, 5, 5);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    connect(this, &CustomColorButton::clicked, this, &CustomColorButton::changeColor);
}

void CustomColorButton::changeColor()
{
    QColorDialog * const dialog = new QColorDialog(color(), this);
    connect(dialog, &QColorDialog::finished, dialog, &QColorDialog::deleteLater);
    connect(dialog, &QColorDialog::colorSelected, this, &CustomColorButton::setColor);

    dialog->open();
}

void CustomColorButton::paintContent(const QRect &content, QPainter &painter) const
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(color());
    painter.drawEllipse(content.center() + QPoint(2, 2), iconSize().width() / 3, iconSize().height() / 3);
}

QSize CustomColorButton::contentSize() const
{
    return iconSize();
}
