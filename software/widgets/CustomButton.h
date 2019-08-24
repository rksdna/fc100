#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QAbstractButton>

class CustomButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit CustomButton(const QString &title, QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

    QString title() const;
    void setTitle(const QString &title);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    virtual void paintContent(const QRect &content, QPainter &painter) const = 0;
    virtual QSize contentSize() const = 0;

    void paintEvent(QPaintEvent *event);

private:
    QString m_title;
    QColor m_color;
};

#endif // CUSTOMBUTTON_H
